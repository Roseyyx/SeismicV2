#pragma once

typedef void* (*InstantiateInterface)();

class CStandartRecvProxies;
struct RecvTable_t;
enum class EClassIndex;

class ClientClass {
public:
	void* CreateFn;
	void* CreateEventFn;
	char* NetworkName;
	RecvTable_t* RecvTable;
	ClientClass* NextClass;
	EClassIndex ClassID;
};

class IBaseClientDll {
public:
	ClientClass* GetAllClasses() {
		return MEM::CallVFunc<ClientClass*>(this, 8);
	}

	CStandartRecvProxies* GetStandardRecvProxies()
	{
		return MEM::CallVFunc<CStandartRecvProxies*>(this, 48);
	}
};