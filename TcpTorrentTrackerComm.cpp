TcpTorrentTrackerComm::TcpTorrentTrackerComm(const std::string tracker, 
							const int newPortNumber, 
							const std::string newFileHash) 
	: TorrentTrackerComm(tracker, newPortNumber, newFileHash) {}

TcpTorrentTrackerComm::TcpTorrentTrackerComm(const std::string tracker, 
							const int newPortNumber, 
							const std::string newFileHash,
							const int newSecondsUntilTimeout) 
	: TorrentTrackerComm(tracker, newPortNumber, newFileHash, newSecondsUntilTimeout) {}

const bool TcpTorrentTrackerComm::initiateConnection(const int amountUploaded,
														const int amountDownloaded,
														const int amountLeft) {

}

const bool TcpTorrentTrackerComm::requestPeers() {
	
}