// Basically, given a user look at the tracker to get a list of peers.
// Then, iterate through that list of peers and look to see if they have the 
// specific piece that tbe user needs.
// If they do, then start a new thread that connects / handshakes through FTP to
// download that piece. 
// Keep looking at the pieces that peer has available until you either hit the limit of 
// pieces you can download simultaneously, or hit the download limit.

//Initiates handshaking
//Requests look like <pstr len><pstr><reserved 8bits><info_hash><peer_id>

#include "peerwire.h"

uint8_t bytes[20];
char hash[SHA_DIGEST_LENGTH];


uint8_t * convert(char* str){

	int i=0;
	while(i<SHA_DIGEST_LENGTH){
		//printf("%x\n", str[i]& 0xff);
		bytes[i] = str[i] & 0xff;
		//printf("%x\n", bytes[i]);
		i++;
	 }
	 return bytes;
}
//For testing because I'm lazy
int main(int argc, char** argv){


	char temp[41] = "8C3760CB651C863861FA9ABE2EF70246943C1994";
	uint8_t* info_hash;
	info_hash  = convert(temp);
	
	thread_pool *pool;
	
	PeerList newPeerList;
	std::vector<std::string> hashpieces;

	TorrentPeerwireProtocol(info_hash,pool,newPeerList,hashpieces);



}



//using namespace libtorrent;
//hashpieces are all the hash files this instantiation of peerwire protocol is reuqired to download
TorrentPeerwireProtocol::TorrentPeerwireProtocol(uint8_t* info_hash,struct thread_pool *pool,
													  PeerList & pList, std::vector<std::string> hashpieces){

	
	printf("Launching Peerwire...\n");
	printf("Searching through peers...\n");

	//const std::string stringtest = "nope";
	uint8_t* peer_id;
	uint8_t id[] = {20,10,20,20,02,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};

	peer_id = id;

	std::vector<Peer> peerlist;
	//peerlist = pList.getPeers();



	//uint8_t i_hash[20] = {20,12,13,15,6,16,16,16,16,1,61};
	
	//uint8_t info_hash[20] = info_hash;
	//const std::string fakePeerId = "FOR TESTING";
	//char PeerId[20];
	//PeerId[0] = 12;
	


	//convert info_hash to something usable
	int port = 2;
	std::string fakehost = "hy";
	//int fakeSocket = 0;
	connectToPeer(info_hash,peer_id,fakehost,port,hashpieces);
	//handshake(i_hash,peer_id, fakeSocket);


	// session s;
	// error_code ec;
	// s.listen_on(std::make_pair(6881, 6889), ec);
	
	// add_torrent_params p;
	// p.save_path = "./";
	// p.ti = new torrent_info(file, ec);
	
	// s.add_torrent(p, ec);
	
	// // wait for the user to end
	// char a;
	// scanf("%c\n", &a);
	
	//sendKeepAlive(stringtest,x);
	//Target host details:
	
	//Figure out how to init threadpool

    // const int N = ntasks;
    // struct future * f[N];

    // for (i = 0; i < N; i++) {
    //     struct callable_data * callable_data = malloc(sizeof *callable_data);
    //     callable_data->number = i;
    //     f[i] = thread_pool_submit(ex, 
    //                            (thread_pool_callable_func_t) callable_task, 
    //                            callable_data);
    // }  printf("%s\n", (char *) future_get(f[i]));
    

	
}

void TorrentPeerwireProtocol::connectToPeer(uint8_t* info_hash,
												uint8_t* peer_id, const std::string host,
												 int port,std::vector<std::string> hashpieces){
	//Set up the sockets
	//int sd;
	//struct sockaddr_in server;



	#define PORT 6881 // use his generate port function
	#define HOST "198.82.31.252"

	//Might want to add ipv6 support if this doesn't work
	// struct in_addr ipv4addr;
	// 

	// sd = socket(AF_INET,SOCK_STREAM,0);
	// server.sin_family = AF_INET;

	// inet_pton(AF_INET, HOST, &ipv4addr);
	// hp = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
	// //hp = gethostbyname(host);

	// bcopy(hp->h_addr, &(server.sin_addr.s_addr), hp->h_length);
	// server.sin_port = htons(port);

	// connect(sd, (const sockaddr *)&server, sizeof(server));
	

    //Send the initial handshake and recieve
    

//------------------------------------------------------------------------------

	int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    struct hostent *hp;


    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT); 
    
    hp = gethostbyname(HOST);
    bcopy(hp->h_addr, &(serv_addr.sin_addr.s_addr), hp->h_length);

    if(inet_pton(AF_INET, HOST, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
      
    } 
    printf("Trying to connect to peer...\n");
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
      
    } 
    printf("Connected to peer!\n");

    handshake(info_hash, peer_id, sockfd);
  	
    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    } 

    if(n < 0)
    {
        printf("\n Read error \n");
    } 


    

    //const int handshake_size = 1+19+8+20+20;
	//char reply[handshake_size];

	//recv(sd,reply,handshake_size,0);
	if(recvBuff[0] != 19);
		printf("Something dun gone wrong with the handshake yo");
    // bitfield();
    // sendKeepAlive();

    //Keep connection alive as long as file is downloading
}
void TorrentPeerwireProtocol::sendMessage(const std::string message, int socket){
	//printf("%s\n", message.c_str());
    send(socket, (char *)message.c_str(), strlen((char *)message.c_str()), 0);
}

void TorrentPeerwireProtocol::handshake(uint8_t * info_hash,uint8_t* peer_id, int socket){
	//Construct the message
	printf("Got in handshake, all the networking is good!");
	const int handshake_size = 1+19+8+20+20;
	char handshake[handshake_size];

	const char prefix = 19;
	const std::string BitTorrent_protocol = "BitTorrent protocol";
	const char reserved8Bytes = 0000000000000000;


	const int protocol_name_offset = 1;
	const int reserved_offset = protocol_name_offset + 19;
	const int info_hash_offset = reserved_offset + 8;
	const int peer_id_offset = info_hash_offset + 20;

	handshake[0] = prefix; 
	std::copy(BitTorrent_protocol.begin(), BitTorrent_protocol.end(),
          &handshake[protocol_name_offset]);
	strncpy(&handshake[reserved_offset], &reserved8Bytes,sizeof(reserved8Bytes));
	std::copy(&info_hash[0], &info_hash[19],
          &handshake[info_hash_offset]);
	std::copy(&peer_id[0],&peer_id[19],
		&handshake[peer_id_offset]);


	sendMessage(handshake, socket);

	
}

// 0 byte payload message, default timeout for connection is two minutes so send this every 90 seconds
// Spawn a new thread on each connection for this
void TorrentPeerwireProtocol::sendKeepAlive(const std::string peer_id,  int socket){
	//const std::string msg = unsigned char[MESSAGE_LENGTH];
	// unsigned char msg[MESSAGE_LENGTH];
 //  	memset(msg, 0, MESSAGE_LENGTH);
	// sendMessage(msg,socket);
	
}

//Choke a peer to prevent them from sending information
//<len=0001><id=0>
void TorrentPeerwireProtocol::choke(const std::string peer_id, int socket){

	//Construct the message
	long length = 1;
	uint8_t id = 0;
	

	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	sendMessage(message, socket);
}

//Unchoke a peer and startup data xfer
//<len=0001><id=1>
void TorrentPeerwireProtocol::unchoke(const std::string peer_id, int socket){

	//Construct the message
	long length = 1;
	uint8_t id = 1;
	

	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	
	sendMessage(message, socket);
}

//Fixed-length and has no payload
//<len=0001><id=2>
void TorrentPeerwireProtocol::interested(const std::string peer_id, int socket){
	//Construct the message
	long length = 1;
	uint8_t id = 2;
	

	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	sendMessage(message, socket);
}

//Fixed-length and has no payload
//<len=0001><id=3>
void TorrentPeerwireProtocol::nonInterested(const std::string peer_id, int socket){
	//Construct the message
	long length = 1;
	uint8_t id = 4;
	

	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	sendMessage(message, socket);
}
// In the for <len0013><id=6><index><begin><length big endian>
// index : int specifying the zero-based piece index
// begin : int specifying the zero-based byte offset within the piece
// length : int specifying the requested length
void TorrentPeerwireProtocol::request(const std::string tracker,const std::string peer_id, int socket){

	long length = 13;
	uint8_t id = 6;
	//const std::string hashpiece = queue.pop_front();
	//int index = 
	//int begin =
	//int length = 
	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();
	//figure out what the hashpieces look like
	//call the tracker.
	sendMessage(message, socket);

}
//<len=0005><id=4><piece_index>
//fixed length, piece_index is the zero-based index of a piece that has just been downloaded and 
//verified via the hash
void TorrentPeerwireProtocol::have(const std::string piece, const std::string peer_id, int socket){

	long length = 1;
	uint8_t id = 4;
	

	std::ostringstream messageStream;

	//May have to extrapolate the piece_index
	messageStream << length << id << piece;
	const std::string message = messageStream.str();

	sendMessage(message, socket);
}

//<len=0001+X><id=5><bitfield>
//Only sent after the initial handshake, and before any other message is sent
//X here is the length of the bitfield.
//The payload, the bitfield, represents pieces that have been downloaded successfully, 
//The high bit in the first byte represents piece index 0. 
void TorrentPeerwireProtocol::bitfield(const std::string peer_id, int socket){

	long length = 1;
	uint8_t id = 5;


	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	sendMessage(message, socket);
}

//<len=0009+X><id=7><index><begin><block>
//X here is the length of the block of the piece
void TorrentPeerwireProtocol::piece(const std::string peer_id, int socket){
	long length = 9;
	uint8_t id = 7;


	//Research more what this is actually used for
	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	sendMessage(message, socket);
}

//<len=0013><id=8><index><begin><length>
void TorrentPeerwireProtocol::cancel(const std::string peer_id,
										int index, int begin, int length, int socket){
	long messagelength = 13;
	uint8_t id = 8;
	//Construct the message
	std::ostringstream messageStream;
	messageStream << messagelength << id << index << begin << length;
	const std::string message = messageStream.str();


	sendMessage(message, socket);
}

