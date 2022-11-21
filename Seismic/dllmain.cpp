#include "Includes/General.h"
#include "Includes/Common.h"
#include "Utils/Log/Log.h"
#include "Core/Hooks/Hooking.h"
#include "Core/Menu/Menu.h"

BOOL WINAPI Detach() {

    Hooks::Restore();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
    Logging::detach();

    return TRUE;
}

DWORD WINAPI OnDllAttach(LPVOID lpParameter) {
    try
    {
        while (GetModuleHandle("serverbrowser.dll") == nullptr)
            Sleep(100);

        LOG("Seismic Injected!");

        Interfaces::Setup();

        CNetvarManager::Get().Setup("k.txt");

        Renderer::Initialize();

        if (Hooks::Setup())
            LOG("Hooks setup!");


        while (!GetAsyncKeyState(VK_END))
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    catch (const std::exception& err)
    {
        LOG(err.what());
		
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    LOG("Cheat Detached!");

    Detach();
	
    FreeLibraryAndExitThread(static_cast<HMODULE>(lpParameter), EXIT_SUCCESS);
}


BOOL APIENTRY DllMain(HINSTANCE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        if (auto hTread = CreateThread(nullptr, NULL, OnDllAttach, hModule, NULL, nullptr); hTread != nullptr)
            CloseHandle(hTread);
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH && !lpReserved) {
		return Detach();
	}
    return TRUE;
}

