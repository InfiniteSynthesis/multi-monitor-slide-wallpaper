// @brief: multi-monitor slide-like wallpaper
// @copyright: T.C.V.
// @license: MIT
// @birth: created by Infinite Synthesis on 2020 May 25
// @version: V0.0.1

#include <io.h>
#include <shobjidl.h>
#include <stdio.h>
#include <windows.h>

#include <string>
#include <vector>

#define FILE_FOLDER_PATH \
  { YOUR_OWN_FOLDER_HERE }
// e.g.{ "E:\\vertical\\", "E:\\horizontal\\" }
#define TIME_INTERVAL (30 * 1000)  // replace interval
#define LOG(format, ...) wprintf(format L"\n", __VA_ARGS__)

class CoUninitializeOnExit {
 public:
  CoUninitializeOnExit() {}
  ~CoUninitializeOnExit() { CoUninitialize(); }
};

class ReleaseOnExit {
 public:
  ReleaseOnExit(IUnknown* p) : m_p(p) {}
  ~ReleaseOnExit() {
    if (nullptr != m_p) {
      m_p->Release();
    }
  }

 private:
  IUnknown* m_p;
};

void getFiles(const std::string& path, std::vector<std::string>& files);

IDesktopWallpaper* pDesktopWallpaper;
std::vector<std::string> picFolderPath;
std::vector<std::vector<std::string>> picFiles;

void CALLBACK _TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime) {
  UINT monitor_count = 0;
  LPWSTR monitorID = (LPWSTR)L"";
  LPCWSTR wallpaper = L"";
  pDesktopWallpaper->GetMonitorDevicePathCount(&monitor_count);
  for (size_t i = 0; i < min(monitor_count, picFolderPath.size()); ++i) {
    std::string fullPath =
        picFolderPath[i] + picFiles[i][rand() % (picFiles[i].size())];
    std::wstring stemp = std::wstring(fullPath.begin(), fullPath.end());
    wallpaper = stemp.c_str();
    pDesktopWallpaper->GetMonitorDevicePathAt((UINT)i, &monitorID);
    if (FAILED(pDesktopWallpaper->SetWallpaper(monitorID, wallpaper))) {
      LOG(L"IDesktopWallpaper::SetWallpaper returned");
    }
  }
}

int _cdecl main(int argc, LPCWSTR argv[]) {
  picFolderPath = FILE_FOLDER_PATH;
  picFiles.resize(picFolderPath.size());
  for (size_t i = 0; i < picFolderPath.size(); ++i) {
    getFiles(picFolderPath[i], picFiles[i]);
  }
  HRESULT hr = CoInitialize(nullptr);
  if (FAILED(hr)) {
    LOG(L"CoInitialize returned 0x%08x", hr);
    return -__LINE__;
  }
  CoUninitializeOnExit cuoe;

  pDesktopWallpaper = nullptr;
  hr = CoCreateInstance(__uuidof(DesktopWallpaper), nullptr, CLSCTX_ALL,
                        IID_PPV_ARGS(&pDesktopWallpaper));
  if (FAILED(hr)) {
    LOG(L"CoCreateInstance(__uuidof(DesktopWallpaper)) returned 0x%08x", hr);
    return -__LINE__;
  }
  ReleaseOnExit releaseDesktopWallpaper(pDesktopWallpaper);

  int iId = SetTimer(NULL, 0, TIME_INTERVAL, _TimerProc);
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    DispatchMessage(&msg);
  }
  KillTimer(NULL, iId);
  return 0;
}

void getFiles(const std::string& path, std::vector<std::string>& files) {
  long long hFile = 0;
  struct _finddata_t fileinfo;
  std::string p;
  int i = 0;
  if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) !=
      -1) {
    do {
      if ((fileinfo.attrib & _A_SUBDIR)) {
      } else {
        files.push_back(p.assign(fileinfo.name));
      }
    } while (_findnext(hFile, &fileinfo) == 0);
    _findclose(hFile);
  }
}