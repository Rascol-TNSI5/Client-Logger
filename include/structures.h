#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    char computer_name[256];
    char username[256];
    char architecture[10];
    char windows_version[80];
} COMPUTER_INFOS;

void get_computer_info(COMPUTER_INFOS *cmp_info);

#endif