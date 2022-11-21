#pragma once

#include "../Structs/Convar.h"

#include "../Interfaces/IBaseClientDll.h"
#include "../Interfaces/ISurface.h"
#include "../Interfaces/IClientEntityList.h"
#include "../Interfaces/IClientModeShared.h"
#include "../Interfaces/IEngineTrace.h"
#include "../Interfaces/IVModelInfo.h"
#include "../Interfaces/IVPanel.h"
#include "../Interfaces/IVEngineClient.h"
#include "../Interfaces/NetChannel.h"
#include "../Interfaces/IConvar.h"
#include "../Interfaces/IGlobalVars.h"
#include "../Interfaces/IGameTypes.h"
#include "../Interfaces/IPrediction.h"
#include "../Interfaces/IMdlCache.h"
#include "../Interfaces/Iinput.h"
#include "../Interfaces/IKeyValueSystem.h"
#include "../Interfaces/IMaterialSystem.h"
#include "../Interfaces/IPhysicsSurfaceProps.h"
#include "../Interfaces/IWeaponSystem.h"
#include "../Interfaces/IGameTypes.h"

class CInterface {
public:
	InstantiateInterface Interface; //0x0000
	char* InterfaceName; //0x0004 
	CInterface* NextInterface; //0x0008 
};

namespace Interfaces {
	// Setup
	void Setup();
	// get interface with type
	template <typename T>
	T* GetInterface(const char* module, const char* interfaceName);

	inline IBaseClientDll* Client;
	inline IClientModeShared* ClientMode;
	inline IClientEntityList* EntityList;
	inline IEngineTrace* EngineTrace;
	inline IVModelInfo* ModelInfo;
	inline ISurface* Surface;
	inline IVPanel* Panel;
	inline IEngineClient* Engine;
	inline INetChannel* NetChannel;
	inline IConVar* ConVar;
	inline IGlobalVarsBase* GlobalVars;
	inline IGameTypes* GameTypes;
	inline IPrediction* Prediction;
	inline IMoveHelper* MoveHelper;
	inline IGameMovement* GameMovement;
	inline IMDLCache* MdlCache;
	inline IInput* Input;
	inline IClientState* ClientState;
	inline IKeyValuesSystem* KeyValuesSystem;
	inline IMaterialSystem* MaterialSystem;
	inline IPhysicsSurfaceProps* PhysicsProps;
	inline IWeaponSystem* WeaponSystem;
}