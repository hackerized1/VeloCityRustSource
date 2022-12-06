#pragma once
#include "player.h"
#include "camera.h"
#include "item.h"
#include "weapon.h"
#include "driver/include.h"

namespace aimbot
{
	structures::BasePlayer_t best = { 0 };
	float min = config::aimbot::fieldOfView;
	bool got_target = false;

	void clearTarget()
	{
		min = config::aimbot::fieldOfView;

		if (ui::chk_aim_sticky->get())
		{
			auto e = playerlist::find(best.address);
			if (e.address != 0 && e._health > 0.f)
			{
				best = e;
			}
			else {
				best = { 0 };
				got_target = false;
			}
		}
		else {
			best = { 0 };
			got_target = false;
		}
	}
	
	bool getTarget(const structures::BasePlayer_t& entity)
	{
		if (got_target) return true;

		//std::cout << "target \n" << std::endl;
		math::vector3 headPos = player::getBasePosition(entity);
		headPos.y += 1.45f;

		if (player::checkFlag(entity, FL_SLEEPING)) return 0;

		if (ui::chk_aim_max_dist_toggle->get())
		{
			if (headPos.distance(camera::get_pos()) > config::aimbot::maxDistance) return 0;
		}

		if (entity._maxHealth > 100.f && !config::aimbot::aimAtNPC)  return 0;

		if (player::checkFlag(entity, FL_WOUNDED) && !config::aimbot::aimAtDowned)  return 0;

		if (player::isInSameTeam(globals::objects::localPlayer, entity) && config::aimbot::teamCheck)  return 0;

		if (!entity.playerModel)  return 0;
		structures::PlayerModel_t playerModel = driver::rpm<structures::PlayerModel_t>(entity.playerModel);
		if (config::aimbot::visCheck && !playerModel.visible)  return 0;

		math::vector2 sHeadPos;
		if (camera::w2s(headPos, &sHeadPos))
		{
			float dist = sqrt(pow(globals::gameSize.x / 2 - sHeadPos.x, 2) + pow(globals::gameSize.y / 2 - sHeadPos.y, 2));
			if (dist < min)
			{
				min = dist;
				best = entity;

				return true;
			}
		}

		return false;
	}

	void run()
	{
		//std::cout << "aimbot \n" << std::endl;
		if (!globals::objects::localActiveWep.uid) return;

		math::vector3 localhead = camera::get_pos();

		math::vector3 targethead = player::getBonePosition(best, config::aimbot::bone);
		if (targethead.empty()) return;

		float dist = localhead.distance(targethead);

		if (config::aimbot::bulletPrediction)
		{
			if (dist > 0.001f)

			if (!best.playerModel) return;
			structures::PlayerModel_t playerModel = driver::rpm<structures::PlayerModel_t>(best.playerModel);

			structures::str activeWepShort = item::getShortname(globals::objects::localActiveWep);
			if (!activeWepShort.buffer) return;

			uint32_t activeWepHash = fnv::hash_runtime(sdk::getString(activeWepShort).c_str());
			if (!activeWepHash) return;

			float travelTimeEst = dist / (weapon::getBulletDrag() * (weapon::getBulletVelocity(activeWepHash, dist) * weapon::getAttachmentMultiplier()));

			math::vector3 predictedVel = playerModel.newVelocity * travelTimeEst;
			targethead = targethead + predictedVel;

			targethead.y += ((9.81f / 2) * travelTimeEst * travelTimeEst);
		}

		math::vector3 angle = sdk::calculateAngle(localhead, targethead);

		if (config::aimbot::smoothing && !config::aimbot::silentaim)
		{
			math::vector3 old = driver::rpm<math::vector3>(globals::objects::localPlayer.input + 0x3C);
			math::vector3 delta{ old.x - angle.x , old.y - angle.y ,0 };

			sdk::clampAngles(delta);
			angle.x = old.x - delta.x / (5.f * config::aimbot::smoothingValue);
			angle.y = old.y - delta.y / (5.f * config::aimbot::smoothingValue);
		}

		sdk::clampAngles(angle);


		if (config::aimbot::silentaim)
		{
			//driver::wpm<math::vector4>(sdk::QuaternionLookRotation(targethead - localhead, math::vector3(0,1,0)), globals::objects::localPlayer.eyes + 0x44);
		}
		else
		{
			driver::wpm<math::vector3>(angle, globals::objects::localPlayer.input + 0x3C);
		}

	}
}