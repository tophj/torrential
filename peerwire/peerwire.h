#ifndef TORRENTIAL_DOWNLOADS_PEERWIRE_PROTOCOL
#define TORRENTIAL_DOWNLOADS_PEERWIRE_PROTOCOL

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




//Look into why threadpool no longer works
#include "../threadpool/threadpool.c"
#include "../threadpool/list.c"
#include "../tracker/TorrentTrackerCommManager.h"
#include "../bencoding/bencode.h"



class TorrentPeerwireProtocol{

	public:

		//Constructor
		TorrentPeerwireProtocol(const std::string tracker, const std::string info_hash,
			struct thread_pool *pool, const std::string host);

		//Some methods

		void sendMessage(const std::string message, int socket);

		void connectToPeer(const std::string info_hash, const std::string peer_id, const std::string host, uint16_t port);
		void handshake(const std::string info_hash,const std::string peer_id, int socket);

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


};


#endif


