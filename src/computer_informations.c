#include <stdio.h>
#include <string.h>
#include <windows.h> // (https://learn.microsoft.com/fr-fr/windows/win32/api/winbase/)
#include "../include/structures.h"

void get_computer_info(COMPUTER_INFOS *cmp_info)
{

    /*Trouve le nom du pc, le nom de la session (username) et l'architecture du pc (ex : AMD64)
    la fonction prend en param le pointeur de la stucture COMPUTER_INFOS */

    char computer_name_var[256];
    char username_var[256];
    char arch_var[10];
    char version[80];

    OSVERSIONINFO osvi;
    SYSTEM_INFO sys_info;
    DWORD size = 256; // DWORD = long unsigned int
    DWORD size_arch = 10;

    GetComputerName(computer_name_var, &size);
    GetUserName(username_var, &size);
    GetEnvironmentVariable("PROCESSOR_ARCHITECTURE", arch_var, size_arch);

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    snprintf(version, 80, "Windows %d.%d.%d", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);

    // Je remplie la structure avec les info qu'il faut
    strcpy(cmp_info->computer_name, computer_name_var);
    strcpy(cmp_info->username, username_var);
    strcpy(cmp_info->architecture, arch_var);
    strcpy(cmp_info->windows_version, version);
}