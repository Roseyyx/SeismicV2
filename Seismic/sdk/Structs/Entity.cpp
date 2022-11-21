#include "../Math/Math.h"
#include "Entity.h"
#include "../Helpers/Interfaces.h"
#include <cassert>

CBaseEntity* CBaseEntity::GetLocalPlayer() {
	return Interfaces::EntityList->Get<CBaseEntity>(Interfaces::Engine->GetLocalPlayer());
}

CBaseCombatWeapon* CBaseEntity::GetWeapon()
{
	return Interfaces::EntityList->Get<CBaseCombatWeapon>(this->GetActiveWeaponHandle());
}

std::optional<Vector> CBaseEntity::GetBonePosition(int iBone)
{
	assert(iBone > BONE_INVALID && iBone < MAXSTUDIOBONES); // given invalid bone index for getboneposition

	std::array<matrix3x4_t, MAXSTUDIOBONES> arrBonesToWorld = { };

	if (this->SetupBones(arrBonesToWorld.data(), arrBonesToWorld.size(), BONE_USED_BY_ANYTHING, 0.f))
		return arrBonesToWorld.at(iBone).at(3);

	return std::nullopt;
}

int CBaseEntity::GetBoneByHash(const FNV1A_t uBoneHash) const
{
	if (const auto pModel = this->GetModel(); pModel != nullptr)
	{
		if (const auto pStudioHdr = Interfaces::ModelInfo->GetStudioModel(pModel); pStudioHdr != nullptr)
		{
			for (int i = 0; i < pStudioHdr->nBones; i++)
			{
				if (const auto pBone = pStudioHdr->GetBone(i); pBone != nullptr && FNV1A::Hash(pBone->GetName()) == uBoneHash)
					return i;
			}
		}
	}

	return BONE_INVALID;
}

Vector CBaseEntity::GetHitboxPosition(int iHitbox){
	if (!this)
		return Vector(0,0,0);
	
	auto HDR = Interfaces::ModelInfo->GetStudioModel(this->GetModel());
	if (!HDR)
		return Vector(0, 0, 0);
	
	auto Set = HDR->GetHitbox(iHitbox, 0);
	if (!Set)
		return Vector(0, 0, 0);
	
	std::array<matrix3x4_t, MAXSTUDIOBONES> arrBonesToWorld = { };
	if (!this->SetupBones(arrBonesToWorld.data(), arrBonesToWorld.size(), BONE_USED_BY_HITBOX, 0.f))
		return Vector(0, 0, 0);
	
	const Vector Min = M::VectorTransform(Set->vecBBMin, arrBonesToWorld.at(Set->iBone));
	const Vector Max = M::VectorTransform(Set->vecBBMax, arrBonesToWorld.at(Set->iBone));
	
	return (Min + Max) * 0.5f;
}


bool CBaseEntity::IsEnemy(CBaseEntity* pEntity)
{
	// check is dangerzone
	if (Interfaces::GameTypes->GetCurrentGameType() == GAMETYPE_FREEFORALL)
		// check is not teammate
		return (this->GetSurvivalTeam() != pEntity->GetSurvivalTeam());

	static CConVar* mp_teammates_are_enemies = Interfaces::ConVar->FindVar("mp_teammates_are_enemies");

	// check is deathmatch
	if (mp_teammates_are_enemies != nullptr && mp_teammates_are_enemies->GetBool() && this->GetTeam() == pEntity->GetTeam() && this != pEntity)
		return true;

	if (this->GetTeam() != pEntity->GetTeam())
		return true;

	return false;
}

bool CBaseEntity::IsTargetingLocal(CBaseEntity* pLocal)
{
	Vector vecForward = { };
	const QAngle angView = this->GetEyeAngles();
	M::AngleVectors(angView, &vecForward);
	vecForward *= MAX_DISTANCE;

	const Vector vecStart = this->GetEyePosition();
	const Vector vecEnd = vecStart + vecForward;

	const Ray_t ray(vecStart, vecEnd);
	CTraceFilter filter(this);

	Trace_t trace = { };
	Interfaces::EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

	if (trace.pHitEntity == pLocal)
		return true;

	return false;
}

bool CBaseEntity::CanShoot(CWeaponCSBase* pBaseWeapon)
{
	const float flServerTime = TICKS_TO_TIME(this->GetTickBase());

	// check is have ammo
	if (pBaseWeapon->GetAmmo() <= 0)
		return false;

	// is player ready to shoot
	if (this->GetNextAttack() > flServerTime)
		return false;

	const short nDefinitionIndex = pBaseWeapon->GetItemDefinitionIndex();

	// check is weapon with burst mode
	if ((nDefinitionIndex == WEAPON_FAMAS || nDefinitionIndex == WEAPON_GLOCK) &&
		// check is burst mode
		pBaseWeapon->IsBurstMode() && pBaseWeapon->GetBurstShotsRemaining() > 0)
		return true;

	// is weapon ready to shoot
	if (pBaseWeapon->GetNextPrimaryAttack() > flServerTime)
		return false;

	// check for revolver cocking ready
	if (nDefinitionIndex == WEAPON_REVOLVER && pBaseWeapon->GetFireReadyTime() > flServerTime)
		return false;

	return true;
}

bool CBaseEntity::IsVisible(CBaseEntity* pEntity, const Vector& vecEnd, bool bSmokeCheck)
{
	const Vector vecStart = this->GetEyePosition();

	const Ray_t ray(vecStart, vecEnd);
	CTraceFilter filter(this);

	Trace_t trace = { };
	Interfaces::EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);


	return false;
}

bool CBaseEntity::IsBreakable()
{
	// @ida isbreakableentity: client.dll @ 55 8B EC 51 56 8B F1 85 F6 74 68

	const int iHealth = this->GetHealth();

	// first check to see if it's already broken
	if (iHealth < 0 && this->IsMaxHealth() > 0)
		return true;

	if (this->GetTakeDamage() != DAMAGE_YES)
	{
		const EClassIndex nClassIndex = this->GetClientClass()->ClassID;

		// force pass cfuncbrush
		if (nClassIndex != EClassIndex::CFuncBrush)
			return false;
	}

	if (const int nCollisionGroup = this->GetCollisionGroup(); nCollisionGroup != COLLISION_GROUP_PUSHAWAY && nCollisionGroup != COLLISION_GROUP_BREAKABLE_GLASS && nCollisionGroup != COLLISION_GROUP_NONE)
		return false;

	if (iHealth > 200)
		return false;

	if (IMultiplayerPhysics* pPhysicsInterface = dynamic_cast<IMultiplayerPhysics*>(this); pPhysicsInterface != nullptr)
	{
		if (pPhysicsInterface->GetMultiplayerPhysicsMode() != PHYSICS_MULTIPLAYER_SOLID)
			return false;
	}
	else
	{
		if (const char* szClassName = this->GetClassname(); !strcmp(szClassName, "func_breakable") || !strcmp(szClassName, "func_breakable_surf"))
		{
			if (!strcmp(szClassName, "func_breakable_surf"))
			{
				CBreakableSurface* pSurface = static_cast<CBreakableSurface*>(this);

				// don't try to break it if it has already been broken
				if (pSurface->IsBroken())
					return false;
			}
		}
		else if (this->PhysicsSolidMaskForEntity() & CONTENTS_PLAYERCLIP)
		{
			// hostages and players use CONTENTS_PLAYERCLIP, so we can use it to ignore them
			return false;
		}
	}

	if (IBreakableWithPropData* pBreakableInterface = dynamic_cast<IBreakableWithPropData*>(this); pBreakableInterface != nullptr)
	{
		// bullets don't damage it - ignore
		if (pBreakableInterface->GetDmgModBullet() <= 0.0f)
			return false;
	}

	return true;
}

void CBaseEntity::ModifyEyePosition(const CCSGOPlayerAnimState* pAnimState, Vector* vecPosition) const
{
	// @ida modifyeyeposition: client.dll @ 55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 38

	if (Interfaces::Engine->IsHLTV() || Interfaces::Engine->IsPlayingDemo())
		return;

	CBaseEntity* pBaseEntity = pAnimState->pEntity;

	if (pBaseEntity == nullptr)
		return;

	IClientEntity* pGroundEntity = Interfaces::EntityList->GetClientEntityFromHandle(pBaseEntity->GetGroundEntityHandle());

	if (!pAnimState->bHitGroundAnimation && pAnimState->flDuckAmount == 0.f && pGroundEntity != nullptr)
		return;

	if (const auto headPosition = pBaseEntity->GetBonePosition(pBaseEntity->GetBoneByHash(FNV1A::HashConst("head_0"))); headPosition.has_value())
	{
		Vector vecHead = headPosition.value();
		vecHead.z += 1.7f;

		if (vecHead.z < vecPosition->z)
		{
			float flFactor = 0.f;
			const float flDelta = std::fabsf(vecPosition->z - vecHead.z);
			const float flOffset = (flDelta - 4.0f) / 6.0f;

			if (flOffset >= 0.f)
				flFactor = std::min(flOffset, 1.0f);

			const float flFactorSquared = (flFactor * flFactor);
			vecPosition->z += ((vecHead.z - vecPosition->z) * ((flFactorSquared * 3.0f) - ((flFactorSquared * 2.0f) * flFactor)));
		}
	}
}

void CBaseEntity::PostThink()
{
	// @ida postthink: client.dll 56 8B 35 ? ? ? ? 57 8B F9 8B CE 8B 06 FF 90 ? ? ? ? 8B 07

	using PostThinkVPhysicsFn = bool(__thiscall*)(CBaseEntity*);
	static auto oPostThinkVPhysics = reinterpret_cast<PostThinkVPhysicsFn>(MEM::FindPattern("client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB"));
	assert(oPostThinkVPhysics != nullptr);

	using SimulatePlayerSimulatedEntitiesFn = void(__thiscall*)(CBaseEntity*);
	static auto oSimulatePlayerSimulatedEntities = reinterpret_cast<SimulatePlayerSimulatedEntitiesFn>(MEM::FindPattern("client.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74"));
	assert(oSimulatePlayerSimulatedEntities != nullptr);

	Interfaces::MdlCache->BeginLock();

	if (this->IsAlive())
	{
		this->UpdateCollisionBounds();

		if (this->GetFlags() & FL_ONGROUND)
			*this->GetFallVelocity() = 0.f;

		if (this->GetSequence() == -1)
			this->SetSequence(0);

		this->StudioFrameAdvance();
		oPostThinkVPhysics(this);
	}

	oSimulatePlayerSimulatedEntities(this);

	Interfaces::MdlCache->EndLock();
}

bool CBaseEntity::IsAlive() {
	if (!this)
		return false;
	
	if (this->GetLifeState() != LIFE_ALIVE)
		return false;
	
	return true;
}