#include "networkManager.h"

//HARD
static std::vector<std::string> pList(10000);
//HARD
static std::vector<std::string> announceV(20);

static uint8_t bytes[20];
static std::vector<std::string> progress = {"0|          |100", "0|.         |100", "0|..        |100", "0|...       |100", "0|....      |100", "0|.....     |100", "0|......    |100" , "0|.......   |100", "0|........  |100", "0|......... |100", "0|..........|100"};

static uint32_t j = 0;
static uint32_t i = 0;
static char hash[SHA_DIGEST_LENGTH];


static int fLength, pieceLen; 
static int tfLength;
static uint32_t info_length;
static int num_pieces;
static int completed;

int main(int argc, char** argv)
{

	bt_args_t bt_args;
 	be_node * node; 
 	parse_args(&bt_args, argc, argv);
	char *announce; 
	char *aList[20];
	unsigned char *pieces;
	FILE* torr;
	uint8_t * info_hash;
	char* file;
	iptool * tool = (iptool*) malloc(sizeof(struct ipchoose));

	for (i = 2; i < (uint32_t) argc; i++)
	{
		tool->add(argv[i]);
	}

	for(i=0;i<MAX_CONNECTIONS;i++){

  		if(bt_args.peers[i] != NULL)
    		print_peer(bt_args.peers[i]);
   	}
   	
   	torr = fopen(argv[1],"r");
   	fseek(torr,0L,SEEK_END);
   	tfLength=ftell(torr);
   	fclose(torr);

	node = load_be_node(argv[1],&file);
	//be_dump(node);
	//Get explicit info
	parser(node, &announce, aList, &fLength, &pieceLen, &pieces);
	//printf("%d----%d-----%d\n",pieceLen,fLength, fLength/pieceLen );
	//get info Hash of info dict
	create_infohash(file);

	printBytes((char*)hash);
	//converts bytes and stores
	info_hash = convert((char*)hash);

	//needed a vector of announces converting...
	std::string converted;
	//aList[19] = NULL;
	memset(aList,'\0', sizeof(char)*20);
	i=0;
	while (aList[i] != '\0') {

		converted=std::string(aList[i]);
		announceV.push_back(converted);
		i++;
	}

	pieceByPiece(file, (char*) pieces);

	//Initialize things
	thread_pool * pool;
	pool = thread_pool_new(8);
	PeerList newPeerList;
	std::vector<Piece> pieceList;

	TorrentTrackerCommManager tracker(pieceLen, tool, pool, newPeerList, bytes, announceV, tool);
	TorrentPeerwireProtocol peerwire(pieceLen, tool, hash, pool, newPeerList, pList);

	tracker.requestPeers(0, 0, fLength);
	peerwire.download(info_hash, newPeerList, pieceList, pieceLen);

	exit(0);
}



////////////////////////////////////////////////////////////////////////////////////////
/*									METHOODS
*///////////////////////////////////////////////////////////////////////////////////////



void parser(be_node *node, char** announce, char* aList[], int* fLength, int* pieceLen, unsigned char** pieces ){
	size_t i=0;
	int list=0;
	if (node == NULL)
	{
		return;
	}
	switch (node->type) {
		case BE_STR:
			if(node->info==0){
				break;
			}
			if (strcmp(node->info,"announce")==0){
				*announce = node->val.s;

			}else if(strcmp(node->info,"announce-list")==0){
				aList[list] =( char*) node->val.s;
				//printf("%s\n", aList[list]);
				list++;
			}else if(strcmp(node->info,"pieces")==0){
				*pieces =(unsigned char*) node->val.s;
			}
			break;
		case BE_INT:
			if(node->info==0){
				break;
			}
			if (strcmp(node->info,"length")==0){
				*fLength = node->val.i;
			}else if(strcmp(node->info,"piece length")==0){
				*pieceLen = node->val.i;
			}
			break;
		case BE_LIST:
		if (node->val.l==0x0)
			{
				//find a null check
				break;
			}
			for (i = 0; node->val.l[i]; ++i){
				if (&(node->val.l[i])==NULL || node->val.l[i]->info==NULL)
			{
				
				break;
			}
				/*if (strcmp(node->val.l[i].val, "files")==0)
				{
					printf("FILESS\n");
				}*/
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

void printBytes(char* str){

	i=0;
	while(i<SHA_DIGEST_LENGTH){
		printf("%02x\n", hash[i] & 0xff);
		i++;
	 }
}
uint8_t * convert(char* str){

	i=0;
	while(i<SHA_DIGEST_LENGTH){
		//printf("%x\n", str[i]& 0xff);
		bytes[i] = str[i] & 0xff;
		//printf("%x\n", bytes[i]);
		i++;
	 }
	 return bytes;
}


void pieceByPiece(char* file, char* pieces){
	//goes through each piece
	//F 1 339140
	uint64_t total = 0;
	uint64_t multi[10];
	
	if (fLength == 0){
		char* temp = strstr(strstr(file,"files"),":lengthi");
		temp=&temp[8];
		multi[0] = strtoull(temp,NULL,10);
		temp = strstr(temp,":lengthi");
		temp=&temp[8];
		multi[1] = strtoull(temp,NULL,10);
		total = (multi[1]+multi[0])/pieceLen;
		if ((multi[0]+multi[1])%pieceLen!=0)
		{
			total++;
		}
	}else{
		if (fLength%pieceLen!=0){
			total = fLength/pieceLen+1;
		}else{
			total = fLength/pieceLen;
		}
	}
	num_pieces = total;
	for (j = 0; j < total; j++) {
		char buf[60];
		int index=0;
	//	uint8_t value = 0xff & pieces[0];
		//for each piece print out its 20 SHA1 values
		for(i=j*20;i<(j*20)+20;i++){
			sprintf((char*)&(buf[index*2]), "%02x", pieces[i] & 0xff);
			index++;
		}
		
		//convert the Hexstring to bye array using <vector>
		//printf("%s\n",buf);
		std::string buff(buf);
		//convert(buf);
		pList.push_back(buff);
	
		memset(buf, 0x0, 60);
	}	
}


char * create_infohash(char* file){
	
	char* info = strstr(file, ":info");
	
	char whole[3000000]; 
	info = &(info[5]);
	info_length = tfLength - (info-file);

	i=0;
	//HARDc  u3   28861
	while(i < info_length-1){

		whole[i] = info[i];
		//printf("%c", whole[i]);
		i++;
	}

	
	info_hash(whole, hash);
	return hash;

}

void info_hash(char* dict, char *id){
 //HARD
  SHA1((unsigned char *) dict, info_length-1, (unsigned char *) hash);   
  
  return;
}

void update(double downloaded, double total){

	int count;
	count=(int)(100*(downloaded/total));
	count/=10;
	printf("%s\r",progress[count].c_str());
	fflush(stdout);
	sleep(1);
}