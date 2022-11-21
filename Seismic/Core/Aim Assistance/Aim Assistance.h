#pragma once

#include "../../Includes/Common.h"

class Point {
public:
	Point() {
		this->AimPoint = Vector(0, 0, 0);
	}
	Point(Vector Aimpoint) {
		this->AimPoint = Aimpoint;
	}

	void Clear() {
		this->AimPoint = Vector(0, 0, 0);
	}

	Vector AimPoint;
};

class AimAssistance : public CSingleton<AimAssistance> {
private:
	std::vector<CBaseEntity*> Targets;
	std::vector<Vector> Points;
public:
	void Run(CBaseEntity* pLocal, CUserCmd* pCmd);
	void GetTargets(CBaseEntity* pLocal);
	std::vector<CBaseEntity*> GetBestTarget(CBaseEntity* pLocal);
	std::vector<int> GetHitboxes();
	void GetPoints(CBaseEntity* pLocal, CBaseEntity* pEntity, int iHitbox, std::vector<Point>& Points);
};
