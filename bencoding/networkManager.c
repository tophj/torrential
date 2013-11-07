#include "networkManager.h"

//HARD
std::vector<std::string> pList(2000);
std::vector<std::string> pListTwo(2000);
std::vector<std::string> pListThree(2000);
//HARD
std::vector<std::string> announceV(20);
uint8_t bytes[20];
int j=0;
int i=0;
char hash[SHA_DIGEST_LENGTH];
int parallel=1;
int fLength, pieceLen; 
int main(int argc, char** argv)
{
	bt_args_t bt_args;
 	be_node * node; // top node in the bencoding
 	parse_args(&bt_args, argc, argv);
	char *announce; 
	char *aList[20];
	unsigned char *pieces;
	
	uint8_t *info_hash; 	//ubuntu desktop should be 14FFE5DD23188FD5CB53A1D47F1289DB70ABF31E
	char* file;
	if (argc>2)
	{
		if (strcmp(argv[2],"-p")==0)
		{
			parallel = atoi(argv[3]);
		}
		
		
	}
	for(i=0;i<MAX_CONNECTIONS;i++){

      		if(bt_args.peers[i] != NULL)
        		print_peer(bt_args.peers[i]);
   	}
   	
	node = load_be_node(argv[1],&file);
	//be_dump(node);
	//Get explicit info
	parser(node, &announce, aList, &fLength, &pieceLen, &pieces);
	printf("%d----%d-----%d\n",pieceLen,fLength, fLength/pieceLen );
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
	while(aList[i]!='\0'){
		converted=std::string(aList[i]);
		announceV.push_back(converted);
		i++;
	}
	pieceByPiece(fLength/pieceLen,(char*)pieces);
	//Initialize things
	thread_pool *pool;
	pool=thread_pool_new(8);
	PeerList newPeerList;

	

	//TorrentTrackerCommManager(struct thread_pool * theThreadPool, PeerList & newPeerList, uint8_t newFileHash[20], std::vector<std::string> & newTrackers);
//TorrentTrackerCommManager(pool,newPeerList, bytes, announceV);
	// TorrentPeerwireProtocol(uint8_t info_hash[20],struct thread_pool *pool, PeerList & pList, uint8_t hashpieces[])
//TorrentPeerwireProtocol((uint8_t*)bytes, pool, newPeerList,pList);


	exit(0);
}



 ////////////////////////////////////////////////////////////////////////////////////
 /*									METHOODS
*//////////////////////////////////////////////////////////////////////////////////////



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
				
				//find a null check
				break;
			}
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
		printf("%x\n", hash[i] & 0xff);
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
void pieceByPiece(int len, char* pieces){
//HARD
	//goes through each piece
	for (j=0; j < fLength/pieceLen; j++) {
		char buf[60];
		int index=0;
		uint8_t value = 0xff & pieces[0];
		//for each piece print out its 20 SHA1 values
		for(i=j*20;i<(j*20)+20;i++){

			sprintf((char*)&(buf[index*2]), "%02x", pieces[i] & 0xff);
			index++;
		}
		
		//convert the Hexstring to bye array using <vector>
		printf("%s\n",buf);
		//convert(buf,bytes);
		//add it to the list
		if (parallel !=1)
		{
			if (j%parallel==1)
			{
				pList.push_back(buf);
			}else if (j%parallel==2)
			{
				pListTwo.push_back(buf);
			}else if (j%parallel==3)
			{
				pListThree.push_back(buf);
			}
		}else{
			pList.push_back(buf);
		}
		memset(buf, 0x0, 60);
	}	
}


char * create_infohash(char* file){
	
	char* info = strstr(file, ":info");
	//HARD?
	char whole[30000]; 
	info = &(info[5]);
	//printf("%s\n", info);
	i=0;
	//HARD
	while(i<28861){

		whole[i] = info[i];
	//	printf("%c", whole[i]);
		i++;
	}

	
	info_hash(whole, hash);
	return hash;

}

void info_hash(char* dict, char *id){
 //HARD
  SHA1((unsigned char *) dict, 28861, (unsigned char *) hash);   
  
  return;
}