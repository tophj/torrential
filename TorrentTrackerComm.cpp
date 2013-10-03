#include "TorrentTrackerComm.h"

TorrentTrackerComm::TorrentTrackerComm(const std::string tracker, 
										const int newPortNumber, 
										const std::string newFileHash) {

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
	SECONDS_UNTIL_TIMEOUT = DEFAULT_SECONDS_UNTIL_TIMEOUT;
	peerId = NULL;
	timeRequestSent = clock();
}

TorrentTrackerComm::TorrentTrackerComm(const std::string tracker, 
										const int newPortNumber, 
										const std::string newFileHash,
										const int newSecondsUntilTimeout) {

	//Check if a hostname or an IP address was passed
	if (isIp4Address(tracker)) {
		trackerAddress = &tracker;	
		trackerHostname = NULL;
	}
	//IPv6 Support needed here.....
	//else if (isIp6Address(tracker)) {
		//do something...exception perhaps
	//}
	else {
		trackerHostname = &tracker;
		trackerAddress = NULL;
	}

	portNumber = newPortNumber;
	fileHash = newFileHash;
	SECONDS_UNTIL_TIMEOUT = newSecondsUntilTimeout;	
	peerId = NULL;
	timeRequestSent = clock();
}

TorrentTrackerComm::~TorrentTrackerComm() {

	if (peerId)
		delete peerId;
}

void TorrentTrackerComm::generatePeerId() {

	std::string newPeerIdStr;
	for (int i = 0; i < 20; i++) {
		(newPeerIdStr) += (rand() % 10) + 48;
	}

	int32_t * newPeerId = new int32_t;
	std::istringstream(newPeerIdStr) >> *newPeerId;

	//Delete the old peerId if it exists
	if (peerId) {

		delete peerId;
	}

	peerId = newPeerId;
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
	for (int i = 0, numberCount = 0; i < theString.length(); i++) {

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
	}
}

const bool TorrentTrackerComm::isIp6Address(const std::string theString) const {

	//Needs to be implemented later
	return false;
}

std::string * TorrentTrackerComm::createTrackerRequest(const int amountUploaded, 
																const int amountDownloaded,
																const int amountLeft) {

	std::string * request = new std::string();

	//Assemble Request
	*request += "GET ";
	*request += "udp://";
	*request += *trackerHostname;
	std::cout << "trackerHostname:: " << *trackerHostname << std::endl;
	*request += "?info_hash=";
	*request += fileHash;
	*request += "&peer_id=";
	*request += *peerId;
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
	*request += "HTTP/1.1";

	std::cout << "THE REQUEST IS:\n" << *request << std::endl;

	return request;
}