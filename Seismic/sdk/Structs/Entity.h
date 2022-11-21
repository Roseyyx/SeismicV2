#pragma once

#include <array>
#include <vector>
#include <optional>

#include "Animations.h"
#include "Definitions.h"
#include "BitBuf.h"
#include "../DataTypes/UserCMD.h"
#include "../../Utils/Netvars/Netvar.h"
#include "../Interfaces/IClientEntityList.h"
#include "../Interfaces/IRefCount.h"
#include "../Interfaces/IVModelInfo.h"

#pragma region entities_definitions
#define INVALID_EHANDLE_INDEX		0xFFFFFFFF
#define NUM_ENT_ENTRY_BITS			(11 + 2)
#define NUM_ENT_ENTRIES				(1 << NUM_ENT_ENTRY_BITS)
#define NUM_SERIAL_NUM_BITS			16
#define NUM_SERIAL_NUM_SHIFT_BITS	(32 - NUM_SERIAL_NUM_BITS)
#define ENT_ENTRY_MASK				((1 << NUM_SERIAL_NUM_BITS) - 1)
#pragma endregion

#pragma region entities_enumerations
enum EDataUpdateType : int
{
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

enum ETeamID : int
{
	TEAM_UNASSIGNED = 0,
	TEAM_SPECTATOR,
	TEAM_TT,
	TEAM_CT
};

enum EThinkMethods : int
{
	THINK_FIRE_ALL_FUNCTIONS = 0,
	THINK_FIRE_BASE_ONLY,
	THINK_FIRE_ALL_BUT_BASE,
};

enum EItemDefinitionIndex : short
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_ZONE_REPULSOR = 20,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SHIELD = 37,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE_GG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS = 69,
	WEAPON_BREACHCHARGE = 70,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE = 75,
	WEAPON_HAMMER = 76,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB = 81,
	WEAPON_DIVERSION = 82,
	WEAPON_FRAG_GRENADE = 83,
	WEAPON_SNOWBALL = 84,
	WEAPON_BUMPMINE = 85,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BROKENFANG = 4725,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T = 5028,
	GLOVE_CT = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_HANDWRAPS = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_STUDDED_HYDRA = 5035,
	SPECIAL_AGENT_BLUEBERRIES_BUCKSHOT = 4619,
	SPECIAL_AGENT_TWO_TIMES_MCCOY_TACP = 4680,
	SPECIAL_AGENT_COMMANDOR_MAE_JAMISON = 4711,
	SPECIAL_AGENT_1ST_LIEUTENANT_FARLOW,
	SPECIAL_AGENT_JOHN_KASK,
	SPECIAL_AGENT_BIO_HAZ_SPECIALIST,
	SPECIAL_AGENT_SERGEANT_BOMBSON,
	SPECIAL_AGENT_CHEM_HAZ_SPECIALIST,
	SPECIAL_AGENT_REZAN_THE_REDSHIRT = 4718,
	SPECIAL_AGENT_SIR_BLOODY_MIAMI_DARRYL = 4726,
	SPECIAL_AGENT_SAFECRACKER_VOLTZMANN,
	SPECIAL_AGENT_LITTLE_KEV,
	SPECIAL_AGENT_GETAWAY_SALLY = 4730,
	SPECIAL_AGENT_NUMBER_K = 4732,
	SPECIAL_AGENT_SIR_BLOODY_SILENT_DARRYL = 4733,
	SPECIAL_AGENT_SIR_BLOODY_SKULLHEAD_DARRYL,
	SPECIAL_AGENT_SIR_BLOODY_DARRYL_ROYALE,
	SPECIAL_AGENT_SIR_BLOODY_LOUDMOUTH_DARRYL,
	SPECIAL_AGENT_T = 5036,
	SPECIAL_AGENT_CT = 5037,
	SPECIAL_AGENT_GROUND_REBEL = 5105,
	SPECIAL_AGENT_OSIRIS,
	SPECIAL_AGENT_SHAHMAT,
	SPECIAL_AGENT_MUHLIK,
	SPECIAL_AGENT_SOLDIER = 5205,
	SPECIAL_AGENT_ENFORCER,
	SPECIAL_AGENT_SLINGSHOT,
	SPECIAL_AGENT_STREET_SOLDIER,
	SPECIAL_AGENT_OPERATOR = 5305,
	SPECIAL_AGENT_MARKUS_DELROW,
	SPECIAL_AGENT_MICHAEL_SYFERS,
	SPECIAL_AGENT_AVA,
	SPECIAL_AGENT_3RD_COMMANDO_COMPANY = 5400,
	SPECIAL_AGENT_SEAL_TEAM_6_SOLDIER,
	SPECIAL_AGENT_BUCKSHOT,
	SPECIAL_AGENT_TWO_TIMES_MCCOY_USAF,
	SPECIAL_AGENT_RICKSAW,
	SPECIAL_AGENT_DRAGOMIR = 5500,
	SPECIAL_AGENT_MAXIMUS,
	SPECIAL_AGENT_REZAN_THE_READY,
	SPECIAL_AGENT_BLACKWOLF = 5503,
	SPECIAL_AGENT_THE_DOCTOR,
	SPECIAL_AGENT_DRAGOMIR_FOOTSOLDIERS,
	SPECIAL_AGENT_B_SQUADRON_OFFICER = 5601
};

enum EWeaponType : int
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL = 1,
	WEAPONTYPE_SUBMACHINEGUN = 2,
	WEAPONTYPE_RIFLE = 3,
	WEAPONTYPE_SHOTGUN = 4,
	WEAPONTYPE_SNIPER = 5,
	WEAPONTYPE_MACHINEGUN = 6,
	WEAPONTYPE_C4 = 7,
	WEAPONTYPE_PLACEHOLDER = 8,
	WEAPONTYPE_GRENADE = 9,
	WEAPONTYPE_HEALTHSHOT = 11,
	WEAPONTYPE_FISTS = 12,
	WEAPONTYPE_BREACHCHARGE = 13,
	WEAPONTYPE_BUMPMINE = 14,
	WEAPONTYPE_TABLET = 15,
	WEAPONTYPE_MELEE = 16
};
#pragma endregion

class IHandleEntity
{
public:
	virtual							~IHandleEntity() { }
	virtual void					SetRefEHandle(const CBaseHandle& hRef) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class IClientUnknown;
class ICollideable
{
public:
	virtual IHandleEntity* GetEntityHandle() = 0;
	virtual const Vector& OBBMins() const = 0;
	virtual const Vector& OBBMaxs() const = 0;
	virtual void					WorldSpaceTriggerBounds(Vector* pVecWorldMins, Vector* pVecWorldMaxs) const = 0;
	virtual bool					TestCollision(const Ray_t& ray, unsigned int fContentsMask, Trace_t& tr) = 0;
	virtual bool					TestHitboxes(const Ray_t& ray, unsigned int fContentsMask, Trace_t& tr) = 0;
	virtual int						GetCollisionModelIndex() = 0;
	virtual const Model_t* GetCollisionModel() = 0;
	virtual Vector& GetCollisionOrigin() const = 0;
	virtual QAngle& GetCollisionAngles() const = 0;
	virtual const matrix3x4_t& CollisionToWorldTransform() const = 0;
	virtual ESolidType				GetSolid() const = 0;
	virtual int						GetSolidFlags() const = 0;
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual int						GetCollisionGroup() const = 0;

	/*
	 * @note: should be equivalent to C_BaseAnimating::ComputeHitboxSurroundingBox
	 * DOESNT NEEDED TRANSORMATION!
	 * and bugged when trying to get non-player entity box
	 */
	virtual void					WorldSpaceSurroundingBounds(Vector* pVecMins, Vector* pVecMaxs) = 0;
	virtual unsigned int			GetRequiredTriggerFlags() const = 0;
	virtual const matrix3x4_t* GetRootParentToWorldTransform() const = 0;
	virtual void* GetVPhysicsObject() const = 0;
};

class IClientAlphaProperty
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void SetAlphaModulation(std::uint8_t uAlpha) = 0;
	virtual void SetRenderFX(int nRenderFx, int nRenderMode, float flStartTime = FLT_MAX, float flDuration = 0.0f) = 0;
	virtual void SetFade(float flGlobalFadeScale, float flDistFadeStart, float flDistFadeEnd) = 0;
	virtual void SetDesyncOffset(int nOffset) = 0;
	virtual void EnableAlphaModulationOverride(bool bEnable) = 0;
	virtual void EnableShadowAlphaModulationOverride(bool bEnable) = 0;
	virtual void SetDistanceFadeMode(int nFadeMode) = 0;
};

class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class CBaseEntity;
class IClientThinkable;
class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual CBaseEntity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
	virtual IClientAlphaProperty* GetClientAlphaProperty() = 0;
};

struct RenderableInstance_t
{
	std::uint8_t uAlpha;
};

using ClientShadowHandle_t = std::uint16_t;
using ClientRenderHandle_t = std::uint16_t;
using ModelInstanceHandle_t = std::uint16_t;
class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual Vector& GetRenderOrigin() = 0;
	virtual QAngle& GetRenderAngles() = 0;
	virtual bool					ShouldDraw() = 0;
	virtual int						GetRenderFlags() = 0;
	virtual bool					IsTransparent() = 0;
	virtual ClientShadowHandle_t	GetShadowHandle() const = 0;
	virtual ClientRenderHandle_t& RenderHandle() = 0;
	virtual const Model_t* GetModel() const = 0;
	virtual int						DrawModel(int nFlags, const RenderableInstance_t& uInstance) = 0;
	virtual int						GetBody() = 0;
	virtual void					GetColorModulation(float* pColor) = 0;
	virtual bool					LODTest() = 0;
	virtual bool					SetupBones(matrix3x4_t* matBoneToWorldOut, int nMaxBones, int fBoneMask, float flCurrentTime) = 0;
	virtual void					SetupWeights(const matrix3x4_t* matBoneToWorld, int nFlexWeightCount, float* flFlexWeights, float* flFlexDelayedWeights) = 0;
	virtual void					DoAnimationEvents() = 0;
	virtual void* GetPVSNotifyInterface() = 0;
	virtual void					GetRenderBounds(Vector& vecMins, Vector& vecMaxs) = 0;
	virtual void					GetRenderBoundsWorldspace(Vector& vecMins, Vector& vecMaxs) = 0;
	virtual void					GetShadowRenderBounds(Vector& vecMins, Vector& vecMaxs, int iShadowType) = 0;
	virtual bool					ShouldReceiveProjectedTextures(int nFlags) = 0;
	virtual bool					GetShadowCastDistance(float* pDistance, int iShadowType) const = 0;
	virtual bool					GetShadowCastDirection(Vector* vecDirection, int iShadowType) const = 0;
	virtual bool					IsShadowDirty() = 0;
	virtual void					MarkShadowDirty(bool bDirty) = 0;
	virtual IClientRenderable* GetShadowParent() = 0;
	virtual IClientRenderable* FirstShadowChild() = 0;
	virtual IClientRenderable* NextShadowPeer() = 0;
	virtual int						ShadowCastType() = 0;
	virtual void					unused2() {}
	virtual void					CreateModelInstance() = 0;
	virtual ModelInstanceHandle_t	GetModelInstance() = 0;
	virtual const matrix3x4_t& RenderableToWorldTransform() = 0;
	virtual int						LookupAttachment(const char* pAttachmentName) = 0;
	virtual bool					GetAttachment(int nIndex, Vector& vecOrigin, QAngle& angView) = 0;
	virtual bool					GetAttachment(int nIndex, matrix3x4_t& matAttachment) = 0;
	virtual bool					ComputeLightingOrigin(int nAttachmentIndex, Vector vecModelLightingCenter, const matrix3x4_t& matrix, Vector& vecTransformedLightingCenter) = 0;
	virtual float* GetRenderClipPlane() = 0;
	virtual int						GetSkin() = 0;
	virtual void					OnThreadedDrawSetup() = 0;
	virtual bool					UsesFlexDelayedWeights() = 0;
	virtual void					RecordToolMessage() = 0;
	virtual bool					ShouldDrawForSplitScreenUser(int nSlot) = 0;
	virtual std::uint8_t			OverrideAlphaModulation(std::uint8_t uAlpha) = 0;
	virtual std::uint8_t			OverrideShadowAlphaModulation(std::uint8_t uAlpha) = 0;
	virtual void* GetClientModelRenderable() = 0;
};

class ClientClass;
class IClientNetworkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void					Release() = 0;
	virtual ClientClass* GetClientClass() = 0;
	virtual void					NotifyShouldTransmit(int iState) = 0;
	virtual void					OnPreDataChanged(EDataUpdateType updateType) = 0;
	virtual void					OnDataChanged(EDataUpdateType updateType) = 0;
	virtual void					PreDataUpdate(EDataUpdateType updateType) = 0;
	virtual void					PostDataUpdate(EDataUpdateType updateType) = 0;
	virtual void					OnDataUnchangedInPVS() = 0;
	virtual bool					IsDormant() const = 0;
	virtual int						GetIndex() const = 0;
	virtual void					ReceiveMessage(EClassIndex classIndex, bf_read& msg) = 0;
	virtual void* GetDataTableBasePtr() = 0;
	virtual void					SetDestroyedOnRecreateEntities() = 0;
};

class CClientThinkHandle;
using ClientThinkHandle_t = CClientThinkHandle*;
class IClientThinkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void					ClientThink() = 0;
	virtual ClientThinkHandle_t		GetThinkHandle() = 0;
	virtual void					SetThinkHandle(ClientThinkHandle_t hThink) = 0;
	virtual void					Release() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual const Vector& GetAbsOrigin() const = 0;
	virtual const QAngle& GetAbsAngles() const = 0;
	virtual void* GetMouth() = 0;
	virtual bool					GetSoundSpatialization(struct SpatializationInfo_t& info) = 0;
	virtual bool					IsBlurred() = 0;

	void SetAbsOrigin(const Vector& vecOrigin)
	{
		using SetAbsOriginFn = void(__thiscall*)(void*, const Vector&);
		static auto oSetAbsOrigin = reinterpret_cast<SetAbsOriginFn>(MEM::FindPattern("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		oSetAbsOrigin(this, vecOrigin);
	}

	void SetAbsAngles(const QAngle& angView)
	{
		using SetAbsAngleFn = void(__thiscall*)(void*, const QAngle&);
		static auto oSetAbsAngles = reinterpret_cast<SetAbsAngleFn>(MEM::FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"));
		oSetAbsAngles(this, angView);
	}

	DataMap_t* GetDataDescMap()
	{
		return MEM::CallVFunc<DataMap_t*>(this, 15);
	}

	DataMap_t* GetPredictionDescMap()
	{
		return MEM::CallVFunc<DataMap_t*>(this, 17);
	}
};

class CWeaponCSBase;
class CBaseEntity : public IClientEntity
{
public:
	N_ADD_VARIABLE(int, GetHealth, "CBasePlayer->m_iHealth");
	N_ADD_VARIABLE(float, GetNextAttack, "CBaseCombatCharacter->m_flNextAttack");
	N_ADD_VARIABLE(CBaseHandle, GetActiveWeaponHandle, "CBaseCombatCharacter->m_hActiveWeapon");
	N_ADD_PVARIABLE(CBaseHandle, GetWeaponsHandle, "CBaseCombatCharacter->m_hMyWeapons");
	N_ADD_PVARIABLE(CBaseHandle, GetWearablesHandle, "CBaseCombatCharacter->m_hMyWearables");
	N_ADD_VARIABLE(int, GetSurvivalTeam, "CCSPlayer->m_nSurvivalTeam");
	N_ADD_VARIABLE(int, GetTeam, "CBaseEntity->m_iTeamNum");
	N_ADD_VARIABLE(int, GetSequence, "CBaseAnimating->m_nSequence");
	N_ADD_VARIABLE(QAngle, GetEyeAngles, "CCSPlayer->m_angEyeAngles");
	N_ADD_PVARIABLE(float, GetFallVelocity, "CBasePlayer->m_flFallVelocity");
	N_ADD_VARIABLE_OFFSET(int, IsUsedNewAnimState, "CCSPlayer->m_flLastExoJumpTime", 0x8);
	N_ADD_VARIABLE(float, GetAnimationTime, "CBaseEntity->m_flAnimTime");
	N_ADD_VARIABLE_OFFSET(CCSGOPlayerAnimState*, GetAnimationState, "CCSPlayer->m_bIsScoped", -0x14);
	N_ADD_VARIABLE(int, GetTickBase, "CBasePlayer->m_nTickBase");
	N_ADD_VARIABLE(int, GetLifeState, "CBasePlayer->m_lifeState");
	N_ADD_PVARIABLE(ICollideable, GetCollision, "CBaseEntity->m_Collision");
	N_ADD_VARIABLE(int, GetCollisionGroup, "CBaseEntity->m_CollisionGroup");
	N_ADD_VARIABLE(int, GetFlags, "CBasePlayer->m_fFlags");
	N_ADD_VARIABLE(CBaseHandle, GetGroundEntityHandle, "CBasePlayer->m_hGroundEntity");
	N_ADD_PVARIABLE_OFFSET(CUserCmd*, GetCurrentCommand, "CBasePlayer->m_hViewEntity", -0x4);
	N_ADD_VARIABLE_OFFSET(int, GetButtonDisabled, "CBasePlayer->m_hViewEntity", -0xC);
	N_ADD_PVARIABLE(int, GetNextThinkTick, "CBasePlayer->m_nNextThinkTick");
	N_ADD_VARIABLE_OFFSET(int, GetButtonForced, "CBasePlayer->m_hViewEntity", -0x8);
	N_ADD_VARIABLE(bool, HasHelmet, "CCSPlayer->m_bHasHelmet");
	N_ADD_VARIABLE(Vector, GetOrigin, "CBaseEntity->m_vecOrigin");
	N_ADD_VARIABLE(Vector, GetViewOffset, "CBasePlayer->m_vecViewOffset[0]");
	N_ADD_VARIABLE(bool, HasHeavyArmor, "CCSPlayer->m_bHasHeavyArmor");
	N_ADD_VARIABLE(int, GetArmor, "CCSPlayer->m_ArmorValue");
	N_ADD_VARIABLE(int, GetHitboxSet, "CBaseAnimating->m_nHitboxSet");

	N_ADD_DATAFIELD(int, GetEFlags, this->GetPredictionDescMap(), "m_iEFlags");
	N_ADD_PDATAFIELD(int, GetButtons, this->GetPredictionDescMap(), "m_nButtons");
	N_ADD_DATAFIELD(int, GetButtonLast, this->GetPredictionDescMap(), "m_afButtonLast");
	N_ADD_DATAFIELD(int, GetButtonPressed, this->GetPredictionDescMap(), "m_afButtonPressed");
	N_ADD_DATAFIELD(int, GetButtonReleased, this->GetPredictionDescMap(), "m_afButtonReleased");
	N_ADD_PDATAFIELD(int, GetImpulse, this->GetPredictionDescMap(), "m_nImpulse");
	N_ADD_DATAFIELD(float, GetSurfaceFriction, this->GetPredictionDescMap(), "m_surfaceFriction");


	bool IsPlayer()
	{
		// @xref: "effects/nightvision"
		return MEM::CallVFunc<bool>(this, 158);
	}

	inline bool IsArmored(const int iHitGroup)
	{
		// @ida isarmored: server.dll @ 55 8B EC 32 D2

		bool bIsArmored = false;

		if (this->GetArmor() > 0)
		{
			switch (iHitGroup)
			{
			case HITGROUP_GENERIC:
			case HITGROUP_CHEST:
			case HITGROUP_STOMACH:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
			case HITGROUP_NECK:
				bIsArmored = true;
				break;
			case HITGROUP_HEAD:
				if (this->HasHelmet())
					bIsArmored = true;
				[[fallthrough]];
			case HITGROUP_LEFTLEG:
			case HITGROUP_RIGHTLEG:
				if (this->HasHeavyArmor())
					bIsArmored = true;
				break;
			default:
				break;
			}
		}

		return bIsArmored;
	}

	void Think()
	{
		// @ida: client.dll @ [FF 90 ? ? ? ? FF 35 ? ? ? ? 8B 4C + 0x2] / sizeof(std::uintptr_t)
		MEM::CallVFunc<void>(this, 139);
	}

	int& GetTakeDamage()
	{
		static const std::uintptr_t uTakeDamageOffset = *reinterpret_cast<std::uintptr_t*>(MEM::FindPattern("client.dll", "80 BE ? ? ? ? ? 75 46 8B 86") + 0x2);
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + uTakeDamageOffset);
	}
	
	const char* GetClassname()
	{
		// @ida: client.dll @ [8B 01 FF 90 ? ? ? ? 90 + 0x4] / sizeof(std::uintptr_t)
		return MEM::CallVFunc<const char*>(this, 143);
	}

	void SetSequence(int iSequence)
	{
		// @ida: FF 90 ? ? ? ? 8B 07 8B CF FF 90 ? ? ? ? 8B CF + 0x2
		MEM::CallVFunc<void>(this, 219, iSequence);
	}

	void StudioFrameAdvance()
	{
		// @ida: FF 90 ? ? ? ? 8B 07 8B CF FF 90 ? ? ? ? 8B CF + 0xC
		MEM::CallVFunc<void>(this, 220);
	}

	unsigned int PhysicsSolidMaskForEntity()
	{
		// @xref: "func_breakable", "func_breakable_surf"
		return MEM::CallVFunc<unsigned int>(this, 152);
	}

	CUserCmd& GetLastCommand()
	{
		static const std::uintptr_t uLastCommandOffset = *reinterpret_cast<std::uintptr_t*>(MEM::FindPattern("client.dll", "8D 8E ? ? ? ? 89 5C 24 3C") + 0x2);
		return *reinterpret_cast<CUserCmd*>(reinterpret_cast<std::uintptr_t>(this) + uLastCommandOffset);
	}

	Vector GetEyePosition()
	{
		static auto Weapon_ShootPosition = reinterpret_cast<float* (__thiscall*)(void*, Vector*)>(MEM::FindPattern("client.dll", "55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF ? ? ? ? ? 80"));
	
		Vector output;
		Weapon_ShootPosition(this, &output);

		return output;
	}

	int IsMaxHealth()
	{
		// @ida: client.dll @ [FF 90 ? ? ? ? 85 C0 0F 8F ? ? ? ? 80 + 0x2] / sizeof(std::uintptr_t)
		return MEM::CallVFunc<int>(this, 123);
	}

	void PreThink()
	{
		MEM::CallVFunc<void>(this, 318);
	}

	void UpdateCollisionBounds()
	{
		MEM::CallVFunc<void>(this, 340);
	}

	bool PhysicsRunThink(int nThinkMethod)
	{
		// @xref: from sub with "CLIENT:  %s(%s) thinking for %.02f ms!!!\n"
		using PhysicsRunThinkFn = bool(__thiscall*)(void*, int);
		static auto oPhysicsRunThink = reinterpret_cast<PhysicsRunThinkFn>(MEM::FindPattern("client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87"));
		return oPhysicsRunThink(this, nThinkMethod);
	}


	static CBaseEntity* GetLocalPlayer();
	CBaseCombatWeapon* GetWeapon();
	std::optional<Vector>	GetBonePosition(int iBone);
	int						GetBoneByHash(const FNV1A_t uBoneHash) const;
	Vector					GetHitboxPosition(int iHitbox);
	bool					IsEnemy(CBaseEntity* pEntity);
	bool					IsTargetingLocal(CBaseEntity* pLocal);
	bool					CanShoot(CWeaponCSBase* pBaseWeapon);
	bool					IsVisible(CBaseEntity* pEntity, const Vector& vecEnd, bool bSmokeCheck = false);
	bool					IsBreakable();
	void					PostThink();
	void					ModifyEyePosition(const CCSGOPlayerAnimState* pAnimState, Vector* vecPosition) const;
	bool					IsAlive();
};

class CCSWeaponData
{
public:
	std::byte pad0[0x14];			// 0x0000
	int iMaxClip1;					// 0x0014
	int iMaxClip2;					// 0x0018
	int iDefaultClip1;				// 0x001C
	int iDefaultClip2;				// 0x0020
	int iPrimaryMaxReserveAmmo;		// 0x0024
	int iSecondaryMaxReserveAmmo;	// 0x0028
	const char* szWorldModel;		// 0x002C
	const char* szViewModel;		// 0x0030
	const char* szDroppedModel;		// 0x0034
	std::byte pad1[0x50];			// 0x0038
	const char* szHudName;			// 0x0088
	const char* szWeaponName;		// 0x008C
	std::byte pad2[0x2];			// 0x0090
	bool bIsMeleeWeapon;			// 0x0092
	std::byte pad3[0x9];			// 0x0093
	float flWeaponWeight;			// 0x009C
	std::byte pad4[0x4];			// 0x00A0
	int iSlot;						// 0x00A4
	int iPosition;					// 0x00A8
	std::byte pad5[0x1C];			// 0x00AC
	int nWeaponType;				// 0x00C8
	std::byte pad6[0x4];			// 0x00CC
	int iWeaponPrice;				// 0x00D0
	int iKillAward;					// 0x00D4
	const char* szAnimationPrefix;	// 0x00D8
	float flCycleTime;				// 0x00DC
	float flCycleTimeAlt;			// 0x00E0
	std::byte pad8[0x8];			// 0x00E4
	bool bFullAuto;					// 0x00EC
	std::byte pad9[0x3];			// 0x00ED
	int iDamage;					// 0x00F0
	float flHeadShotMultiplier;		// 0x00F4
	float flArmorRatio;				// 0x00F8
	int iBullets;					// 0x00FC
	float flPenetration;			// 0x0100
	std::byte pad10[0x8];			// 0x0104
	float flRange;					// 0x010C
	float flRangeModifier;			// 0x0110
	float flThrowVelocity;			// 0x0114
	std::byte pad11[0xC];			// 0x0118
	bool bHasSilencer;				// 0x0124
	std::byte pad12[0xF];			// 0x0125
	float flMaxSpeed[2];			// 0x0134
	std::byte pad13[0x4];			// 0x013C
	float flSpread[2];				// 0x0140
	float flInaccuracyCrouch[2];	// 0x0148
	float flInaccuracyStand[2];		// 0x0150
	std::byte pad14[0x8];			// 0x0158
	float flInaccuracyJump[2];		// 0x0160
	float flInaccuracyLand[2];		// 0x0168
	float flInaccuracyLadder[2];	// 0x0170
	float flInaccuracyFire[2];		// 0x0178
	float flInaccuracyMove[2];		// 0x0180
	float flInaccuracyReload;		// 0x0188
	int iRecoilSeed;				// 0x018C
	float flRecoilAngle[2];			// 0x0190
	float flRecoilAngleVariance[2];	// 0x0198
	float flRecoilMagnitude[2];		// 0x01A0
	float flRecoilMagnitudeVariance[2]; // 0x01A8
	int iSpreadSeed;				// 0x01B0

	bool IsGun() const
	{
		switch (this->nWeaponType)
		{
		case WEAPONTYPE_PISTOL:
		case WEAPONTYPE_SUBMACHINEGUN:
		case WEAPONTYPE_RIFLE:
		case WEAPONTYPE_SHOTGUN:
		case WEAPONTYPE_SNIPER:
		case WEAPONTYPE_MACHINEGUN:
			return true;
		}

		return false;
	}
};

class IRefCounted;
class CEconItemView
{
public:
};

class CBaseCombatWeapon : public IClientEntity
{
public:
	N_ADD_VARIABLE(float, GetNextPrimaryAttack, "CBaseCombatWeapon->m_flNextPrimaryAttack");
	N_ADD_VARIABLE(int, GetAmmo, "CBaseCombatWeapon->m_iClip1");
	N_ADD_VARIABLE(short, GetItemDefinitionIndex, "CBaseAttributableItem->m_iItemDefinitionIndex");
};

class CTEFireBullets
{
public:
	std::byte	pad0[0x10];				//0x0000
	int			nPlayer;				//0x0010
	int			nItemDefinitionIndex;	//0x0014
	Vector		vecOrigin;				//0x0018
	Vector		vecAngles;				//0x0024
	int			iWeapon;				//0x0030
	int			nWeaponID;				//0x0034
	int			iMode;					//0x0038
	int			nSeed;					//0x003C
	float		flInaccuracy;			//0x0040
	float		flRecoilIndex;			//0x0044
	float		flSpread;				//0x0048
	int			nSoundType;				//0x004C
}; // Size: 0x0050

class CWeaponCSBase : public CBaseCombatWeapon
{
public:
	N_ADD_VARIABLE(bool, IsBurstMode, "CWeaponCSBase->m_bBurstMode");
	N_ADD_VARIABLE(int, GetBurstShotsRemaining, "CWeaponCSBaseGun->m_iBurstShotsRemaining");
	N_ADD_VARIABLE(float, GetFireReadyTime, "CWeaponCSBase->m_flPostponeFireReadyTime");
};

class CBaseCSGrenade : public CWeaponCSBase
{
public:
};


class CSmokeGrenade
{
public:
};

class CInferno
{
public:
};


class CPlantedC4
{
public:
};

class CBaseViewModel
{
public:
};

class CEnvTonemapController
{
public:
};

class CBreakableSurface : public CBaseEntity, public IBreakableWithPropData
{
public:
	N_ADD_VARIABLE(bool, IsBroken, "CBreakableSurface->m_bIsBroken");
};