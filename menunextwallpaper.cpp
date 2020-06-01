// @brief: menu next wallpaper
// @copyright: T.C.V.
// @license: MIT
// @birth: created by Infinite Synthesis on 2020 May 26
// @version: V0.1.0

#include <tlhelp32.h>
#include <windows.h>

#include <string>

DWORD FindProcessId(const std::wstring& processName) {
  PROCESSENTRY32 processInfo;
  processInfo.dwSize = sizeof(processInfo);

  HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
  if (processesSnapshot == INVALID_HANDLE_VALUE) {
    return NULL;
  }

  Process32First(processesSnapshot, &processInfo);
  if (!processName.compare(processInfo.szExeFile)) {
    CloseHandle(processesSnapshot);
    return processInfo.th32ProcessID;
  }

  while (Process32Next(processesSnapshot, &processInfo)) {
    if (!processName.compare(processInfo.szExeFile)) {
      CloseHandle(processesSnapshot);
      return processInfo.th32ProcessID;
    }
  }

  CloseHandle(processesSnapshot);
  return NULL;
}

DWORD ListProcessThreads(DWORD dwOwnerPID) {
  HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
  THREADENTRY32 te32;

  hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
  if (hThreadSnap == INVALID_HANDLE_VALUE)
    return NULL;

  te32.dwSize = sizeof(THREADENTRY32);

  if (!Thread32First(hThreadSnap, &te32)) {
    CloseHandle(hThreadSnap);
    return NULL;
  }

  do {
    if (te32.th32OwnerProcessID == dwOwnerPID) {
      CloseHandle(hThreadSnap);
      return te32.th32ThreadID;
    }
  } while (Thread32Next(hThreadSnap, &te32));

  CloseHandle(hThreadSnap);
  return NULL;
}

int main() {
  DWORD targetID =
      ListProcessThreads(FindProcessId(L"MultiMonitorSlideWallpaper.exe"));
  ::PostThreadMessage(targetID, WM_TIMER, 0, 0);
  return 0;
}