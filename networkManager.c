#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bt_lib.h"
#include "bt_setup.h"
#include "bencode.h"

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
	for(i=0;i<MAX_CONNECTIONS;i++){
      		if(bt_args.peers[i] != NULL)
        		print_peer(bt_args.peers[i]);
    	}
	//char* types[4] = {"BE_STR","BE_INT","BE_LIST","BE_DICT"};
	char *announce; 
	char *aList[10];
	char *pieces;
	int fLength, pieceLen; 
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
	char* printPieces = (char*)malloc(sizeof(char)*pieceLen+2);
	char* buf_ptr= printPieces;
	//printf("%d\n", strlen(pieces));
	for(i=0;i<pieceLen;i++){
		printf("%c and %x\n", &buf_ptr[i], pieces[i]);
		/*if(i%pieceLen == 0 && i!=0){
			sprintf(&buf_ptr[i],"\n");	
		}
		*/
		sprintf(&buf_ptr[i],"%x",pieces[i]);
		

	}
	//sprintf(buf_ptr,"\n");
	//*(buf_ptr + 1) = '\0';
	
	printf("pieces is:%s\n", printPieces);
	free(printPieces);
	exit(0);
	////////////////////////////////////////////////////////////
	//Get a socket	
	if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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
}
