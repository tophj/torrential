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

//Class used to communicate with peers
class TorrentPeerwireProtocol{

	public:

		//Constructor
		TorrentPeerwireProtocol(uint8_t * info_hash, struct thread_pool * pool,
									  PeerList & pList, std::vector<std::string> & hashpieces);

		void sendMessage (const void * message, uint32_t messageSize);

		void connectToPeer(uint8_t * info_hash, uint8_t *peer_id, const std::string host, int port,
							 std::vector<std::string> hashpieces);

		void handshake();


		void keepAlive();

		void choke();

		void unchoke();

		void interested();

		void nonInterested();

		void bitfield();

		void have(uint32_t pieceIndex);

		void request (uint32_t index, uint32_t begin, uint32_t requestedLength);

		void piece (uint32_t index, uint32_t begin, uint32_t requestedLength);

		void cancel (uint32_t index, uint32_t begin, uint32_t requestedLength);


	private:

		/* The socket this peerwire object is communicating with a peer over. */
		int sockFd;

		/* The info hash of the torrent file we are currently interested in downloading. */
		uint8_t * infoHash;

		/* The peerId of this peerwire. */
		uint8_t * peerId;

		/* The id to send with the handshake etc. */
		const char * BIT_TORRENT_ID;

		/* Method used to print out Handshake structs. */
		void printHandshake(const uint8_t * h) const;
};

#endif