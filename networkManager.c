#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include "bt_lib.h"
#include "bt_setup.h"
#include "bencode.h"
#include <openssl/sha.h>
#include "list.h"
#include <vector>
#include <string>
struct piece{
	char* hash;
	struct list_elem* elem;
};
int main(int argc, char** argv)
{
	struct sockaddr_in servaddr;
	struct hostent *hp;
	int sock_id;
	char message[1024*1024];
	char msglen;
	bt_args_t bt_args;
 	be_node * node; // top node in the bencoding
 	parse_args(&bt_args, argc, argv);
	int i;
	//struct list pList;
	//list_init(&pList);
	for(i=0;i<MAX_CONNECTIONS;i++){
      		if(bt_args.peers[i] != NULL)
        		print_peer(bt_args.peers[i]);
    	}
	//char* types[4] = {"BE_STR","BE_INT","BE_LIST","BE_DICT"};
	char *announce; 
	char *aList[10];
	unsigned char *pieces;
	int fLength, pieceLen; 
	std::vector<std::string> pList;
	node = load_be_node(argv[1]);
	//be_dump(node);
	parser(node, &announce, aList, &fLength, &pieceLen, &pieces);
	//exit(1);
	/*if(fp = fopen(argv[1], "r")<0){
		perror("failed to open file..\n");
	
	*/
	printf("announce is:%s\n", announce);
	printf("announce-list is:%s and %s \n", aList[0], aList[1]);
	printf("flength is:%d\n", fLength);
	printf("piecelen is:%d\n", pieceLen);
	//printf("pieces is:%s\n", pieces);
	
	/*unsigned char temp[pieceLen];
   	
    	memset(temp, 0x0, pieceLen);
	*/
	int j=0;
		//go through all pieces in file
		printf("%d---\n", fLength/pieceLen);
		for (j=0; j < 16957; j++) {
			char buf[40];
			int index=0;
		//for each piece print out its 20 SHA1 values
			for(i=j*20;i<(j*20)+20;i++){
				sprintf((char*)&(buf[index*2]), "%02x", pieces[i]);
				index++;
			}
			// make piece a list elem
			struct piece* p = (struct piece*)malloc(sizeof(struct piece));
			p->hash = buf;
			p->elem = (struct list_elem*)malloc(sizeof(struct list_elem));
			
			//push it to the back
			//list_push_back(&pList,p->elem);
			free(p);
			//sprintf((char*)&(buf[(i+1)*2]), "\n");
			printf("%d SHA1 is: %s\n",j, buf);
			memset(buf, 0x0, 40);
    		}
    		//memset(temp, 0x0, pieceLen):


	//printf("pieces is:%s\n", buf_ptr);
	//free(printPieces);
	//printf("List size = %d\n", list_size(&pList));
	exit(0);
}
	////////////////////////////////////////////////////////////
void parser(be_node *node, char** announce, char* aList[], int* fLength, int* pieceLen, unsigned char** pieces){
	size_t i=0;
	int list=0;
	switch (node->type) {
		case BE_STR:
			if (strcmp(node->info,"announce")==0){
				*announce = node->val.s;

			}else if(strcmp(node->info,"announce-list")==0){
				aList[list] =( char*) node->val.s;
				list++;
			}else if(strcmp(node->info,"pieces")==0){
				*pieces =(unsigned char*) node->val.s;
			}
			break;
		case BE_INT:
			if (strcmp(node->info,"length")==0){
				*fLength = node->val.i;
			}else if(strcmp(node->info,"piece length")==0){
				*pieceLen = node->val.i;
			}
			break;
		case BE_LIST:
			for (i = 0; node->val.l[i]; ++i){

				node->val.l[i]->info =node->info;
				parser(node->val.l[i],announce,aList,fLength,pieceLen, pieces);
			}
			break;
		case BE_DICT:
			for (i = 0; node->val.d[i].val; ++i){
				node->val.d[i].val->info = node->val.d[i].key;
				parser(node->val.d[i].val,announce,aList,fLength,pieceLen, pieces);
			}
			break;
	}
}
	////////////////////////////////////////////////////////////
	//Get a socket	
/*	if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr,"Couldn't get a socket.\n"); exit(EXIT_FAILURE);
	}
	else {
		fprintf(stderr,"Got a socket.\n");
	}
	
	//book uses bzero which my man pages say is deprecated
	//the man page said to use memset instead. :-)
	memset(&servaddr,0,sizeof(servaddr));
	
	//get address for google.com
	if((hp = gethostbyname(announce)) == NULL) {
		fprintf(stderr,"Couldn't get an address.\n"); exit(EXIT_FAILURE);
	}
	else {
		fprintf(stderr,"Got an address.\n");
	}
	
	//bcopy is deprecated also, using memcpy instead
	memcpy((char *)&servaddr.sin_addr.s_addr, (char *)hp->h_addr, hp->h_length);
	
	//fill int port number and type
	servaddr.sin_port = htons(80);
	servaddr.sin_family = AF_INET;
	
	//make the connection
	if(connect(sock_id, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		fprintf(stderr, "Couldn't connect.\n");
	}
	else {
		fprintf(stderr,"Got a connection!!!\n");
	}
	
	//NOW THE HTTP PART!!!
	
	//send the request
	char* request = "";
	write(sock_id,request,strlen(request));
	
	//read the response
	msglen = read(sock_id,message,1024*1024);
	
	//print the reasponse
	fprintf(stdout,"%s",message);
	
	return 0;
*/


