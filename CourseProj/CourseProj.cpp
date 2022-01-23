// CourseProj.cpp : Defines the entry point for the application.

#include "pch.h"
#include "framework.h"
#include "CourseProj.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COURSEPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COURSEPROJ));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;// создаём экземпляр для обращения к членам класса WNDCLASSEX

    wcex.cbSize = sizeof(WNDCLASSEX);// размер структуры (в байтах)

    wcex.style = CS_HREDRAW | CS_VREDRAW;// стиль класса окошка
    wcex.lpfnWndProc = WndProc;// указатель на функцию обработки сообщений
    wcex.cbClsExtra = 0; // число дополнительных байтов при создании экземпляра приложения
    wcex.cbWndExtra = 0;// число дополнительных байтов в конце структуры
    wcex.hInstance = hInstance;// указатель на строку, содержащую имя меню, применяемого для класса
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_COURSEPROJ));// декриптор пиктограммы
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);// дескриптор курсора
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);// дескриптор кисти для закраски фона окна
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_COURSEPROJ);// указатель на имя меню (у нас его нет)
    wcex.lpszClassName = szWindowClass;// указатель на имя класса
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));// дескриптор маленькой пиктограммы

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass,  // имя класса
       szTitle, // заголовок окошка
       WS_OVERLAPPEDWINDOW | WS_VSCROLL, // режимы отображения окошка
       CW_USEDEFAULT, // позиция окошка по оси х
       NULL, // позиция окошка по оси у (у нас х по умолчанию и этот параметр писать не нужно)
       CW_USEDEFAULT, // ширина окошка
       NULL, // высота окошка (раз ширина по умолчанию, то писать не нужно)
       (HWND)NULL, // дескриптор родительского окна
       NULL, // дескриптор меню
       HINSTANCE(hInst), // дескриптор экземпляра приложения
       NULL); // ничего не передаём из WndProc

   if (!hWnd)
   {
       // в случае некорректного создания окошка (неверные параметры и т.п.):
       MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
       return FALSE;
   }


   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, TIMER_ID, TIMER_TIME, NULL);
        break;
    }
    case WM_TIMER:
    {
        GetFilesList(inputFolderPath, extension, myFilesInfo);
        for (int i = 0;i < myFilesInfo.size(); i++)
        {
            if (ReadFileEx(myFilesInfo[i].hSourceFile, myFilesInfo[i].buffer, myFilesInfo[i].size, &myFilesInfo[i].overlap, (LPOVERLAPPED_COMPLETION_ROUTINE)CompletionRoutine) == FALSE)
            {
                DWORD dwError = GetLastError();
                MessageBox(NULL, ErrorMessage(dwError), L"ReadFile error", MB_OK);
            }
            myFilesInfo[i].buffer[myFilesInfo[i].size] = '\0';
        }
        Encrypt(myFilesInfo);
        if (stop)
        {
            KillTimer(hWnd, TIMER_ID);
            Decrypt(myFilesInfo);
        }
        for (int i = 0;i < myFilesInfo.size(); i++)
        {
            if (WriteFileEx(myFilesInfo[i].hSourceFile, myFilesInfo[i].buffer, myFilesInfo[i].size, &myFilesInfo[i].overlap, (LPOVERLAPPED_COMPLETION_ROUTINE)CompletionRoutine) == FALSE)
            {
                DWORD dwError = GetLastError();
                MessageBox(NULL, ErrorMessage(dwError), L"Error", MB_OK);
            }
        }
        if (stop)
        {
            Sleep(TIMER_TIME);
            SendMessage(hWnd, WM_DESTROY, 0, 0);
        }
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void GetFilesList(std::string filePath, std::string extension, std::vector<MyFileInfo>& myFilesInfo)
{
    WIN32_FIND_DATA fileInfo;
    HANDLE hFind;
    std::wstring wFilePath = std::wstring(filePath.begin(), filePath.end());
    std::string  fullPath = filePath + extension;
    std::wstring wFullPath = std::wstring(fullPath.begin(), fullPath.end());
    hFind = FindFirstFile(wFullPath.c_str(), &fileInfo);
    if (hFind != INVALID_HANDLE_VALUE) {
        std::wstring fullFileName = wFilePath;
        std::wstring fileName(fileInfo.cFileName);
        fullFileName += fileName;
        if (IsNewFile(myFilesInfo, fullFileName))
        {
            myFilesInfo.push_back(CreateMyInfo(fileInfo, fileName, fullFileName));
        }

        while (FindNextFile(hFind, &fileInfo) != 0) {
            std::wstring fullFileName = wFilePath;
            std::wstring fileName(fileInfo.cFileName);
            fullFileName += fileName;
            if (IsNewFile(myFilesInfo, fullFileName))
            {
                myFilesInfo.push_back(CreateMyInfo(fileInfo, fileName, fullFileName));
            }
        }
    }
}

void Encrypt(std::vector<MyFileInfo>& myFilesInfo)
{
    for (int i=0,j;i < myFilesInfo.size();i++)
    {
        if (!myFilesInfo[i].isEncrypt)
        {
            for (j = 0;j < myFilesInfo[i].size;j++)
            {
                if (myFilesInfo[i].buffer[j] != '\0')
                {
                    myFilesInfo[i].buffer[j] += 1;
                    if (myFilesInfo[i].buffer[j] >= 128)
                    {
                        myFilesInfo[i].buffer[j] = (myFilesInfo[i].buffer[j] % 128) + 1;
                    }
                }
            }
            myFilesInfo[i].isEncrypt = true;
        }
    }
}

void Decrypt(std::vector<MyFileInfo>& myFilesInfo)
{
    for (int i = 0, j;i < myFilesInfo.size();i++)
    {
        if (myFilesInfo[i].isEncrypt)
        {
            for (j = 0;j < myFilesInfo[i].size;j++)
            {
                if (myFilesInfo[i].buffer[j] != '\0')
                {
                    myFilesInfo[i].buffer[j] -= 1;
                    if (myFilesInfo[i].buffer[j] <= 0)
                    {
                        myFilesInfo[i].buffer[j] += 128;
                    }
                } 
            }
            myFilesInfo[i].isEncrypt = false;
        }
        
    }
}

MyFileInfo CreateMyInfo(WIN32_FIND_DATA fileInfo,std::wstring filename,std::wstring fullFileName)
{
    MyFileInfo newInfo;
    newInfo.fileName = filename;
    if (filename.compare(stopStr) == 0)
    {
        stop = true;
    }
    newInfo.fullFileName = fullFileName;
    newInfo.size = fileInfo.nFileSizeLow;
    newInfo.overlap = { 0 };
    ZeroMemory(&newInfo.overlap, sizeof(OVERLAPPED));
    newInfo.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (newInfo.hEvent == NULL)
    {
        DWORD dwError = GetLastError();
        MessageBox(NULL, ErrorMessage(dwError), L"hEvent error", MB_OK);
    }
    newInfo.overlap.hEvent = newInfo.hEvent;
    newInfo.overlap.Offset = 0;
    newInfo.buffer = new char[newInfo.size + 1];
    newInfo.hSourceFile = CreateFileW(newInfo.fullFileName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        NULL);
    if (newInfo.hSourceFile == INVALID_HANDLE_VALUE)
    {
        DWORD dwError = GetLastError();
        MessageBox(NULL, ErrorMessage(dwError), L"CreateFile error", MB_OK);
    }
    newInfo.isEncrypt = false;
    return newInfo;
}


bool IsNewFile(std::vector<MyFileInfo> myFilesInfo, std::wstring fullFileName)
{
    for (int i = 0, j;i < myFilesInfo.size();i++)
    {
        if (myFilesInfo[i].fullFileName.compare(fullFileName) == 0)
        {
            return false;
        }
    }
    return true;
}

//исполняется,когда опеция выполнена и WaitForSingleObjectEx ждёт с bAlertable=TRUE
VOID WINAPI CompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    CloseHandle(lpOverlapped->hEvent);
}


LPCTSTR ErrorMessage(DWORD error)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);
    return((LPCTSTR)lpMsgBuf);
}