#ifndef TORRENTIAL_DOWNLOADS_PEERWIRE_PROTOCOL
#define TORRENTIAL_DOWNLOADS_PEERWIRE_PROTOCOL

#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>

//Network related includes:
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#include "../bencoding/ipTool.h"
#include "../threadpool/threadpool.h"
#include "../tracker/PeerList.h"
#include "../bencoding/networkManager.h"
#include "../tracker/SystemFunctionWrappers.h"

//Function pointer definitions for send and recv messages for tcp and udp
typedef void (* SendMessage)(const void * message, uint32_t messageSize, const Peer & p);
typedef int (* RecvMessage)(void * message, uint32_t messageSize, const Peer & p);

uint8_t * convert(const char* str);
int tcpRecvMessage(void * message, uint32_t messageSize, const Peer & p);
void tcpSendMessage(const void * message, uint32_t messageSize, const Peer & p);
void * peerDownload(void * peer);

//Struct used to send/receive a handshake with a peer
struct Handshake_t {
	uint8_t pstrLen;
	uint8_t pstr[19];
	uint8_t reserved[8];
	uint8_t infoHash[20];
	uint8_t peerId[20];
} typedef Handshake;

//Struct used to send/receive a piece
struct Piece_t{
  int index; //which piece index
  int begin; //offset within piece
  unsigned char piece[0]; //pointer to start of the data for a piece
} typedef Piece_t;

//Enumeration for the return value of connect, used to determine if the peer is invalid or maybe UDP
enum ConnectStatus {SUCCESS = 0, TIMEOUT, FAIL};

//Class used to communicate with peers
class TorrentPeerwireProtocol {

	public:

		//~Constructor------------------------------------------------
		/* Create a new TorrentPeerwireProtocol object with 
		   a thread pool to use for tasks. */
		//TorrentPeerwireProtocol(struct thread_pool * pool);
		TorrentPeerwireProtocol(int pieceLen, iptool * itool, char* hash, struct thread_pool* thePool, PeerList newPeerList, std::vector<std::string> pList);
		//TorrentPeerwireProtocol(int pieceLen, char* hash ,struct thread_pool* pool ,PeerList newPeerList,std::vector<std::string> pList);
		//~Methods----------------------------------------------------------
		void recieve(Peer & currentPeer, int pieceLen);

		void download(uint8_t * infoHash, PeerList & pList, 
                   		std::vector<Piece> & hashPieces,
                        int pieceLen);

		ConnectStatus connect(uint8_t * infoHash, uint8_t * peerId, Peer & p);

		void listen(uint8_t * infoHash);

		bool handshake(uint8_t * infoHash, uint8_t * peerId, const Peer & p, SendMessage sendMessage, RecvMessage recvMessage);

		void parseHandshake(void * buffer);

		void keepAlive(const Peer & p, SendMessage sendMessage);

		void choke(const Peer & p, SendMessage sendMessage);

		void unchoke(const Peer & p, SendMessage sendMessage);

		void interested(const Peer & p, SendMessage sendMessage);

		void notInterested(const Peer & p, SendMessage sendMessage);

		void bitfield(const Peer & p, SendMessage sendMessage);

		void parseBitfield(uint8_t * buffer, uint32_t length, Peer & p);

		std::string * parseByte(uint8_t byte);

		void have(uint32_t pieceIndex, const Peer & p, SendMessage sendMessage);

		void request (uint32_t index, uint32_t begin, 
						uint32_t requestedLength, 
						const Peer & p, SendMessage sendMessage);

		void parseRequest(void * buffer);

		void piece (uint32_t index, uint32_t begin, 
                    uint8_t * block, uint32_t blockLength, 
                    const Peer & p, SendMessage sendMessage);

		void parsePiece(void * buffer);

		void cancel (uint32_t index, uint32_t begin, 
						uint32_t requestedLength, 
						const Peer & p, SendMessage sendMessage);


	private:

		/* Thread pool being used by this peerwire object. */
		struct thread_pool * pool;
		iptool * tool;
    
		/* The info hash of the torrent file we are currently interested in downloading. */
		//uint8_t * infoHash;

		/* The id to send with the handshake etc. */
		const char * BIT_TORRENT_ID;

		/* Method used to print out Handshake structs. */
		void printHandshake(const uint8_t * h) const;
};

#endif
