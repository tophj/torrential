#ifndef TORRENTIAL_DOWNLOADS_PEERWIRE_PROTOCOL
#define TORRENTIAL_DOWNLOADS_PEERWIRE_PROTOCOL

#include <stdint.h>
#include <stdio.h>
#include <sstream>
#include "threadpool.c"

class TorrentPeerwireProtocol{

	public:

		//Constructor
		TorrentPeerwireProtocol(const std::string tracker);

		//Some methods

		void TorrentPeerwireProtocol::sendMessage(const std::string message);
		void TorrentPeerwireProtocol::handshake(const std::string info_hash,
														const std::string peer_id);

		void TorrentPeerwireProtocol::sendKeepAlive(const std::string peer_id, long seconds);

		void TorrentPeerwireProtocol::choke(const std::string peer_id);

		void TorrentPeerwireProtocol::unchoke(const std::string peer_id);

		void TorrentPeerwireProtocol::interested(const std::string peer_id);

		void TorrentPeerwireProtocol::nonInterested(const std::string peer_id);

		void TorrentPeerwireProtocol::request(const std::queue pieces
												const std::str tracker,const std::string peer_id);






		void TorrentPeerwireProtocol::have(const std:string piece, const std::string peer_id);


		void TorrentPeerwireProtocol::bitfield(const std::string peer_id);

		void TorrentPeerwireProtocol::piece(const std::string peer_id);

		void TorrentPeerwireProtocol::cancel(const std::string peer_id);


};


#endif


