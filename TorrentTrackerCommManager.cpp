#include "TorrentTrackerCommManager.h"

TorrentTrackerCommManager::TorrentTrackerCommManager() {
	
	trackers = new std::vector<TorrentTrackerComm>();
}

TorrentTrackerCommManager::TorrentTrackerCommManager(const std::string newFileHash, 
														const std::vector<std::string> newTrackers) {
	
	trackers = new std::vector<TorrentTrackerComm>();

	std::vector<std::string>::iterator it;
	for (it = newTrackers.begin(); it != newTrackers.back(); ++it) {

		const TorrentTrackerComm const * tracker = makeTorrentTrackerComm(newFileHash, *it);
		if (tracker != NULL) {

			trackers.push_back(tracker);
		}
	}		
	
}

TorrentTrackerCommManager::~TorrentTrackerCommManager() {

	while (!trackers.empty()) {
	
		trackers.pop_back();
	}

	delete trackers;
}

const bool TorrentTrackerCommManager::addTracker(const std::string newFileHash, 
													const std::string newTracker) {

	const TorrentTrackerComm const * theTracker = makeTorrentTrackerComm(newFileHash, newTracker);

	if (theTracker != NULL) {

		trackers.push_back(*theTracker);
		return true;
	}
	else {

		return false;
	}
}

const bool TorrentTrackerCommManager::removeTracker(std::string deleteTracker) {

	std::vector<TorrentTrackerComm>::iterator it;
	for (it = trackers.begin(); it != trackers.back(); ++it) {

		//if ((*it).)
	}	
}

const TorrentTrackerComm const * TorrentTrackerCommManager::makeTorrentTrackerComm(std::string trackerString, 
																					std::string fileHash) const {

	const TorrentTrackerComm const * trackerComm;

	size_t portNumIndex = trackerString.find_first_of(":");
	if (portNumIndex == std::string::npos) {
		return NULL;
	}

	size_t endPortNumIndex = trackerString.find_first_of("/", portNumIndex);
	if (endPortNumIndex == std::string::npos) {
		return NULL;
	}

	size_t protocolIndex = trackerString.find_first_of("udp://");
	if (protocolIndex == std::string::npos) {
		
		protocolIndex = trackerString.find_first_of("tcp://");
		
		if protocolIndex == std::string::npos) {
		
			return NULL;
		}
		else {

			trackerComm = new TcpTorrentTrackerComm(trackerString, portNumber, fileHash);
		}
	}
	else {

		trackerComm = new UdpTorrentTrackerComm(trackerString.substr(6, portNumIndex), portNumber, fileHash);
	}

	return trackerComm;
}

void TorrentTrackerCommManager::requestPeers() const {

	std::vector<TorrentTrackerComm>::iterator it;
	for (it = trackers.begin(); it != trackers.back(); ++it) {

		//spawn thread to requestPeers in each tracker
		//if return null, re-establish connection
		
	}
}

