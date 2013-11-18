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

#include "../threadpool/threadpool.h"
#include "../tracker/PeerList.h"
#include "../bencoding/networkManager.h"
#include "../tracker/SystemFunctionWrappers.h"

uint8_t * convert(const char* str);

//Struct used to send/receive a handshake with a peer
struct Handshake_t {
	uint8_t pstrLen;
	const char * pstr;
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


//Class used to communicate with peers
class TorrentPeerwireProtocol{

	public:

		//~Constructor------------------------------------------------
		/* Create a new TorrentPeerwireProtocol object with 
		   a thread pool to use for tasks. */
		TorrentPeerwireProtocol(struct thread_pool * pool);

		//~Methods----------------------------------------------------------
		void upload(Peer & currentPeer);

		void download(uint8_t * info_hash, PeerList & pList, 
						std::vector<Piece> & hashPieces);

		void recvMessage(void * message, uint32_t messageSize, const Peer & p);

		void sendMessage (const void * message, uint32_t messageSize, 
							const Peer & p);

		void connect(uint8_t * infoHash, uint8_t * peerId, 
						Peer & p);

		void handshake(uint8_t * peerId, const Peer & p);

		void keepAlive(const Peer & p);

		void choke(const Peer & p);

		void unchoke(const Peer & p);

		void interested(const Peer & p);

		void notInterested(const Peer & p);

		void bitfield(const Peer & p);

		void have(uint32_t pieceIndex, const Peer & p);

		void request (uint32_t index, uint32_t begin, 
						uint32_t requestedLength, const Peer & p);

		void piece (uint32_t index, uint32_t begin, 
                    uint8_t * block, uint32_t blockLength, 
                    const Peer & p);


		void upload(Peer currentPeer);


		void cancel (uint32_t index, uint32_t begin, 
						uint32_t requestedLength, const Peer & p);


	private:

		/* Thread pool being used by this peerwire object. */
		struct thread_pool * pool;

		/* The info hash of the torrent file we are currently interested in downloading. */
		uint8_t * infoHash;

		/* The id to send with the handshake etc. */
		const char * BIT_TORRENT_ID;

		/* Method used to print out Handshake structs. */
		void printHandshake(const uint8_t * h) const;
};

#endif