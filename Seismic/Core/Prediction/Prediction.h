#pragma once

#include "../../Includes/General.h"
#include "../../sdk/DataTypes/UserCMD.h"
#include "../../sdk/Interfaces/IPrediction.h"
#include "../../sdk/Structs/Entity.h"

class CPrediction : public CSingleton<CPrediction>
{
public:
	CPrediction()
	{
		uPredictionRandomSeed = *reinterpret_cast<unsigned int**>(MEM::FindPattern("client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 0x2); // @xref: "SelectWeightedSequence"
		pPredictionPlayer = *reinterpret_cast<CBaseEntity***>(MEM::FindPattern("client.dll", "89 35 ? ? ? ? F3 0F 10 48 20") + 0x2);
	}

	// Get
	void Start(CUserCmd* pCmd, CBaseEntity* pLocal);
	void End(CUserCmd* pCmd, CBaseEntity* pLocal) const;

	/*
	 * required cuz prediction works on frames, not ticks
	 * corrects tickbase if your framerate goes below tickrate and m_nTickBase won't update every tick
	 */
	static int GetTickBase(CUserCmd* pCmd, CBaseEntity* pLocal);
private:
	// Values
	/* prediction seed */
	unsigned int* uPredictionRandomSeed = nullptr;
	/* current predictable player */
	CBaseEntity** pPredictionPlayer = nullptr;
	/* encapsulated input parameters to player movement */
	CMoveData moveData = { };

	/* backup */
	float flOldCurrentTime = 0.f;
	float flOldFrameTime = 0.f;
	int iOldTickCount = 0;
};
