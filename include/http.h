#ifndef HTTP_REQ_H
#define HTTP_REQ_H

void send_to_server(const char* route, const char* data);
int upload_to_server(char * file_path, char* func_name, char* param);

#endif // HTTP_REQ_H