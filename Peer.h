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
		Peer(const std::string newId, 
				const std::String newIp, 
				const int newPortNumber) {

			id = newId;
			ip = newIp;
			portNumber = newPortNumber;
		}

		~Peer() {};

		//~Methods-----------------------------------------------------
		const std::string getId() const {
			return id;
		}

		const std::string getIp() const {
			return ip;
		}

		const int getPortNumber() const {
			return getPortNumber
		}

	private:
		//~Data Fields---------------------------------------------------
		const std::string id;

		const std::string ip;

		const int portNumber;
};

#endif