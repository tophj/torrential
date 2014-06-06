#include "../bencoding/ipTool.h"
#include "TorrentTrackerCommManager.h"

TorrentTrackerCommManager::TorrentTrackerCommManager(int pieceLen, iptool* tool, struct thread_pool * theThreadPool,
														PeerList & newPeerList,
														uint8_t newFileHash[20], 
														std::vector<std::string> & newTrackers, 
														iptool * newIps) 
													: peerList(newPeerList) {

	std::cout << "Passed a list of trackers of size: " << newTrackers.size() << std::endl;
	sleep(5);

	peerList = newPeerList;
	threadPool = theThreadPool;
	ips = newIps;

	//Copy over file hash
	for (int i = 0; i < 20; i++) {

		fileHash[i] = newFileHash[i];
	}

	//Loop over all trackers
	std::vector<std::string>::iterator it;
	for (it = newTrackers.begin(); it != newTrackers.end(); ++it) {

		//Create a new TorrentTrackerComm
		TorrentTrackerComm * tracker = makeTorrentTrackerComm(*it);
		if (tracker != NULL) {

			//If the tracke succeeds in connecting
			if (tracker->initiateConnection(ips->getBest())) {

				trackers.push_back(tracker);

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

	while (!trackers.empty()) {
	
		trackers.pop_back();
	}
}

const uint16_t TorrentTrackerCommManager::generatePortNumber() const {

	uint16_t basePortNum = 55000;

	while (basePortNum != 55000 
		&& portSet.find(basePortNum) != portSet.end()) {

		(basePortNum) += (rand() % 1000);
		(basePortNum) += (rand() % 100);
		(basePortNum) += (rand() % 10);
	}

	return basePortNum;
}

const bool TorrentTrackerCommManager::addTracker(const std::string & newTracker, const std::string ipAddress) {

	TorrentTrackerComm * theTracker = makeTorrentTrackerComm(newTracker);

	if (theTracker != NULL) {

		theTracker->initiateConnection(ips->getBest());

		trackers.push_back(theTracker);
		return true;
	}
	else {

		return false;
	}

}

const bool TorrentTrackerCommManager::removeTracker(const std::string & deleteTracker) {

	std::vector<TorrentTrackerComm *>::iterator it;
	for (it = trackers.begin(); it != trackers.end(); ++it) {

		if (*((*it)->trackerHostname) == deleteTracker) {

			trackers.erase(it);
			return true;
		}
	}

	return false;
}

TorrentTrackerComm * TorrentTrackerCommManager::makeTorrentTrackerComm(const std::string & trackerString) const {

	std::cout << "Creating a new TorrentTrackerComm using the trackerString: |" << trackerString << "|\n";

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

			trackerComm = new TcpTorrentTrackerComm(trackerString.substr(6, portNumIndex - 6), 
				portNumber, fileHash, generatePortNumber());
		}
	}
	else {

		trackerComm = new UdpTorrentTrackerComm(trackerString.substr(6, portNumIndex - 6), 
			portNumber, fileHash, generatePortNumber());
	}

	return trackerComm;
}


void * callRequestPeers(void * theParam) {

	CallRequestPeersParams * param = (CallRequestPeersParams *) theParam;

	const std::vector<Peer *> * peers = param->tracker->requestPeers(param->amountUploaded, param->amountDownloaded, param->amountLeft);
	if (peers != NULL) {

		param->peerList->addPeers(*peers);
	}
	else {

		std::vector<TorrentTrackerComm *>::iterator it;
		for (it = param->trackers->begin(); it != param->trackers->end(); it++) {

			if (**it == *(param->tracker)) {

				param->trackers->erase(it);
			}
		}
	}

	delete param;

	return NULL;
}

void TorrentTrackerCommManager::requestPeers(const uint64_t amountUploaded, 
												const uint64_t amountDownloaded, 
												const uint64_t amountLeft) {

	std::vector<future_t *> futures;

	std::vector<TorrentTrackerComm *>::iterator it;
	for (it = trackers.begin(); it != trackers.end(); ++it) {

		//spawn thread to requestPeers in each tracker
		//if return null, re-establish connection
		CallRequestPeersParams * param = new CallRequestPeersParams();
		param->amountUploaded = amountUploaded;
		param->amountDownloaded = amountDownloaded;
		param->amountLeft = amountLeft;
		param->tracker = *it;
		param->trackers = &trackers;
		param->peerList = &peerList;

		future_t * f = thread_pool_submit(threadPool, (thread_pool_callable_func_t) &callRequestPeers, param);
		futures.push_back(f);
	}

	//Finish and delete all running threads
	std::vector<future_t *>::iterator futureIt;
	for (futureIt = futures.begin(); futureIt != futures.end(); futureIt++) {

		future_get(*futureIt);
		future_free(*futureIt);
	}
}