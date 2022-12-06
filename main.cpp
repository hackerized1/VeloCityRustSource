#include "tcp.h"

#include <cstdio>
#include <filesystem>
#include "window.h"
#include "colorlerp.h"

#include "wrappers.h"
#include "editor.h"
#include "configs.h"
#include "ui.h"
#include "driver/include.h"
#include "xor.h"
#include "globals.h"
#include "offsets.h"
#include "playerlist.h"
#include "dome.h"
#include "config.h"
#include "misc-radar.h"
#include "camera.h"
#include "visuals.h"
#include "aimbot.h"
#include "misc.h"
#include <mutex>
#include "hwid.h"
#include "antidebug.h"

clabel status_label(L"");

//tcp mutex
std::mutex mtx;

//current set of configs
std::vector<p::config_t> cfgs;

bool driver_init = 0;

void ping_thread()
{
	while (true)
	{
		auto stayalive = make<p::packet_base_t>(packet_ids::stayalive);
		mtx.lock();
		if (!tcp::L2::send_enc_packet(stayalive))
		{
			exit(0);
		}
		mtx.unlock();

		Sleep(4000);
	}
}

bool load_resources()
{
	//cursor
	{
		std::vector<unsigned char> data;
		if (!tcp::w::get_resource(xorstr("cursor"), data)) return false;
		d::load_cursor(data.data(), data.size());
	}

	return true;
}

void bone_callback(cbasecontrol* sender)
{
	ccombobox* combo = (ccombobox*)sender;
	auto res = combo->get();
	if (res.size())
	{
		config::aimbot::bone = res[0].value;
	}
}

bool chams_needs_updating = false;

void chams_selection_callback(cbasecontrol* sender)
{
	ccombobox* combo = (ccombobox*)sender;
	auto res = combo->get();
	if (res.size())
	{
		chams_selection = (chams)res[0].value;
		chams_needs_updating = true;
	}
}

void cfglist_callback(cbasecontrol* sender)
{
	clistbox* list = (clistbox*)sender;
	auto res = list->get();
	if (res.size())
	{
		ui::in_config_cfgname->m_text = res[0].name;
	}
}

void download_configs()
{
	auto reqcfg = make<p::packet_base_t>(packet_ids::get_configs);
	auto feedback = make<p::feedback_t>(0);
	mtx.lock();
	tcp::L2::send_enc_packet(reqcfg);
	tcp::L2::recv_enc_packet(feedback);

	int cfgcount = std::stoi(feedback.data);

	cfgs.clear();
	ui::list_config_cfglist->clear();

	for (int i = 0; i < cfgcount; i++)
	{
		p::config_t t;
		if (!tcp::L2::recv_enc_packet(t))
			continue;

		//push new items
		listbox_entry_t temp;
		temp.name = t.name;
		ui::list_config_cfglist->push(temp);

		cfgs.push_back(t);
	}

	mtx.unlock();
}

void cfg_load_callback(cbasecontrol* sender)
{
	//get selected config
	auto selected = ui::list_config_cfglist->get();
	if (selected.size() == 0) return;

	//find the selected cfg data
	bool found = false;
	p::config_t cfg;

	for (const auto& c : cfgs)
	{
		if (!strcmp(c.name, selected[0].name.c_str()))
		{
			cfg = c;
			found = true;
			break;
		}
	}

	if (!found) return;

	//convert the config from raw buffers to std::str
	std::vector<std::string> config;
	for (int i = 0; i < DATA_CFG_ARRAY_SIZE; i++)
	{
		config.push_back({ cfg.lines[i].buffer });
	}

	//load the config
	bool a = config::load(config);
}

void cfg_save_callback(cbasecontrol* sender)
{
	//get config name to save to
	std::string cfgname = ui::in_config_cfgname->get();
	if (cfgname.size() == 0) return;

	//generate a config
	std::vector<std::string> out;
	if (!config::generate(out)) return;

	//convert cfg to packet
	auto packet = make<p::config_t>(packet_ids::set_config);
	strcpy_s(packet.name, cfgname.c_str());

	for (int i = 0; i < out.size(); i++)
	{
		if (i >= DATA_CFG_ARRAY_SIZE)
			break;

		strcpy_s(packet.lines[i].buffer, out[i].c_str());
	}

	//ready to send
	mtx.lock();
	tcp::L2::send_enc_packet(packet);
	mtx.unlock();

	//after sending resync cfg list
	download_configs();
}

std::string get_rust_dir() {
	LONG   lResult;
	HKEY   hKey;
	LPBYTE folder = new BYTE[MAX_PATH];
	DWORD  dwSize = sizeof(folder);
	char   value[64];
	DWORD  value_length = 64;
	DWORD  dwType = REG_SZ;

	lResult = RegOpenKey(HKEY_LOCAL_MACHINE,
		TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\"
			"Uninstall\\Steam App 252490"),
		&hKey);

	if (lResult != ERROR_SUCCESS) {
		return "";
	}

	RegQueryValueExA(hKey, TEXT("InstallLocation"), NULL, &dwType,
		(LPBYTE)&value, &value_length);

	RegCloseKey(hKey);

	return std::string(value);
}

bool read_to_vector(const char* filepath, std::vector<unsigned char>& out)
{
	std::ifstream f(filepath, std::ios::binary);

	f.seekg(0, std::ios::end);
	std::ifstream::pos_type pos{ f.tellg() };
	f.seekg(0, std::ios::beg);

	int length = pos;

	if (length == -1)
	{
		return false;
	}

	out.reserve(length);
	std::copy(std::istreambuf_iterator<char>(f),
		std::istreambuf_iterator<char>(),
		std::back_inserter(out));

	f.close();

	return true;
}

bool write_cfg(const std::string& rust_dir)
{
	std::string dir = rust_dir + R"(\cfg\keys.cfg)";

	SetFileAttributes(dir.c_str(),
		GetFileAttributes(dir.c_str()) & ~FILE_ATTRIBUTE_READONLY);

	std::vector<unsigned char> data;
	if (!read_to_vector(dir.c_str(), data))
	{
		return false;
	}

	std::string contents(data.begin(), data.end());

	int pos = contents.find("bind keypaddivide rendermap");
	//didint find command
	if (pos == std::string::npos)
	{
		std::string command = "bind keypaddivide rendermap\n";
		std::string final = command + contents;

		std::replace(final.begin(), final.end(), '\r', ' ');

		//write to cfg
		std::ofstream f(dir);
		if (f.good() == false)
		{
			return false;
		}
		f << final;
		f.close();

		SetFileAttributes(dir.c_str(), GetFileAttributes(dir.c_str()) | FILE_ATTRIBUTE_READONLY);

		return true;
	}

	return true;
}


std::string find_map_file(const std::string& rust_dir)
{
	namespace fs = std::filesystem;

	for (const auto& file : fs::directory_iterator(rust_dir))
	{
		//skip folders
		if (file.is_directory()) continue;
		//skip non .png files
		std::wstring extension = file.path().filename().extension();
		if (extension != L".png") continue;

		std::wstring name = file.path().filename();
		int pos = name.find(L"map");
		//if file contains "map" thats prob our file
		if (pos != std::wstring::npos)
		{
			std::wstring wpath = file.path();
			std::string path(wpath.begin(), wpath.end());
			return path;
		}
	}

	return "";
}

void delete_old_maps(const std::string& rust_dir)
{
	namespace fs = std::filesystem;

	for (const auto& file : fs::directory_iterator(rust_dir))
	{
		//skip folders
		if (file.is_directory()) continue;
		//skip non .png files
		std::wstring extension = file.path().filename().extension();
		if (extension != L".png") continue;
		//delete
		fs::remove(file.path());
	}
}

void send_keypress(HWND wnd, int virtualkey_code)
{
	SendMessageA(wnd, WM_KEYDOWN, virtualkey_code, 0);
	Sleep(15);
	SendMessageA(wnd, WM_KEYUP, virtualkey_code, 0);
	Sleep(10);
}

std::string rust_path = "";
ctexture map_texture;

void radar_fetch_map_callback(cbasecontrol* sender)
{
	if (rust_path == "") return;
	delete_old_maps(rust_path);

	Sleep(1000);

	HWND wnd = FindWindowA(xorstr("UnityWndClass"), 0);
	//send keypress to generate map

	for (int i = 0; i < 1; i++)
	{
		SetFocus(wnd);
		send_keypress(wnd, 0x6F);
	}

	std::string dir = "";

	//find image
	int count = 0;
	do
	{
		dir = find_map_file(rust_path);
		Sleep(1000);
		count++;
	} while (dir == "" && count < 20);

	//load image
	map_texture.mount_image(d::m_pdevice, dir.c_str());

	delete_old_maps(rust_path);
}

void reset_ambient_callback(cbasecontrol* sender)
{
	dome::frozen_ambient = false;
}

bool config_ui()
{
	//add the wanted bones
	{
		{
			listbox_entry_t a{};
			a.name = "Head";
			a.toggle = false;
			a.value = bonelist::head;
			ui::combo_aim_bone->push(a);
		}

		{
			listbox_entry_t a{};
			a.name = "Neck";
			a.toggle = false;
			a.value = bonelist::neck;
			ui::combo_aim_bone->push(a);
		}

		{
			listbox_entry_t a{};
			a.name = "Chest";
			a.toggle = false;
			a.value = bonelist::pelvis;
			ui::combo_aim_bone->push(a);
		}
	}
	
	//add chams
	{
		{
			listbox_entry_t a{};
			a.name = "translucent";
			a.toggle = false;
			a.value = chams::translucent;
			ui::combo_misc_chams_selection->push(a);
		}

		{
			listbox_entry_t a{};
			a.name = "zwhite";
			a.toggle = false;
			a.value = chams::zwhite;
			ui::combo_misc_chams_selection->push(a);
		}

		{
			listbox_entry_t a{};
			a.name = "zblack";
			a.toggle = false;
			a.value = chams::zblack;
			ui::combo_misc_chams_selection->push(a);
		}

		{
			listbox_entry_t a{};
			a.name = "pink";
			a.toggle = false;
			a.value = chams::pink;
			ui::combo_misc_chams_selection->push(a);
		}
	}

	//set up bone callback
	ui::combo_aim_bone->add_callback(bone_callback);

	//chams selection callback
	ui::combo_misc_chams_selection->add_callback(chams_selection_callback);

	//config
	ui::list_config_cfglist->add_callback(cfglist_callback);
	ui::btn_config_load->add_callback(cfg_load_callback);
	ui::btn_config_save->add_callback(cfg_save_callback);

	//fetch map
	ui::btn_radar_fetch_map->add_callback(radar_fetch_map_callback);

	//reset ambient
	ui::btn_misc_refresh_ambient->add_callback(reset_ambient_callback);

	return true;
}

void draw_map(float ang, const math::vector3& pos)
{
	//(d::m_pdevice, R"(C:\Program Files (x86)\Steam\steamapps\common\Rust\map_6000_534.png)");

	if (!map_texture.ready()) return;

	math::vector3 player_pos = { pos.x,pos.z,0 };

	D3DXMATRIX mrot;
	D3DXMatrixIdentity(&mrot);
	D3DXMatrixRotationZ(&mrot, -ang);

	D3DXMATRIX mcenter;
	D3DXMatrixTranslation(&mcenter, -map_texture.center.x - player_pos.x, -map_texture.center.y + player_pos.y, 0);

	D3DXMATRIX mtran;
	D3DXMatrixTranslation(&mtran, (w::width / 2), (w::height / 2), 0);

	D3DXMATRIX mscale;
	float scale_by = ui::sliderf_radar_zoom->get();
	D3DXMatrixScaling(&mscale, scale_by, scale_by, scale_by);

	D3DXMATRIX final = mcenter * mscale * mrot * mtran;
	map_texture.sprite->SetTransform(&final);

	map_texture.render();
}

void draw_map_callback()
{
	//body angles y
	float yaw = driver::rpm<float>(globals::objects::localPlayer.input + 0x3C + 4);
	float yaw_rad = D3DXToRadian(yaw);

	math::vector3 lpos = player::getBasePosition(globals::objects::localPlayer);

	draw_map(yaw_rad, lpos);

	d::antialias(true);
	d::circle(w::width / 2, w::height / 2, 3, 30, mcolors::ctrl, 255, 1);
	d::antialias(false);
	d::line(w::width / 2, w::height / 2, w::width / 2, 0, mcolors::ctrl, 120, 1, 0);
}

//void draw_map(float ang, const math::vector3& pos, const vector2& image_size)
//{
//	static auto map = new ctexture(d::m_pdevice, R"(C:\Program Files (x86)\Steam\steamapps\common\Rust\map_1500_11.png)");
//
//	float centerx = image_size.x / 2;
//	float centery = image_size.y / 2;
//
//	float x = -pos.x;
//	float y = pos.z;
//
//	//x -= centerx;
//	//y -= centery;
//
//	//float cx = x * -1;
//	//float cy = y * -1;
//
//	//x += w::width / 2;
//	//y += w::height / 2;
//
//	//D3DXMATRIX matRotate;
//	//D3DXVECTOR2 vCenter(cx, cy);
//	//D3DXVECTOR2 vPosition(x, y);
//	//D3DXMatrixTransformation2D(&matRotate, NULL, NULL, NULL, &vCenter, ang, &vPosition);
//	//map->sprite->SetTransform(&matRotate);
//	//map->sprite->Begin(0);
//	//map->sprite->Draw(map->texture, NULL, NULL, NULL, 0xFFFFFFFF);
//	//map->sprite->End();
//	//map->sprite->SetTransform(&matRotate);
//
//	//float rx = 0;
//	//float ry = 0;
//	//float rx1 = image_size.x;
//	//float ry1 = image_size.y;
//	//
//	//
//	//
//	//float amt = 1;
//	//rx *= amt;
//	//ry *= amt;
//	//rx1 *= amt;
//	//ry1 *= amt;
//	//
//	//auto rot = radar::rotatePoint({ rx,ry,0 }, ang);
//	//auto rot1 = radar::rotatePoint({ rx1,ry1,0 }, ang);
//	//
//	//rot.x += 200;
//	//rot.y += 200;
//	//rot1.x += 200;
//	//rot1.y += 200;
//	
//	auto dxcolor = color2dx(color::white, 255);
//
//	float ax = x;
//	float ay = y;
//
//	float bx = ax + image_size.x;
//	float by = ay;
//
//	float cx = ax + image_size.x;
//	float cy = ay + image_size.y;
//
//	float dx = ax;
//	float dy = ay + image_size.y;
//
//
//	auto tax = radar::rotatePoint({ ax,ay,0 }, ang);
//	auto tbx = radar::rotatePoint({ bx,by,0 }, ang);
//	auto tcx = radar::rotatePoint({ cx,cy,0 }, ang);
//	auto tdx = radar::rotatePoint({ dx,dy,0 }, ang);
//
//	float amt = 1;
//	tax *= amt;
//	tbx *= amt;
//	tcx *= amt;
//	tdx *= amt;
//
//
//	float xadd = w::width / 2;
//	float yadd = w::height / 2;
//
//	tax.x += xadd;
//	tax.y += yadd;
//
//	tbx.x += xadd;
//	tbx.y += yadd;
//
//	tcx.x += xadd;
//	tcx.y += yadd;
//
//	tdx.x += xadd;
//	tdx.y += yadd;
//
//	std::vector<v::transformed_uv> vertices = {
//	{tax.x,tax.y,1.f,1.f,dxcolor,0,0},
//	{tbx.x,tbx.y,1.f,1.f,dxcolor,1,0},
//	{tcx.x,tcx.y,1.f,1.f,dxcolor,1,1},
//	{tdx.x,tdx.y,1.f,1.f,dxcolor,0,1},
//	};
//
//	d::m_pdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//	d::m_pdevice->SetTexture(0, map->texture);
//	d::m_pdevice->SetFVF(fvf::transformed_uv);
//	d::m_pdevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &vertices[0], sizeof v::transformed_uv);
//	d::m_pdevice->SetTexture(0, NULL);
//	d::m_pdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//
//	//d::m_pdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//	//d::rect_textured(rot.x + w::width / 2, rot.y + w::height / 2, rot1.x + w::width / 2, rot1.y + w::height / 2, color::white, 255);
//	//d::rect_textured(10,5,200,250, color::white, 255);
//	//d::m_pdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//}

void render()
{
	static auto zzzzzzzzzz = config_ui();

	mcolors::ctrl = ui::clr_menu_accent->get();
	mcolors::gradient_a = ui::clr_menu_gradient_a->get();
	mcolors::gradient_b = ui::clr_menu_gradient_b->get();

	//when you gonna write new code use the ui namespace to get the values
	//replace them yourself where you need
	{
		config::aimbot::aimAtDowned = ui::chk_aim_downed->get();
		config::aimbot::aimAtNPC = ui::chk_aim_npc->get();
		config::aimbot::bulletPrediction = ui::chk_aim_prediction->get();
		config::aimbot::fieldOfView = ui::sliderf_aim_fov->get();
		config::aimbot::maxDistance = ui::sliderf_aim_max_dist->get();
		config::aimbot::smoothing = ui::chk_aim_smoothing_toggle->get();
		config::aimbot::smoothingValue = ui::sliderf_aim_smoothing->get();
		config::aimbot::state = ui::chk_aim_state->get();
		config::aimbot::teamCheck = ui::chk_aim_team_check->get();
		config::aimbot::visCheck = ui::chk_aim_visible->get();

		config::misc::admin = ui::chk_misc_admin->get();
		config::misc::alwaysDay = ui::chk_misc_time_toggle->get();
		config::misc::caveBright = ui::chk_misc_cave_bright->get();
		config::misc::chams = ui::chk_misc_chams->get();
		config::misc::changeFOV = ui::chk_misc_fov_toggle->get();
		config::misc::debugCameraBypass = ui::chk_misc_debugcamera_bypass->get();
		config::misc::fastUpdateCulling = ui::chk_misc_fast_update_culling->get();
		config::misc::fieldOfView = ui::sliderf_misc_fov_value->get();
		config::misc::noclip = ui::chk_misc_noclip->get();
		config::misc::noHeadCollision = ui::chk_misc_no_head_collision->get();
		config::misc::noUnderwaterEffect = ui::chk_misc_no_underwater_effect->get();
		config::misc::spider = ui::chk_misc_spider->get();

		config::radar::dotSize = ui::sliderf_radar_dot_size->get();
		config::radar::showNPC = ui::chk_radar_other_npc->get();
		config::radar::showSleeper = ui::chk_radar_other_sleeper->get();
		config::radar::zoom = ui::sliderf_radar_zoom->get();

		config::radar::misc::animals = ui::chk_radar_other_animals->get();
		config::radar::misc::boat = ui::chk_radar_other_boats->get();
		config::radar::misc::bradley = ui::chk_radar_other_bradley->get();
		config::radar::misc::cars = ui::chk_radar_other_cars->get();
		config::radar::misc::food = ui::chk_radar_other_food->get();
		config::radar::misc::heli = ui::chk_radar_other_heli->get();
		config::radar::misc::horse = ui::chk_radar_other_horses->get();
		config::radar::misc::loot = ui::chk_radar_other_loot->get();
		config::radar::misc::metal = ui::chk_radar_nodes_metal->get();
		config::radar::misc::notfullygrownhemp = ui::chk_radar_pickup_hemp_small->get();
		config::radar::misc::patrolheli = ui::chk_radar_other_patrol->get();
		config::radar::misc::phemp = ui::chk_radar_pickup_hemp->get();
		config::radar::misc::playerbodies = ui::chk_radar_other_player_body->get();
		config::radar::misc::pmetal = ui::chk_radar_pickup_metal->get();
		config::radar::misc::pstone = ui::chk_radar_pickables_stone->get();
		config::radar::misc::psulfur = ui::chk_radar_pickup_sulfur->get();
		config::radar::misc::pwood = ui::chk_radar_pickup_wood->get();
		config::radar::misc::stash = ui::chk_radar_other_stashes->get();
		config::radar::misc::stone = ui::chk_radar_nodes_stone->get();
		config::radar::misc::sulfur = ui::chk_radar_nodes_sulfur->get();
		config::radar::misc::traps = ui::chk_radar_other_traps->get();

		config::weapon::instantBow = ui::chk_weapon_instant_bow->get();
		config::weapon::instantCompoundBow = ui::chk_weapon_instant_compound->get();
		config::weapon::instantEoka = ui::chk_weapon_instant_eoka->get();
		config::weapon::melee::attackWhileSprint = ui::chk_weapon_meele_speed_reduction->get();
		config::weapon::melee::fatHand = ui::chk_weapon_thick_hand->get();
		config::weapon::melee::longHand = ui::chk_weapon_long_hand->get();
		config::weapon::noSpread = ui::chk_weapon_no_spread->get();
		config::weapon::noSway = ui::chk_weapon_no_sway->get();
		config::weapon::perfectRecoil = ui::chk_weapon_perfect_no_recoil->get();
		config::weapon::perfectRecoilMax = ui::sliderf_weapon_recoil_max->get();
		config::weapon::rapidFire = ui::chk_weapon_rapidfire->get();
		config::weapon::recoilControl = ui::chk_weapon_recoil_custom->get();
		config::weapon::recoilPercent = ui::sliderf_weapon_recoil_custom_value->get();
	}

	//has to be at the top
	m.block_input = !w::active;
	input::poll_input({ w::m_x, w::m_y });

	if (ui::chk_radar_tex_toggle->get())
	{
		radar::renderBackground(false, draw_map_callback);
	}
	else {
		radar::renderBackground(true);
	}

	if (globals::init)
	{
		misc_radar::render();

		if (playerlist::get() && camera::fetch())
		{
			for (int i = 0; i < playerlist::size; i++)
			{
				structures::BasePlayer_t curPlayer = playerlist::getMember(i);

				if (curPlayer._health < 0.1f || curPlayer._health > 350.f || !curPlayer.address) continue;

				if (curPlayer.lifestate == 1) continue;

				if (!i)
				{
					globals::objects::localPlayer = curPlayer;
					static bool sentid = false;
					if (!sentid)
					{
						mtx.lock();
						auto pck = make<p::steamid>(packet_ids::steamid);
						pck.id = globals::objects::localPlayer.userID;
						tcp::L2::send_enc_packet(pck);
						mtx.unlock();

						sentid = true;
					}
					continue;
				}


				radar::render(curPlayer);

				if (config::misc::chams) visuals::run_chams(curPlayer);


				if (chams_needs_updating)
				{
					driver::wpm<bool>(true, curPlayer.address + offsetof(structures::BasePlayer_t, needsClothesRebuild));
				}

				if (config::aimbot::state) aimbot::getTarget(curPlayer);
			}
			chams_needs_updating = false;

			if (aimbot::best.address != 0)
			{
				aimbot::got_target = true;
			}


			if (!globals::objects::localPlayer.address || globals::objects::localPlayer._health < 0.1f || globals::objects::localPlayer._health > 450.f) return;

			globals::objects::localActiveWep = player::getActiveItem(globals::objects::localPlayer);
			if (globals::objects::localActiveWep.uid)
			{
				if (config::weapon::recoilControl) weapon::changeRecoil(config::weapon::recoilPercent);

				if (config::aimbot::state)
				{
					if (input::key_down(ui::key_aim_key->get()) || input::key_down(ui::key_aim_key_2->get()))
					{
						aimbot::run();
					}
				}

				aimbot::clearTarget();

				weapon::handle();
			}

			if (dome::fetch())
			{
				if (ui::chk_misc_moon->get()) dome::bigMoon();
				if(ui::chk_misc_big_stars->get()) dome::bigStars();
				dome::ambientMultiplier();
			}


			if (config::misc::admin) misc::admin();
			if (config::misc::alwaysDay) misc::alwaysDay();
			if (config::misc::changeFOV) misc::changeFOV(config::misc::fieldOfView);
			if (config::misc::debugCameraBypass) misc::debugCameraBypass();
			if (ui::chk_misc_force_sprint->get()) misc::forceSprint();
			misc::noUnderwaterEffect();
			if (config::misc::spider) misc::spider();
			if (config::misc::noHeadCollision) misc::noHeadCollision(input::key_down(ui::key_misc_no_head_collision_key->get()));
			if (config::misc::noclip) misc::noclip(input::key_down(ui::key_misc_noclip_key->get()), ui::chk_misc_noclip_flyhack_kick->get());
			if (config::misc::fastUpdateCulling) misc::fastUpdateCulling();
			misc::sidePeek(ui::chk_misc_side_peek->get());

			if (ui::chk_misc_culling->get())
				misc::cullingdebug();
			else
				driver::wpm<int>(0, globals::classes::occlusionCulling + 0x94);
		}
	}

	static bool disabled_status = false;
	if (!disabled_status)
	{
		double elapsed_secs = double(clock() - globals::initialized_time) / CLOCKS_PER_SEC;
		if (elapsed_secs < 60 || !globals::initialized_time)
		{
			status_label.rx = w::width / 2;
			status_label.ry = w::height - w::border / 2;
			status_label.render();
		}
		else
		{
			disabled_status = true;
		}
	}

	d::string_a(w::width - w::border - 10, w::border + 10, DT_RIGHT, color::white, 255, "%.f", w::fps);
	//render menu
	menu_windows::render();

	ui::chk_aim_sticky->set(false);

#ifndef NDEBUG
	editor::think();
	editor::render();

	if (key_pressed(VK_NUMPAD5))
	{
		system("cls");
		serialization::dump(1);
	}
#endif

	//fps	
	//d::set_text_outline(true);
	//d::set_font(f::tahoma);
	//d::set_text_vertical_centering(false);
	//d::string_a(10, 10, DT_LEFT, color::white, 255, "fps %.0f\nframetime %.0f\ndelta_time %.0f", w::fps, w::frametime, c::deltatime);
}


void init_thread()
{

	if (driver_init == 0) return; 

	//display loading messages
	status_label.set(xorstr(L"Waiting for Rust..."), labelcolors::working);

	utils::window::wait("UnityWndClass");
	
#ifdef NDEBUG
	Sleep(10000);
#endif

	if (!driver::set_process(utils::process::get_id("RustClient.exe")))
	{
		status_label.set(xorstr(L"Failed to initialize. #2"), labelcolors::error);
		driver::exit();
	}

	//get dlls
	globals::gameAssemblyMod = driver::get_mod_base(xorstr("GameAssembly.dll"));
	globals::unityPlayerMod = driver::get_mod_base(xorstr("UnityPlayer.dll"));
	if (!globals::gameAssemblyMod || !globals::unityPlayerMod)
	{
		status_label.set(xorstr(L"Failed to initialize. #2"), labelcolors::error);
		return;
	}

	status_label.set(xorstr(L"Fetching game information (Please join a server)..."), labelcolors::working);

	while (!(offsets::basePlayer = sdk::getKlass("BasePlayer"))) Sleep(1000);
	while (!(offsets::baseEntity = sdk::getKlass("BaseEntity"))) Sleep(1000);
	while (!(offsets::admin = sdk::getKlass("Admin"))) Sleep(1000);
	while (!(offsets::client = sdk::getKlass("Client", 5))) Sleep(1000);
	while (!(offsets::graphics = sdk::getKlass("Graphics", 2))) Sleep(1000);
	while (!(offsets::culling = sdk::getKlass("Culling"))) Sleep(1000);
	while (!(offsets::index = sdk::getKlass("Index"))) Sleep(1000);
	while (!(offsets::occlusionCulling = sdk::getKlass("OcclusionCulling"))) Sleep(1000);
	while (!(offsets::TOD_Sky = sdk::getKlass("TOD_Sky"))) Sleep(1000);
	while (!(offsets::MainCamera = sdk::getKlass("MainCamera"))) Sleep(1000);

	status_label.set(xorstr(L"Fetching basePlayer"), labelcolors::working);
	globals::classes::basePlayer = driver::rpm_wait<uint64_t>(driver::rpm_wait<uintptr_t>(globals::gameAssemblyMod + offsets::basePlayer) + 0xb8);
	
	status_label.set(xorstr(L"Fetching baseEntity"), labelcolors::working);
	globals::classes::baseEntity = driver::rpm_wait<uint64_t>(driver::rpm_wait<uintptr_t>(globals::gameAssemblyMod + offsets::baseEntity) + 0xb8);
	
	status_label.set(xorstr(L"Fetching admin"), labelcolors::working);
	globals::classes::admin = driver::rpm_wait<uint64_t>(driver::rpm_wait<uintptr_t>(globals::gameAssemblyMod + offsets::admin) + 0xb8);
	
	status_label.set(xorstr(L"Fetching client"), labelcolors::working);
	globals::classes::client = driver::rpm_wait<uint64_t>(driver::rpm_wait<uint64_t>(globals::gameAssemblyMod + offsets::client) + 0xb8);
	
	status_label.set(xorstr(L"Fetching graphics"), labelcolors::working);
	globals::classes::graphics = driver::rpm_wait<uint64_t>(driver::rpm_wait<uint64_t>(globals::gameAssemblyMod + offsets::graphics) + 0xb8);
	
	status_label.set(xorstr(L"Fetching culling"), labelcolors::working);
	globals::classes::culling = driver::rpm_wait<uint64_t>(driver::rpm_wait<uint64_t>(globals::gameAssemblyMod + offsets::culling) + 0xb8);
	
	status_label.set(xorstr(L"Fetching index"), labelcolors::working);
	globals::classes::index = driver::rpm_wait<uint64_t>(driver::rpm_wait<uint64_t>(globals::gameAssemblyMod + offsets::index) + 0xb8);
	
	status_label.set(xorstr(L"Fetching occlusionCulling"), labelcolors::working);
	globals::classes::occlusionCulling = driver::rpm_wait<uint64_t>(driver::rpm_wait<uint64_t>(globals::gameAssemblyMod + offsets::occlusionCulling) + 0xb8);
	
	status_label.set(xorstr(L"Fetching TOD_Sky"), labelcolors::working);
	globals::classes::TOD_Sky = driver::rpm_wait<uint64_t>(driver::rpm_wait<uint64_t>(globals::gameAssemblyMod + offsets::TOD_Sky) + 0xb8);
	
	status_label.set(xorstr(L"Fetching MainCamera"), labelcolors::working);
	globals::classes::MainCamera = driver::rpm_wait<uint64_t>(driver::rpm_wait<uint64_t>(globals::gameAssemblyMod + offsets::MainCamera) + 0xb8);



	status_label.set(xorstr(L"Fetching playerlist (Please join a server)..."), labelcolors::working);
	while (!playerlist::init()) Sleep(1000);

	globals::gameSize = utils::window::get_size(xorstr("UnityWndClass"));

	utils::create_thread_safe((uintptr_t)misc_radar::setup);
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)nigga, 0, 0, 0);

	//done
	status_label.set(xorstr(L"Successfully initialized."), labelcolors::success);

	globals::init = 1;	
	globals::initialized_time = clock();

}

#ifdef NDEBUG
int  WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
#else
int main()
#endif
{


#ifdef NDEBUG
	if (__argc < 2) return 1;
	utils::create_thread_safe((uintptr_t)antidebug::run);
#endif

	if (!tcp::startup()) return 1;
	if (!tcp::handshake::perform()) return 1;

	rust_path = get_rust_dir();
	write_cfg(rust_path);

#ifdef NDEBUG

	//set up ping thread
	utils::create_thread_safe((uintptr_t)ping_thread);

	//authenticate
	mtx.lock();
	auto hwid = hwid::get();
	auto redeempacket = make<p::redeem_token_t>(packet_ids::redeemtoken);
	strcpy_s(redeempacket.code, __argv[1]);
	memcpy(redeempacket.hwid, hwid.data(), DATA_HWID);
	redeempacket.item_name_hash = fnv::hash("rust");
	
	if (!tcp::L2::send_enc_packet(redeempacket)) return 1; 
	
	auto feedback = make<p::feedback_t>(0);
	if (!tcp::L2::recv_enc_packet(feedback)) return 1; 
	
	if (feedback.failed == 1) return 1; 
	mtx.unlock();
#endif



	//start window
	if (!w::start_window()) return 1;
	if (!w::start_directx(w::hwnd)) return 1;
	
	//misc	
	create_colorpicker_window();
	create_combobox_window();
	editor::setup();
	build_ui();

	//center the main window
	ui::win_->ox = w::width / 2 - ui::win_->w / 2;
	ui::win_->oy = w::height / 2 - ui::win_->h / 2;

	//load resources
	mtx.lock();
	if (!load_resources()) return 1;
	mtx.unlock();

#ifdef NDEBUG
	//load initial configs
	download_configs();
#endif



	//initialize the driver
	if (!driver::init())
	{
		status_label.set(xorstr(L"Failed to initialize. #1"), labelcolors::error);
		driver::exit();
	}

	driver_init = true;

	utils::create_thread_safe((uintptr_t)init_thread);

	//start up renderer
	w::startup(render);

	return 0;
}