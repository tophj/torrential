#include "SystemFunctionWrappers.h"


uint64_t htonll(uint64_t value) {

	const uint32_t high_part = htonl(value >> 32);
	const uint32_t low_part = htonl(value & 0xFFFFFFFF);
	return (((uint64_t) low_part) << 32) | (uint64_t) high_part;
}

uint64_t ntohll(uint64_t value) {

	const uint32_t high_part = ntohl(value >> 32);
	const uint32_t low_part = ntohl(value & 0xFFFFFFFF);
	return (((uint64_t) low_part) << 32) | (uint64_t) high_part;
}

int GetAddrInfo(const char * node, const char * service,
                const struct addrinfo * hints,
                struct addrinfo ** res) {

	int returnValue = -1;
	if ((returnValue = getaddrinfo(node, service, hints, res)) != 0) {

		std::cout << "The error is: " << gai_strerror(returnValue) << std::endl;
		std::cout << "Node passed is: " << node << std::endl;
	}
	return returnValue;
}

int Socket(int domain, int type, int protocol) {

	int returnFileDescriptor = socket(domain, type, protocol);
	if (returnFileDescriptor == -1) {

		std::cout << "CALLING SOCKET RETURNED AN INVALID FILE DESCRIPTOR WITH ARGUMENTS: " 
			<< domain << ", " << type << ", " << protocol << std::endl;
		std::cout << "ERRNO IS: " << strerror(errno) << std::endl;
	}
	return returnFileDescriptor;
}

int SetSockOpt(int sockFd, int level, int optName) {

	int optVal = 1;
	int returnValue = setsockopt(sockFd, level, optName, &optVal, sizeof(int));
	if (returnValue == -1) {

		std::cout << "CALLING SETSOCKOPT FAILED WITH ARGUMENTS: "
			<< sockFd << ", " << level << ", " << optName << ", " 
			<< optVal << ", " << sizeof(int) << std::endl;
		std::cout << "ERRNO IS: " << strerror(errno) << std::endl;
	}
	return returnValue;
}

int Bind(int sockFd, struct sockaddr * myAddr, int addrLen) {

	int returnValue = bind(sockFd, myAddr, addrLen);
	if (returnValue == -1) {

		std::cout << "CALLING BIND FAILED WITH ARGUMENTS: "
			<< sockFd << ", " << myAddr << ", " << addrLen << std::endl;
		std::cout << "ERRNO IS: " << strerror(errno) << std::endl;
	}
	return returnValue;
}

int Connect(int sockfd, struct sockaddr * serverAddress, int addressLength) {

	int returnValue = connect(sockfd, serverAddress, addressLength);
	if (returnValue == -1) {

		//std::cout << "CALLING CONNECT FAILED WITH ARGUMENTS: " 
		//	<< sockfd << ", " << serverAddress << ", " << addressLength << std::endl;
		//std::cout << "ERRNO IS: " << strerror(errno) << std::endl;
	}
	return returnValue;
}

int Select(int nfds, fd_set * readFds, fd_set * writeFds, fd_set * exceptFds, struct timeval * timeout) {

	int returnValue = select(nfds, readFds, writeFds, exceptFds, timeout);
	if (returnValue == -1) {

		std::cout << "CALLING SELECT FAILED WITH ARGUMENTS: " 
			<< nfds << ", " << readFds << ", " << writeFds 
			<< ", " << exceptFds << ", " << timeout 
			<< std::endl;
		std::cout << "ERRNO IS: " << strerror(errno) << std::endl;
	}
	return returnValue;
}

ssize_t Recv(int sockFd, void * buf, size_t len, int flags) {

	int returnValue = recv(sockFd, buf, len, flags);
	if (returnValue == -1) {

	//	std::cout << "CALLING RECV FAILED WITH ARGUMENTS: "
	//		<< sockFd << ", " << len << std::endl;
	//	std::cout << "ERRNO IS: " << strerror(errno) << std::endl;
	}
	
	return returnValue;
}

ssize_t RecvFrom(int sockFd, void * buffer, size_t length, int flags, 
	struct sockaddr * address, socklen_t * addressLength) {

	int returnValue = recvfrom(sockFd, buffer, length, flags, address, addressLength);
	if (returnValue == -1) {

		std::cout << "CALLING RECVFROM FAILED WITH ARGUMENTS: "
			<< sockFd << ", " << buffer << ", " << length
			<< flags << ", " << address << ", " << addressLength
			<< std::endl;
		std::cout << "ERRNO IS: " << strerror(errno) << std::endl;
	}
	return returnValue;
}

ssize_t Send(int sockFd, const void * buf, size_t len, int flags) {

	ssize_t numBytesSent = -1;
	if ((numBytesSent = send(sockFd, buf, len, flags)) == -1) {

		// std::cout << "CALLING SEND FAILED WITH ARGUMENTS: " 
		// 	<< sockFd << ", " << (char *) buf << ", " << len
		// 	<< std::endl;
		// std::cout << "ERRNO IS: " << strerror(errno) << std::endl;
	}

	return numBytesSent;	
}

ssize_t SendTo(int socket, const void * message, size_t length, 
	int flags, const struct sockaddr * destAddr, socklen_t destLen) {

	ssize_t numBytesSent = -1;
	if ((numBytesSent = sendto(socket, message, length, flags, destAddr, destLen)) == -1) {

		std::cout << "CALLING SENDTO FAILED WITH ARGUMENTS: " 
			<< socket << ", " << message << ", " << length
			<< ", " << flags << ", " << destAddr 
			<< ", " << destLen << std::endl;
		std::cout << "ERRNO IS: " << strerror(errno) << std::endl;
	}
	return numBytesSent;
}

int Close(int fd) {

	int returnValue = close(fd);
	if (returnValue == -1) {

		std::cout << "CALLING CLOSE FAILED WITH ARGUMENTS: " << fd << std::endl; 
	}
	return returnValue;	
}