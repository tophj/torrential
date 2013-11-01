#include "networkManager.h"


std::vector<uint8_t> pList(32);
std::vector<std::string> announceV(20);
uint8_t bytes[20];
int j=0;
int i=0;
char* hash;
int main(int argc, char** argv)
{
	bt_args_t bt_args;
 	be_node * node; // top node in the bencoding
 	parse_args(&bt_args, argc, argv);
	char *announce; 
	char *aList[20];
	unsigned char *pieces;
	int fLength, pieceLen; 
	char* info_hash; 	//ubuntu desktop should be 14FFE5DD23188FD5CB53A1D47F1289DB70ABF31E
	char* file;
	for(i=0;i<MAX_CONNECTIONS;i++){

      		if(bt_args.peers[i] != NULL)
        		print_peer(bt_args.peers[i]);
   	}
   	
	node = load_be_node(argv[1],&file);
	//be_dump(node);
	//Get explicit info
	parser(node, &announce, aList, &fLength, &pieceLen, &pieces);
	
	//get info Hash of info dict
	info_hash = create_infohash(file);
	//check it matches
	printf("%s\n" ,hash);
	convert(hash, bytes);


	//needed a vector of announces converting...
	std::string converted;
	while(aList[i]!=NULL){
		converted=std::string(aList[i]);
		announceV.push_back(converted);
		i++;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;

	uint8_t temp[40];
	//int index=0;
	char create[40];
	char buf[16857];

	for (j=0; j < 16957; j++) {
		sprintf((char*)&(buf[j*2]), "%02x", pieces[j]);

	}
	printf("%s\n",buf );
	for (i=0;i<20;i++){
		sprintf(create,"%c", temp[i]);
	}

	//sprintf(create,"%s", byt[]);

	//go through all pieces in file
	//printf("___%d____\n", r);
	
	tp_t *pool;
	pool=thread_pool_new(8);
	PeerList newPeerList;

	

		/*TorrentTrackerCommManager(struct thread_pool * theThreadPool,
									PeerList & newPeerList,
									uint8_t newFileHash[20], 
									std::vector<std::string> & newTrackers);*/


		//requestPeers(0,0,fLength);
		


	//main program execution shiz
	
	TorrentTrackerCommManager(pool,newPeerList, bytes, announceV);
	TorrentPeerwireProtocol(bytes, pool, newPeerList);

	exit(0);
}



 ////////////////////////////////////////////////////////////////////////////////////
 /*


								
										METHODS
										_______








*//////////////////////////////////////////////////////////////////////////////////////



void parser(be_node *node, char** announce, char* aList[], int* fLength, int* pieceLen, unsigned char** pieces){
	size_t i=0;
	int list=0;
	switch (node->type) {
		case BE_STR:
			if (strcmp(node->info,"announce")==0){
				*announce = node->val.s;

			}else if(strcmp(node->info,"announce-list")==0){
				aList[list] =( char*) node->val.s;
				printf("%s\n", aList[list]);
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
			if (strcmp(node->val.d[0].key,"info")==0)
			{
				printf("WOOOHOO\n");
				printf("%s\n",node->val.d[0].key);
			}
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
	
void pieceByPiece(int len, char* pieces){
//add len here ---|||||------
	for (j=0; j < 16957; j++) {
		char buf[40];
		int index=0;
		//for each piece print out its 20 SHA1 values
		for(i=j*20;i<(j*20)+20;i++){
			sprintf((char*)&(buf[index*2]), "%02x", pieces[i]);
			index++;
		}
		
		//convert the Hexstring to bye array using <vector>
		convert(buf,bytes);
		memset(buf, 0x0, 40);
	}
}


char * create_infohash(char* file,char* hash){
	char* info = strstr(file, ":info");
	info = &info[5];
	info_hash(info, hash);
	return hash;

}

void info_hash(char* dict, char *id){
  char data[256];
  int len;
  
  //format print
  len = snprintf(data,256,"%s%u",dict);

  SHA1((unsigned char *) data, len, (unsigned char *) id); 

  return;
}