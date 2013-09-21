#ifndef TORRENTIAL_DOWNLOADS_PEER
#define TORRENTIAL_DOWNLOADS_PEER

#include <string>

class Peer {

	public:
		/* Creates a new Peer with the specified ID, 
		   sets either the IP or the DNS string depending on which was passed,
		   sets the portNumber of the Peer. */
		Peer(std::string newId, std::String newIpOrDns, int newPortNumber);
		~Peer();
		std::string getId();
		std::string getIp();
		std::string getDns();
		std::string getPortNumber();
	private:
		std::string id;
		std::string ip;
		std::string dns;
		int portNumber;
};

#endif