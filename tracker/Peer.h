#ifndef TORRENTIAL_DOWNLOADS_PEER
#define TORRENTIAL_DOWNLOADS_PEER

#include <string>

/* A Peer class that wraps up the fields needed to represent a peer.
   Includes an id, an ip, and a port number. */
class Peer {

	public:
		//~Constructors/Destructor-------------------------------------
		/* Creates a new Peer with the specified ID, 
		   sets the IP string, and
		   sets the portNumber of the Peer. */
		Peer(const std::string newIp, 
				const int newPortNumber) 
			: ip(newIp), 
				portNumber(newPortNumber) {}

		~Peer() {};

		//~Methods-----------------------------------------------------
		void setId(std::string newId) {
			id = newId;
		}

		const std::string getId() const {
			return id;
		}

		const std::string getIp() const {
			return ip;
		}

		const int getPortNumber() const {
			return portNumber;
		}

	private:
		//~Data Fields---------------------------------------------------
		std::string id;

		const std::string ip;

		const int portNumber;
};

#endif