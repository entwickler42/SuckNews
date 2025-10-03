#include "basic.h"

#define _DEBUG_ 1

#define NNTP_ERR_CONLIMIT 1008
#define NNTP_ERR_NOHEADER 1007
#define NNTP_ERR_NOGROUP 1006
#define NNTP_ERR_NOMEM 1005
#define NNTP_ERR_NOLIST 1005
#define NNTP_ERR_SOCK 1004
#define NNTP_ERR_INVALID_HOST 1003
#define NNTP_ERR_CANT_CONNECT 1002
#define NNTP_ERR_UNKNOWN 1001
#define NNTP_ERR_NOERR 1000
#define NNTP_READY_POSTALLOW 200
#define NNTP_READY_POSTDENY 201
#define NNTP_ACCESS_RESTRICTED 502
#define NNTP_AUTH_OK 281
#define NNTP_AUTH_MORE 381
#define NNTP_LIST_FOLLOWS 215
#define NNTP_GROUP_OK 211
#define NNTP_STAT_OK 223
#define NNTP_NEXT_OK 223
#define NNTP_HEAD_OK 221
#define NNTP_NEWNEWS 230
#define NNTP_NONEXT 421
#define NNTP_CONLIMIT_REACHED 400

#define CMD_LOGOUT "QUIT"
#define CMD_AUTHUSER "AUTHINFO USER"
#define CMD_AUTHPASS "AUTHINFO PASS"
#define CMD_LIST "LIST"
#define CMD_GROUP "GROUP"
#define CMD_NEXT "NEXT"
#define CMD_STAT "STAT"
#define CMD_HEAD "HEAD"
#define CMD_NEWNEWS "NEWNEWS"

#define LAST_RESPONSE_TEXT_SIZE 256
extern char LAST_RESPONSE_TEXT[LAST_RESPONSE_TEXT_SIZE];
extern unsigned int NNTP_LAST_ERROR;
extern unsigned int LAST_RESPONSE;

typedef struct _NNTP_SERVER
{
	char name[32];
	char hostname[32];
	char username[32];
	char passwd[32];
	unsigned short port;
	unsigned int max_con;
} NNTP_SERVER;

typedef struct _NNTP_SERVER_LIST
{
	unsigned int num;
	NNTP_SERVER *server;
} NNTP_SERVER_LIST;

typedef struct _GROUP
{
	unsigned char moderated;
	unsigned long int first;
	unsigned long int last;
	unsigned int num;
	char name[96];
} GROUP;

typedef struct _GROUP_LIST
{
	unsigned int num;
	GROUP *group;
} GROUP_LIST;

typedef struct _HEADER
{
	unsigned long int nid;
	unsigned int is_reply;
	char* tid;
	char* subject;
} HEADER;

typedef struct _HEADER_LIST
{
	time_t last_update;
	unsigned int num;
	HEADER* header;
} HEADER_LIST;

int read_response( int socket );
int authenticate( int socket , const char* user, const char* passwd);

int connect_nntp( const NNTP_SERVER nntp );
int disconnect_nntp( int socket );

int fetch_group_list( const NNTP_SERVER nntp, GROUP_LIST* list );
int free_group_list( GROUP_LIST* list );

int fetch_header_list(const NNTP_SERVER nntp, const GROUP grp, HEADER_LIST* list);
int read_header( int socket , HEADER* hdr );
int free_header_list( HEADER_LIST* list );

int update_group(int sock, GROUP* group);
int update_group_list(const NNTP_SERVER nntp, GROUP_LIST* list);
int update_header_list(const NNTP_SERVER nntp, const GROUP grp, HEADER_LIST* list);

int add_group(const GROUP group, GROUP_LIST* list);
int del_group(const GROUP group, GROUP_LIST* list);

