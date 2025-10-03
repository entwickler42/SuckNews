#include "nntp.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

char LAST_RESPONSE_TEXT[LAST_RESPONSE_TEXT_SIZE];
unsigned int NNTP_LAST_ERROR;
unsigned int LAST_RESPONSE;


int read_response( int socket )
{
	unsigned int cnt,pos;
	char buf[4],ch;
	memset(buf,0,4);

	cnt = recv(socket,buf,3,0);
	if(cnt != 3)
	{
		#ifdef _DEBUG_
		perror("recv()");
		printf("RESPN: got invaild or no response code !");
		#endif
		return -1;
	}

	memset(LAST_RESPONSE_TEXT,0,LAST_RESPONSE_TEXT_SIZE);
	pos = -1;

	do
	{
		cnt = recv(socket,&ch,1,0);

		if(pos >= 0 && pos < LAST_RESPONSE_TEXT_SIZE) LAST_RESPONSE_TEXT[pos]=ch;
		pos++;

	} while(ch != '\n' && cnt != -1);

	#ifdef _DEBUG_
	printf("RESPN: %s %s",buf,LAST_RESPONSE_TEXT);
	#endif
	LAST_RESPONSE = atoi(buf);

	return LAST_RESPONSE;
}

int authenticate( int socket , const char* user, const char* passwd)
{
	char buf[256];
	memset(buf,0,256);

	sendcmd(socket,CMD_AUTHUSER,user);
	if(read_response(socket) == NNTP_AUTH_MORE)
	{
		sendcmd(socket,CMD_AUTHPASS,passwd);
		read_response(socket);
	}

	return LAST_RESPONSE;
}

int connect_nntp( const NNTP_SERVER nntp )
{
	struct sockaddr_in server;
	struct hostent* host;
	unsigned long addr;
	int s,cnt;

	if((addr = inet_aton(nntp.hostname)) == 0)
		if((host = gethostbyname(nntp.hostname))!=NULL)
		{
			memcpy(&addr,host->h_addr_list[0],4);
		}
		else
		{
			#ifdef _DEBUG_
			printf("DEBUG: no vaild host !\n");
			perror("inet_addr");
			#endif

			NNTP_LAST_ERROR = NNTP_ERR_INVALID_HOST;
			return -1;
		}

	server.sin_family = AF_INET;
	server.sin_port	= htons(nntp.port);
	memcpy(&server.sin_addr,&addr,4);
	memset(server.sin_zero,0,8);

	if((s = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		#ifdef _DEBUG_
		printf("DEBUG: no socket available !\n");
		perror("socket()");
		#endif

		NNTP_LAST_ERROR = NNTP_ERR_SOCK;
		return -1;
	}

	#ifdef _DEBUG_
	printf("DEBUG: trying to connect to %s\n",inet_ntoa(server.sin_addr));
	#endif
	if(connect(s,(struct sockaddr*)&server,sizeof(struct sockaddr_in)) != 0)
	{
		#ifdef _DEBUG_
		printf("DEBUG: can't connect to host !\n");
		perror("connect()");
		#endif

		NNTP_LAST_ERROR = NNTP_ERR_CANT_CONNECT;
		return -1;
	}
	#ifdef _DEBUG_
	printf("DEBUG: connected to host !\n");
	#endif

	read_response(s);

	if(LAST_RESPONSE == NNTP_CONLIMIT_REACHED)
	{
		NNTP_LAST_ERROR = NNTP_ERR_CONLIMIT;
		disconnect_nntp(s);
		return -1;
	}

	if(LAST_RESPONSE != NNTP_READY_POSTALLOW && LAST_RESPONSE != NNTP_READY_POSTDENY)
	{
		#ifdef _DEBUG_
		printf("DEBUG: Error while reading server response !\n");
		#endif

		NNTP_LAST_ERROR = NNTP_ERR_UNKNOWN;
		disconnect_nntp(s);
		return -1;
	}

	if(strlen(nntp.username) > 0 )
	{
		#ifdef _DEBUG_
		printf("DEBUG: Authentication required !\n");
		#endif

		if(authenticate(s,nntp.username,nntp.passwd) == NNTP_ACCESS_RESTRICTED)
		{
			#ifdef _DEBUG_
			printf("DEBUG: invaild username or password !\n");
			#endif
			NNTP_LAST_ERROR = NNTP_ACCESS_RESTRICTED;
			disconnect_nntp(s);
			return -1;
		}
	}
	#ifdef _DEBUG_
	else
		printf("DEBUG: No Authentication required ( so far ... ) !\n");
	#endif

	NNTP_LAST_ERROR = NNTP_ERR_NOERR;
	return s;
}

int disconnect_nntp( int socket )
{
	sendcmd(socket,CMD_LOGOUT,NULL);
	if(NNTP_LAST_ERROR == NNTP_ERR_NOERR) read_response(socket);
	close(socket);

	#ifdef _DEBUG_
	printf("DEBUG: Disconnected !\n");
	#endif
}

int fetch_group_list( const NNTP_SERVER nntp, GROUP_LIST* list )
{
	char rBuf[512];
	GROUP *grps,*grps_tmp;
	int sock,cnt,num,i,mem;

	if((sock = connect_nntp(nntp)) == -1) return sock;

	sendcmd(sock,CMD_LIST,NULL);

	if(read_response(sock) != NNTP_LIST_FOLLOWS)
	{
		#ifdef _DEBUG_
		printf("DEBUG: %s\n",LAST_RESPONSE_TEXT);
		#endif

		NNTP_LAST_ERROR = NNTP_ERR_NOLIST;
		disconnect_nntp(sock);
		return -1;
	}

	mem  = 1000;
	num  = 0;
	grps = calloc(mem,sizeof(GROUP));

	if(grps == NULL)
	{
		#ifdef _DEBUG_
		printf("DEBUG: out of memory !\n");
		#endif

		NNTP_LAST_ERROR = NNTP_ERR_NOMEM;
		disconnect_nntp(sock);
		return -1;
	}

	do
	{
		cnt = readline(sock, rBuf, 512 );

		if(cnt != -1 && rBuf[0] != '.')
		{
			sscanf(rBuf,"%s %lu %lu %c",grps[num].name,&grps[num].last,&grps[num].first,&grps[num].moderated);
			grps[num].num = grps[num].last - grps[num].first;
		}

		if(++num == mem)
		{
			mem = mem + num;
			if((grps_tmp=elarge_array(grps,num*sizeof(GROUP),mem*sizeof(GROUP))) == NULL)
			{
				NNTP_LAST_ERROR = NNTP_ERR_NOMEM;
				disconnect_nntp(sock);
				free(grps);
				return -1;
			}
			else
			{
				free(grps);
				grps = grps_tmp;
			}
		}

		#ifdef _DEBUG_
		if(cnt != -1 && rBuf[0] != '.')
			printf("%i: %s %010i %010i %c\n",num,grps[num-1].name,grps[num-1].last,grps[num-1].first,grps[num-1].moderated);
		#endif
	}while(rBuf[0] != '.' && cnt != -1);

	list->num   = num;
	list->group = calloc(num,sizeof(GROUP));
	for(i=0;i<num;i++) list->group[i] = grps[i];
	free(grps);

	if(cnt == -1)
	{
		#ifdef _DEBUG_
		printf("DEBUG: Error while receving list !\n");
		#endif

		NNTP_LAST_ERROR = NNTP_ERR_NOLIST;
		free_group_list(list);
		disconnect_nntp(sock);
		return -1;
	}

	#ifdef _DEBUG_
	printf("DEBUG: List recept !\n");
	#endif

	NNTP_LAST_ERROR = NNTP_ERR_NOERR;
	disconnect_nntp(sock);
	return list->num;
}

int free_group_list( GROUP_LIST* list )
{
	free(list->group);

	list->num = 0;
	list->group = NULL;

	return 0;
}

int fetch_header_list(const NNTP_SERVER nntp, const GROUP grp, HEADER_LIST* list)
{
	char rBuf[512];
	int sock;
	unsigned int mem,cnt,i;
	HEADER* hdrs,*hdrs_tmp;

	if((sock = connect_nntp(nntp)) == -1) return sock;

	sendcmd(sock,CMD_GROUP,grp.name);

	if(read_response(sock) != NNTP_GROUP_OK)
	{
		#ifdef _DEBUG_
		printf("DEBUG: %i Group Error !\n",LAST_RESPONSE);
		#endif

		NNTP_LAST_ERROR = NNTP_ERR_NOGROUP;
		disconnect_nntp(sock);
		return -1;
	}

	cnt = 0;
	mem = 1000;
	hdrs = calloc(mem,sizeof(HEADER));

	if(hdrs == NULL)
	{
		#ifdef _DEBUG_
		printf("DEBUG: out of memory !\n");
		#endif

		NNTP_LAST_ERROR = NNTP_ERR_NOMEM;
		disconnect_nntp(sock);
		return -1;
	}

	sendcmd(sock,CMD_STAT,NULL);

	while(read_response(sock) == NNTP_NEXT_OK)
	{
		sendcmd(sock,CMD_HEAD,NULL);
		read_header(sock,&hdrs[cnt]);

		if(++cnt == mem)
		{
			mem = mem + cnt;
			if((hdrs_tmp=elarge_array(hdrs,cnt*sizeof(HEADER),mem*sizeof(HEADER))) == NULL)
			{
				NNTP_LAST_ERROR = NNTP_ERR_NOMEM;
				disconnect_nntp(sock);
				free(hdrs);
				return -1;
			}
			else
			{
				free(hdrs);
				hdrs = hdrs_tmp;
			}
		}

		sendcmd(sock,CMD_NEXT,NULL);
	}

	list->num = cnt;
	hdrs_tmp = calloc(cnt,sizeof(HEADER));
	memcpy(hdrs_tmp,hdrs,cnt*sizeof(HEADER));
	list->header = hdrs_tmp;
	free(hdrs);

	for(i=0;i<list->num;i++)
		printf("%010i %010li %s\n",i, list->header[i].nid,list->header[i].subject);

	if(LAST_RESPONSE != NNTP_NONEXT)
	{
		#ifdef _DEBUG_
		printf("DEBUG: unknown error while reciving headers!\n");
		#endif

		NNTP_LAST_ERROR = NNTP_ERR_UNKNOWN;
		disconnect_nntp(sock);
		return -1;
	}

	list->last_update = time(NULL);

	NNTP_LAST_ERROR = NNTP_ERR_NOERR;
	disconnect_nntp(sock);
	return 0;
}

int read_header( int socket , HEADER* hdr )
{
	char rBuf[512],*chp;
	int cnt;

	if(read_response(socket) != NNTP_HEAD_OK )
	{
		#ifndef _DEBUG_
		printf("DEBUG: %i Error while fetching header!\n",LAST_RESPONSE);
		#endif

		NNTP_LAST_ERROR = NNTP_ERR_NOHEADER;
		return NNTP_LAST_ERROR;
	}

	hdr->nid = 0;
	hdr->tid = NULL;
	hdr->subject  = NULL;
	hdr->is_reply = 0;

	sscanf(LAST_RESPONSE_TEXT,"%ui",&hdr->nid);

	do
	{
		cnt = readline(socket,rBuf,512);
		if(cnt != -1 && rBuf[0] != '.')
		{
			if(strstr(rBuf,"Subject: "))
			{
				chp = rBuf+9;
				hdr->subject = malloc(strlen(chp)+1);
				strcpy(hdr->subject,chp);
			}
			else if(strstr(rBuf,"Message-ID: "))
			{
				chp = rBuf+12;
				hdr->tid = malloc(strlen(chp)+1);
				strcpy(hdr->tid,chp);
			}
			else if(strstr(rBuf,"Reply-To"))
			{
				hdr->is_reply = 1;
			}
		}
	}while(rBuf[0] != '.' && cnt != -1);

	NNTP_LAST_ERROR = NNTP_ERR_NOERR;
	return NNTP_LAST_ERROR;
}

int free_header_list( HEADER_LIST* list )
{
	unsigned int i;

	for(i=0; i<list->num; i++)
	{
		free(list->header[i].tid);
		free(list->header[i].subject);

		list->header[i].nid = 0;
		list->header[i].tid = NULL;
		list->header[i].subject  = NULL;
		list->header[i].is_reply = 0;
	}

	free(list->header);

	list->num = 0;
	list->header = NULL;

	return 0;
}

int update_group(int sock, GROUP* group)
{
	#ifdef _DEBUG_
	printf("DEBUG: updating group: %s\n",group->name);
	#endif

	sendcmd(sock,CMD_GROUP,group->name);

	if(read_response(sock) != NNTP_GROUP_OK)
	{
		#ifdef _DEBUG_
		printf("DEBUG: %i Group Error !\n",LAST_RESPONSE);
		#endif

		NNTP_LAST_ERROR = NNTP_ERR_NOGROUP;
		return -1;
	}

	sscanf(LAST_RESPONSE_TEXT,"%i %lu %lu",&group->num,&group->first,&group->last);

	#ifdef _DEBUG_
	printf("DEBUG: Group %s updated : Num: %010i First: %010lu Last: %010lu\n",group->name,group->num,group->first,group->last);
	#endif

	NNTP_LAST_ERROR = NNTP_ERR_NOERR;
	return 0;
}

int update_group_list(const NNTP_SERVER nntp, GROUP_LIST* list)
{
	int sock,i;
	if((sock = connect_nntp(nntp)) == -1) return sock;

	for(i=0; i<list->num; i++)
		update_group(sock,&list->group[i]);

	disconnect_nntp(sock);
	NNTP_LAST_ERROR = NNTP_ERR_NOERR;
	return 0;
}

int update_header_list(const NNTP_SERVER nntp, const GROUP grp, HEADER_LIST* list)
{
	int sock,cnt,num,mem,i,j,add;
	char buf[512],t[14];
	HEADER* tmp_list, *tmp;

	if((sock = connect_nntp(nntp)) == -1) return sock;

	time_to_nntp(list->last_update,t);
	strcpy(buf,grp.name);
	strcat(buf," ");
	strcat(buf,t);
	strcat(buf,"\r\n");

	sendcmd(sock,CMD_NEWNEWS,buf);

	#ifdef _DEBUG_
	printf("DEBUG: sending NEWNEWS cmd = %s",buf);
	#endif

	if(read_response(sock) != NNTP_NEWNEWS)
	{
		disconnect_nntp(sock);
		NNTP_LAST_ERROR = NNTP_ERR_UNKNOWN;
		return -1;
	}

	num		= 0;
	mem		= 100;
	tmp_list = calloc(mem,sizeof(HEADER));

	if(tmp_list = NULL)
	{
		disconnect_nntp(sock);
		NNTP_LAST_ERROR = NNTP_ERR_NOMEM;
		return -1;
	}

	do
	{
		cnt = readline(sock,buf,512);

		if(buf[0] != '.')
		{
			sendcmd(sock,CMD_HEAD,buf);
			read_header(sock,&tmp_list[num++]);

			if(num == mem)
			{
				mem = mem + cnt;
				tmp = elarge_array(tmp_list,cnt*sizeof(HEADER),mem*sizeof(HEADER));

				free(tmp_list);
				tmp_list = tmp;

				if(tmp == NULL)
				{
					disconnect_nntp(sock);
					NNTP_LAST_ERROR = NNTP_ERR_NOMEM;
					return -1;
				}
			}
		}

	} while(cnt > 0 && buf[0] != '.');

	list->last_update = time();
	disconnect_nntp(sock);

	for(i=0; i<cnt; i++)
	{
		add = 1;

		for(j=0; j<list->num; j++)
			if(strcmp(list->header[j].tid,tmp_list[i].tid)==0)
			{
				add=0;
				break;
			}

		if(add)
		{
			tmp = calloc(list->num+1,sizeof(HEADER));
			if(tmp == NULL)
			{
				free(tmp_list);
				NNTP_LAST_ERROR = NNTP_ERR_NOMEM;
				return -1;
			}

			memcpy(tmp,list->header,list->num*sizeof(HEADER));
			memcpy(tmp+list->num++,&tmp_list[i],sizeof(HEADER));

			free(list->header);
			list->header = tmp;
		}
	}

	NNTP_LAST_ERROR = NNTP_ERR_NOERR;
	return 0;
}

int add_group(const GROUP group, GROUP_LIST* list)
{
	GROUP* tmp_grps;
	HEADER* new_hrd;

	tmp_grps = elarge_array(list->group,list->num*sizeof(GROUP),list->num*sizeof(GROUP)+sizeof(GROUP));

	if(tmp_grps == NULL)
	{
		NNTP_LAST_ERROR = NNTP_ERR_NOMEM;
		return -1;
	}

	memcpy(tmp_grps+list->num,&group,sizeof(GROUP));

	free(list->group);
	list->group = tmp_grps;
	list->num++;

	return 0;
}

int del_group(const GROUP group, GROUP_LIST* list)
{
	unsigned int i;
	GROUP* tmp_pos;
	GROUP* tmp_grps;

	tmp_grps = calloc(list->num-1,sizeof(GROUP));
	if(tmp_grps == NULL)
	{
		NNTP_LAST_ERROR = NNTP_ERR_NOMEM;
		return -1;
	}

	tmp_pos = tmp_grps;

	for(i=0; i<list->num; i++)
		if(strcmp(list->group[i].name,group.name))
			memcpy(tmp_pos++,list->group+i,sizeof(GROUP));

	free(list->group);
	list->group = tmp_grps;
	list->num--;

	return 0;
}

