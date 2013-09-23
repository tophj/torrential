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
														const int amountDownloaded, 
														const int amountLeft) {

	struct sockaddr_in serverAddress;
	struct hostent * host;
	struct in_addr address;

	//Add IPv6 in the future
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);

	//If we have an IP
	if (trackerAddress) {

		if (isIp4Address(*trackerAddress)) {
			
			//retrieve hostname from ip address	
			if (inet_aton(trackerAddress->c_str(), &address)) {

				host = gethostbyaddr((const char *) &address, 
														sizeof(address), 
														AF_INET);
				trackerHostname = new std::string(host->h_name);
			}
			else {
				return false;
			}
		}
		//else if (isIp6Address(trackerAddress)) {
			//retrieve hostname from ip address
		//}
		else {
			return false;
		}
	}
	else {
		//retrieve ip address from hostname
		host = gethostbyname(trackerHostname->c_str());
		address.s_addr = ((struct in_addr *) host->h_addr_list)->s_addr;
		trackerAddress = new std::string(inet_ntoa(address));
	}

	if(!inet_aton(trackerAddress->c_str(), &serverAddress.sin_addr)) {
		return false;
	}

	int sockFd = -1;
	//Add IPv6 in the future
	if ((sockFd = Socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		return false;
	}

	int requestLength = 0;
	char request[2000];
	
	//Assemble Request
	strncat(request, trackerHostname->c_str(), trackerHostname->length());
	requestLength += trackerHostname->length();
	strncat(request, "?info_hash=", 11);
	requestLength += 11;
	strncat(request, fileHash.c_str(), 20);
	requestLength += 20;
	strncat(request, "&peer_id=", 9);
	requestLength += 9;
	generatePeerId();
	strncat(request, peerId->c_str(), 20);
	requestLength += 20;
	strncat(request, "&port=", 6);
	requestLength += 6;
	std::ostringstream ss;
	ss << portNumber;
	std::string portNumberString = ss.str();
	ss.clear();
	ss.str("");
	strncat(request, portNumberString.c_str(), portNumberString.length());
	requestLength += portNumberString.length();
	strncat(request, "&uploaded=", 10);
	requestLength += 10;
	std::string amountUploadedString = ss.str();
	ss.clear();
	ss.str("");
	strncat(request, amountUploadedString.c_str(), amountUploadedString.length());
	requestLength += amountUploadedString.length();
	strncat(request, "&downloaded=", 12);
	requestLength += 12;
	std::string amountDownloadedString = ss.str();
	ss.clear();
	ss.str("");
	strncat(request, amountDownloadedString.c_str(), amountDownloadedString.length());
	requestLength += amountDownloadedString.length();
	strncat(request, "&left=", 6);
	requestLength += 6;
	std::string amountLeftString = ss.str();
	ss.clear();
	ss.str("");
	strncat(request, amountLeftString.c_str(), amountLeftString.length());
	requestLength += amountLeftString.length();
	std::cout << "THE REQUEST IS:\n" << request << std::endl;
	if (SendTo(sockFd, request, requestLength, 0, 
		(struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {

		return false;
	}
	timeRequestSent = clock();

	Close(sockFd);

	return true;
}

const bool UdpTorrentTrackerComm::waitForResponse() const {

	int sockFd = -1;
	if ((sockFd = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {

		return false;
	}
	std::cout << "AFTER SOCKET\n";
	int portNumber = 6666;
	int broadcast = 1;

	if (SetSockOpt(sockFd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1) {

		return false;
	}
	std::cout << "AFTER SETSOCKOPT\n";
	struct sockaddr_in addrMe, addrOther;
	addrMe.sin_family = AF_INET;
	addrMe.sin_port = htons(portNumber);
	addrMe.sin_addr.s_addr = INADDR_ANY;

	if (Bind(sockFd, (struct sockaddr *) &addrMe, sizeof(struct sockaddr)) == -1) {

		return false;
	}
	std::cout << "AFTER BIND\n";
	while (!isTimedOut()) {
		std::cout << "WHILE LOOPING \n";
		char buffer[1000];
		socklen_t sLen = sizeof(struct sockaddr);
		RecvFrom(sockFd, buffer, sizeof(buffer), 0, 
			(struct sockaddr *) &addrOther, &sLen);

		std::cout << "Received: " << buffer << std::endl;
	}

	return true;
}