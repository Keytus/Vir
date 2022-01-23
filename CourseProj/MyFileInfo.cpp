#include "pch.h"
#include <string>


typedef struct
{
	DWORD size;
	std::wstring fullFileName;
	std::wstring fileName;
	HANDLE hSourceFile;
	OVERLAPPED overlap;
	HANDLE hEvent;
	char* buffer;
	bool isEncrypt;
} MyFileInfo, * LPMyFileInfo;


