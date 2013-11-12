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

uint8_t * convert(char * str){

	uint8_t * bytes = (uint8_t *) malloc(20);
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
	uint8_t * info_hash;
	info_hash  = convert(temp);
	
	thread_pool pool;
	
	PeerList newPeerList;
	std::vector<std::string> hashpieces;

	TorrentPeerwireProtocol peerwire = TorrentPeerwireProtocol(info_hash, &pool, newPeerList, hashpieces);

    peerwire.handshake();

	return 0;
}



//using namespace libtorrent;
//hashpieces are all the hash files this instantiation of peerwire protocol is reuqired to download
TorrentPeerwireProtocol::TorrentPeerwireProtocol(uint8_t * info_hash, struct thread_pool *pool,
												  PeerList & pList, std::vector<std::string> & hashpieces){

	printf("Launching Peerwire...\n");
	printf("Searching through peers...\n");

    infoHash = info_hash;

	uint8_t id[] = {20,10,20,20,02,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
    BIT_TORRENT_ID = "\023BitTorrent protocol";

	peerId = id;

	connectToPeer(info_hash, peerId, "", 4, hashpieces);
    handshake();
}

void TorrentPeerwireProtocol::connectToPeer(uint8_t* info_hash,
											uint8_t* peer_id, 
											const std::string host,
											int port,
											std::vector<std::string> hashpieces) {

	#define HOST "24.85.80.212"
	#define PORT 46928
    
	///////////////////////////////////////////////////////////////////////////////////////////////////
	struct sockaddr * saddr;
    struct addrinfo hints, * ai,  * it;
    //int sockFd = -1;
    size_t addrlen;
    char strportnum[25];

    memset(&hints, '\0', sizeof(hints));
    hints.ai_flags = AI_ADDRCONFIG;
    hints.ai_socktype = SOCK_STREAM;

    snprintf(strportnum, 10, "%d", PORT);

    GetAddrInfo(HOST, strportnum, &hints, &ai);

    for (it = ai; it != NULL; it = it->ai_next) {

    	std::cout << "looping over results of Getaddrinfo,.....\n";
    	std::cout << "SOCKET-ING!\n";
        if ((sockFd = Socket(it->ai_family, it->ai_socktype, it->ai_protocol)) != -1) {
        	std::cout << "SOCKETED!\n";

            saddr = ai->ai_addr;
            saddr->sa_family = AF_INET;
            addrlen = ai->ai_addrlen;

			//////////////////////////////////////////////////////////////////////////////////
			//////////////////////this shizzzzzzzzzzzzzz................/////////////////////
			//////////////////////////////////////////////////////////////////////////////////
			int res; 
			//struct sockaddr_in addr; 
			long arg; 
			fd_set myset; 
			struct timeval tv; 
			int valopt; 
			socklen_t lon; 

			// Set non-blocking 
			if( (arg = fcntl(sockFd, F_GETFL, NULL)) < 0) { 
			 fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno)); 
			 exit(0); 
			} 
			arg |= O_NONBLOCK; 
			if( fcntl(sockFd, F_SETFL, arg) < 0) { 
			 fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno)); 
			 exit(0); 
			} 

			// Trying to connect with timeout 
			res = Connect(sockFd, saddr, sizeof(*saddr)); 
			if (res < 0) { 
			 if (errno == EINPROGRESS) { 
			    fprintf(stderr, "EINPROGRESS in connect() - selecting\n"); 
			    do { 
			       tv.tv_sec = 15; 
			       tv.tv_usec = 0; 
			       FD_ZERO(&myset); 
			       FD_SET(sockFd, &myset); 
			       res = select(sockFd + 1, NULL, &myset, NULL, &tv); 
			       if (res < 0 && errno != EINTR) { 
			          fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); 
			          exit(0); 
			       } 
			       else if (res > 0) { 
			          // Socket selected for write 
			          lon = sizeof(int); 
			          if (getsockopt(sockFd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) { 
			             fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno)); 
			             exit(0); 
			          } 
			          // Check the value returned... 
			          if (valopt) { 
			             fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt)); 
			             exit(0); 
			          } 
			          break; 
			       } 
			       else { 
			          fprintf(stderr, "Timeout in select() - Cancelling!\n"); 
			          exit(0); 
			       } 
			    } while (1); 
			 } 
			 else { 
			    fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); 
			    exit(0); 
			 } 
			} 
			// Set to blocking mode again... 
			if( (arg = fcntl(sockFd, F_GETFL, NULL)) < 0) { 
			 fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno)); 
			 exit(0); 
			} 
			arg &= (~O_NONBLOCK); 
			if( fcntl(sockFd, F_SETFL, arg) < 0) { 
			 fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno)); 
			 exit(0); 
			} 
			// I hope that is all 
			//////////////////////////////////////////////////////////////////////////////////
			//////////////////////this shizzzzzzzzzzzzzz................/////////////////////
			//////////////////////////////////////////////////////////////////////////////////

        }
    }
    std::cout << "I MADE IT\n";
    freeaddrinfo(ai);

    ///////////////////////////////////////////////////////////////////////////////////////////////////


}


//attempt number 2?
int TorrentPeerwireProtocol::dial(int type, char* addr,unsigned short port){
	
	struct addrinfo *ai , hint;
    struct sockaddr_in *saddr;
    int sock;
    int gaierr;

    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = type;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = IPPROTO_TCP;

    if((gaierr = getaddrinfo(addr, NULL, &hint, &ai)) > 0){
        fprintf(stderr, "dial: %s", gai_strerror(gaierr));
        exit(1);
    }
    saddr = (struct sockaddr_in*) ai->ai_addr;
    saddr->sin_port = htons(port);

    if((sock = socket(type, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("dial: socket");
        exit(1);
    }

    if(connect(sock, ai->ai_addr, sizeof(struct sockaddr)) < 0){
        perror("dial: connect");
        exit(1);
    }
   	printf("Got to the end!");
    freeaddrinfo(ai);

    return sock;
}

void TorrentPeerwireProtocol::sendMessage(char * message){
	//printf("%s\n", message);
	int i = 0;

	printf("Got to send message\n");

    i = Send(sockFd, message, strlen(message), 0);
    if(i < 0){
    	printf("MAIN: ERROR send() %i:  %s\n", errno, strerror(errno));
	    exit(1);
    }
    else{
    	printf("Send worked!");
    }   
}

void TorrentPeerwireProtocol::handshake() {

    printf("Got in handshake, all the networking is good!");
    const int handshakeSize = 1 + strlen(BIT_TORRENT_ID) + 8 + 20 + 20;
    uint8_t * handshake = new uint8_t[handshakeSize];

    if (Send(sockFd, handshake, handshakeSize, 0) > 0) {
std::cout << "Oh lookie, success!\n";
    }
    else {
std::cout << "Oh lookie, YOU'RE FUCKED\n";
    }
/*
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
*/	
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
	

	// const int choke_size = 1;
	// char choke[choke_size];

	// const char prefix = 19;
	// const std::string BitTorrent_protocol = "BitTorrent protocol";
	// const char reserved8Bytes = 0000000000000000;


	// const int protocol_name_offset = 1;
	// const int reserved_offset = protocol_name_offset + 19;
	// const int info_hash_offset = reserved_offset + 8;
	// const int peer_id_offset = info_hash_offset + 20;

	// handshake[0] = prefix; 
	// std::copy(BitTorrent_protocol.begin(), BitTorrent_protocol.end(),
 //          &handshake[protocol_name_offset]);
	// strncpy(&handshake[reserved_offset], &reserved8Bytes,sizeof(reserved8Bytes));
	// std::copy(&info_hash[0], &info_hash[19],
 //          &handshake[info_hash_offset]);
	// std::copy(&peer_id[0],&peer_id[19],
	// 	&handshake[peer_id_offset]);


	// sendMessage(handshake, socket);

	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	//sendMessage(message, socket);
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

	
	//sendMessage(message, socket);
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

	//sendMessage(message, socket);
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

	//sendMessage(message, socket);
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
	//sendMessage(message, socket);

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

	//sendMessage(message, socket);
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

	//sendMessage(message, socket);
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

	//sendMessage(message, socket);
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


	//sendMessage(message, socket);
}

