#pragma once

#include "resource.h"

#include "MyFileInfo.cpp"
#include <string>
#include <filesystem>
#include <vector>

#define MAX_LOADSTRING 100
#define TIMER_ID 1
#define TIMER_TIME 5000

// Global Variables:
HINSTANCE               hInst;                                // current instance
WCHAR                   szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR                   szWindowClass[MAX_LOADSTRING];            // the main window class name
std::string             inputFolderPath = "C:\\Users\\User\\Desktop\\ABC\\coursework\\CourseProj\\Test\\";
std::string             extension = "*json";
std::string             subExtension = extension.substr(1, extension.size());
std::wstring            stopStr = L"Stop." + std::wstring(subExtension.begin(),subExtension.end());
std::vector<MyFileInfo> myFilesInfo;
bool                    stop = false;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void                GetFilesList(std::string filePath,
                        std::string extension,
                        std::vector<MyFileInfo>& myFilesInfo);
VOID WINAPI         CompletionRoutine(
                        DWORD dwErrorCode,               // код завершения 
                        DWORD dwNumberOfBytesTransfered, // количество прочитанных 
                        LPOVERLAPPED lpOverlapped);  // адрес структуры OVERLAPPED 
MyFileInfo          CreateMyInfo(WIN32_FIND_DATA fileInfo,
                        std::wstring filename,
                        std::wstring fullFileName);
void                Encrypt(std::vector<MyFileInfo>& myFilesInfo);
void                Decrypt(std::vector<MyFileInfo>& myFilesInfo);
LPCTSTR             ErrorMessage(DWORD error);
bool                IsNewFile(std::vector<MyFileInfo> myFilesInfo, std::wstring fullFileName);
