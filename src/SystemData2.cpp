// SwissArmyKnife.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SystemData2.h"

CNWNXSystemData2 SystemData2;
extern "C" __declspec(dllexport) CNWNXBase* GetClassObject()
{
	return &SystemData2;
}

BOOL APIENTRY DllMain (HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}
