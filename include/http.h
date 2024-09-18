#ifndef HTTP_REQ_H
#define HTTP_REQ_H

void send_to_server(const char* route, const char* data);
int upload_to_server(const char *file_path);

#endif // HTTP_REQ_H