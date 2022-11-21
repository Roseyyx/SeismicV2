#include "Prediction.h"
#include "../../Utils/Log/Log.h"
#include "../../sdk/Hash/md5.h"
#include "../../sdk/Helpers/Interfaces.h"

void CPrediction::Start(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	// @xref: "CPrediction::ProcessMovement"

	if (!pLocal->IsAlive() || Interfaces::MoveHelper == nullptr)
		return;

	// start command
	*pLocal->GetCurrentCommand() = pCmd;
	pLocal->GetLastCommand() = *pCmd;

	// random seed was already generated in CInput::CreateMove
	*uPredictionRandomSeed = pCmd->iRandomSeed;
	// set ourselves as a predictable entity
	*pPredictionPlayer = pLocal;

	// backup globals
	flOldCurrentTime = Interfaces::GlobalVars->flCurrentTime;
	flOldFrameTime = Interfaces::GlobalVars->flFrameTime;
	iOldTickCount = Interfaces::GlobalVars->iTickCount;

	// backup tickbase
	const int iOldTickBase = pLocal->GetTickBase();

	// backup prediction states
	const bool bOldIsFirstPrediction = Interfaces::Prediction->bIsFirstTimePredicted;
	const bool bOldInPrediction = Interfaces::Prediction->bInPrediction;

	// set corrected values
	Interfaces::GlobalVars->flCurrentTime = TICKS_TO_TIME(GetTickBase(pCmd, pLocal));
	Interfaces::GlobalVars->flFrameTime = Interfaces::Prediction->bEnginePaused ? 0.f : TICK_INTERVAL;
	Interfaces::GlobalVars->iTickCount = GetTickBase(pCmd, pLocal);

	Interfaces::Prediction->bIsFirstTimePredicted = false;
	Interfaces::Prediction->bInPrediction = true;

	/* skipped weapon select and vehicle predicts */

	// synchronize m_afButtonForced & m_afButtonDisabled
	pCmd->iButtons |= pLocal->GetButtonForced();
	pCmd->iButtons &= ~(pLocal->GetButtonDisabled());

	Interfaces::GameMovement->StartTrackPredictionErrors(pLocal);

	// update button state
	const int iButtons = pCmd->iButtons;
	const int nLocalButtons = *pLocal->GetButtons();
	const int nButtonsChanged = iButtons ^ nLocalButtons;

	// synchronize m_afButtonLast
	pLocal->GetButtonLast() = nLocalButtons;

	// synchronize m_nButtons
	*pLocal->GetButtons() = iButtons;

	// synchronize m_afButtonPressed
	pLocal->GetButtonPressed() = nButtonsChanged & iButtons;

	// synchronize m_afButtonReleased
	pLocal->GetButtonReleased() = nButtonsChanged & (~iButtons);

	// check if the player is standing on a moving entity and adjusts velocity and basevelocity appropriately
	Interfaces::Prediction->CheckMovingGround(pLocal, Interfaces::GlobalVars->flFrameTime);

	// copy angles from command to player
	Interfaces::Prediction->SetLocalViewAngles(pCmd->ViewAngles);

	// run prethink
	if (pLocal->PhysicsRunThink(THINK_FIRE_ALL_FUNCTIONS))
		pLocal->PreThink();

	// run think
	if (int* iNextThinkTick = pLocal->GetNextThinkTick(); *iNextThinkTick > 0 && *iNextThinkTick <= GetTickBase(pCmd, pLocal))
	{
		*iNextThinkTick = TICK_NEVER_THINK;

		/*
		 * handle no think function
		 * pseudo i guess didnt seen before but not sure, most likely unnecessary

		nEFlags = pPlayer->GetEFlags();
		result = pPlayer->GetEFlags() & EFL_NO_THINK_FUNCTION;
		if (!result)
		{
			result = [&]()
			{
				if (pPlayer->GetNextThinkTick() > 0)
					return 1;

				v3 = *(_DWORD *)(pPlayer + 0x2BC);
				v4 = 0;
				if (v3 > 0)
				{
				v5 = (_DWORD *)(*(_DWORD *)(pPlayer + 0x2B0) + 0x14);
				while (*v5 <= 0)
				{
					++v4;
					v5 += 8;
					if (v4 >= v3)
						return 0;
				}
				return 1;
			}();

			if (!result)
				pPlayer->GetEFlags() = nEFlags | EFL_NO_THINK_FUNCTION;
		}

		 */

		pLocal->Think();
	}

	// set host player
	Interfaces::MoveHelper->SetHost(pLocal);

	// setup move
	Interfaces::Prediction->SetupMove(pLocal, pCmd, Interfaces::MoveHelper, &moveData);
	Interfaces::GameMovement->ProcessMovement(pLocal, &moveData);

	// finish move
	Interfaces::Prediction->FinishMove(pLocal, pCmd, &moveData);
	Interfaces::MoveHelper->ProcessImpacts();

	// run post think
	pLocal->PostThink();

	// restore tickbase
	pLocal->GetTickBase() = iOldTickBase;

	// restore prediction states
	Interfaces::Prediction->bInPrediction = bOldInPrediction;
	Interfaces::Prediction->bIsFirstTimePredicted = bOldIsFirstPrediction;
}

void CPrediction::End(CUserCmd* pCmd, CBaseEntity* pLocal) const
{
	if (!pLocal->IsAlive() || Interfaces::MoveHelper == nullptr)
		return;

	Interfaces::GameMovement->FinishTrackPredictionErrors(pLocal);

	// reset host player
	Interfaces::MoveHelper->SetHost(nullptr);

	// restore globals
	Interfaces::GlobalVars->flCurrentTime = flOldCurrentTime;
	Interfaces::GlobalVars->flFrameTime = flOldFrameTime;
	Interfaces::GlobalVars->iTickCount = iOldTickCount;

	// finish command
	*pLocal->GetCurrentCommand() = nullptr;

	// reset prediction seed
	*uPredictionRandomSeed = -1;

	// reset prediction entity
	*pPredictionPlayer = nullptr;

	// reset move
	Interfaces::GameMovement->Reset();
}

int CPrediction::GetTickBase(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	static int iTick = 0;

	if (pCmd != nullptr)
	{
		static CUserCmd* pLastCmd = nullptr;

		// if command was not predicted - increment tickbase
		if (pLastCmd == nullptr || pLastCmd->bHasBeenPredicted)
			iTick = pLocal->GetTickBase();
		else
			iTick++;

		pLastCmd = pCmd;
	}

	return iTick;
}
