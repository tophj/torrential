#include "networkManager.h"


std::vector<uint8_t> pList(32);
std::vector<std::string> announceV(20);

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
	char *aList[20];
	unsigned char *pieces;
	int fLength, pieceLen; 
	//std::vector<std::string> pList;
	node = load_be_node(argv[1]);
	//be_dump(node);
	parser(node, &announce, aList, &fLength, &pieceLen, &pieces);
	

/*	struct addrinfo hint, *ap;
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	int r = getaddrinfo(announce, "http", &hint, &ap);
	printf("%s\n", strerror(errno));
	printf("%s\n",gai_strerror(r));
*/	

	int j=0;
	//go through all pieces in file
	//printf("___%d____\n", r);
	for (j=0; j < 16957; j++) {
		char buf[40];
		int index=0;
	//for each piece print out its 20 SHA1 values
		for(i=j*20;i<(j*20)+20;i++){
			sprintf((char*)&(buf[index*2]), "%02x", pieces[i]);
			index++;
		}
		
		//convert the Hexstring to bye array using <vector>
		uint8_t bytes[20];
		convert(buf,bytes);
		//Passing threadpool to Tracker and peerwire
		tp_t *pool;
		pool=thread_pool_new(8);
		std::string convert;
		i=0;
		while(aList[i]!=NULL){
			convert=std::string(aList[i]);
			announceV.push_back(convert);
		}
		PeerList newPeerList;
		//send bytes to Tracker

		/*TorrentTrackerCommManager(struct thread_pool * theThreadPool,
									PeerList & newPeerList,
									uint8_t newFileHash[20], 
									std::vector<std::string> & newTrackers);*/
		TorrentTrackerCommManager(pool,newPeerList, bytes, announceV);
		TorrentPeerwireProtocol(bytes, pool, newPeerList);
		

		//requestPeers(0,0,fLength);
		memset(buf, 0x0, 40);

    }

	exit(0);
}






	/////////////////JOHN KWIATKOSKI///////////////////////
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

void convert(char* str, uint8_t bytes[]){
	unsigned int value;
	unsigned int i=0;
	int idx=0;
	int nullIdx=0;
	while(i<strlen(str)){
		sscanf(&str[i],"%02x",&value);
		value = value & 0xff;
		if(bytes==NULL){
			//printf("%02x\n",value);
			//pList.push_back(value);			
			pList[nullIdx] = value;
			nullIdx++;
		}else{
			bytes[idx]=value;
		}
		i+=2;
		idx++;
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


