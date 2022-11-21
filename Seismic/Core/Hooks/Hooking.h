#pragma once

#include "../../Includes/General.h"

#include "../../Utils/Netvars/Netvar.h"
#include "../../sdk/Helpers/Interfaces.h"

#include "../../minhook/minhook.h"

class MinDetoursHook
{
public:
	MinDetoursHook() = default;
	explicit MinDetoursHook(void* pFunction, void* pDetour)
		: pBaseFn(pFunction), pReplaceFn(pDetour) { }
	~MinDetoursHook() {
		this->remove();
	}
	bool create(void* pFunction, void* pDetour) {
		pBaseFn = pFunction;
		if (pBaseFn == nullptr)
			return false;
		pReplaceFn = pDetour;
		if (pReplaceFn == nullptr)
			return false;

		const MH_STATUS status = MH_CreateHook(pBaseFn, pReplaceFn, &pOriginalFn);

		if (status != MH_OK)
			return false;

		if (!this->replace())
			return false;
		
		return true;
	}
	bool remove() {
		if (!this->restore())
			return false;
		
		const MH_STATUS status = MH_RemoveHook(pBaseFn);
		
		if (status != MH_OK)
			return false;
			
		return true;
	}
	bool restore()
	{
		// check is function hooked
		if (!bIsHooked)
			return false;

		const MH_STATUS status = MH_DisableHook(pBaseFn);

		if (status != MH_OK)
			return false;

		// switch hook state
		bIsHooked = false;
		return true;
	}
	bool replace() {
		// check is hook be created
		if (pBaseFn == nullptr)
			return false;

		// check is function not already hooked
		if (bIsHooked)
			return false;

		const MH_STATUS status = MH_EnableHook(pBaseFn);

		if (status != MH_OK)
			return false;

		// switch hook state
		bIsHooked = true;
		return true;
	}

	template <typename Fn>
	Fn get_original() {
		return (Fn)pOriginalFn;
	}
	
private:
	bool bIsHooked = false;
	void* pBaseFn = nullptr;
	void* pReplaceFn = nullptr;
	void* pOriginalFn = nullptr;
};


namespace Hooks {

	bool Setup();
	void Restore();
	
	
	inline MinDetoursHook CreateMoveProxy;
	inline MinDetoursHook PaintTraverse;
	

	void __fastcall	hkCreateMoveProxy(IBaseClientDll* thisptr, int edx, int nSequenceNumber, float flInputSampleFrametime, bool bIsActive);
	void __fastcall hkPaintTraverse(ISurface* thisptr, int edx, unsigned int Panel, bool ForceRepaint, bool AllowForce);
}
