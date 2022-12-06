#pragma once
#include "radar.h"

namespace misc_radar
{
	struct miscrender
	{
		uint64_t visualstate = 0;
		uint32_t modelhash = 0;
	};

	std::vector<miscrender> buffer1(250);
	std::vector<miscrender> buffer2(250);

	auto* renderBuffer = &buffer1;
	auto* fillBuffer = &buffer2;


	bool bRenderisFirst = 1;
	bool bShouldswapBuffers = 0;

	void swapBuffers()
	{
		if (bRenderisFirst)
		{
			renderBuffer = &buffer2;
			fillBuffer = &buffer1;
			bRenderisFirst = 0;
		}
		else
		{
			renderBuffer = &buffer1;
			fillBuffer = &buffer2;
			bRenderisFirst = 1;
		}
	}

	void render()
	{
		if (!globals::objects::localPlayer.input) return;

		for (int i = 0; i < renderBuffer->size(); ++i)
		{
			if (!renderBuffer->at(i).visualstate) continue;

			math::vector3 orePos = driver::rpm<math::vector3>(renderBuffer->at(i).visualstate + 0x90);
			math::vector3 lpos = player::getBasePosition(globals::objects::localPlayer);

			DWORD color = 0;
			const char* name = "";

			switch (renderBuffer->at(i).modelhash)
			{

			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/stone/sulfur-collectable.prefab"):
				color = darkorange(255);
				name = xorstr("sulfur pickup");
				break;
			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/stone/stone-collectable.prefab"):
				color = lightgrey(255);
				name = xorstr("stone pickup");
				break;
			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/wood/wood-collectable.prefab"):
				color = brown(255);
				name = xorstr("wood pickup");
				break;
			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/hemp/hemp-collectable.prefab"):
				color = lawngreen(255);
				name = xorstr("hemp pickup");
				break;

			case fnv::hash("assets/prefabs/plants/hemp/hemp.entity.prefab"):
				color = lawngreen(255);
				name = xorstr("hemp pickup");
				break;

			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/stone/metal-collectable.prefab"):
				color = gray(255);
				name = xorstr("metal pickup");
				break;




			case fnv::hash("assets/prefabs/food/bota bag/bota_bag.entity.prefab"):
				color = green(255);
				name = xorstr("bota bag");
				break;

			case fnv::hash("assets/bundled/prefabs/radtown/dmloot/dm food.prefab"):
				color = green(255);
				name = xorstr("dm food");
				break;

			case fnv::hash("assets/bundled/prefabs/radtown/foodbox.prefab"):
				color = green(255);
				name = xorstr("food box");
				break;

			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/mushrooms/mushroom-cluster-5.prefab"):
				color = green(255);
				name = xorstr("mushrooms");
				break;

			case fnv::hash("assets/prefabs/food/water jug/waterjug.entity.prefabassets/bundled/prefabs/autospawn/collectable/mushrooms/mushroom-cluster-6.prefab"):
				color = green(255);
				name = xorstr("mushrooms");
				break;

			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/pumpkin/pumpkin-collectable.prefab"):
				color = green(255);
				name = xorstr("pumpkin");
				break;

			case fnv::hash("assets/prefabs/plants/pumpkin/pumpkin.entity.prefab"):
				color = green(255);
				name = xorstr("pumpkin");
				break;

			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/corn/corn-collectable.prefab"):
				color = green(255);
				name = xorstr("corn");
				break;
			case fnv::hash("assets/prefabs/plants/corn/corn.entity.prefab"):
				color = green(255);
				name = xorstr("corn");
				break;			
			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-black/berry-black-collectable.prefab"):
					color = green(255);
					name = xorstr("berry");
					break;	
			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-blue/berry-blue-collectable.prefab"):
						color = green(255);
						name = xorstr("berry");
						break;
			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-green/berry-green-collectable.prefab"):
				color = green(255);
				name = xorstr("berry");
				break;
			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-red/berry-red-collectable.prefab"):
				color = green(255);
				name = xorstr("berry");
				break;
			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-white/berry-white-collectable.prefabb"):
				color = green(255);
				name = xorstr("berry");
				break;
			case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-yellow/berry-yellow-collectable.prefab"):
				color = green(255);
				name = xorstr("berry");
				break;



			case fnv::hash("assets/prefabs/deployable/single shot trap/guntrap.deployed.prefab"):
				color = red(255);
				name = xorstr("guntrap");
				break;
			case fnv::hash("assets/prefabs/deployable/survivalfishtrap/survivalfishtrap.deployed.prefab"):
				color = red(255);
				name = xorstr("fishtrap");
				break;
			case fnv::hash("assets/prefabs/deployable/bear trap/beartrap.prefab"):
				color = red(255);
				name = xorstr("beartrap");
				break;
			case fnv::hash("assets/prefabs/npc/autoturret/autoturret_deployed.prefab"):
				color = red(255);
				name = xorstr("auto turret");
				break;
			case fnv::hash("assets/prefabs/npc/sam_site_turret/sam_site_turret_deployed.prefab"):
				color = red(255);
				name = xorstr("sam turret");
				break;
			case fnv::hash("assets/prefabs/npc/flame turret/flameturret.deployed.prefab"):
				color = red(255);
				name = xorstr("flame turret");
				break;

			case fnv::hash("assets/prefabs/npc/patrol helicopter/patrolhelicopter.prefab"):
				color = yellow(255);
				name = xorstr("patrol helicopter");
				break;


			case fnv::hash("assets/prefabs/npc/m2bradley/bradleyapc.prefab"):
				color = yellow(255);
				name = xorstr("bradley");
				break;



			case fnv::hash("assets/content/vehicles/modularcar/4module_car_spawned.entity.prefab"):
				color = blue(255);
				name = xorstr("4 module car");
				break;

			case fnv::hash("assets/content/vehicles/modularcar/3module_car_spawned.entity.prefab"):
				color = blue(255);
				name = xorstr("3 module car");
				break;


			case fnv::hash("assets/content/vehicles/modularcar/2module_car_spawned.entity.prefab"):
				color = blue(255);
				name = xorstr("2 module car");
				break;


			case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores/metal-ore.prefab"):
				color = gray(255);
				name = xorstr("metal node");
				break;
			case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_sand/metal-ore.prefab"):
				color = gray(255);
				name = xorstr("metal node");
				break;

			case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_snow/metal-ore.prefab"):
				color = gray(255);
				name = xorstr("metal node");
				break;


			case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores/sulfur-ore.prefab"):
				color = orange(255);
				name = xorstr("sulfur node");
				break;
			case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_sand/sulfur-ore.prefab"):
				color = orange(255);
				name = xorstr("sulfur node");
				break;
			case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_snow/sulfur-ore.prefab"):
				color = orange(255);
				name = xorstr("sulfur node");
				break;


			case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores/stone-ore.prefab"):
				color = lightgrey(255);
				name = xorstr("stone node");
				break;
			case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_sand/stone-ore.prefab"):
				color = lightgrey(255);
				name = xorstr("stone node");
				break;

			case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_snow/stone-ore.prefab"):
				color = lightgrey(255);
				name = xorstr("stone node");
				break;


				//plr bodies
			case fnv::hash("assets/prefabs/player/player_corpse.prefab"):
				color = yellow(255);
				name = xorstr("corpse");
				break;
			case fnv::hash("assets/prefabs/misc/item drop/item_drop_backpack.prefab"):
				color = yellow(255);
				name = xorstr("backpack");
				break;

				//loot
			case fnv::hash("assets/bundled/prefabs/radtown/crate_elite.prefab"):
				color = purple(255);
				name = xorstr("elite crate");
				break;

			case fnv::hash("assets/bundled/prefabs/radtown/crate_normal_2.prefab"):
				color = lawngreen(255);
				name = xorstr("crate");
				break;

			case fnv::hash("assets/bundled/prefabs/radtown/crate_normal.prefab"):
				color = orange(255);
				name = xorstr("military crate");
				break;

			case fnv::hash("assets/bundled/prefabs/radtown/crate_tools.prefab"):
				color = red(255);
				name = xorstr("tool crate");
				break;

			case fnv::hash("assets/prefabs/npc/patrol helicopter/heli_crate.prefab"):
				color = purple(255);
				name = xorstr("heli crate");
				break;
			case fnv::hash("assets/prefabs/npc/m2bradley/bradley_crate.prefab"):
				color = purple(255);
				name = xorstr("bradley crate");
				break;

			case fnv::hash("assets/prefabs/deployable/chinooklockedcrate/codelockedhackablecrate.prefab"):
				color = purple(255);
				name = xorstr("hackable crate");
				break;

			case fnv::hash("assets/prefabs/misc/supply drop/supply_drop.prefab"):
				color = purple(255);
				name = xorstr("supply drop");
				break;

				//transport
			case fnv::hash("assets/content/vehicles/minicopter/minicopter.entity.prefab"):
				color = cyan(255);
				name = xorstr("minicopter");
				break;

			case fnv::hash("assets/content/vehicles/scrap heli carrier/scraptransporthelicopter.prefab"):
				color = cyan(255);
				name = xorstr("scrap heli");
				break;
			case fnv::hash("assets/rust.ai/nextai/testridablehorse.prefab"):
				color = cyan(255);
				name = xorstr("horse");
				break;

			case fnv::hash("assets/content/vehicles/boats/rhib/rhib.prefab"):
				color = cyan(255);
				name = xorstr("rhib");
				break;

			case fnv::hash("assets/content/vehicles/boats/rowboat/rowboat.prefab"):
				color = cyan(255);
				name = xorstr("rowboat");
				break;

				//stash
			case fnv::hash("assets/prefabs/deployable/small stash/small_stash_deployed.prefab"):
				color = yellow(255);
				name = xorstr("stash");
				break;


				//animals
			case fnv::hash("assets/rust.ai/agents/bear/bear.prefab"):
				color = darkorange(255);
				name = xorstr("bear");
				break;

			case fnv::hash("assets/rust.ai/agents/bear/polarbear.prefab"):
				color = darkorange(255);
				name = xorstr("polar bear");
				break;

			case fnv::hash("assets/rust.ai/agents/boar/boar.prefab"):
				color = darkorange(255);
				name = xorstr("boar");
				break;

			case fnv::hash("assets/rust.ai/agents/horse/horse.prefab"):
				color = darkorange(255);
				name = xorstr("horse");
				break;

			case fnv::hash("assets/rust.ai/agents/stag/stag.prefab"):
				color = darkorange(255);
				name = xorstr("stag");
				break;

			case fnv::hash("assets/rust.ai/agents/wolf/wolf.prefab"):
				color = darkorange(255);
				name = xorstr("wolf");
				break;

			default:
				continue;
			}




			//body angles y
			float yaw = driver::rpm<float>(globals::objects::localPlayer.input + 0x3C + 4);
			yaw += 180.f;
			yaw = (yaw * (float)PI / 180.0f);


			math::vector3 t = orePos - lpos;
			t *= config::radar::zoom;

			math::vector3 diff = { t.x,t.z,t.y };
			math::vector3 out = radar::rotatePoint(diff, yaw);

			float rmx = radar::pos.x + radar::size.x / 2.f;
			float rmy = radar::pos.y + radar::size.y / 2.f;

			out.x += rmx;
			out.y += rmy;

			//clamp
			out.x = std::clamp(out.x, radar::pos.x, radar::pos.y + radar::size.x);
			out.y = std::clamp(out.y, radar::pos.y, radar::pos.y + radar::size.y);

			//drawing part
			float dotsize = config::radar::dotSize;

			//save old rect
			RECT old = d::get_clip();

			//our custom clip rectangle
			RECT newrect = { radar::pos.x + 1,radar::pos.y + 1,radar::pos.x + radar::size.x - 1,radar::pos.y + radar::size.y - 1 };

			//override
			d::set_clip(newrect);
			d::antialias(true);
			d::circle(out.x, out.y, dotsize, 100, dx2color(color), 255, 1);
			d::circle(out.x, out.y, dotsize, 100, { 0,0,0 }, 255);
			d::antialias(false);

			d::set_font(f::tahoma);
			d::set_text_outline(false);
			d::set_text_vertical_centering(false);

			d::string_a(out.x, out.y + dotsize + 1, DT_CENTER, dx2color(color), 255, xorstr("%s"), name);

			d::set_clip(old);

		}
		if (bShouldswapBuffers)
		{
			for (int i = 0; i < renderBuffer->size(); ++i)
			{
				renderBuffer->at(i).visualstate = 0;
				renderBuffer->at(i).modelhash = 0;
			}
			swapBuffers();
			bShouldswapBuffers = 0;
		}

	}

	void setup()
	{
		if (!globals::classes::baseEntity) return;

		uintptr_t ptr = driver::rpm<uint64_t>(globals::classes::baseEntity + 0x10);
		if (!ptr) return;

		ptr = driver::rpm<uint64_t>(ptr + 0x10);
		if (!ptr) return;

		ptr = driver::rpm<uint64_t>(ptr + 0x28);
		if (!ptr) return;

		while (1)
		{

			uint64_t entityRealm = driver::rpm<uint64_t>(ptr + 0x18);
			if (!entityRealm) continue;

			uint32_t count = driver::rpm<int>(ptr + 0x10);
			if (!count) continue;


			int g = 0;
			for (int n = 0; n < count; ++n)
			{
				uint64_t element = driver::rpm<uint64_t>(entityRealm + 0x20 + (n * 0x8));
				if (!element) continue;
				uint64_t baseObject = driver::rpm<uint64_t>(element + 0x10);
				if (!baseObject) continue;
				uint64_t gameObject = driver::rpm<uint64_t>(baseObject + 0x30);
				if (!gameObject) continue;

				structures::str_2 objName = driver::rpm<structures::str_2>(driver::rpm<uint64_t>(gameObject + 0x60));


				uint32_t hashedname = fnv::hash_runtime(objName.buffer);

				switch (hashedname)
				{
					//pickables
				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/stone/sulfur-collectable.prefab"):
					if (config::radar::misc::psulfur) break;
					continue;

				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/stone/stone-collectable.prefab"):
					if (config::radar::misc::pstone) break;
					continue;

				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/wood/wood-collectable.prefab"):
					if (config::radar::misc::pwood) break;
					continue;

				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/hemp/hemp-collectable.prefab"):
					if (config::radar::misc::phemp) break;
					continue;

				case fnv::hash("assets/prefabs/plants/hemp/hemp.entity.prefab"):
					if (config::radar::misc::notfullygrownhemp) break;
					continue;

				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/stone/metal-collectable.prefab"):
					if (config::radar::misc::pmetal) break;
					continue;


					//food
				case fnv::hash("assets/prefabs/food/bota bag/bota_bag.entity.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/radtown/dmloot/dm food.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/radtown/foodbox.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/mushrooms/mushroom-cluster-5.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/prefabs/food/water jug/waterjug.entity.prefabassets/bundled/prefabs/autospawn/collectable/mushrooms/mushroom-cluster-6.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/pumpkin/pumpkin-collectable.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/prefabs/plants/pumpkin/pumpkin.entity.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/corn/corn-collectable.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/prefabs/plants/corn/corn.entity.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-black/berry-black-collectable.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-blue/berry-blue-collectable.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-green/berry-green-collectable.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-red/berry-red-collectable.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-white/berry-white-collectable.prefab"):
					if (config::radar::misc::food) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/collectable/berry-yellow/berry-yellow-collectable.prefab"):
					if (config::radar::misc::food) break;
					continue;


					//traps
				case fnv::hash("assets/prefabs/deployable/single shot trap/guntrap.deployed.prefab"):
					if (config::radar::misc::traps) break;
					continue;
				case fnv::hash("assets/prefabs/deployable/survivalfishtrap/survivalfishtrap.deployed.prefab"):
					if (config::radar::misc::traps) break;
					continue;
				case fnv::hash("assets/prefabs/deployable/bear trap/beartrap.prefab"):
					if (config::radar::misc::traps) break;
					continue;
				case fnv::hash("assets/prefabs/npc/autoturret/autoturret_deployed.prefab"):
					if (config::radar::misc::traps) break;
					continue;
				case fnv::hash("assets/prefabs/npc/sam_site_turret/sam_site_turret_deployed.prefab"):
					if (config::radar::misc::traps) break;
					continue;
				case fnv::hash("assets/prefabs/npc/flame turret/flameturret.deployed.prefab"):
					if (config::radar::misc::traps) break;
					continue;

					//patrol heli

				case fnv::hash("assets/prefabs/npc/patrol helicopter/patrolhelicopter.prefab"):
					if (config::radar::misc::patrolheli) break;
					continue;

					//Bradley
				case fnv::hash("assets/prefabs/npc/m2bradley/bradleyapc.prefab"):
					if (config::radar::misc::bradley) break;
					continue;

					//nodes
				case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores/metal-ore.prefab"):
					if (config::radar::misc::metal) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_sand/metal-ore.prefab"):
					if (config::radar::misc::metal) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_snow/metal-ore.prefab"):
					if (config::radar::misc::metal) break;
					continue;


				case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores/sulfur-ore.prefab"):
					if (config::radar::misc::sulfur) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_sand/sulfur-ore.prefab"):
					if (config::radar::misc::sulfur) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_snow/sulfur-ore.prefab"):
					if (config::radar::misc::sulfur) break;
					continue;


				case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores/stone-ore.prefab"):
					if (config::radar::misc::stone) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_sand/stone-ore.prefab"):
					if (config::radar::misc::stone) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/autospawn/resource/ores_snow/stone-ore.prefab"):
					if (config::radar::misc::stone) break;
					continue;

					//player bodies
				case fnv::hash("assets/prefabs/player/player_corpse.prefab"):
					if (config::radar::misc::playerbodies) break;
					continue;
				case fnv::hash("assets/prefabs/misc/item drop/item_drop_backpack.prefab"):
					if (config::radar::misc::playerbodies) break;
					continue;

					//loot
				case fnv::hash("assets/bundled/prefabs/radtown/crate_elite.prefab"):
					if (config::radar::misc::loot) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/radtown/crate_normal_2.prefab"):
					if (config::radar::misc::loot) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/radtown/crate_normal.prefab"):
					if (config::radar::misc::loot) break;
					continue;
				case fnv::hash("assets/bundled/prefabs/radtown/crate_tools.prefab"):
					if (config::radar::misc::loot) break;
					continue;
				case fnv::hash("assets/prefabs/npc/patrol helicopter/heli_crate.prefab"):
					if (config::radar::misc::loot) break;
					continue;
				case fnv::hash("assets/prefabs/npc/m2bradley/bradley_crate.prefab"):
					if (config::radar::misc::loot) break;
					continue;
				case fnv::hash("assets/prefabs/deployable/chinooklockedcrate/codelockedhackablecrate.prefab"):
					if (config::radar::misc::loot) break;
					continue;
				case fnv::hash("assets/prefabs/misc/supply drop/supply_drop.prefab"):
					if (config::radar::misc::loot) break;
					continue;

					//transport
				case fnv::hash("assets/content/vehicles/minicopter/minicopter.entity.prefab"):
					if (config::radar::misc::heli) break;
					continue;
				case fnv::hash("assets/content/vehicles/scrap heli carrier/scraptransporthelicopter.prefab"):
					if (config::radar::misc::heli) break;
					continue;
				case fnv::hash("assets/rust.ai/nextai/testridablehorse.prefab"):
					if (config::radar::misc::horse) break;
					continue;
				case fnv::hash("assets/content/vehicles/boats/rhib/rhib.prefab"):
					if (config::radar::misc::boat) break;
					continue;
				case fnv::hash("assets/content/vehicles/boats/rowboat/rowboat.prefab"):
					if (config::radar::misc::boat) break;
					continue;


				case fnv::hash("assets/content/vehicles/modularcar/4module_car_spawned.entity.prefab"):
					if (config::radar::misc::cars) break;
					continue;

				case fnv::hash("assets/content/vehicles/modularcar/3module_car_spawned.entity.prefab"):
					if (config::radar::misc::cars) break;
					continue;


				case fnv::hash("assets/content/vehicles/modularcar/2module_car_spawned.entity.prefab"):
					if (config::radar::misc::cars) break;
					continue;

					//stash
				case fnv::hash("assets/prefabs/deployable/small stash/small_stash_deployed.prefab"):
					if (config::radar::misc::stash) break;
					continue;

					//animals
				case fnv::hash("assets/rust.ai/agents/bear/bear.prefab"):
					if (config::radar::misc::animals) break;
					continue;
				case fnv::hash("assets/rust.ai/agents/bear/polarbear.prefab"):
					if (config::radar::misc::animals) break;
					continue;
				case fnv::hash("assets/rust.ai/agents/boar/boar.prefab"):
					if (config::radar::misc::animals) break;
					continue;
				case fnv::hash("assets/rust.ai/agents/horse/horse.prefab"):
					if (config::radar::misc::animals) break;
					continue;
				case fnv::hash("assets/rust.ai/agents/stag/stag.prefab"):
					if (config::radar::misc::animals) break;
					continue;
				case fnv::hash("assets/rust.ai/agents/wolf/wolf.prefab"):
					if (config::radar::misc::animals) break;
					continue;


				default:
					continue;
				}

				uint64_t objectClass = driver::rpm<uint64_t>(gameObject + 0x30);
				if (!objectClass) continue;
				uint64_t Transform = driver::rpm<uint64_t>(objectClass + 0x8);
				if (!Transform) continue;
				uint64_t VisualState = driver::rpm<uint64_t>(Transform + 0x38);
				if (!VisualState) continue;

				if (g > fillBuffer->size() - 1) continue;
				fillBuffer->at(g).visualstate = VisualState;
				fillBuffer->at(g).modelhash = hashedname;
				g++;
			}
			bShouldswapBuffers = 1;
			Sleep(2000);

		}

	}
}


