#include <windows.h>
#include <stdio.h>
#include <iostream>

#define INFO_BUFFER_SIZE 32767
#define BUFSIZE MAX_PATH
#define WINVER 0x0502
#define MAX_DATA_LENGTH 16383
using namespace std;

int main()
{
    // 1.1

    OSVERSIONINFO osvi = {0};
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);

    DWORD dwMinorVersion = osvi.dwMinorVersion;
    DWORD dwMajorVersion = osvi.dwMajorVersion;
    DWORD dwBuildNumber = osvi.dwBuildNumber;

    printf("Version is %d.%d (%d)\n",
                dwMajorVersion,
                dwMinorVersion,
                dwBuildNumber);
    printf("--------------------------------\n");

    // 1.2

    char system_dir[MAX_PATH];
    GetSystemDirectory(system_dir, MAX_PATH);
    printf("System directory: %s \n", system_dir);
    printf("--------------------------------\n");

    // 1.3

    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    DWORD  bufCharCount = INFO_BUFFER_SIZE;

    bufCharCount = INFO_BUFFER_SIZE;

    GetComputerName(infoBuf, &bufCharCount);

    bufCharCount = INFO_BUFFER_SIZE;
    printf("Computer name: %s\n", infoBuf );

    bufCharCount = INFO_BUFFER_SIZE;

    GetUserName(infoBuf, &bufCharCount);

    printf("User name: %s\n", infoBuf );
    printf("--------------------------------\n");

    // 1.4

    char buffer[MAX_PATH];
    DWORD  cchBufferLength = MAX_PATH;
    char Names[MAX_PATH];
    ULARGE_INTEGER total, available, free;


    HANDLE search = FindFirstVolume(buffer, sizeof(buffer));

    do {
        printf("\n%s", buffer);
        GetVolumePathNamesForVolumeName(buffer, Names, cchBufferLength, &cchBufferLength);
        printf("\n  First path: %s", Names);
        GetDiskFreeSpaceEx(
                           buffer,
                           (PULARGE_INTEGER)&available,
                           (PULARGE_INTEGER)&total,
                           (PULARGE_INTEGER)&free);

        if (GetDiskFreeSpaceEx(
                               buffer,
                               (PULARGE_INTEGER)&available,
                               (PULARGE_INTEGER)&total,
                               (PULARGE_INTEGER)&free)==0)
        {
            printf("\n  No information available.\n ");
        }
        else
        {
        cout<<"\n  Free space: "<<available.QuadPart<<" bytes";
        cout<<"\n  Total size: "<<total.QuadPart<<" bytes"<<endl;
        }

    } while (FindNextVolume(search, buffer, sizeof(buffer)));
    FindVolumeClose(search);

    printf("\n--------------------------------\n");

    // 1.5

    HKEY key;
    DWORD dwIndex = 0;
    TCHAR tcValue[MAX_PATH] = { 0 };
    TCHAR lpValueName[MAX_DATA_LENGTH];
    DWORD lpcchValueName = MAX_DATA_LENGTH;
    DWORD lpDataLength = MAX_DATA_LENGTH;

    long lResult = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",0, KEY_READ, &key);
    if (lResult == ERROR_SUCCESS)
        {
            while (RegEnumValue(key, dwIndex, lpValueName, &lpcchValueName, NULL, NULL, (LPBYTE)tcValue, &lpDataLength) == ERROR_SUCCESS)
            {
            printf("Programm %d: %s : %s \n", dwIndex+1, lpValueName, tcValue);
            dwIndex++;
            lpDataLength = MAX_DATA_LENGTH;
            }
        }

    RegCloseKey(key);

    printf("\n--------------------------------\n");

    // 2

    LARGE_INTEGER freq;
    LARGE_INTEGER t_1;
    LARGE_INTEGER t_2;

    QueryPerformanceCounter(&t_1);
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t_2);

    double tact = t_2.QuadPart - t_1.QuadPart;
    double frequency = freq.QuadPart;;
    double us = 1000000 * tact / frequency;

    printf("CPU Frequency = %u Hz\n",  freq);
    printf("CPU clock = %f us\n",  us);
}
