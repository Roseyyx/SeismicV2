#include "Hooking.h"
#include "../../Includes/Common.h"
#include "../../Utils/Log/Log.h"
#include "../Menu/Menu.h"
#include "../Prediction/Prediction.h"
#include "../Aim Assistance/Aim Assistance.h"

bool Hooks::Setup() {

	if (MH_Initialize() != MH_OK)
		return false;

	if (CreateMoveProxy.create(MEM::GetVFunc(Interfaces::Client, 22), &hkCreateMoveProxy))
		LOG("CreateMove->Hooked");

	if (PaintTraverse.create(MEM::GetVFunc(Interfaces::Panel, 41), &hkPaintTraverse))
		LOG("PaintTraverse->Hooked");
	
	return true;
}

void Hooks::Restore() {
	CreateMoveProxy.remove();
	PaintTraverse.remove();

	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

static void __stdcall CreateMove(int nSequenceNumber, float flInputSampleFrametime, bool bIsActive, bool& bSendPacket) {

	auto oCreateMove = Hooks::CreateMoveProxy.get_original<decltype(&Hooks::hkCreateMoveProxy)>();

	oCreateMove(Interfaces::Client, 0, nSequenceNumber, flInputSampleFrametime, bIsActive);

	CUserCmd* pCmd = Interfaces::Input->GetUserCmd(nSequenceNumber);
	CVerifiedUserCmd* pVerifiedCmd = Interfaces::Input->GetVerifiedCmd(nSequenceNumber);
	
	if (pCmd == nullptr || pVerifiedCmd == nullptr || !bIsActive)
		return;

	Common::pCmd = pCmd;
	
	CBaseEntity* pLocal = Common::pLocal = CBaseEntity::GetLocalPlayer();
	INetChannel* pNetChannel = Interfaces::ClientState->pNetChannel;
	QAngle angOldViewPoint = pCmd->ViewAngles;

	if (Interfaces::ClientState->iDeltaTick > 0)
		Interfaces::Prediction->Update(Interfaces::ClientState->iDeltaTick, Interfaces::ClientState->iDeltaTick > 0, Interfaces::ClientState->iLastCommandAck, Interfaces::ClientState->iLastOutgoingCommand + Interfaces::ClientState->nChokedCommands);
	
	CPrediction::Get().Start(pCmd, pLocal); {
		AimAssistance::Get().Run(pLocal, pCmd);
	} CPrediction::Get().End(pCmd, pLocal);

	pVerifiedCmd->userCmd = *pCmd;
	pVerifiedCmd->uHashCRC = pCmd->GetChecksum();
}

__declspec(naked) void __fastcall Hooks::hkCreateMoveProxy([[maybe_unused]] IBaseClientDll* thisptr, [[maybe_unused]] int edx, [[maybe_unused]] int nSequenceNumber, [[maybe_unused]] float flInputSampleFrametime, [[maybe_unused]] bool bIsActive)
{
	__asm
	{
		push	ebp
		mov		ebp, esp; // store the stack
		push	ebx; // bSendPacket
		push	esp; // restore the stack
		push	dword ptr[bIsActive]; // ebp + 16
		push	dword ptr[flInputSampleFrametime]; // ebp + 12
		push	dword ptr[nSequenceNumber]; // ebp + 8
		call	CreateMove
			pop		ebx
			pop		ebp
			retn	0Ch
	}
}

void __fastcall Hooks::hkPaintTraverse(ISurface* thisptr, int edx, unsigned int Panel, bool ForceRepaint, bool AllowForce) {

	auto oPaintTraverse = PaintTraverse.get_original<decltype(&hkPaintTraverse)>();
	const FNV1A_t PanelHash = FNV1A::Hash(Interfaces::Panel->GetName(Panel));

	oPaintTraverse(thisptr, edx, Panel, ForceRepaint, AllowForce);

	if (PanelHash == FNV1A::HashConst("MatSystemTopPanel")) {
		Menu::Toggle();

		std::wstring build = L"Seismic | build: " __DATE__ " " __TIME__;
		Renderer::DrawTexts(10, 10, Color::white(), Renderer::Fonts::Default, build.c_str());
	}
	else if (PanelHash == FNV1A::HashConst("FocusOverlayPanel")) {
		Interfaces::Panel->SetKeyBoardInputEnabled(Panel, Config::MenuSettings::IsOpened);
		Interfaces::Panel->SetMouseInputEnabled(Panel, Config::MenuSettings::IsOpened);
	}

}