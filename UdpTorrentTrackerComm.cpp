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

	struct sockaddr_in serverAddress, clientAddress;
	struct hostent * host;
	struct in_addr address;

	//Setup dummy client address
	clientAddress.sin_family = AF_INET;
	clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddress.sin_port = htons(0);

	//Setup server address
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);

	//SETUP in_addr server address
	//If we have an IP
	if (trackerAddress) {

		if (isIp4Address(*trackerAddress)) {
			
			//retrieve hostname from ip address	
			if (inet_aton(trackerAddress->c_str(), &address)) {

				host = gethostbyaddr((const char *) &address, sizeof(address), AF_INET);
				trackerHostname = new std::string(host->h_name);
			}
			else {
				return false;
			}
		}
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

std::cout << *trackerAddress << std::endl;
std::cout << "tracker port number " << portNumber << std::endl;

	//Convert trackerAddress to network format
	if(!inet_aton(trackerAddress->c_str(), &serverAddress.sin_addr)) {
		return false;
	}

	int sockFd = -1;
	//Add IPv6 in the future
	if ((sockFd = Socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		return false;
	}

	//Bind my address to the socket
	if (Bind(sockFd, (struct sockaddr *) &clientAddress, sizeof(clientAddress)) == - 1) {
		return false;
	}

	std::cout << "SendTo\n";
	ConnectionIdRequest * idRequest = createConnectionIdRequest();
	if (SendTo(sockFd, idRequest, sizeof(*idRequest), 0, 
		(struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
		return false;
	}
	timeRequestSent = clock();

std::cout << "Sent: " << idRequest->connectionId << "|||" << idRequest->action << "|||" << idRequest->transactionId << std::endl;

	std::cout << "RecvFrom\n";
	char buffer[3000];
	socklen_t serverAddressLength = sizeof(serverAddress);
	while(true) {
		if (RecvFrom(sockFd, buffer, 3000, 0, 
			(struct sockaddr *) &serverAddress, &serverAddressLength) == - 1) {
			break;
			std::cout << "breaking...\n";
		}
		std::cout << "RECEIVED....something....\n";
	}
	std::cout << "The buffer is: " << buffer << std::endl;
	//while()


	//createTrackerRequest(amountUploaded, amountDownloaded, amountLeft);

	//Create the request for the tracker
	//if (SendTo(sockFd, request->c_str(), request->size(), 0, 
	//	(struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {

	//	return false;
	//}

	timeRequestSent = clock();

	Close(sockFd);

	return true;
}

ConnectionIdRequest * UdpTorrentTrackerComm::createConnectionIdRequest() {

	ConnectionIdRequest * idRequest = new ConnectionIdRequest;
	generatePeerId();
	idRequest->connectionId = htonll(0x41727101980);
	idRequest->action = htonl(CONNECT);
	idRequest->transactionId = htonl(*peerId);

	return idRequest;
}

const bool UdpTorrentTrackerComm::waitForResponse() const {

	int sockFd = -1;
	if ((sockFd = Socket(AF_INET, SOCK_DGRAM, 0)) == -1) {

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