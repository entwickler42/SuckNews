#include "basic.h"

#include <sys/types.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int readline(int socket, char* buf, unsigned int size)
{
	char ch;
	int cnt,pos;

	memset(buf,0,size);
	pos = 0;

	do
	{
		cnt = recv(socket,&ch,1,0);
		if(ch != '\n' && ch != '\r' && pos < size) buf[pos++] = ch;
	} while(ch != '\n' && cnt > 0);

	if(cnt == -1)
	{
		#ifdef _DEBUG_
		perror("recv()");
		printf("DEBUG: Connecting died unexpected !\n");
		#endif

		return -1;
	}

	return pos;
}

int sendcmd(int socket, const char* cmd , const char* parm)
{
	char buf[BUF_SIZE];
	memset(buf,0,BUF_SIZE);

	strcpy(buf,cmd);
	if(parm != NULL)
	{
		strcat(buf," ");
		strcat(buf,parm);
	}
	strcat(buf,"\r\n");

	return send(socket,buf,strlen(buf),0);
}

void* elarge_array(void* src, unsigned int src_size, unsigned int dst_size)
{
	void *dst;

	if(dst_size <= src_size) return NULL;

	dst = malloc(dst_size);

	if(dst == NULL)
	{
		#ifdef _DEBUG_
		printf("DEBUG: OUT OF MEMORY !\n");
		#endif

		return NULL;
	}

	if(src_size > 0)
		memcpy(dst,src,src_size);

	return dst;
}

int time_to_nntp(const time_t time, char buf[14])
{
	struct tm* c_time;

	c_time = gmtime(&time);

	sprintf(buf,"%02i%02i%02i %02i%02i%02i\0",c_time->tm_year-100,c_time->tm_mon+1,c_time->tm_mday,c_time->tm_hour,c_time->tm_min,c_time->tm_sec);
	printf("TIME=%s\n",buf);

	return 0;
}

