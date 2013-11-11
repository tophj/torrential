#include "TorrentTrackerComm.h"

TorrentTrackerComm::TorrentTrackerComm(const std::string tracker, 
										const uint16_t newPortNumber, 
										const uint8_t newFileHash[20],
										const uint16_t myNewPortNumber) {

	//Check if a hostname or an IP address was passed
	if (isIp4Address(tracker)) {
		trackerAddress = new std::string(tracker);	
		trackerHostname = NULL;
	}
	//IPv6 Support needed here.....
	//else if (isIp6Address(tracker)) {
		//do something...exception perhaps
	//}
	else {
std::cout << "PASSED trackerHostname: == ||" << tracker << "||\n";
		trackerHostname = new std::string(tracker);
std::cout << "trackerHostname: == ||" << *trackerHostname << "||\n";
		trackerAddress = NULL;
	}
	
	//Copy over fileHash
	for (int i = 0; i < 20; i++) {

		fileHash[i] = newFileHash[i];
	}
	serverPortNumber = newPortNumber;
	myPortNumber = myNewPortNumber;
	
	connectionId = 0x41727101980;
	transactionId = NULL;
	timeRequestSent = 0;
	timeOfLastResponse = 0;
	activeSocket = -1;
	curEvent = NONE;

	SECONDS_UNTIL_TIMEOUT = DEFAULT_SECONDS_UNTIL_TIMEOUT;
	requestInterval = SECONDS_UNTIL_TIMEOUT;
}

TorrentTrackerComm::TorrentTrackerComm(const std::string tracker, 
										const uint16_t newPortNumber, 
										const uint8_t newFileHash[20],
										const uint16_t myNewPortNumber,
										const int newSecondsUntilTimeout) {

	//Check if a hostname or an IP address was passed
	if (isIp4Address(tracker)) {
		trackerAddress = new std::string(tracker);	
		trackerHostname = NULL;
	}
	//IPv6 Support needed here.....
	//else if (isIp6Address(tracker)) {
		//do something...exception perhaps
	//}
	else {
		trackerHostname = new std::string(tracker);
		trackerAddress = NULL;
	}

	//Copy over fileHash
	for (int i = 0; i < 20; i++) {

		fileHash[i] = newFileHash[i];
	}
	serverPortNumber = newPortNumber;
	SECONDS_UNTIL_TIMEOUT = newSecondsUntilTimeout;
	requestInterval = SECONDS_UNTIL_TIMEOUT;
	myPortNumber = myNewPortNumber;
	
	connectionId = 0x41727101980;
	transactionId = NULL;
	timeRequestSent = 0;
	timeOfLastResponse = 0;
	activeSocket = -1;
	curEvent = NONE;
}

TorrentTrackerComm::~TorrentTrackerComm() {

	if (transactionId)
		delete transactionId;
	if (trackerAddress)
		delete trackerAddress;
	if (trackerHostname)
		delete trackerHostname;
	Close(activeSocket);
}

const uint32_t TorrentTrackerComm::getRequestInterval() const{

	return requestInterval;
}

const time_t TorrentTrackerComm::getTimeOfLastResponse() const {

	return timeOfLastResponse;
}

void TorrentTrackerComm::generateTransactionId() {

	uint32_t * newTransactionId = new uint32_t[20];
	for (uint32_t i = 0; i < 20; i++) {

		newTransactionId[i] = (rand() % 10);
	}

	//Delete the old transactionId if it exists
	if (transactionId) {

		delete [] transactionId;
	}

	transactionId = newTransactionId;
}

const bool TorrentTrackerComm::isIntervalPassed() const {


	time_t curTime = time(NULL);
	return difftime(curTime, timeOfLastResponse) >= requestInterval;
}

const bool TorrentTrackerComm::isTimedOut() const {

	time_t curTime = time(NULL);
	return difftime(curTime, timeRequestSent) >= SECONDS_UNTIL_TIMEOUT;
}

const bool TorrentTrackerComm::isIp4Address(const std::string theString) const {

	//shortest is 7, longest is 15
	if (theString.length() < 7 || theString.length() > 15) {

		return false;
	}

	//Loop over characters
	for (uint i = 0, numberCount = 0, periodCount = 0; i < theString.length(); i++) {

		//Can only include numbers and periods
		if (theString[i] > 57 && theString[i] < 48 && theString[i] != 46) {

			return false;
		}
		//Can't start with a period
		else if (i == 0 && theString[i] == 46) {

			return false;
		}
		//Can't have two periods in a row
		else if (i > 0 && theString[i] == 46 && theString[i - 1] == 46) {

			return false;
		}
		//Count numbers between periods
		else if (theString[i] < 57 && theString[i] > 48) {

			numberCount++;

			//Check if there are more than 3 numbers between periods
			if (numberCount > 3) {
			
				return false;
			}
		}
		//Reset number count, and count total periods
		else if (theString[i] == 46) {

			numberCount = 0;
			periodCount++;

			//Check if there are more than 3 periods
			if (periodCount > 3) {

				return false;
			}
		}
	}

	return true;
}

const bool TorrentTrackerComm::isIp6Address(const std::string theString) const {

	//Needs to be implemented later
	return false;
}

ConnectionIdRequest * TorrentTrackerComm::createConnectionIdRequest() {

	ConnectionIdRequest * idRequest = new ConnectionIdRequest;
	generateTransactionId();
	idRequest->connectionId = htonll(connectionId);
	idRequest->action = htonl(CONNECT);
	idRequest->transactionId = htonl(*transactionId);

	return idRequest;
}

std::string * TorrentTrackerComm::convertInfoHashToString() const {

	std::string * infoHashString = new std::string();
	std::stringstream ss;

	for (int i = 0; i < 20; i++) {

		*infoHashString += "%";
		ss << std::hex << (int)fileHash[i];

		//Add a leading 0 if we have a length of 1
		if (ss.str().size() == 1) {
			*infoHashString += "0";
			*infoHashString += ss.str();
		}
		else {
			*infoHashString += ss.str();	
		}

		ss.clear();
		ss.str("");
	}

	return infoHashString;
}

std::string * TorrentTrackerComm::createTrackerRequest(const int amountUploaded, 
														const int amountDownloaded,
														const int amountLeft,
														const TrackerEvent event) {

	std::string * request = new std::string();
	
	*request += "GET ";

	*request += "/announce?info_hash=";
	std::string * fileHashString = convertInfoHashToString();
	*request += *fileHashString;
	delete fileHashString;

	*request += "&peer_id=";
	generateTransactionId();
	std::string transactionIdString;
	for (int i = 0; i < 20; i++) {
		transactionIdString += (char)(transactionId[i] + 48);
	}
	*request += transactionIdString;

	*request += "&port=";
	std::stringstream ss;
	ss << serverPortNumber;
	*request += ss.str();
	ss.clear();
	ss.str("");

	*request += "&uploaded=";
	ss << amountUploaded;
	*request += ss.str();
	ss.clear();
	ss.str("");

	*request += "&downloaded=";
	ss << amountDownloaded;
	*request += ss.str();
	ss.clear();
	ss.str("");

	*request += "&left=";
	ss << amountLeft;
	*request += ss.str();
	ss.clear();
	ss.str("");

	*request += "&event=";

	//Place the event 
	switch (event) {

		case NONE:
			*request += "none";
			break;
		case COMPLETED:
			*request += "completed";
			break;
		case STARTED:
			*request += "started";
			break;
		case STOPPED:
			*request += "stopped";
			break;
	}

	*request += " HTTP/1.1\r\n";
	*request += "Host: ";
	*request += *trackerHostname;
	//*request += ":";
	//std::stringstream ssPortNumber;
	//ssPortNumber << serverPortNumber;
	//*request += ssPortNumber.str();
	*request += "\r\n\r\n";

	return request;
}

void TorrentTrackerComm::printPeerResponseError(const void * response) {

	PeerResponseError * error = (PeerResponseError *) response;
		std::cout << "action = " << ntohl(error->action)
					<< "\ntransactionId = " << ntohl(error->transactionId)
					<< "\nerror string = ||" << error->errorString 
					<< "||\n\n";
}

void TorrentTrackerComm::printConnectionIdResponse(const ConnectionIdResponse * response) {

	if (!response)
		return;
	std::cout << "PRINTING CONNECTIONIDRESPONSE OBJECT\n";
	std::cout << "action == " << response->action
				<< "\ntransactionId == " << response->transactionId
				<< "\nconnectionId == " << response->connectionId 
				<< std::endl;
}

std::vector<Peer * > * TorrentTrackerComm::parseAnnounceResponse(const PeerResponse * response) {

	std::vector<Peer * > * peers = new std::vector<Peer * >();

	uint32_t add = 0;
	uint16_t port = 0;

	const uint8_t * peerIt = response->sources;
	uint32_t numSources = ntohl(response->seeders) + ntohl(response->leechers);

	for (uint32_t i = 0; i < numSources; i++) {

		add = *((uint32_t *) peerIt);
		peerIt += 4;
		port = *((uint16_t *) peerIt);
		peerIt += 2;

		std::stringstream ss;
		ss << inet_ntoa(*(struct in_addr *)&add);

		Peer * peer = new Peer(ss.str(), port);
		peers->push_back(peer);

		add = 0;
		port = 0;
	}	

	return peers;
}

void TorrentTrackerComm::printPeerResponse(const PeerResponse * response) {

	if (!response)
		return;
	std::cout << "\nPRINTING PEERRESPONSE OBJECT\n";
	if (response->action != 3) {
		
		std::cout << "action = " << ntohl(response->action)
					<< "\ntransactionId = " << ntohl(response->transactionId)
					<< "\ninterval = " << ntohl(response->interval)
					<< "\nleechers = " << ntohl(response->leechers)
					<< "\nseeders = " << ntohl(response->seeders) << std::endl;

		uint32_t add = 0;
		uint16_t port = 0;

		const uint8_t * peerIt = response->sources;	
		uint32_t numSources = ntohl(response->seeders) + ntohl(response->leechers);
		std::cout << "looping over peers...\n";
		for (uint32_t i = 0; i < numSources; i++) {

			add = *((uint32_t *)peerIt);
			peerIt += 4;
			port = *((uint16_t *) peerIt);
			peerIt += 2;

			std::cout << "ip address == " << inet_ntoa(*(struct in_addr *)&add)
						<< "\nTCP port == " << port
						<< std::endl << std::endl;
		}
		
	}
	else {

		printPeerResponseError(response);
	}
	std::cout << "-------------------------------------------------------------\n\n";
}

const void TorrentTrackerComm::printTorrentTrackerComm() const {

	std::cout << "\n\n--------------------------------------------------------------\n";
	std::cout << "PRINTING TORRENT TRACKER COMM OBJECT\n";
	std::cout << "------------------------------------\n";

	std::cout << "fileHash = |";
	for (int i = 0; i < 20; i++) {

		std::cout << fileHash[i];
	}
	std::cout << "|\n";
	if (trackerHostname)
		std::cout << "trackerHostname = |" << *trackerHostname << "|\n";
	if (trackerAddress)
		std::cout << "trackerAddress = |" << *trackerAddress << "|\n\n";
	
	std::cout << "activeSocket = |" << activeSocket << "|\n";
	std::cout << "serverPortNumber = |" << serverPortNumber << "|\n";
	std::cout << "myPortNumber = |" << myPortNumber << "|\n\n";

	std::cout << "curEvent = |" << curEvent << "|\n";
	if (transactionId)
		std::cout << "transactionId = |" << *transactionId << "|\n";
	std::cout << "connectionId = |" << connectionId << "|\n";
	std::cout << "peerId = |" << peerId << "|\n";
	std::cout << "requestInterval = |" << requestInterval << "|\n";
	std::cout << "--------------------------------------------------------------\n\n\n";
}