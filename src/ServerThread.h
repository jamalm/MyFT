#ifndef S_THREAD_H_
#define S_THREAD_H_

void StartThread(int cs);
void *thread_server(void* csock_ptr);

void HandleClient(int cs);
char* HandleRequest(char *message);

#endif
//S_THREAD_H_
