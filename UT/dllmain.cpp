// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "hook.h"
#include "openGL/glDraw.h"

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);
twglSwapBuffers swapBuffersGateway;

BOOL __stdcall hkglSwapBuffers(HDC hDc)
{
	static bool isContextCreated = false;
	static HGLRC hkContext;

	HGLRC oContext = wglGetCurrentContext();

	if (!isContextCreated)
	{
		hkContext = wglCreateContext(hDc);
		wglMakeCurrent(hDc, hkContext);

		GL::SetupOrtho();
		isContextCreated = true;
	}

	wglMakeCurrent(hDc, hkContext);

	GL::DrawLine(0, 0, 500, 500, color::red);

	wglMakeCurrent(hDc, oContext);
	return swapBuffersGateway(hDc);
}

void WINAPI MainThread(HMODULE hModule)
{
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

	std::cout << "Initialization OK" << std::endl;

	Hook swapBuffersHook("opengl32.dll", "wglSwapBuffers", (BYTE*)hkglSwapBuffers, (BYTE*)&swapBuffersGateway, 5);
	swapBuffersHook.Apply();

	while (true)
	{
		if (GetAsyncKeyState(VK_F1) & 1) break;
		Sleep(1);
	}

	swapBuffersHook.Restore();
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, NULL);
	}

	return TRUE;
}