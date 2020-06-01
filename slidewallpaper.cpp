// @brief: multi-monitor slide-like wallpaper
// @copyright: T.C.V.
// @license: MIT
// @birth: created by Infinite Synthesis on 2020 May 25
// @version: V0.1.0

#include <io.h>
#include <shobjidl.h>
#include <stdio.h>
#include <windows.h>

#include <algorithm>
#include <codecvt>
#include <locale>
#include <string>
#include <vector>

#include "ctime"

#define FILE_FOLDER_PATH YOUR_GALLERY_FOLDERS_HERE
// e.g. { "E:\\vertical pixiv\\", "E:\\background\\" }
#define TIME_INTERVAL (60 * 1000)
#define MAX_FILE_COUNT 4000
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

void getTargetFile(const std::string& path,
                   float rand,
                   std::string& targetFilePath);
bool IsImageByTail(const std::string& path);

IDesktopWallpaper* pDesktopWallpaper;
std::vector<std::string> picFolderPath = FILE_FOLDER_PATH;
const char* GBK_LOCALE_NAME = ".936";
std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> Conver_GBK(
    new std::codecvt_byname<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));

void TimeProc() {
  UINT monitor_count = 0;
  std::string fullPath = "";
  LPWSTR monitorID = (LPWSTR)L"";
  LPCWSTR wallpaper = L"";
  pDesktopWallpaper->GetMonitorDevicePathCount(&monitor_count);
  for (size_t i = 0; i < min(monitor_count, picFolderPath.size()); ++i) {
    getTargetFile(picFolderPath[i],
                  (rand() % (MAX_FILE_COUNT + 1) / (float)(MAX_FILE_COUNT + 1)),
                  fullPath);
    std::wstring stemp = Conver_GBK.from_bytes(fullPath);
    wallpaper = stemp.c_str();
    pDesktopWallpaper->GetMonitorDevicePathAt((UINT)i, &monitorID);
    if (FAILED(pDesktopWallpaper->SetWallpaper(monitorID, wallpaper))) {
      LOG(L"IDesktopWallpaper::SetWallpaper failed with %s", wallpaper);
    }
  }
}

int _cdecl main(int argc, LPCWSTR argv[]) {
  srand(time(NULL));
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

  int iId = SetTimer(NULL, 0, TIME_INTERVAL, NULL);
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    if (msg.message == WM_TIMER) {
      TimeProc();
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  KillTimer(NULL, iId);
  return 0;
}

void getTargetFile(const std::string& path,
                   float rand,
                   std::string& targetFilePath) {
  long long hFileCount = 0;   // fast to traverse the folder
  long long hFileSelect = 0;  // slow to select the target wallpapaer file
  struct _finddata_t fileinfoCount;
  struct _finddata_t fileinfoSelect;
  std::string p = path + "\\*";

  unsigned int count = 0;
  unsigned int select = 0;
  if (((hFileCount = _findfirst(p.c_str(), &fileinfoCount)) != -1) &&
      ((hFileSelect = _findfirst(p.c_str(), &fileinfoSelect)) != -1)) {
    // select move to first image to prevent ../.
    while (_findnext(hFileSelect, &fileinfoSelect) == 0) {
      if (!(fileinfoSelect.attrib & _A_SUBDIR) &&
          IsImageByTail(p.assign(fileinfoSelect.name))) {
        break;
      }
    }

    // start traverse
    do {
      if (!(fileinfoCount.attrib & _A_SUBDIR) &&
          IsImageByTail(p.assign(fileinfoCount.name))) {
        count++;
        if (unsigned(count * rand) > select) {
          // find the next select target, break immediately
          while (_findnext(hFileSelect, &fileinfoSelect) == 0) {
            if (!(fileinfoSelect.attrib & _A_SUBDIR) &&
                IsImageByTail(p.assign(fileinfoSelect.name))) {
              break;
            }
          }
          select++;
        }
      }
    } while (_findnext(hFileCount, &fileinfoCount) == 0 &&
             count < MAX_FILE_COUNT);
    targetFilePath = p.assign(path).append(fileinfoSelect.name);
    _findclose(hFileCount);
    _findclose(hFileSelect);
  }
}

bool IsImageByTail(const std::string& path) {
  std::string file_exten = path.substr(path.find_last_of('.') + 1);
  transform(file_exten.begin(), file_exten.end(), file_exten.begin(),
            ::tolower);

  return (file_exten == "jpg" || file_exten == "tif" || file_exten == "png" ||
          file_exten == "bmp" || file_exten == "gif" || file_exten == "jfif");
}