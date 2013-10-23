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

//input, is the tracker
//TODO figure out how to get peerlist once that is working
TorrentPeerwireProtocol::TorrentPeerwireProtocol(const std::string tracker,
													const std::string info_hash){

	//int nthreads = 20; //Who knows, we can play with this
	//struct thread_pool * ex = thread_pool_new(nthreads);

	//How big out db will be
	//int maxConnectedPeers = 200;
	printf("%s", "Hey baby");

	//tracker.getPeerList()
	//Spawn new thread
	//for peers in getPeerList(), spawn new thread.
	//       handshake
	//       send bitfield
	//       unchoke
	//       interested
	//       request piece
	//       keep-alive loop
	//       cancel

}

void TorrentPeerwireProtocol::sendMessage(const std::string message){

	int sd;
    struct sockaddr_in server;

    //Might want to add ipv6 support if this doesn't work
    struct in_addr ipv4addr;
    struct hostent *hp;

    sd = socket(AF_INET,SOCK_STREAM,0);
    server.sin_family = AF_INET;

    inet_pton(AF_INET, HOST, &ipv4addr);
    hp = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
    //hp = gethostbyname(HOST);

    bcopy(hp->h_addr, &(server.sin_addr.s_addr), hp->h_length);
    server.sin_port = htons(PORT);

    connect(sd, (const sockaddr *)&server, sizeof(server));
    send(sd, (char *)message.c_str(), strlen((char *)message.c_str()), 0);
}

void TorrentPeerwireProtocol::handshake(const std::string info_hash,
												const std::string peer_id){
	//Construct the message
	uint8_t pstrlen = 19;
	const std::string pstr = "BitTorrent protocol";
	double reserved8Bytes = 0;

	std::ostringstream messageStream;
	messageStream << pstrlen << pstr << reserved8Bytes << info_hash << peer_id;
	const std::string message = messageStream.str();

	sendMessage(message);

	
}


// 0 byte payload message, default timeout for connection is two minutes so send this every 90 seconds
// Spawn a new thread on each connection for this
void TorrentPeerwireProtocol::sendKeepAlive(const std::string peer_id, long seconds){

	while(1){

		seconds = seconds * 1000;
		//delay(seconds);
		const std::string message = "";

		sendMessage(message);
	}
	
}

//Choke a peer to prevent them from sending information
//<len=0001><id=0>
void TorrentPeerwireProtocol::choke(const std::string peer_id){

	//Construct the message
	long length = 1;
	uint8_t id = 0;
	

	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	sendMessage(message);
}

//Unchoke a peer and startup data xfer
//<len=0001><id=1>
void TorrentPeerwireProtocol::unchoke(const std::string peer_id){

	//Construct the message
	long length = 1;
	uint8_t id = 1;
	

	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	
	sendMessage(message);
}

//Fixed-length and has no payload
//<len=0001><id=2>
void TorrentPeerwireProtocol::interested(const std::string peer_id){
	//Construct the message
	long length = 1;
	uint8_t id = 2;
	

	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	sendMessage(message);
}

//Fixed-length and has no payload
//<len=0001><id=3>
void TorrentPeerwireProtocol::nonInterested(const std::string peer_id){
	//Construct the message
	long length = 1;
	uint8_t id = 4;
	

	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	sendMessage(message);
}
// In the for <len0013><id=6><index><begin><length big endian>
// index : int specifying the zero-based piece index
// begin : int specifying the zero-based byte offset within the piece
// length : int specifying the requested length
void TorrentPeerwireProtocol::request(const std::string tracker,const std::string peer_id){

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
	sendMessage(message);

}
//<len=0005><id=4><piece_index>
//fixed length, piece_index is the zero-based index of a piece that has just been downloaded and 
//verified via the hash
void TorrentPeerwireProtocol::have(const std::string piece, const std::string peer_id){

	long length = 1;
	uint8_t id = 4;
	

	std::ostringstream messageStream;

	//May have to extrapolate the piece_index
	messageStream << length << id << piece;
	const std::string message = messageStream.str();

	sendMessage(message);
}

//<len=0001+X><id=5><bitfield>
//Only sent after the initial handshake, and before any other message is sent
//X here is the length of the bitfield.
//The payload, the bitfield, represents pieces that have been downloaded successfully, 
//The high bit in the first byte represents piece index 0. 
void TorrentPeerwireProtocol::bitfield(const std::string peer_id){

	long length = 1;
	uint8_t id = 5;


	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	sendMessage(message);
}

//<len=0009+X><id=7><index><begin><block>
//X here is the length of the block of the piece
void TorrentPeerwireProtocol::piece(const std::string peer_id){
	long length = 9;
	uint8_t id = 7;


	//Research more what this is actually used for
	std::ostringstream messageStream;
	messageStream << length << id;
	const std::string message = messageStream.str();

	sendMessage(message);
}

//<len=0013><id=8><index><begin><length>
void TorrentPeerwireProtocol::cancel(const std::string peer_id,
										int index, int begin, int length){
	long messagelength = 13;
	uint8_t id = 8;
	//Construct the message
	std::ostringstream messageStream;
	messageStream << messagelength << id << index << begin << length;
	const std::string message = messageStream.str();


	sendMessage(message);
}

