#pragma once
#include "wrappers.h"

#include "themida.h"

#pragma once
#include "wrappers.h"

#include "themida.h"

namespace ui
{
    cwindow* win_ = nullptr;
    ctabcontrol* mtabctrl_ = nullptr;
    ctab* tab_aimbot = nullptr;
    cgroupbox* grp_ = nullptr;
    ccheckbox* chk_aim_state = nullptr;
    csliderfloat* sliderf_aim_fov = nullptr;
    ckeybind* key_aim_key = nullptr;
    ckeybind* key_aim_key_2 = nullptr;
    cgroupbox* grp_1 = nullptr;
    ccheckbox* chk_aim_npc = nullptr;
    ccheckbox* chk_aim_downed = nullptr;
    cgroupbox* grp_2 = nullptr;
    ccombobox* combo_aim_bone = nullptr;
    cgroupbox* grp_3 = nullptr;
    ccheckbox* chk_aim_prediction = nullptr;
    ccheckbox* chk_aim_team_check = nullptr;
    ccheckbox* chk_aim_visible = nullptr;
    csliderfloat* sliderf_aim_max_dist = nullptr;
    csliderfloat* sliderf_aim_smoothing = nullptr;
    ctab* tab_radar = nullptr;
    ccheckbox* chk_aim_smoothing_toggle = nullptr;
    ccheckbox* chk_aim_max_dist_toggle = nullptr;
    cgroupbox* grp_4 = nullptr;
    cgroupbox* grp_5 = nullptr;
    csliderfloat* sliderf_radar_dot_size = nullptr;
    cgroupbox* grp_6 = nullptr;
    csliderfloat* sliderf_radar_zoom = nullptr;
    cgroupbox* grp_7 = nullptr;
    cgroupbox* grp_8 = nullptr;
    cgroupbox* grp_9 = nullptr;
    ccheckbox* chk_radar_nodes_sulfur = nullptr;
    ccheckbox* chk_radar_nodes_metal = nullptr;
    ccheckbox* chk_radar_nodes_stone = nullptr;
    ccheckbox* chk_radar_pickup_sulfur = nullptr;
    ccheckbox* chk_radar_pickup_metal = nullptr;
    ccheckbox* chk_radar_pickup_wood = nullptr;
    ccheckbox* chk_radar_pickup_hemp = nullptr;
    ccheckbox* chk_radar_pickup_hemp_small = nullptr;
    ccheckbox* chk_radar_other_sleeper = nullptr;
    ccheckbox* chk_radar_other_npc = nullptr;
    ccheckbox* chk_radar_other_player_body = nullptr;
    ccheckbox* chk_radar_other_loot = nullptr;
    ccheckbox* chk_radar_other_heli = nullptr;
    ccheckbox* chk_radar_other_horses = nullptr;
    ccheckbox* chk_radar_other_stashes = nullptr;
    ccheckbox* chk_radar_other_boats = nullptr;
    ccheckbox* chk_radar_other_food = nullptr;
    ccheckbox* chk_radar_other_patrol = nullptr;
    ccheckbox* chk_radar_other_traps = nullptr;
    ccheckbox* chk_radar_other_animals = nullptr;
    ccheckbox* chk_radar_other_bradley = nullptr;
    ccheckbox* chk_radar_other_cars = nullptr;
    ctab* tab_weapon = nullptr;
    cgroupbox* grp_10 = nullptr;
    ccheckbox* chk_weapon_no_sway = nullptr;
    ccheckbox* chk_weapon_no_spread = nullptr;
    ccheckbox* chk_weapon_rapidfire = nullptr;
    ccheckbox* chk_weapon_instant_eoka = nullptr;
    ccheckbox* chk_weapon_perfect_no_recoil = nullptr;
    ccheckbox* chk_weapon_instant_bow = nullptr;
    ccheckbox* chk_weapon_instant_compound = nullptr;
    ccheckbox* chk_weapon_thick_hand = nullptr;
    ccheckbox* chk_weapon_long_hand = nullptr;
    ccheckbox* chk_weapon_meele_speed_reduction = nullptr;
    cgroupbox* grp_11 = nullptr;
    csliderfloat* sliderf_weapon_recoil_max = nullptr;
    ccheckbox* chk_weapon_recoil_custom = nullptr;
    csliderfloat* sliderf_weapon_recoil_custom_value = nullptr;
    ctab* tab_misc = nullptr;
    cgroupbox* grp_12 = nullptr;
    ccheckbox* chk_misc_admin = nullptr;
    ccheckbox* chk_misc_chams = nullptr;
    ccheckbox* chk_misc_spider = nullptr;
    ccheckbox* chk_misc_noclip = nullptr;
    ccheckbox* chk_misc_cave_bright = nullptr;
    ccheckbox* chk_misc_no_underwater_effect = nullptr;
    ccheckbox* chk_misc_force_sprint = nullptr;
    ccheckbox* chk_misc_no_head_collision = nullptr;
    ccheckbox* chk_misc_debugcamera_bypass = nullptr;
    ccheckbox* chk_misc_fast_update_culling = nullptr;
    ccheckbox* chk_misc_fov_toggle = nullptr;
    csliderfloat* sliderf_misc_fov_value = nullptr;
    ccheckbox* chk_misc_time_toggle = nullptr;
    csliderfloat* sliderf_misc_time_value = nullptr;
    ctab* tab_config = nullptr;
    cgroupbox* grp_14 = nullptr;
    clistbox* list_config_cfglist = nullptr;
    ctextbox* in_config_cfgname = nullptr;
    cbutton* btn_config_load = nullptr;
    cbutton* btn_config_save = nullptr;
    ctab* tab_menu = nullptr;
    cgroupbox* grp_15 = nullptr;
    ccolorbutton* clr_menu_accent = nullptr;
    ccolorbutton* clr_menu_gradient_a = nullptr;
    ccolorbutton* clr_menu_gradient_b = nullptr;
    ccheckbox* chk_radar_pickables_stone = nullptr;
    ckeybind* key_misc_noclip_key = nullptr;
    ckeybind* key_misc_no_head_collision_key = nullptr;
    ccheckbox* chk_misc_moon = nullptr;
    cbutton* btn_radar_fetch_map = nullptr;
    ccheckbox* chk_radar_tex_toggle = nullptr;
    ccheckbox* chk_misc_big_stars = nullptr;
    ccheckbox* chk_misc_nightmode = nullptr;
    cgroupbox* grp_16 = nullptr;
    cbutton* btn_misc_refresh_ambient = nullptr;
    cgroupbox* grp_17 = nullptr;
    ccheckbox* chk_misc_side_peek = nullptr;
    ckeybind* key_misc_side_peek_left = nullptr;
    ckeybind* key_misc_side_peek_right = nullptr;
    ckeybind* key_misc_side_peek_forward = nullptr;
    ccombobox* combo_misc_chams_selection = nullptr;
    ccheckbox* chk_misc_culling = nullptr;
    ccheckbox* chk_misc_noclip_flyhack_kick = nullptr;
    ccheckbox* chk_aim_sticky = nullptr;
}

void build_ui()
{
    using namespace ui;
    using namespace menu;

    set_variable_hash(0xdd149a72);
    //set_variable("win_");
    set_position(274, 303, 270, 330);
    win_ = window(L"");
    win_->m_disabled = 0;
    win_->m_draggable = 1;
    win_->m_closable = 0;
    win_->m_popup = 0;
    win_->m_draw_outline = 1;

    set_variable_hash(0x65b11f1b);
    //set_variable("mtabctrl_");
    set_parent(win_);
    set_position(5, 20, 260, 305);
    mtabctrl_ = tabcontrol();

    set_variable_hash(0x8b1dff13);
    //set_variable("tab_aimbot");
    set_parent(mtabctrl_);
    tab_aimbot = tab(L"aimbot");

    set_variable_hash(0x947129c1);
    //set_variable("grp_");
    set_parent(tab_aimbot);
    set_position(5, 20, 250, 30);
    grp_ = groupbox(L"");

    set_variable_hash(0x40ca5837);
    //set_variable("chk_aim_state");
    set_parent(tab_aimbot);
    set_position(10, 30, 45, 10);
    chk_aim_state = checkbox(L"state");

    set_variable_hash(0xf3baadf4);
    //set_variable("sliderf_aim_fov");
    set_parent(tab_aimbot);
    set_position(60, 30, 130, 10);
    sliderf_aim_fov = sliderfloat(5.000000, 700.000000);

    set_variable_hash(0x222ffdf6);
    //set_variable("key_aim_key");
    set_parent(tab_aimbot);
    set_position(195, 25, 25, 20);
    key_aim_key = keybind();
    key_aim_key->set(0);

    set_variable_hash(0x23e3dcbb);
    //set_variable("key_aim_key_2");
    set_parent(tab_aimbot);
    set_position(225, 25, 25, 20);
    key_aim_key_2 = keybind();
    key_aim_key_2->set(0);

    set_variable_hash(0x9e2504d0);
    //set_variable("grp_1");
    set_parent(tab_aimbot);
    set_position(5, 60, 70, 55);
    grp_1 = groupbox(L"target");

    set_variable_hash(0xc57f8e5d);
    //set_variable("chk_aim_npc");
    set_parent(tab_aimbot);
    set_position(10, 70, 50, 15);
    chk_aim_npc = checkbox(L"npc");

    set_variable_hash(0x81251fbb);
    //set_variable("chk_aim_downed");
    set_parent(tab_aimbot);
    set_position(10, 90, 55, 15);
    chk_aim_downed = checkbox(L"injured");

    set_variable_hash(0xa1250989);
    //set_variable("grp_2");
    set_parent(tab_aimbot);
    set_position(80, 60, 100, 40);
    grp_2 = groupbox(L"bone");

    set_variable_hash(0xbf7fa3de);
    //set_variable("combo_aim_bone");
    set_parent(tab_aimbot);
    set_position(85, 70, 90, 20);
    combo_aim_bone = combobox();

    set_variable_hash(0xa02507f6);
    //set_variable("grp_3");
    set_parent(tab_aimbot);
    set_position(5, 125, 250, 175);
    grp_3 = groupbox(L"settings");

    set_variable_hash(0x608d9edf);
    //set_variable("chk_aim_prediction");
    set_parent(tab_aimbot);
    set_position(10, 135, 70, 10);
    chk_aim_prediction = checkbox(L"prediction");

    set_variable_hash(0xf7d7ed3c);
    //set_variable("chk_aim_team_check");
    set_parent(tab_aimbot);
    set_position(10, 150, 75, 10);
    chk_aim_team_check = checkbox(L"team check");

    set_variable_hash(0xcdf032f4);
    //set_variable("chk_aim_visible");
    set_parent(tab_aimbot);
    set_position(10, 165, 80, 10);
    chk_aim_visible = checkbox(L"visible check");

    set_variable_hash(0x525beaee);
    //set_variable("sliderf_aim_max_dist");
    set_parent(tab_aimbot);
    set_position(85, 195, 80, 10);
    sliderf_aim_max_dist = sliderfloat(30.000000, 390.000000);

    set_variable_hash(0x6c1a34d1);
    //set_variable("sliderf_aim_smoothing");
    set_parent(tab_aimbot);
    set_position(85, 180, 80, 10);
    sliderf_aim_smoothing = sliderfloat(1.000000, 12.000000);

    set_variable_hash(0xd04c27f);
    //set_variable("tab_radar");
    set_parent(mtabctrl_);
    tab_radar = tab(L"radar");

    set_variable_hash(0x4e5759f9);
    //set_variable("chk_aim_smoothing_toggle");
    set_parent(tab_aimbot);
    set_position(10, 180, 70, 10);
    chk_aim_smoothing_toggle = checkbox(L"smoothing");

    set_variable_hash(0xb67e2496);
    //set_variable("chk_aim_max_dist_toggle");
    set_parent(tab_aimbot);
    set_position(10, 195, 70, 10);
    chk_aim_max_dist_toggle = checkbox(L"max dist");

    set_variable_hash(0xa3250caf);
    //set_variable("grp_4");
    set_parent(tab_radar);
    set_position(5, 25, 250, 50);
    grp_4 = groupbox(L"radar settings");

    set_variable_hash(0xa2250b1c);
    //set_variable("grp_5");
    set_parent(tab_radar);
    set_position(10, 40, 115, 25);
    grp_5 = groupbox(L"dot size");

    set_variable_hash(0x46be75af);
    //set_variable("sliderf_radar_dot_size");
    set_parent(tab_radar);
    set_position(15, 50, 105, 10);
    sliderf_radar_dot_size = sliderfloat(1.000000, 30.000000);

    set_variable_hash(0xa5250fd5);
    //set_variable("grp_6");
    set_parent(tab_radar);
    set_position(130, 40, 120, 25);
    grp_6 = groupbox(L"zoom");

    set_variable_hash(0xdff5c89d);
    //set_variable("sliderf_radar_zoom");
    set_parent(tab_radar);
    set_position(135, 50, 110, 10);
    sliderf_radar_zoom = sliderfloat(0.200000, 3.000000);

    set_variable_hash(0xa4250e42);
    //set_variable("grp_7");
    set_parent(tab_radar);
    set_position(5, 85, 65, 60);
    grp_7 = groupbox(L"nodes");

    set_variable_hash(0xa72512fb);
    //set_variable("grp_8");
    set_parent(tab_radar);
    set_position(75, 85, 180, 60);
    grp_8 = groupbox(L"pickables");

    set_variable_hash(0xa6251168);
    //set_variable("grp_9");
    set_parent(tab_radar);
    set_position(5, 155, 250, 145);
    grp_9 = groupbox(L"other");

    set_variable_hash(0xfe730636);
    //set_variable("chk_radar_nodes_sulfur");
    set_parent(tab_radar);
    set_position(10, 95, 50, 10);
    chk_radar_nodes_sulfur = checkbox(L"sulfur");

    set_variable_hash(0x7edc20ae);
    //set_variable("chk_radar_nodes_metal");
    set_parent(tab_radar);
    set_position(10, 110, 50, 10);
    chk_radar_nodes_metal = checkbox(L"metal");

    set_variable_hash(0x95a43888);
    //set_variable("chk_radar_nodes_stone");
    set_parent(tab_radar);
    set_position(10, 125, 50, 10);
    chk_radar_nodes_stone = checkbox(L"stone");

    set_variable_hash(0xb6d05d6b);
    //set_variable("chk_radar_pickup_sulfur");
    set_parent(tab_radar);
    set_position(80, 95, 55, 10);
    chk_radar_pickup_sulfur = checkbox(L"sulfur");

    set_variable_hash(0x51d17421);
    //set_variable("chk_radar_pickup_metal");
    set_parent(tab_radar);
    set_position(80, 110, 50, 10);
    chk_radar_pickup_metal = checkbox(L"metal");

    set_variable_hash(0x6e86e653);
    //set_variable("chk_radar_pickup_wood");
    set_parent(tab_radar);
    set_position(80, 125, 50, 10);
    chk_radar_pickup_wood = checkbox(L"wood");

    set_variable_hash(0xb96b9394);
    //set_variable("chk_radar_pickup_hemp");
    set_parent(tab_radar);
    set_position(140, 95, 55, 10);
    chk_radar_pickup_hemp = checkbox(L"hemp");

    set_variable_hash(0x7b05d468);
    //set_variable("chk_radar_pickup_hemp_small");
    set_parent(tab_radar);
    set_position(140, 110, 105, 10);
    chk_radar_pickup_hemp_small = checkbox(L"hemp not grown");

    set_variable_hash(0x98855c16);
    //set_variable("chk_radar_other_sleeper");
    set_parent(tab_radar);
    set_position(10, 165, 55, 10);
    chk_radar_other_sleeper = checkbox(L"sleeper");

    set_variable_hash(0x6e089bed);
    //set_variable("chk_radar_other_npc");
    set_parent(tab_radar);
    set_position(10, 180, 55, 10);
    chk_radar_other_npc = checkbox(L"npcs");

    set_variable_hash(0xb6028508);
    //set_variable("chk_radar_other_player_body");
    set_parent(tab_radar);
    set_position(10, 195, 80, 10);
    chk_radar_other_player_body = checkbox(L"player body");

    set_variable_hash(0xa1f8bac);
    //set_variable("chk_radar_other_loot");
    set_parent(tab_radar);
    set_position(10, 210, 45, 10);
    chk_radar_other_loot = checkbox(L"loot");

    set_variable_hash(0xe0f5f6ba);
    //set_variable("chk_radar_other_heli");
    set_parent(tab_radar);
    set_position(10, 225, 75, 10);
    chk_radar_other_heli = checkbox(L"helicopters");

    set_variable_hash(0x4b054f94);
    //set_variable("chk_radar_other_horses");
    set_parent(tab_radar);
    set_position(10, 240, 55, 10);
    chk_radar_other_horses = checkbox(L"horses");

    set_variable_hash(0x6b09cab7);
    //set_variable("chk_radar_other_stashes");
    set_parent(tab_radar);
    set_position(10, 255, 60, 10);
    chk_radar_other_stashes = checkbox(L"stashes");

    set_variable_hash(0xc18a5089);
    //set_variable("chk_radar_other_boats");
    set_parent(tab_radar);
    set_position(10, 270, 50, 10);
    chk_radar_other_boats = checkbox(L"boats");

    set_variable_hash(0x90ef7972);
    //set_variable("chk_radar_other_food");
    set_parent(tab_radar);
    set_position(110, 165, 50, 10);
    chk_radar_other_food = checkbox(L"food");

    set_variable_hash(0x784bbee8);
    //set_variable("chk_radar_other_patrol");
    set_parent(tab_radar);
    set_position(110, 180, 70, 10);
    chk_radar_other_patrol = checkbox(L"patrol heli");

    set_variable_hash(0x2d896e);
    //set_variable("chk_radar_other_traps");
    set_parent(tab_radar);
    set_position(110, 195, 50, 10);
    chk_radar_other_traps = checkbox(L"traps");

    set_variable_hash(0xfa14d1ef);
    //set_variable("chk_radar_other_animals");
    set_parent(tab_radar);
    set_position(110, 210, 60, 10);
    chk_radar_other_animals = checkbox(L"animals");

    set_variable_hash(0x5f9f3603);
    //set_variable("chk_radar_other_bradley");
    set_parent(tab_radar);
    set_position(110, 225, 60, 10);
    chk_radar_other_bradley = checkbox(L"bradley");

    set_variable_hash(0x607381cd);
    //set_variable("chk_radar_other_cars");
    set_parent(tab_radar);
    set_position(110, 240, 55, 10);
    chk_radar_other_cars = checkbox(L"cars");

    set_variable_hash(0xce67bfa5);
    //set_variable("tab_weapon");
    set_parent(mtabctrl_);
    tab_weapon = tab(L"weapon");

    set_variable_hash(0xd446aca0);
    //set_variable("grp_10");
    set_parent(tab_weapon);
    set_position(5, 20, 250, 90);
    grp_10 = groupbox(L"");

    set_variable_hash(0x3f7bf477);
    //set_variable("chk_weapon_no_sway");
    set_parent(tab_weapon);
    set_position(10, 30, 65, 10);
    chk_weapon_no_sway = checkbox(L"no sway");

    set_variable_hash(0x5561bcac);
    //set_variable("chk_weapon_no_spread");
    set_parent(tab_weapon);
    set_position(10, 45, 70, 10);
    chk_weapon_no_spread = checkbox(L"no spread");

    set_variable_hash(0xfef1ccc9);
    //set_variable("chk_weapon_rapidfire");
    set_parent(tab_weapon);
    set_position(10, 60, 70, 10);
    chk_weapon_rapidfire = checkbox(L"rapid fire");

    set_variable_hash(0xa949f7a7);
    //set_variable("chk_weapon_instant_eoka");
    set_parent(tab_weapon);
    set_position(115, 30, 80, 10);
    chk_weapon_instant_eoka = checkbox(L"instant eoka");

    set_variable_hash(0x2eaaa243);
    //set_variable("chk_weapon_perfect_no_recoil");
    set_parent(tab_weapon);
    set_position(10, 130, 95, 10);
    chk_weapon_perfect_no_recoil = checkbox(L"perfect no recoil");

    set_variable_hash(0x75319621);
    //set_variable("chk_weapon_instant_bow");
    set_parent(tab_weapon);
    set_position(115, 60, 80, 10);
    chk_weapon_instant_bow = checkbox(L"instant bow");

    set_variable_hash(0x759feffa);
    //set_variable("chk_weapon_instant_compound");
    set_parent(tab_weapon);
    set_position(10, 75, 100, 10);
    chk_weapon_instant_compound = checkbox(L"instant compound");

    set_variable_hash(0x508837f6);
    //set_variable("chk_weapon_thick_hand");
    set_parent(tab_weapon);
    set_position(115, 45, 100, 10);
    chk_weapon_thick_hand = checkbox(L"thick hand");

    set_variable_hash(0x7a9fc471);
    //set_variable("chk_weapon_long_hand");
    set_parent(tab_weapon);
    set_position(115, 75, 70, 10);
    chk_weapon_long_hand = checkbox(L"long hand");

    set_variable_hash(0x1ab922ff);
    //set_variable("chk_weapon_meele_speed_reduction");
    set_parent(tab_weapon);
    set_position(10, 90, 135, 10);
    chk_weapon_meele_speed_reduction = checkbox(L"no melee speed reduction");

    set_variable_hash(0xd546ae33);
    //set_variable("grp_11");
    set_parent(tab_weapon);
    set_position(5, 120, 250, 45);
    grp_11 = groupbox(L"recoil settings");

    set_variable_hash(0x93e1ccdf);
    //set_variable("sliderf_weapon_recoil_max");
    set_parent(tab_weapon);
    set_position(110, 130, 100, 10);
    sliderf_weapon_recoil_max = sliderfloat(0.000000, 20.000000);

    set_variable_hash(0xcb7145f7);
    //set_variable("chk_weapon_recoil_custom");
    set_parent(tab_weapon);
    set_position(10, 145, 90, 10);
    chk_weapon_recoil_custom = checkbox(L"custom recoil");

    set_variable_hash(0xff8cbb8e);
    //set_variable("sliderf_weapon_recoil_custom_value");
    set_parent(tab_weapon);
    set_position(110, 145, 100, 10);
    sliderf_weapon_recoil_custom_value = sliderfloat(0.000000, 100.000000);

    set_variable_hash(0x2cfa99c5);
    //set_variable("tab_misc");
    set_parent(mtabctrl_);
    tab_misc = tab(L"misc");

    set_variable_hash(0xd646afc6);
    //set_variable("grp_12");
    set_parent(tab_misc);
    set_position(5, 20, 250, 210);
    grp_12 = groupbox(L"");

    set_variable_hash(0x85e527d4);
    //set_variable("chk_misc_admin");
    set_parent(tab_misc);
    set_position(10, 30, 50, 10);
    chk_misc_admin = checkbox(L"admin");
    chk_misc_admin->tooltip = L"this feature might get you banned on some servers";

    set_variable_hash(0xc305eeb9);
    //set_variable("chk_misc_chams");
    set_parent(tab_misc);
    set_position(105, 105, 55, 15);
    chk_misc_chams = checkbox(L"chams");

    set_variable_hash(0x572dbdc8);
    //set_variable("chk_misc_spider");
    set_parent(tab_misc);
    set_position(10, 60, 50, 10);
    chk_misc_spider = checkbox(L"spider");

    set_variable_hash(0x5a0391cc);
    //set_variable("chk_misc_noclip");
    set_parent(tab_misc);
    set_position(10, 75, 50, 10);
    chk_misc_noclip = checkbox(L"noclip");

    set_variable_hash(0xdcba2885);
    //set_variable("chk_misc_cave_bright");
    set_parent(tab_misc);
    set_position(10, 245, 75, 10);
    chk_misc_cave_bright = checkbox(L"cave bright");

    set_variable_hash(0x82d81f3a);
    //set_variable("chk_misc_no_underwater_effect");
    set_parent(tab_misc);
    set_position(105, 90, 95, 10);
    chk_misc_no_underwater_effect = checkbox(L"no water effect");

    set_variable_hash(0x7aa2c391);
    //set_variable("chk_misc_force_sprint");
    set_parent(tab_misc);
    set_position(105, 30, 75, 10);
    chk_misc_force_sprint = checkbox(L"force sprint");

    set_variable_hash(0x9f1eca0e);
    //set_variable("chk_misc_no_head_collision");
    set_parent(tab_misc);
    set_position(105, 45, 100, 10);
    chk_misc_no_head_collision = checkbox(L"no head collision");

    set_variable_hash(0x73b93f08);
    //set_variable("chk_misc_debugcamera_bypass");
    set_parent(tab_misc);
    set_position(105, 60, 120, 10);
    chk_misc_debugcamera_bypass = checkbox(L"debugcamera bypass");

    set_variable_hash(0x1e6010d6);
    //set_variable("chk_misc_fast_update_culling");
    set_parent(tab_misc);
    set_position(105, 75, 100, 10);
    chk_misc_fast_update_culling = checkbox(L"fast update culling");

    set_variable_hash(0x62211c79);
    //set_variable("chk_misc_fov_toggle");
    set_parent(tab_misc);
    set_position(10, 105, 40, 10);
    chk_misc_fov_toggle = checkbox(L"fov");

    set_variable_hash(0x4c0ee375);
    //set_variable("sliderf_misc_fov_value");
    set_parent(tab_misc);
    set_position(55, 105, 50, 10);
    sliderf_misc_fov_value = sliderfloat(60.000000, 200.000000);

    set_variable_hash(0x728ade21);
    //set_variable("chk_misc_time_toggle");
    set_parent(tab_misc);
    set_position(10, 120, 45, 10);
    chk_misc_time_toggle = checkbox(L"time");

    set_variable_hash(0x87933e83);
    //set_variable("sliderf_misc_time_value");
    set_parent(tab_misc);
    set_position(55, 120, 50, 10);
    sliderf_misc_time_value = sliderfloat(0.000000, 24.000000);

    set_variable_hash(0xc54c101b);
    //set_variable("tab_config");
    set_parent(mtabctrl_);
    tab_config = tab(L"configs");

    set_variable_hash(0xd846b2ec);
    //set_variable("grp_14");
    set_parent(tab_config);
    set_position(5, 20, 250, 120);
    grp_14 = groupbox(L"");

    set_variable_hash(0x83656791);
    //set_variable("list_config_cfglist");
    set_parent(tab_config);
    set_position(10, 30, 110, 100);
    list_config_cfglist = listbox();
    list_config_cfglist->m_canselectmultiple = 0;

    set_variable_hash(0xf7fe9813);
    //set_variable("in_config_cfgname");
    set_parent(tab_config);
    set_position(130, 35, 115, 15);
    in_config_cfgname = textbox();
    in_config_cfgname->m_hint = "";
    in_config_cfgname->m_hidetext = 0;

    set_variable_hash(0x3800736b);
    //set_variable("btn_config_load");
    set_parent(tab_config);
    set_position(135, 55, 105, 15);
    btn_config_load = button(L"load");

    set_variable_hash(0x62632006);
    //set_variable("btn_config_save");
    set_parent(tab_config);
    set_position(135, 75, 105, 15);
    btn_config_save = button(L"save");

    set_variable_hash(0xff53493e);
    //set_variable("tab_menu");
    set_parent(mtabctrl_);
    tab_menu = tab(L"menu");

    set_variable_hash(0xd946b47f);
    //set_variable("grp_15");
    set_parent(tab_menu);
    set_position(5, 25, 250, 30);
    grp_15 = groupbox(L"colors");

    set_variable_hash(0x4bc9011d);
    //set_variable("clr_menu_accent");
    set_parent(tab_menu);
    set_position(15, 35, 25, 10);
    clr_menu_accent = colorpicker();
    clr_menu_accent->m_color = color_t{ 133,16,196 };

    set_variable_hash(0x1c0d0d2f);
    //set_variable("clr_menu_gradient_a");
    set_parent(tab_menu);
    set_position(45, 35, 25, 10);
    clr_menu_gradient_a = colorpicker();
    clr_menu_gradient_a->m_color = color_t{ 255,255,255 };

    set_variable_hash(0x1d0d0ec2);
    //set_variable("clr_menu_gradient_b");
    set_parent(tab_menu);
    set_position(75, 35, 25, 10);
    clr_menu_gradient_b = colorpicker();
    clr_menu_gradient_b->m_color = color_t{ 0,0,0 };

    set_variable_hash(0xe774f953);
    //set_variable("chk_radar_pickables_stone");
    set_parent(tab_radar);
    set_position(140, 125, 50, 10);
    chk_radar_pickables_stone = checkbox(L"stone");

    set_variable_hash(0xde9a80c3);
    //set_variable("key_misc_noclip_key");
    set_parent(tab_misc);
    set_position(65, 75, 25, 10);
    key_misc_noclip_key = keybind();
    key_misc_noclip_key->set(0);

    set_variable_hash(0x7f3d3cb7);
    //set_variable("key_misc_no_head_collision_key");
    set_parent(tab_misc);
    set_position(210, 45, 25, 10);
    key_misc_no_head_collision_key = keybind();
    key_misc_no_head_collision_key->set(0);

    set_variable_hash(0xb3795c00);
    //set_variable("chk_misc_moon");
    set_parent(tab_misc);
    set_position(10, 45, 65, 10);
    chk_misc_moon = checkbox(L"big moon");

    set_variable_hash(0x3ef480d6);
    //set_variable("btn_radar_fetch_map");
    set_parent(tab_radar);
    set_position(185, 280, 65, 15);
    btn_radar_fetch_map = button(L"fetch map");

    set_variable_hash(0x7a17ec43);
    //set_variable("chk_radar_tex_toggle");
    set_parent(tab_radar);
    set_position(130, 280, 50, 15);
    chk_radar_tex_toggle = checkbox(L"toggle");

    set_variable_hash(0x32a2bfeb);
    //set_variable("chk_misc_big_stars");
    set_parent(tab_misc);
    set_position(10, 90, 60, 10);
    chk_misc_big_stars = checkbox(L"big stars");

    set_variable_hash(0x374eb1fe);
    //set_variable("chk_misc_nightmode");
    set_parent(tab_misc);
    set_position(95, 245, 75, 10);
    chk_misc_nightmode = checkbox(L"nightmode");
    chk_misc_nightmode->tooltip = L"toggle at daytime";

    set_variable_hash(0xda46b612);
    //set_variable("grp_16");
    set_parent(tab_misc);
    set_position(5, 235, 250, 30);
    grp_16 = groupbox(L"ambient");

    set_variable_hash(0x91f4dd57);
    //set_variable("btn_misc_refresh_ambient");
    set_parent(tab_misc);
    set_position(175, 240, 75, 20);
    btn_misc_refresh_ambient = button(L"apply change");

    set_variable_hash(0xdb46b7a5);
    //set_variable("grp_17");
    set_parent(tab_misc);
    set_position(5, 270, 250, 30);
    grp_17 = groupbox(L"side peek");

    set_variable_hash(0x13553638);
    //set_variable("chk_misc_side_peek");
    set_parent(tab_misc);
    set_position(10, 280, 50, 10);
    chk_misc_side_peek = checkbox(L"toggle");

    set_variable_hash(0x4a4f61a7);
    //set_variable("key_misc_side_peek_left");
    set_parent(tab_misc);
    set_position(65, 275, 25, 20);
    key_misc_side_peek_left = keybind();
    key_misc_side_peek_left->set(0);

    set_variable_hash(0xf7bc3858);
    //set_variable("key_misc_side_peek_right");
    set_parent(tab_misc);
    set_position(125, 275, 25, 20);
    key_misc_side_peek_right = keybind();
    key_misc_side_peek_right->set(0);

    set_variable_hash(0xe2842ac7);
    //set_variable("key_misc_side_peek_forward");
    set_parent(tab_misc);
    set_position(95, 275, 25, 20);
    key_misc_side_peek_forward = keybind();
    key_misc_side_peek_forward->set(0);

    set_variable_hash(0xd96f8f56);
    //set_variable("combo_misc_chams_selection");
    set_parent(tab_misc);
    set_position(160, 105, 85, 15);
    combo_misc_chams_selection = combobox();

    set_variable_hash(0x66df47bb);
    //set_variable("chk_misc_culling");
    set_parent(tab_misc);
    set_position(10, 150, 55, 10);
    chk_misc_culling = checkbox(L"admin esp");

    set_variable_hash(0x246c43de);
    //set_variable("chk_misc_noclip_flyhack_kick");
    set_parent(tab_misc);
    set_position(10, 135, 100, 10);
    chk_misc_noclip_flyhack_kick = checkbox(L"no flyhack kick");

    set_variable_hash(0x8ca7b29);
    //set_variable("chk_aim_sticky");
    set_parent(tab_aimbot);
    set_position(10, 210, 50, 10);
    chk_aim_sticky = checkbox(L"sticky");

    for (const auto& ctrl : vars::callstack) ctrl.control->think();
}