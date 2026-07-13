#undef UNICODE
#undef _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>
#include <ShellApi.h>
#include <cstdio>
#include <string>
#include <vector>

static auto FindProcessId(const char* szName) -> DWORD
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) return 0;
	PROCESSENTRY32 pe = { sizeof(pe) };
	if (Process32First(hSnap, &pe))
	{
		do {
			if (_stricmp(pe.szExeFile, szName) == 0)
			{
				CloseHandle(hSnap);
				return pe.th32ProcessID;
			}
		} while (Process32Next(hSnap, &pe));
	}
	CloseHandle(hSnap);
	return 0;
}

static auto IsProcessAlive(DWORD dwPid) -> bool
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwPid);
	if (!hProcess) return false;
	DWORD dwExitCode = 0;
	BOOL bResult = GetExitCodeProcess(hProcess, &dwExitCode);
	CloseHandle(hProcess);
	return bResult && dwExitCode == STILL_ACTIVE;
}

static auto IsModuleLoaded(DWORD dwPid, const char* szModuleName) -> bool
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwPid);
	if (hSnap == INVALID_HANDLE_VALUE) return false;
	MODULEENTRY32 me = { sizeof(me) };
	if (Module32First(hSnap, &me))
	{
		do {
			if (_stricmp(me.szModule, szModuleName) == 0)
			{
				CloseHandle(hSnap);
				return true;
			}
		} while (Module32Next(hSnap, &me));
	}
	CloseHandle(hSnap);
	return false;
}

static auto InjectDll(DWORD dwProcessId, const char* szDllPath) -> bool
{
	HANDLE hProcess = OpenProcess(
		PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
		FALSE, dwProcessId);

	if (!hProcess) return false;

	for (int i = 0; i < 30; i++)
	{
		if (IsModuleLoaded(dwProcessId, "client.dll")) break;
		Sleep(1000);
	}

	void* pRemoteMem = VirtualAllocEx(hProcess, NULL, strlen(szDllPath) + 1,
		MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pRemoteMem) { CloseHandle(hProcess); return false; }

	WriteProcessMemory(hProcess, pRemoteMem, szDllPath, strlen(szDllPath) + 1, NULL);

	HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
	FARPROC pLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)pLoadLibraryA, pRemoteMem, 0, NULL);

	if (!hThread)
	{
		VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return false;
	}

	WaitForSingleObject(hThread, 15000);
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	return true;
}

int main()
{
	SetConsoleTitleA("EggLock");

	char szDllPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szDllPath, MAX_PATH);
	std::string s(szDllPath);
	auto pos = s.find_last_of('\\');
	if (pos != std::string::npos) s = s.substr(0, pos + 1);
	s += "EggLock.dll";

	if (GetFileAttributesA(s.c_str()) == INVALID_FILE_ATTRIBUTES)
	{
		printf("EggLock.dll not found!\n");
		system("pause");
		return 1;
	}

	printf("\n");
	printf("  ============================================\n");
	printf("             EGGLOCK - Deadlock\n");
	printf("  ============================================\n\n");

	DWORD dwPid = FindProcessId("deadlock.exe");

	if (dwPid == 0)
	{
		printf("  Launching Deadlock...\n");
		ShellExecuteA(NULL, "open", "steam://rungameid/1422450", NULL, NULL, SW_SHOWNORMAL);

		printf("  Waiting for game");
		for (int i = 0; i < 60; i++)
		{
			Sleep(1000);
			dwPid = FindProcessId("deadlock.exe");
			if (dwPid != 0) break;
			if ((i + 1) % 5 == 0) printf(".");
		}
		printf("\n");
	}

	if (dwPid == 0)
	{
		printf("  Game not found!\n");
		system("pause");
		return 1;
	}

	printf("  Game found (PID: %lu)\n", dwPid);
	printf("  Injecting...");

	if (InjectDll(dwPid, s.c_str()))
	{
		printf(" OK!\n\n");
		printf("  Press Insert in-game to open menu.\n\n");
	}
	else
	{
		printf(" FAILED!\n");
	}

	system("pause");
	return 0;
}
