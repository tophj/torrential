// Basically, given a user look at the tracker to get a list of peers.
// Then, iterate through that list of peers and look to see if they have the 
// specific piece that tbe user needs.
// If they do, then start a new thread that connects / handshakes through FTP to
// download that piece. 
// Keep looking at the pieces that peer has available until you either hit the limit of 
// pieces you can download simultaneously, or hit the download limit.

//Initiates handshaking
//Requests look like <pstr len><pstr><reserved 8bits><info_hash><peer_id>
#include <stdint.h>
#include <stdio.h>
#include <sstream>
#include "threadpool.c"

//input, is the tracker
//TODO figure out how to get peerlist once that is working
TorrentPeerwireProtocol::TorrentPeerwireProtocol(const std::str tracker){

	int nthreads = 20; //Who knows, we can play with this
	struct thread_pool * ex = thread_pool_new(nthreads);

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

	//prolly something with sockets, figure this out
}

void TorrentPeerwireProtocol::handshake(const std::string info_hash,
												const std::string peer_id){
	//Construct the message
	uint8_t pstrlen = 19;
	const std::string pstr = 'BitTorrent protocol';
	double reserved8Bytes = 0;

	std::ostringstream message;
	message << pstrlen << pstr << reserved8Bytes << info_hash << peer_id;
	const std::string query(message.str());

	sendMessage(message);

	
}


// 0 byte payload message, default timeout for connection is two minutes so send this every 90 seconds
// Spawn a new thread on each connection for this
void TorrentPeerwireProtocol::sendKeepAlive(const std::string peer_id, long seconds){
	seconds = seconds * 1000;
	Sleep(seconds);
	const std::string message = "";

	sendMessage(message);
}

//Choke a peer to prevent them from sending information
//<len=0001><id=0>
void TorrentPeerwireProtocol::choke(const std::string peer_id){

	//Construct the message
	long length = 1;
	uint8_t id = 0;
	

	std::ostringstream message;
	message << length << id;
	const std::string query(message.str());

	sendMessage(message);
}

//Unchoke a peer and startup data xfer
//<len=0001><id=1>
void TorrentPeerwireProtocol::unchoke(const std::string peer_id){

	//Construct the message
	long length = 1;
	uint8_t id = 1;
	

	std::ostringstream message;
	message << length << id;
	const std::string query(message.str());

	
	sendMessage(message);
}

//Fixed-length and has no payload
//<len=0001><id=2>
void TorrentPeerwireProtocol::interested(const std::string peer_id){
	//Construct the message
	long length = 1;
	uint8_t id = 2;
	

	std::ostringstream message;
	message << length << id;
	const std::string query(message.str());

	sendMessage(message);
}

//Fixed-length and has no payload
//<len=0001><id=3>
void TorrentPeerwireProtocol::nonInterested(const std::string peer_id){
	//Construct the message
	long length = 1;
	uint8_t id = 4;
	

	std::ostringstream message;
	message << length << id;
	const std::string query(message.str());

	sendMessage(message);
}
// In the for <len0013><id=6><index><begin><length big endian>
// index : int specifying the zero-based piece index
// begin : int specifying the zero-based byte offset within the piece
// length : int specifying the requested length
void TorrentPeerwireProtocol::request(const std::queue pieces
										const std::str tracker,const std::string peer_id){

	const std::string hashpiece = queue.pop_front();
	//figure out what the hashpieces look like
	//call the tracker.
	sendMessage(message);

}
//<len=0005><id=4><piece_index>
//fixed length, piece_index is the zero-based index of a piece that has just been downloaded and 
//verified via the hash
void TorrentPeerwireProtocol::have(const std:string piece, const std::string peer_id){

	long length = 1;
	uint8_t id = 4;
	

	std::ostringstream message;

	//May have to extrapolate the piece_index
	message << length << id << piece;
	const std::string query(message.str());

	sendMessage(message);
}

//<len=0001+X><id=5><bitfield>
//Only sent after the initial handshake, and before any other message is sent
//X here is the length of the bitfield.
//The payload, the bitfield, represents pieces that have been downloaded successfully, 
//The high bit in the first byte represents piece index 0. 
void TorrentPeerwireProtocol::bitfield(const std::string peer_id){
	sendMessage(message);
}

//<len=0009+X><id=7><index><begin><block>
//X here is the length of the block of the piece
void TorrentPeerwireProtocol::piece(const std::string peer_id){
	//Research more what this is actually used for
	sendMessage(message);
}

//<len=0013><id=8><index><begin><length>
void TorrentPeerwireProtocol::cancel(const std::string peer_id){
	sendMessage(message);
}

