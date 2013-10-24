#include "TorrentTrackerCommManager.h"

TorrentTrackerCommManager::TorrentTrackerCommManager() {
	
	trackers = new std::vector<TorrentTrackerComm *>();
	portSet = new std::unordered_set<uint16_t>();
}

TorrentTrackerCommManager::TorrentTrackerCommManager(const std::string & newFileHash, 
														std::vector<std::string> & newTrackers) {
	
	trackers = new std::vector<TorrentTrackerComm *>();
	portSet = new std::unordered_set<uint16_t>();

	std::vector<std::string>::iterator it;
	for (it = newTrackers.begin(); it != newTrackers.end(); ++it) {

		TorrentTrackerComm * tracker = makeTorrentTrackerComm(newFileHash, *it);
		if (tracker != NULL) {

			tracker->printTorrentTrackerComm();

			if (tracker->initiateConnection()) {
			
				trackers->push_back(tracker);
			}
			else {

				std::cout << "FAILED TO CONNECT TO TRACKER: |" << *it << "|\n";
			}
		}
		else {

			std::cout << "FAILED TO ADD TRACKER: |" << *it << "|\n";
		}
	}		
	
}

TorrentTrackerCommManager::~TorrentTrackerCommManager() {

	while (!trackers->empty()) {
	
		trackers->pop_back();
	}

	delete trackers;
}

const uint16_t TorrentTrackerCommManager::generatePortNumber() const {

	uint16_t basePortNum = 50000;

	while (basePortNum != 50000 
		&& portSet->find(basePortNum) != portSet->end()) {

		(basePortNum) += (rand() % 10000);
		(basePortNum) += (rand() % 1000);
		(basePortNum) += (rand() % 100);
		(basePortNum) += (rand() % 10);
	}

	return basePortNum;
}

const bool TorrentTrackerCommManager::addTracker(const std::string & newFileHash, 
													const std::string & newTracker) {

	TorrentTrackerComm * theTracker = makeTorrentTrackerComm(newFileHash, newTracker);

	if (theTracker != NULL) {

		theTracker->initiateConnection();

		trackers->push_back(theTracker);
		return true;
	}
	else {

		return false;
	}

}

const bool TorrentTrackerCommManager::removeTracker(const std::string & deleteTracker) {

	std::vector<TorrentTrackerComm *>::iterator it;
	for (it = trackers->begin(); it != trackers->end(); ++it) {

		if (*((*it)->trackerHostname) == deleteTracker) {

			trackers->erase(it);
			return true;
		}
	}

	return false;
}

TorrentTrackerComm * TorrentTrackerCommManager::makeTorrentTrackerComm(const std::string & fileHash, 
																		const std::string & trackerString) const {

	TorrentTrackerComm * trackerComm;
	uint16_t portNumber;

	//Find port number
	size_t portNumIndex = trackerString.find_first_of(":");
	portNumIndex = trackerString.find_first_of(":", portNumIndex + 1);

	if (portNumIndex == std::string::npos) {
		return NULL;
	}

	//Find the /announce
	size_t endPortNumIndex = trackerString.find_first_of("/", portNumIndex);
	if (endPortNumIndex == std::string::npos) {
		return NULL;
	}

	//Extract port number from string
	portNumber = std::stoi(trackerString.substr(portNumIndex + 1, endPortNumIndex));

	//Check if UDP
	size_t protocolIndex = trackerString.find_first_of("udp://");
	if (protocolIndex == std::string::npos) {
		
		//Check if TCP
		protocolIndex = trackerString.find_first_of("tcp://");
		if (protocolIndex == std::string::npos) {
		
			return NULL;
		}
		else {

			trackerComm = new TcpTorrentTrackerComm(trackerString.substr(6, portNumIndex - 6), portNumber, fileHash, generatePortNumber());
		}
	}
	else {

		trackerComm = new UdpTorrentTrackerComm(trackerString.substr(6, portNumIndex - 6), portNumber, fileHash, generatePortNumber());
	}

	return trackerComm;
}

void TorrentTrackerCommManager::requestPeers(const uint64_t amountUploaded, 
												const uint64_t amountDownloaded, 
												const uint64_t amountLeft) const {

	std::vector<TorrentTrackerComm *>::iterator it;
	for (it = trackers->begin(); it != trackers->end(); ++it) {

		//spawn thread to requestPeers in each tracker
		//if return null, re-establish connection
		(*it)->requestPeers(amountUploaded, amountDownloaded, amountLeft);
	}
}

void TorrentTrackerCommManager::requestPeers(const uint64_t amountUploaded, 
												const uint64_t amountDownloaded, 
												const uint64_t amountLeft, 
												const TrackerEvent event) const {

	std::vector<TorrentTrackerComm *>::iterator it;
	for (it = trackers->begin(); it != trackers->end(); ++it) {

		std::cout << "looping...\n";
		//spawn thread to requestPeers in each tracker
		//if return null, re-establish connection
		(*it)->requestPeers(amountUploaded, amountDownloaded, amountLeft, event);
	}
}

