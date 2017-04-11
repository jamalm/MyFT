#ifndef CSPROTOCOL_H_
#define CSPROTOCOL_H_
void Authenticated(int, int);
char *HandleAuth(int);
void HandleFileTransfer(int);

int Auth(char *, char *, int);
char *FileTransfer(char *, char *,char *, int);

void EndConnection(int);
#endif
//CSPROTOCOL_H_
