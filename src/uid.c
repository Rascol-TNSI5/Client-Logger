#include <stdio.h>
#include <stdlib.h>
#include "../include/structures.h"
#include "../include/computer_informations.h"

void save_uid(char *uid){

    COMPUTER_INFOS computer;
    get_computer_info(&computer);

    char softwareDataDirectory[1024];
    snprintf(softwareDataDirectory, 1024, "C:\\Users\\%s\\AppData\\Local\\G666", computer.username);

    char *filename = "user";
    char uid_file_path[500];
    snprintf(uid_file_path, 500 , "%s\\%s", softwareDataDirectory, filename);

    FILE *file = fopen(uid_file_path, "w");
    if (file == NULL) {
        perror("Erreur à l'ouverture du fichier en écriture");
    }
    fprintf(file, "%s", uid);
    fclose(file);
}

void get_uid(char *uid){

    COMPUTER_INFOS computer;
    get_computer_info(&computer);

    char softwareDataDirectory[1024];
    snprintf(softwareDataDirectory, 1024, "C:\\Users\\%s\\AppData\\Local\\G666", computer.username);

    char *filename = "user";
    char uid_file_path[500];
    snprintf(uid_file_path, 500 , "%s\\%s", softwareDataDirectory, filename);

    FILE *file = fopen(uid_file_path, "r");
    if (file == NULL) {
        perror("Erreur à l'ouverture du fichier en lecture");
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);

    snprintf(uid, sizeof(uid), "%s", buffer);
    fclose(file);
}
