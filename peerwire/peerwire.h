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

uint8_t * convert(char* str);

class TorrentPeerwireProtocol{

	public:

		//Constructor
		TorrentPeerwireProtocol(uint8_t * info_hash, struct thread_pool * pool,
									  PeerList & pList, std::vector<std::string> & hashpieces);

		void sendMessage(char* message);

		void connectToPeer(uint8_t * info_hash, uint8_t *peer_id, const std::string host, int port,
							 std::vector<std::string> hashpieces);

		void handshake();

		int dial(int type, char* addr,unsigned short port);

		void sendKeepAlive(const std::string peer_id, int socket);

		void choke(const std::string peer_id, int socket);

		void unchoke(const std::string peer_id, int socket);

		void interested(const std::string peer_id, int socket);

		void nonInterested(const std::string peer_id, int socket);

		void request(const std::string tracker,const std::string peer_id, int socket);

		void have(const std::string piece, const std::string peer_id, int socket);

		void bitfield(const std::string peer_id, int socket);

		void piece(const std::string peer_id, int socket);

		void cancel(const std::string peer_id, int index, int begin, int length, int socket);


	private:

		/* The socket this peerwire object is communicating with a peer over. */
		int sockFd;

		/* The info hash of the torrent file we are currently interested in downloading. */
		uint8_t * infoHash;

		/* The peerId of this peerwire. */
		uint8_t * peerId;

		/* The id to send with the handshake etc. */
		char * BIT_TORRENT_ID;
};


#endif


