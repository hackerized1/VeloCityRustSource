#pragma once
#include "defs.h"
#include "math.h"


namespace structures
{
	using UnityEngine_Vector3_o = math::vector3;
	using UnityEngine_Vector4_o = math::vector4;
	using UnityEngine_Quaternion_o = math::vector4;


	struct str
	{
		char pad[0x10];
		uint32_t size;
		char buffer[128];
	};

	struct str_w
	{
		char pad[0x10];
		uint32_t size;
		wchar_t buffer[128];
	};

	struct str_2
	{
		char buffer[128];
	};



	struct OcclusionCulling_Sphere_Fields {
		math::vector3 position;
		float radius;
	};

	struct OcclusionCulling_Sphere_o {
		OcclusionCulling_Sphere_Fields fields;
	};

	struct OccludeeSphere_Fields {
		int32_t id;
		struct OccludeeState_o* state;
		struct OcclusionCulling_Sphere_o sphere;
	};
	struct OccludeeSphere_o {
		OccludeeSphere_Fields fields;
	};
	struct UnityEngine_Bounds_Fields {
		math::vector3 m_Center;
		math::vector3 m_Extents;
	};

	struct UnityEngine_Bounds_o {
		UnityEngine_Bounds_Fields fields;
	};


	struct BaseCombatEntity_Pickup_Fields {
		bool enabled;
		struct ItemDefinition_o* itemTarget;
		int32_t itemCount;
		bool setConditionFromHealth;
		float subtractCondition;
		bool requireBuildingPrivilege;
		bool requireHammer;
		bool requireEmptyInv;
	};

	struct BaseCombatEntity_Pickup_o {
		BaseCombatEntity_Pickup_Fields fields;
	};
	struct BaseCombatEntity_Repair_Fields {
		bool enabled;
		struct ItemDefinition_o* itemTarget;
		struct GameObjectRef_o* repairEffect;
		struct GameObjectRef_o* repairFullEffect;
		struct GameObjectRef_o* repairFailedEffect;
	};

	struct BaseCombatEntity_Repair_o {
		BaseCombatEntity_Repair_Fields fields;
	};



	struct System_Nullable_bool__Fields {
		bool value;
		bool has_value;
	};

	struct System_Nullable_bool__o {
		System_Nullable_bool__Fields fields;
	};

	struct System_Nullable_float__Fields {
		float value;
		bool has_value;
	};

	struct System_Nullable_float__o {
		System_Nullable_float__Fields fields;
	};


	struct TimeUntil_Fields {
		float time;
	};
	struct TimeUntil_o {
		TimeUntil_Fields fields;
	};
	struct RealTimeSince_Fields {
		float time;
	};
	struct TimeSince_Fields {
		float time;
	};
	struct TimeSince_o {
		TimeSince_Fields fields;
	};

	struct RealTimeSince_o {
		RealTimeSince_Fields fields;
	};

	struct EntityRef_Fields {
		uintptr_t ent_cached;
		uint32_t id_cached;
	};
	struct EntityRef_o {
		EntityRef_Fields fields;
	};

	struct BasePlayer_CapsuleColliderInfo_Fields {
		float height;
		float radius;
		struct math::vector3 center;
	};

	struct BasePlayer_CapsuleColliderInfo_o {
		BasePlayer_CapsuleColliderInfo_Fields fields;
	};

	struct GameMenu_Option_Fields {
		uintptr_t  title;
		uintptr_t  desc;
		uintptr_t  requirements;
		uintptr_t  icon;
		bool showOnItem;
		int32_t order;
		float time;
		bool usableWhileWounded;
		bool longUseOnly;
		bool showDisabled;
		bool show;
		uintptr_t function;
		uintptr_t timeStart;
		uintptr_t timeProgress;
		uintptr_t iconSprite;
		uintptr_t  command;
	};
	struct GameMenu_Option_o {
		GameMenu_Option_Fields fields;
	};


	struct BaseNetworkable_Fields {
		char bumper[0x18];

		bool _JustCreated_k__BackingField;
		struct DeferredAction_o* entityDestroy;
		struct System_Collections_Generic_List_Component__o* postNetworkUpdateComponents;
		struct EntityRef_o parentEntity;
		struct System_Collections_Generic_List_BaseEntity__o* children;
		uint32_t prefabID;
		bool globalBroadcast;
		struct Network_Networkable_o* net;
		bool _IsDestroyed_k__BackingField;
		uintptr_t  _prefabName;
		uintptr_t  _prefabNameWithoutExtension;
	};



	struct BaseEntity_t : BaseNetworkable_Fields {
		struct Ragdoll_o* ragdoll;
		struct PositionLerp_o* positionLerp;
		struct System_Collections_Generic_List_Option__o* menuOptions;
		uint32_t broadcastProtocol;
		struct System_Collections_Generic_List_EntityLink__o* links;
		bool linkedToNeighbours;
		struct System_Collections_Generic_List_BaseEntity_ServerFileRequest__o* _pendingFileRequests;
		struct System_Action_o* updateParentingAction;
		struct BaseEntity_o* addedToParentEntity;
		struct ItemSkin_o* itemSkin;
		struct EntityRef_array* entitySlots;
		struct System_Collections_Generic_List_TriggerBase__o* triggers;
		bool isVisible;
		bool isAnimatorVisible;
		bool isShadowVisible;
		struct OccludeeSphere_o localOccludee;
		float _Weight_k__BackingField;
		struct UnityEngine_Bounds_o bounds;
		struct GameObjectRef_o* impactEffect;
		bool enableSaving;
		bool syncPosition;
		uintptr_t model;
		int32_t flags;
		uint32_t parentBone;
		uint64_t skinID;
		struct EntityComponentBase_array* _components;
		bool HasBrain;
		struct System_String_o* _name;
		uint64_t _OwnerID_k__BackingField;
	};

	struct BaseCombatEntity_t : BaseEntity_t {
		struct GameMenu_Option_o __menuOption_Menu_Pickup;
		struct SkeletonProperties_o* skeletonProperties;
		struct ProtectionProperties_o* baseProtection;
		float startHealth;
		struct BaseCombatEntity_Pickup_o pickup;
		struct BaseCombatEntity_Repair_o repair;
		bool ShowHealthInfo;
		int32_t lifestate;
		bool sendsHitNotification;
		bool sendsMeleeHitNotification;
		bool markAttackerHostile;
		float _health;
		float _maxHealth;
		int32_t faction;
		float deathTime;
		int32_t lastNotifyFrame;
	};

	struct ModelState_t
	{
		char pad_0000[20]; //0x0000
		float waterLevel; //0x0014
		char pad_0018[12]; //0x0018
		uint32_t flags; //0x0024
		char pad_0028[91]; //0x0028
	}; //Size: 0x0083

	struct PlayerModel_t
	{
		char bumper[0x18]; //custom
		float _smoothAimWeight;
		float _smoothVelocity;
		math::vector3 _smoothlookAngle;
		bool allowMountedHeadLook;
		math::vector3 smoothLookDir;
		math::vector3 lastSafeLookDir;
		struct UnityEngine_Transform_array* Shoulders;
		struct UnityEngine_Transform_array* AdditionalSpineBones;
		struct System_Collections_Generic_List_SkinnedMeshRenderer__o* LegParts;
		struct UnityEngine_Transform_array* fakeSpineBones;
		float extraLeanBack;
		struct System_Nullable_bool__o drawState;
		float timeInArmsMode;
		struct UnityEngine_BoxCollider_o* collision;
		struct UnityEngine_GameObject_o* censorshipCube;
		struct UnityEngine_GameObject_o* censorshipCubeBreasts;
		struct UnityEngine_GameObject_o* jawBone;
		struct UnityEngine_GameObject_o* neckBone;
		struct UnityEngine_GameObject_o* headBone;
		struct EyeController_o* eyeController;
		struct EyeBlink_o* blinkController;
		struct UnityEngine_Transform_array* SpineBones;
		struct UnityEngine_Transform_o* leftFootBone;
		struct UnityEngine_Transform_o* rightFootBone;
		struct UnityEngine_Transform_o* leftHandPropBone;
		struct UnityEngine_Transform_o* rightHandPropBone;
		math::vector3 rightHandTarget;
		math::vector3 leftHandTargetPosition;
		math::vector4 leftHandTargetRotation;
		math::vector3 rightHandTargetPosition;
		math::vector4 rightHandTargetRotation;
		float steeringTargetDegrees;
		math::vector3 rightFootTargetPosition;
		math::vector4 rightFootTargetRotation;
		math::vector3 leftFootTargetPosition;
		math::vector4 leftFootTargetRotation;
		struct UnityEngine_RuntimeAnimatorController_o* CinematicAnimationController;
		struct UnityEngine_Avatar_o* DefaultAvatar;
		struct UnityEngine_Avatar_o* CinematicAvatar;
		struct UnityEngine_RuntimeAnimatorController_o* DefaultHoldType;
		struct UnityEngine_RuntimeAnimatorController_o* SleepGesture;
		struct UnityEngine_RuntimeAnimatorController_o* CrawlToIncapacitatedGesture;
		struct UnityEngine_RuntimeAnimatorController_o* StandToIncapacitatedGesture;
		struct UnityEngine_RuntimeAnimatorController_o* CurrentGesture;
		struct SkinSetCollection_o* MaleSkin;
		struct SkinSetCollection_o* FemaleSkin;
		struct SubsurfaceProfile_o* subsurfaceProfile;
		float voiceVolume;
		float skinColor;
		float skinNumber;
		float meshNumber;
		float hairNumber;
		int32_t skinType;
		struct MovementSounds_o* movementSounds;
		bool showSash;
		int32_t tempPoseType;
		uint32_t underwearSkin;
		uint64_t _overrideSkinSeed_k__BackingField;
		math::vector4 _AimAngles_k__BackingField;
		math::vector4 _LookAngles_k__BackingField;
		uintptr_t modelState;
		math::vector3 position;
		math::vector3 velocity;
		math::vector3 speedOverride;
		math::vector3 newVelocity;
		math::vector4 rotation;
		math::vector4 mountedRotation;
		math::vector3 smoothLeftFootIK;
		math::vector3 smoothRightFootIK;
		bool drawShadowOnly;
		bool isIncapacitated;
		uint32_t flinchLocation;
		bool visible;
		struct PlayerNameTag_o* nameTag;
		bool animatorNeedsWarmup;
		bool isLocalPlayer;
		struct SoundDefinition_o* aimSoundDef;
		struct SoundDefinition_o* aimEndSoundDef;
		bool InGesture;
		struct GestureConfig_o* CurrentGestureConfig;
		bool InCinematic;
		struct UnityEngine_RuntimeAnimatorController_o* defaultAnimatorController;
		uintptr_t _multiMesh;
		struct UnityEngine_Animator_o* _animator;
		struct UnityEngine_LODGroup_o* _lodGroup;
		struct UnityEngine_RuntimeAnimatorController_o* _currentGesture;
		float holdTypeLock;
		bool wasMountedRightAim;
		int32_t cachedMask;
		int32_t cachedConstructionMask;
		struct HeldEntity_o* WorkshopHeldEntity;
		bool wasCrawling;
		float mountedSpineLookWeight;
		float mountedAnimSpeed;
		bool preserveBones;
		struct System_Nullable_float__o downLimitOverride;
		bool _IsNpc_k__BackingField;
		struct TimeSince_o timeSinceReactionStart;
		struct TimeSince_o timeSinceLeftFootTest;
		math::vector3 cachedLeftFootPos;
		math::vector3 cachedLeftFootNormal;
		struct TimeSince_o timeSinceRightFootTest;
		math::vector3 cachedRightFootPos;
		math::vector3 cachedRightFootNormal;
	};

	struct BasePlayer_t : BaseCombatEntity_t
	{
		struct GameMenu_Option_o __menuOption_Climb;
		struct GameMenu_Option_o __menuOption_Drink;
		struct GameMenu_Option_o __menuOption_InviteToTeam;
		struct GameMenu_Option_o __menuOption_Menu_AssistPlayer;
		struct GameMenu_Option_o __menuOption_Menu_LootPlayer;
		struct GameMenu_Option_o __menuOption_Promote;
		struct GameMenu_Option_o __menuOption_SaltWater;
		int32_t currentViewMode;
		int32_t selectedViewMode;
		math::vector3 lastRevivePoint;
		math::vector3 lastReviveDirection;
		uintptr_t playerModel;
		bool Frozen;
		uintptr_t voiceRecorder;
		uintptr_t voiceSpeaker;
		uintptr_t input;
		uintptr_t movement;
		uintptr_t collision;
		uintptr_t _lookingAt;
		uintptr_t _lookingAtEntity;
		uintptr_t _lookingAtCollider;
		math::vector3 _lookingAtPoint_k__BackingField;
		float wakeTime;
		bool needsClothesRebuild;
		bool wasSleeping;
		bool wokeUpBefore;
		bool wasDead;
		uint32_t lastClothesHash;
		bool _IsWearingDiveGoggles_k__BackingField;
		uintptr_t GestureViewModel;
		struct RealTimeSince_o timeSinceUpdatedLookingAt;
		float nextTopologyTestTime;
		float usePressTime;
		float useHeldTime;
		uintptr_t lookingAtTest;
		math::vector3 cachedWaterDrinkingPoint;
		struct GestureCollection_o* gestureList;
		struct TimeUntil_o gestureFinishedTime;
		struct TimeSince_o blockHeldInputTimer;
		struct GestureConfig_o* currentGesture;
		struct HeldEntity_o* disabledHeldEntity;
		float nextGestureMenuOpenTime;
		struct TimeSince_o lastGestureCancel;
		float client_lastHelloTime;
		uint64_t currentTeam;
		uintptr_t clientTeam;
		float lastReceivedTeamTime;
		uint64_t lastPresenceTeamId;
		int32_t lastPresenceTeamSize;
		uintptr_t playerGroupKey;
		uintptr_t playerGroupSizeKey;
		uint32_t clActiveItem;
		struct ProtoBuf_MapNote_o* ClientCurrentMapNote;
		struct ProtoBuf_MapNote_o* ClientCurrentDeathNote;
		struct System_Collections_Generic_List_BaseMission_MissionInstance__o* missions;
		int32_t _activeMission;
		uintptr_t modelState;
		struct EntityRef_o mounted;
		float nextSeatSwapTime;
		uintptr_t PetEntity;
		float lastPetCommandIssuedTime;
		uint32_t PetPrefabID;
		uint32_t PetID;
		float cachedBuildingPrivilegeTime;
		struct BuildingPrivlidge_o* cachedBuildingPrivilege;
		int32_t maxProjectileID;
		float lastUpdateTime;
		float cachedThreatLevel;
		int32_t serverTickRate;
		int32_t clientTickRate;
		float serverTickInterval;
		float clientTickInterval;
		float lastSentTickTime;
		uintptr_t lastSentTick;
		float nextVisThink;
		float lastTimeSeen;
		bool debugPrevVisible;
		struct GameObjectRef_o* fallDamageEffect;
		struct GameObjectRef_o* drownEffect;
		int32_t playerFlags;
		uintptr_t eyes;
		uintptr_t inventory;
		uintptr_t blueprints;
		struct PlayerMetabolism_o* metabolism;
		struct PlayerModifiers_o* modifiers;
		struct UnityEngine_CapsuleCollider_o* playerCollider;
		uintptr_t Belt;
		struct UnityEngine_Rigidbody_o* playerRigidbody;
		uint64_t userID;
		uintptr_t UserIDString;
		int32_t gamemodeteam;
		int32_t reputation;
		uintptr_t _displayName;
		uintptr_t _lastSetName;
		struct BasePlayer_CapsuleColliderInfo_o playerColliderStanding;
		struct BasePlayer_CapsuleColliderInfo_o playerColliderDucked;
		struct BasePlayer_CapsuleColliderInfo_o playerColliderCrawling;
		struct BasePlayer_CapsuleColliderInfo_o playerColliderLyingDown;
		struct ProtectionProperties_o* cachedProtection;
		float lastHeadshotSoundTime;
		float nextColliderRefreshTime;
		bool clothingBlocksAiming;
		float clothingMoveSpeedReduction;
		float clothingWaterSpeedBonus;
		float clothingAccuracyBonus;
		bool equippingBlocked;
		float eggVision;
		struct PhoneController_o* activeTelephone;
		uintptr_t designingAIEntity;
		uintptr_t address = 0;
	};




	//struct HeldEntity_t
	//{
	//	char pad_0000[336]; //0x0000
	//	math::vector3 punchAdded; //0x0150
	//	float lastPunchTime; //0x015C
	//	uintptr_t worldModelAnimator; //0x0160
	//	char pad_0168[24]; //0x0168
	//	uintptr_t viewModel; //0x0180
	//	bool isDeployed; //0x0188
	//	char pad_0189[24]; //0x0189
	//	bool N000002A5; //0x01A1
	//	char pad_01A2[2]; //0x01A2
	//	float hostileScore; //0x01A4
	//	uintptr_t holsterInfo; //0x01A8
	//	uintptr_t HeldCameraMode; //0x01B0
	//	char pad_01B8[8]; //0x01B8
	//	float FirstPersonRotationStrength; //0x01C0
	//	char pad_01C4[12]; //0x01C4
	//	uint32_t ownerItemUID; //0x01D0
	//	char pad_01D4[651]; //0x01D4
	//};

	struct HeldEntity_t
	{
		char pad_0x0000[0x168]; //0x0000
		math::vector3 punchAdded; //0x0168 
		float lastPunchTime; //0x0174 
		uintptr_t worldModelAnimator; //0x0178 
		char pad_0x0180[0x18]; //0x0180
		uintptr_t viewModel; //0x0198 
		bool isDeployed; //0x01A0 
		char pad_0x01A1[0x1B]; //0x01A1
		float hostileScore; //0x01BC 
		uintptr_t holsterInfo; //0x01C0 
		uintptr_t HeldCameraMode; //0x01C8 
		char pad_0x01D0[0x14]; //0x01D0
		float FirstPersonRotationStrength; //0x01E4 
		uint32_t ownerItemUID; //0x01E8 
	}; //Size=0x01EC


	struct Item_t
	{
		char pad_0000[32]; //0x0000
		uintptr_t info; //0x0020
		uint32_t uid; //0x0028
		bool dirty; //0x002C
		char pad_002D[3]; //0x002D
		uint32_t amount; //0x0030
		char pad_0034[4]; //0x0034
		float busyTime; //0x0038
		float removeTime; //0x003C
		float fuel; //0x0040
		char pad_0044[12]; //0x0044
		uintptr_t skin; //0x0050
		char pad_0058[24]; //0x0058
		uintptr_t flags; //0x0070
		uintptr_t contents; //0x0078
		uintptr_t parent; //0x0080
		uintptr_t worldEnt; //0x0088
		char pad_0090[8]; //0x0090
		uintptr_t heldEntity; //0x0098
		char pad_00A0[920]; //0x00A0


	};

	struct BeltItems_t
	{
		Item_t slot[6] = { 0 };
	};



	struct NoRecoil_t
	{
		float minYaw; //0x0018 
		float maxYaw; //0x001C 
		float minPitch; //0x0020 
		float maxPitch; //0x0024 
	}NoRecoil;

	struct NoAimcone_t
	{
		float m = -1.f; //0x02D0 
		float m1 = -1.f; //0x02D4 
		float m2 = -1.f; //0x02D8 
		float m3 = -1.f; //0x02DC 
		float m4 = -1.f; //0x02E0 
		char pad_0x02E4[0x24]; //0x02E4
		float m5 = -1.f; //0x0308 
	}NoAimcone;


	struct transformReadOnly
	{
		uintptr_t	pTransformData;
		int			index;
	};

	struct transform
	{
		uintptr_t pTransformArray;
		uintptr_t pTransformIndices;
	};

}