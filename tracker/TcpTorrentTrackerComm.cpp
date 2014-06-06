#include "TcpTorrentTrackerComm.h"

TcpTorrentTrackerComm::TcpTorrentTrackerComm(const std::string tracker, 
												const uint16_t newPortNumber, 
												const uint8_t newFileHash[20],
												const uint16_t myNewPortNumber) 
	: TorrentTrackerComm(tracker, newPortNumber, newFileHash, myNewPortNumber) {}

TcpTorrentTrackerComm::TcpTorrentTrackerComm(const std::string tracker, 
												const uint16_t newPortNumber, 
												const uint8_t newFileHash[20],
												const uint16_t myNewPortNumber,
												const int newSecondsUntilTimeout) 
	: TorrentTrackerComm(tracker, newPortNumber, newFileHash, myNewPortNumber, newSecondsUntilTimeout) {}

TcpTorrentTrackerComm::~TcpTorrentTrackerComm(){

}

const bool TcpTorrentTrackerComm::initiateConnection(std::string ipAddress) {

	return false;
}

const std::vector<Peer * > * TcpTorrentTrackerComm::requestPeers(const uint64_t amountUploaded, 
																	const uint64_t amountDownloaded, 
																	const uint64_t amountLeft) {

	return requestPeers(amountUploaded, amountDownloaded, amountLeft);
}

const std::vector<Peer * > * TcpTorrentTrackerComm::requestPeers(const uint64_t amountUploaded, 
																	const uint64_t amountDownloaded, 
																	const uint64_t amountLeft, 
																	const TrackerEvent event) {

	return NULL;
}