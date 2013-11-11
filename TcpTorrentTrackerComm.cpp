#include "TcpTorrentTrackerComm.h"

TcpTorrentTrackerComm::TcpTorrentTrackerComm(const std::string tracker, 
												const uint16_t newPortNumber, 
												const uint8_t newFileHash[20],
												const uint16_t myNewPortNumber) 
	: TorrentTrackerComm(tracker, newPortNumber, newFileHash, myNewPortNumber) {}

TcpTorrentTrackerComm::TcpTorrentTrackerComm(const std::string tracker, 
												const uint16_t newPortNumber, 
												const uint8_t newFileHash[20],
												const uint16_t myNewPortNumber,
												const int newSecondsUntilTimeout) 
	: TorrentTrackerComm(tracker, newPortNumber, newFileHash, myNewPortNumber, newSecondsUntilTimeout) {}

TcpTorrentTrackerComm::~TcpTorrentTrackerComm(){

}

const bool TcpTorrentTrackerComm::initiateConnection() {

	return true;
}

const std::vector<Peer * > * TcpTorrentTrackerComm::requestPeers(const uint64_t amountUploaded, 
																	const uint64_t amountDownloaded, 
																	const uint64_t amountLeft) {

	return requestPeers(amountUploaded, amountDownloaded, amountLeft);
}

const std::vector<Peer * > * TcpTorrentTrackerComm::requestPeers(const uint64_t amountUploaded, 
																	const uint64_t amountDownloaded, 
																	const uint64_t amountLeft, 
																	const TrackerEvent event) {

	std::cout << "TCP initiateConnection...\n";

	//Setup dummy client address
	clientAddress.sin_family = AF_INET;
	clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddress.sin_port = htons(myPortNumber);

	//Setup server address
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPortNumber);

	//SETUP in_addr server address
	//If we have an IP
	if (trackerAddress) {

		if (isIp4Address(*trackerAddress)) {
			
			//Convert human readable trackerAddress to network byte order ip address and place in serverAddress.sin_addr
			if (inet_pton(AF_INET, trackerAddress->c_str(), &(serverAddress.sin_addr))) {

				//retrieve hostname and service type from ip address		
				char hostBuffer[100], serviceBuffer[100];
				getnameinfo((struct sockaddr *) &serverAddress, sizeof(serverAddress), 
					hostBuffer, sizeof(hostBuffer), 
					serviceBuffer, sizeof(serviceBuffer), 
					NI_NAMEREQD | NI_DGRAM);

				trackerHostname = new std::string(hostBuffer);
			}
			else {
				return NULL;
			}
		}
		else {
			return NULL;
		}
	}
	else {

		//Setup structs to be used in getaddrinfo
		struct addrinfo hints;
		struct addrinfo * result, * resultPointer;
		hints.ai_family = AF_INET;
	    hints.ai_socktype = SOCK_DGRAM;
	    hints.ai_flags = 0;
	    hints.ai_protocol = 0;

		//Convert port number to string to pass to getaddrinfo
		std::stringstream ss;
		ss << serverPortNumber;
		std::string portNumberString = ss.str();

		//retrieve ip address from hostname--------
		if (GetAddrInfo(trackerHostname->c_str(), portNumberString.c_str(), &hints, &result) != 0) {
			return NULL;
		}

		//Iterate over results for IP address V4 (ADD V6 later!)
		char ipBuffer[INET_ADDRSTRLEN];
		for (resultPointer = result; resultPointer != NULL; resultPointer = resultPointer->ai_next) {

			//If we have an IPv4 address
			if (resultPointer->ai_family == AF_INET) {

				//convert to presentation format and store in ipBuffer
				inet_ntop(AF_INET, &((struct sockaddr_in *) resultPointer->ai_addr)->sin_addr, ipBuffer, INET_ADDRSTRLEN);
			}
		}
		//Free result
		freeaddrinfo(result);

		//Convert ipBuffer to std::string and store in trackerAddress field
		trackerAddress = new std::string(ipBuffer);

		//Convert trackerAddress to network format
		if(!inet_pton(AF_INET, trackerAddress->c_str(), &serverAddress.sin_addr)) {
			return NULL;
		}
	}

	int sockFd = -1;
	//Add IPv6 in the future
	if ((sockFd = Socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		return NULL;
	}

	//Set options for this socket
	if (SetSockOpt(sockFd, SOL_SOCKET, SO_REUSEADDR) == -1) {
		return NULL;
	}

	//Bind my address to the socket
	if (Bind(sockFd, (struct sockaddr *) &clientAddress, sizeof(clientAddress)) == - 1) {
		return NULL;
	}

	//Create request
	std::string * requestString = createTrackerRequest(amountUploaded, 
		amountDownloaded, amountLeft, event);

std::cout << "SENDING....\n";
	const char * writeBuffer = requestString->c_str();
std::cout << "writeBuffer:\n" << writeBuffer << "\n\n";
	if (Send(sockFd, writeBuffer, requestString->size(), 0) == -1) {
		return NULL;
	}

std::cout << "RECEIVING....\n";
	char recvBuffer[10000];
	if (Recv(sockFd, recvBuffer, sizeof(recvBuffer), 0) == -1) {
		return NULL;
	}

std::cout << "it works!\n";

	return NULL;
}