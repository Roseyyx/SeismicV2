#include "Interfaces.h"
#include "../../Utils/Log/Log.h"

void Interfaces::Setup() {
	Client = GetInterface<IBaseClientDll>("client.dll", "VClient");
	EntityList = GetInterface<IClientEntityList>("client.dll", "VClientEntityList");
	ClientMode = **reinterpret_cast<IClientModeShared***>((*reinterpret_cast<uintptr_t**>(Client))[10] + 0x5);
	EngineTrace = GetInterface<IEngineTrace>("engine.dll", "EngineTraceClient");
	ModelInfo = GetInterface<IVModelInfo>("engine.dll", "VModelInfoClient");
	Surface = GetInterface<ISurface>("vguimatsurface.dll", "VGUI_Surface");
	Panel = GetInterface<IVPanel>("vgui2.dll", "VGUI_Panel");
	Engine = GetInterface<IEngineClient>("engine.dll", "VEngineClient");
	ConVar = GetInterface<IConVar>("vstdlib.dll", "VEngineCvar");
	GlobalVars = **reinterpret_cast<IGlobalVarsBase***>(MEM::GetVFunc<std::uintptr_t>(Client, 11) + 0xA);
	Prediction = GetInterface<IPrediction>("client.dll", "VClientPrediction");
	GameMovement = **reinterpret_cast<IGameMovement***>(MEM::GetVFunc<std::uintptr_t>(Prediction, 1) + 0x1C);	
	MdlCache = GetInterface<IMDLCache>("datacache.dll", "MDLCache");
	Input = *reinterpret_cast<IInput**>(MEM::FindPattern("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 0x1);
	ClientState = **reinterpret_cast<IClientState***>(MEM::FindPattern("engine.dll", "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07") + 0x1);
	KeyValuesSystem = GetInterface<IKeyValuesSystem>("vstdlib.dll", "VKeyValuesSystem");
	MaterialSystem = GetInterface<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");
	PhysicsProps = GetInterface<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps");
	WeaponSystem = *reinterpret_cast<IWeaponSystem**>(MEM::FindPattern("client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0") + 0x2);
	GameTypes = GetInterface<IGameTypes>("matchmaking.dll", "VENGINE_GAMETYPES_VERSION");
}

template <typename T>
T* Interfaces::GetInterface(const char* module, const char* interfaceName){
	ULONG CreateInterface = (ULONG)GetProcAddress(GetModuleHandle(module), "CreateInterface");

	ULONG ShortJump = (ULONG)CreateInterface + 5; //magic number shit explained above

	ULONG Jump = (((ULONG)CreateInterface + 5) + *(ULONG*)ShortJump) + 4;

	CInterface* List = **(CInterface***)(Jump + 6);

	do {
		if (List) {
			if (strstr(List->InterfaceName, interfaceName) && (strlen(List->InterfaceName) - strlen(interfaceName)) < 5 /*arbitrary number to see that both names aren't too different*/) {
				return static_cast<T*>(List->Interface());
			}
		}
	} while (List = List->NextInterface);

	return 0;
}