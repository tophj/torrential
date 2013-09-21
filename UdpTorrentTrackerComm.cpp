#include "UdpTorrentTrackerComm.h"

UdpTorrentTrackerComm::UdpTorrentTrackerComm(const std::string tracker, 
							const int newPortNumber, 
							const std::string newFileHash) 
	: TorrentTrackerComm(tracker, newPortNumber, newFileHash) {}

UdpTorrentTrackerComm::UdpTorrentTrackerComm(const std::string tracker, 
							const int newPortNumber, 
							const std::string newFileHash,
							const int newSecondsUntilTimeout) 
	: TorrentTrackerComm(tracker, newPortNumber, newFileHash, newSecondsUntilTimeout) {}

UdpTorrentTrackerComm::~UdpTorrentTrackerComm() {


}

const bool UdpTorrentTrackerComm::initiateConnection(const int amountUploaded, 
			const int amountDownloaded, const int amountLeft) {

	struct sockaddr_in serverAddress;
	//memset(&serverAddress, 0, sizeof(serverAddress));
	//Add IPv6 in the future
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	if (isIp4Address(trackerAddress)) {
		//retrieve hostname from ip address
	}
	else {
		//retrieve ip address from hostname
	}

	int sockFd = -1;
	//Add IPv6 in the future
	if ((sockFd = Socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		return false;
	}

	int requestLength = 0;
	char request[2000];
	
	//Assemble Request
	requestLength += snprintf(request, trackerHostname.length(), trackerHostname.c_str());
	requestLength += snprintf(request, 11, "%s", "?info_hash=");
	requestLength += snprintf(request, 20, "%s", fileHash.c_str());
	requestLength += snprintf(request, 9, "%s", "&peer_id=");
	generatePeerId();
	requestLength += snprintf(request, 20, "%s", peerId->c_str());
	requestLength += snprintf(request, 6, "%s", "&port=");
	std::ostringstream ss;
	ss << portNumber;
	std::string portNumberString = ss.str();
	ss.clear();
	ss.str("");
	requestLength += snprintf(request, portNumberString.length(), "%s", portNumberString.c_str());
	requestLength += snprintf(request, 10, "%s", "&uploaded=");
	std::string amountUploadedString = ss.str();
	ss.clear();
	ss.str("");
	requestLength += snprintf(request, amountUploadedString.length(), "%s", amountUploadedString.c_str());
	requestLength += snprintf(request, 12, "%s", "&downloaded=");
	std::string amountDownloadedString = ss.str();
	ss.clear();
	ss.str("");
	requestLength += snprintf(request, amountDownloadedString.length(), "%s", amountDownloadedString.c_str());
	requestLength += snprintf(request, 6, "%s", "&left=");
	std::string amountLeftString = ss.str();
	ss.clear();
	ss.str("");
	requestLength += snprintf(request, amountLeftString.length(), "%s", amountLeftString.c_str());

	
	if (SendTo(sockFd, request, requestLength, 0, 
		(struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {

		return false;
	}

	return true;
}

const bool UdpTorrentTrackerComm::waitForResponse() const {

	return true;
}

void UdpTorrentTrackerComm::closeConnection() const {

}