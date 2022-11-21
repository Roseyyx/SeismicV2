#include "Aim Assistance.h"
#include "../../Utils/Renderer/Renderer.h"
#include "AutoWall.h"
#include "../../sdk/Math/Math.h"
#include "../../Utils/Log/Log.h"

void AimAssistance::Run(CBaseEntity* pLocal, CUserCmd* pCmd) {
	if (!pLocal || !pLocal->IsAlive() || !Interfaces::Engine->IsInGame())
		return;

	if (Targets.empty())
		GetTargets(pLocal);
	
	std::vector<CBaseEntity*> BestTargets = GetBestTarget(pLocal);
	
	if (BestTargets.empty())
		return;

	float Distance = 0.f;
	float tempDistance = 0.f;

	CBaseEntity* pTarget = nullptr;

	for (auto& possibleTarget : BestTargets) {
		Distance = pLocal->GetEyePosition().DistTo(possibleTarget->GetEyePosition());
		// get closest target
		if (tempDistance == 0.f || Distance < tempDistance) {
			tempDistance = Distance;
			pTarget = possibleTarget;
		}
	}

	if (!pTarget || !pTarget->IsAlive()) {
		Targets.erase(std::remove(Targets.begin(), Targets.end(), pTarget), Targets.end());
		return;
	}

	std::vector<Point> Points;
	std::vector<int> iHitboxes = GetHitboxes();
	for (auto& iHitbox : iHitboxes)
		GetPoints(pLocal, pTarget, iHitbox, Points);


	if (Points.empty())
		return;
	
	float highestDamage = 0.f;
	float tempDamage = 0.f;
	
	for (auto& points : Points) {
		highestDamage = CAutoWall::GetDamage(pLocal, points.AimPoint);
		if (tempDamage < highestDamage) {
			tempDamage = highestDamage;
			QAngle angView = M::CalcAngle(pLocal->GetEyePosition(), points.AimPoint).Clamp();
			Interfaces::Engine->SetViewAngles(angView);
		}
	}
	
}

void AimAssistance::GetTargets(CBaseEntity* pLocal) {
	for (int i = 1; i < Interfaces::GlobalVars->nMaxClients; i++) {
		CBaseEntity* pEntity = Interfaces::EntityList->Get<CBaseEntity>(i);

		if (!pEntity || pEntity == pLocal || pEntity->IsDormant() || !pEntity->IsAlive() || !pLocal->IsEnemy(pEntity))
			continue;

		Targets.push_back(pEntity);
	}
}

std::vector<CBaseEntity*> AimAssistance::GetBestTarget(CBaseEntity* pLocal) {
	std::sort(Targets.begin(), Targets.end(), [pLocal](CBaseEntity* a, CBaseEntity* b) {
		return pLocal->GetEyePosition().DistTo(a->GetEyePosition()) < pLocal->GetEyePosition().DistTo(b->GetEyePosition());
		});
	
	return Targets;
}

std::vector<int> AimAssistance::GetHitboxes() {
	std::vector <int> iHitboxes;
	iHitboxes.emplace_back(HITBOX_HEAD);
	iHitboxes.emplace_back(HITBOX_NECK);
	iHitboxes.emplace_back(HITBOX_PELVIS);
	iHitboxes.emplace_back(HITBOX_STOMACH);
	iHitboxes.emplace_back(HITBOX_THORAX);
	iHitboxes.emplace_back(HITBOX_CHEST);
	iHitboxes.emplace_back(HITBOX_UPPER_CHEST);
	iHitboxes.emplace_back(HITBOX_RIGHT_THIGH);
	iHitboxes.emplace_back(HITBOX_LEFT_THIGH);
	iHitboxes.emplace_back(HITBOX_RIGHT_CALF);
	iHitboxes.emplace_back(HITBOX_LEFT_CALF);
	iHitboxes.emplace_back(HITBOX_RIGHT_FOOT);
	iHitboxes.emplace_back(HITBOX_LEFT_FOOT);
	iHitboxes.emplace_back(HITBOX_RIGHT_HAND);
	iHitboxes.emplace_back(HITBOX_LEFT_HAND);
	iHitboxes.emplace_back(HITBOX_RIGHT_UPPER_ARM);
	iHitboxes.emplace_back(HITBOX_RIGHT_FOREARM);
	iHitboxes.emplace_back(HITBOX_LEFT_UPPER_ARM);
	iHitboxes.emplace_back(HITBOX_LEFT_FOREARM);

	return iHitboxes;
}

void AimAssistance::GetPoints(CBaseEntity* pLocal, CBaseEntity* pEntity, int iHitbox, std::vector<Point>& Points) {
	auto Model = pEntity->GetModel();

	if (!Model)
		return;

	auto StudioModel = Interfaces::ModelInfo->GetStudioModel(Model);

	if (!StudioModel)
		return;

	auto Hitbox = StudioModel->GetHitbox(iHitbox, 0);

	if (!Hitbox)
		return;

	std::array<matrix3x4_t, MAXSTUDIOBONES> arrBonesToWorld = { };
	if (!pEntity->SetupBones(arrBonesToWorld.data(), arrBonesToWorld.size(), BONE_USED_BY_HITBOX, 0.f))
		return;

	const Vector Min = M::VectorTransform(Hitbox->vecBBMin, arrBonesToWorld.at(Hitbox->iBone));
	const Vector Max = M::VectorTransform(Hitbox->vecBBMax, arrBonesToWorld.at(Hitbox->iBone));

	auto center = (Min + Max) * 0.5f;
	auto current_angles = M::CalcAngle(center, pLocal->GetEyePosition());

	Vector forward;
	M::AngleVectors(current_angles, &forward);

	auto top = Vector(0.0f, 0.0f, 1.0f);
	auto bottom = Vector(0.0f, 0.0f, -1.0f);

	auto right = forward.CrossProduct(Vector(0.0f, 0.0f, 1.0f));
	auto left = Vector(-right.x, -right.y, right.z);

	auto top_right = top + right;
	auto top_left = top + left;

	auto bottom_right = bottom + right;
	auto bottom_left = bottom + left;

	auto angle = M::CalcAngle(pEntity->GetEyePosition(), pLocal->GetAbsOrigin()).y;
	auto delta = M::NormalizeYaw(pEntity->GetEyeAngles().y - angle);


	switch (iHitbox)
	{
	default:
		auto multi_points_scale = 0.2f + 0.6f * 0.6f;
		Points.push_back(Point(center));
		Points.push_back(Point(center + right * Hitbox->flRadius * multi_points_scale));
		Points.push_back(Point(center + left * Hitbox->flRadius * multi_points_scale));
		Points.push_back(Point(center + top_right * Hitbox->flRadius * multi_points_scale));
		Points.push_back(Point(center + top_left * Hitbox->flRadius * multi_points_scale));
		Points.push_back(Point(center + bottom_left * Hitbox->flRadius * multi_points_scale));
		Points.push_back(Point(center + bottom_right * Hitbox->flRadius * multi_points_scale));
		Points.push_back(Point(center + bottom * Hitbox->flRadius * multi_points_scale));
		Points.push_back(Point(center + top * Hitbox->flRadius * multi_points_scale));
		break;
	}
}