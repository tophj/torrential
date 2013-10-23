#ifndef TORRENTIAL_DOWNLOADS_PEERWIRE_PROTOCOL
#define TORRENTIAL_DOWNLOADS_PEERWIRE_PROTOCOL

#include <stdint.h>
#include <stdio.h>
#include <sstream>


//Look into why threadpool no longer works
//#include "../threadpool/threadpool.c"

class TorrentPeerwireProtocol{

	public:

		//Constructor
		TorrentPeerwireProtocol(const std::string tracker, const std::string info_hash);

		//Some methods

		void sendMessage(const std::string message);
		void handshake(const std::string info_hash,
														const std::string peer_id);

		void sendKeepAlive(const std::string peer_id, long seconds);

		void choke(const std::string peer_id);

		void unchoke(const std::string peer_id);

		void interested(const std::string peer_id);

		void nonInterested(const std::string peer_id);

		void request(const std::string tracker,const std::string peer_id);






		void have(const std::string piece, const std::string peer_id);


		void bitfield(const std::string peer_id);

		void piece(const std::string peer_id);

		void cancel(const std::string peer_id, int index, int begin, int length);


};


#endif


