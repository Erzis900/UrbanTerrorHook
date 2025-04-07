#pragma once

class Hook
{
public:
	Hook(LPCSTR moduleName, LPCSTR functionName, BYTE* dst, BYTE* gatewayPtr, int len);

	void Apply();
	void Restore();
private:
	bool Detour32(BYTE* src, BYTE* dst, int len);
	BYTE* TrampolineHook32(BYTE* src, BYTE* dst, int len);
	void RestoreBytes(BYTE* src, BYTE* gateway, int len);

	BYTE* m_src;
	BYTE* m_dst;
	int m_len;

	BYTE* m_gateway;
	std::vector<BYTE> m_original;
};