#include "TorrentTrackerComm.h"

TorrentTrackerComm::TorrentTrackerComm(const std::string tracker, 
										const uint16_t newPortNumber, 
										const std::string newFileHash) {

	//Check if a hostname or an IP address was passed
	if (isIp4Address(tracker)) {
		trackerAddress = new std::string(tracker);	
		trackerHostname = NULL;
		std::cout << "isIp4Address passed..........................&&&&&&&&&&**********#######$$$$$4\n";
	}
	//IPv6 Support needed here.....
	//else if (isIp6Address(tracker)) {
		//do something...exception perhaps
	//}
	else {
		trackerHostname = new std::string(tracker);
		trackerAddress = NULL;
	}
	
	portNumber = newPortNumber;
	fileHash = newFileHash;
	
	connectionId = 0x41727101980;
	transactionId = NULL;
	timeRequestSent = 0;
	activeSocket = -1;

	SECONDS_UNTIL_TIMEOUT = DEFAULT_SECONDS_UNTIL_TIMEOUT;
}

TorrentTrackerComm::TorrentTrackerComm(const std::string tracker, 
										const uint16_t newPortNumber, 
										const std::string newFileHash,
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

	portNumber = newPortNumber;
	fileHash = newFileHash;
	SECONDS_UNTIL_TIMEOUT = newSecondsUntilTimeout;
	
	connectionId = 0x41727101980;
	transactionId = NULL;
	timeRequestSent = 0;
	activeSocket = -1;
}

TorrentTrackerComm::~TorrentTrackerComm() {

	if (transactionId)
		delete transactionId;
	if (trackerAddress)
		delete trackerAddress;
	if (trackerHostname)
		delete trackerHostname;
}

void TorrentTrackerComm::generateTransactionId() {

	std::string newTransactionIdStr;
	for (uint32_t i = 0; i < 20; i++) {
		(newTransactionIdStr) += (rand() % 10) + 48;
	}

	uint32_t * newTransactionId = new uint32_t;
	std::istringstream(newTransactionIdStr) >> *newTransactionId;

	//Delete the old transactionId if it exists
	if (transactionId) {

		delete transactionId;
	}

	transactionId = newTransactionId;
}

const bool TorrentTrackerComm::isTimedOut() const {

	return (clock() - timeRequestSent) / CLOCKS_PER_SEC >= SECONDS_UNTIL_TIMEOUT;
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

std::string * TorrentTrackerComm::createTrackerRequest(const int amountUploaded, 
																const int amountDownloaded,
																const int amountLeft) const {

	std::string * request = new std::string();

	//Assemble Request
	*request += "GET ";
	*request += "udp://";
	*request += *trackerHostname;

	*request += "?info_hash=";
	*request += fileHash;

	*request += "&peer_id=";
	*request += *transactionId;

	*request += "&port=";
	std::stringstream ss;
	ss << portNumber;
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

	//*request += "HTTP/1.1";

	std::cout << "THE REQUEST IS:\n" << *request << std::endl;

	return request;
}

void TorrentTrackerComm::printPeerResponse(const PeerResponse * response) {

	if (!response)
		return;
	if (response->action != 3) {
		
		std::cout << "action = " << ntohl(response->action)
					<< "\ntransactionId = " << ntohl(response->transactionId)
					<< "\ninterval = " << ntohl(response->interval)
					<< "\nleechers = " << ntohl(response->leechers)
					<< "\nseeders = " << ntohl(response->seeders) << std::endl;

		PeerAddress * peerIt = response->peers;
		for (uint32_t i = 0; i < ntohl(response->seeders); i++, peerIt++) {

			std::cout << "ip address == " << peerIt->ipAddress 
						<< "TCP port == " << peerIt->tcpPort
						<< std::endl;
		}
	}
	else {

		PeerResponseError * error = (PeerResponseError *) response;
		std::cout << "action = " << error->action
					<< "\ntransactionId = " << ntohl(error->transactionId)
					<< "\nerror string = ||" << error->errorString 
					<< "||\n\n";
	}
}

void TorrentTrackerComm::printConnectionIdResponse(const ConnectionIdResponse * response) {

	if (!response)
		return;

	std::cout << "action == " << response->action
				<< "\ntransactionId == " << response->transactionId
				<< "\nconnectionId == " << response->connectionId 
				<< std::endl;
}