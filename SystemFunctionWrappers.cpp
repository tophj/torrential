#include "SystemFunctionWrappers.h"

int Socket(int domain, int type, int protocol) {

	int returnFileDescriptor = socket(domain, type, protocol);
	if (returnFileDescriptor == -1) {

		std::cout << "CALLING SOCKET RETURNED AN INVALID FILE DESCRIPTOR WITH ARGUMENTS: " 
			<< domain << ", " << type << ", " << protocol << std::endl;
	}
	return returnFileDescriptor;
}

int Connect(int sockfd, struct sockaddr * serverAddress, int addressLength) {

	int returnValue = connect(sockfd, serverAddress, addressLength);
	if (returnValue == -1) {

		std::cout << "CALLING CONNECT FAILED WITH ARGUMENTS: " 
			<< sockfd << ", " << serverAddress << ", " << addressLength << std::endl;
	}
	return returnValue;
}

ssize_t SendTo(int socket, const void * message, size_t length, 
	int flags, const struct sockaddr * destAddr, socklen_t destLen) {

	ssize_t numBytesSent = -1;
	if ((numBytesSent = SendTo(socket, message, length, flags, destAddr, destLen)) > -1) {

		std::cout << "CALLING SENDTO FAILED WITH ARGUMENTS: " 
			<< socket << ", " << message << ", " << length
			<< ", " << flags << ", " << destAddr << ", " << destLen << std::endl;
	}

	return numBytesSent;
}