#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int readline(int socket, char* buf, unsigned int size);
int sendcmd(int socket, const char* cmd , const char* parm);
void* elarge_array(void* src,unsigned int src_size, unsigned int dst_size);
int time_to_nntp(const time_t time, char buf[14]);
