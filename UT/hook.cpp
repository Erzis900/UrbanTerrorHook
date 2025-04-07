#include "pch.h"
#include "hook.h"

Hook::Hook(LPCSTR moduleName, LPCSTR functionName, BYTE* dst, BYTE* gatewayPtr, int len)
{
	HMODULE hMod = GetModuleHandleA(moduleName);
	if (hMod)
	{
		m_src = (BYTE*)GetProcAddress(hMod, functionName);
		m_dst = dst;
		m_len = len;
		m_gateway = gatewayPtr;
		m_original.resize(m_len);
	}
}

void Hook::Apply()
{
	std::memcpy(m_original.data(), m_src, m_len);
	*(uintptr_t*)m_gateway = (uintptr_t)TrampolineHook32(m_src, m_dst, m_len);
}

void Hook::Restore()
{
	RestoreBytes(m_src, m_original.data(), m_len);
}

bool Hook::Detour32(BYTE* src, BYTE* dst, int len)
{
	if (len < 5) return false;

	DWORD oldProtect;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);

	uintptr_t relativeAddress = dst - src - 5;
	*src = 0xE9;

	*(uintptr_t*)(src + 1) = relativeAddress;

	VirtualProtect(src, len, oldProtect, &oldProtect);

	return true;
}

BYTE* Hook::TrampolineHook32(BYTE* src, BYTE* dst, int len)
{
	if (len < 5) return nullptr;

	BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	std::memcpy(gateway, src, len);

	uintptr_t relativeAddress = src - gateway - 5;

	*(gateway + len) = 0xE9;
	*(uintptr_t*)(gateway + len + 1) = relativeAddress;

	Detour32(src, dst, len);

	return gateway;
}

void Hook::RestoreBytes(BYTE* src, BYTE* gateway, int len)
{
	DWORD oldProtect;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);

	std::memcpy(src, gateway, len);

	VirtualProtect(src, len, oldProtect, &oldProtect);
}