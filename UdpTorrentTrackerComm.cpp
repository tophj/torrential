#include "UdpTorrentTrackerComm.h"

UdpTorrentTrackerComm::UdpTorrentTrackerComm(const std::string tracker, 
							const uint16_t newPortNumber, 
							const std::string newFileHash) 
	: TorrentTrackerComm(tracker, newPortNumber, newFileHash) {}

UdpTorrentTrackerComm::UdpTorrentTrackerComm(const std::string tracker, 
							const uint16_t newPortNumber, 
							const std::string newFileHash,
							const int newSecondsUntilTimeout) 
	: TorrentTrackerComm(tracker, newPortNumber, newFileHash, newSecondsUntilTimeout) {}

UdpTorrentTrackerComm::~UdpTorrentTrackerComm() {


}

const bool UdpTorrentTrackerComm::initiateConnection() {

	//struct sockaddr_in serverAddress, clientAddress;

	//Setup dummy client address
	clientAddress.sin_family = AF_INET;
	clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddress.sin_port = htons(51413);

	//Setup server address
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);

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
				return false;
			}
		}
		else {
			return false;
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
		ss << portNumber;
		std::string portNumberString = ss.str();

		//retrieve ip address from hostname--------
		if (GetAddrInfo(trackerHostname->c_str(), portNumberString.c_str(), &hints, &result) != 0) {
			return false;
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
			return false;
		}

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

	//Send a request to the tracker
	ConnectionIdRequest * idRequest = createConnectionIdRequest();
	if (SendTo(sockFd, idRequest, sizeof(*idRequest), 0, 
		(struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
		return false;
	}
	timeRequestSent = clock();

	//Re-send until timeout.....
	ConnectionIdResponse idResponse;
	socklen_t serverAddressLength = sizeof(serverAddress);
	while((timeRequestSent - clock()) / 1000 < SECONDS_UNTIL_TIMEOUT) {

		//Response received!
		if (RecvFrom(sockFd, &idResponse, sizeof(idResponse), 0, 
			(struct sockaddr *) &serverAddress, &serverAddressLength) > 0) {
			break;
		}
	}
	
	//Set class fields that will persist
	activeSocket = sockFd;
	connectionId = ntohll(idResponse.connectionId);

	delete idRequest;

	return true;
}

const std::string * UdpTorrentTrackerComm::requestPeers(const uint64_t amountUploaded, 
												const uint64_t amountDownloaded, 
												const uint64_t amountLeft,
												const TrackerEvent event) {

	//Check if we have an active connection
	if (activeSocket == -1) {
		return NULL;
	}

	//Setup announce message
	uint8_t tempPeerId[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,16, 17, 18, 19, 20};
	PeerRequest announce;
	announce.connectionId = htonll(connectionId);
	announce.action = htonl(ANNOUNCE);
	generateTransactionId();
	announce.transactionId = htonl(*transactionId);
	memcpy(announce.fileHash, fileHash.c_str(), 20);
	memcpy(announce.peerId, tempPeerId, 20);
	announce.amountDownloaded = htonll(amountDownloaded);
	announce.amountLeft = htonll(amountLeft);
	announce.amountUploaded = htonll(amountUploaded);
	announce.event = htonl(event);
	announce.ipAddress = htonl(0);
	announce.key = htonl(443241343); ////////////////////////////////////////////////////wtf is this....
	announce.numWant = htonl(-1);
	announce.portNumber = htonl(51413); //make dynamic later

	if (SendTo(activeSocket, &announce, sizeof(announce), 0, 
		(struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
		return NULL;
	}

	//Receive the peer list
	PeerResponse response;
	socklen_t serverAddressLength = sizeof(serverAddress);
	while((timeRequestSent - clock()) / 1000 < SECONDS_UNTIL_TIMEOUT) {

		//Response received!
		if (RecvFrom(activeSocket, &response, sizeof(response), 0, 
			(struct sockaddr *) &serverAddress, &serverAddressLength) > 0) {
			break;
		}
	}

	printPeerResponse(&response);
	//std::string * bencodedPeerList = new std::string(peersBuffer);

	return NULL;;
}