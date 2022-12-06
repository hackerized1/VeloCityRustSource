#pragma once

namespace config
{
	namespace radar
	{
		float dotSize = 4.f;
		float zoom = 3.f;

		bool showSleeper = false;
		bool showNPC = 1;

		namespace misc
		{
			bool playerbodies = false;
			bool loot = false;
			bool heli = false;
			bool horse = false;
			bool stash = false;
			bool boat = false;
			bool food = false;
			bool patrolheli = false;
			bool traps = false;
			bool animals = false;
			bool bradley = false;
			bool cars = false;



			bool psulfur = false;
			bool pmetal = false;
			bool pwood = false;
			bool pstone = false;

			bool phemp = false;
			bool notfullygrownhemp = false;

			bool sulfur = false;
			bool metal = false;
			bool stone = false;

		}

	}


	namespace weapon
	{
		bool noSway = false;
		bool noSpread = false;
		bool recoilControl = 0;
		float recoilPercent = 60.f;

		bool perfectRecoil = 0;
		float perfectRecoilMax = 4.f;

		bool instantEoka = false;

		bool rapidFire = false;

		bool instantBow = false;
		bool instantCompoundBow = false;

		namespace melee
		{
			bool longHand = false;
			bool fatHand = false;
			bool attackWhileSprint = false;
		}
	}

	namespace misc
	{
		bool admin = false;
		bool spider = false;
		bool noclip = false;
		bool chams = false;
		bool alwaysDay = false;
		bool noUnderwaterEffect = false;
		bool forceSprint = false;
		bool noHeadCollision = false;
		bool debugCameraBypass = false;
		bool changeFOV = false;
		float fieldOfView = 90.f;
		bool fastUpdateCulling = false;
		bool boostVoicechatVolume = false;
		bool caveBright = false;
		bool noFog = false;
		float voicechatVolume = 190.f;
	}

	namespace aimbot
	{
		bool state = 0;
		bool visCheck = false;
		bool aimAtNPC = false;
		bool aimAtDowned = false;
		bool teamCheck = false;

		bool bulletPrediction = false;

		bool smoothing = false;
		bool silentaim = false;
		float smoothingValue = 5.f;

		bool rcs = true;
		float rcsAmount = 60.f;

		float fieldOfView = 200.f;
		float maxDistance = 200.f;
		int bone = 47;
	}
}