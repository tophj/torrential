#ifndef TORRENTIAL_DOWNLOADS_PEER
#define TORRENTIAL_DOWNLOADS_PEER

#include <string>

/* A Peer class that wraps up the fields needed to represent a peer.
   Includes an id, an ip, and a port number. */
class Peer {

	private:
		//~Data Fields---------------------------------------------------
		//std::string id;

		std::string ip;

		uint16_t portNumber;

	public:
		//~Constructors/Destructor-------------------------------------
		/* Creates a new Peer with the specified ID, 
		   sets the IP string, and
		   sets the portNumber of the Peer. */
		Peer(const std::string newIp, 
				const uint16_t newPortNumber) 
			: ip(newIp), 
				portNumber(newPortNumber) {}

		Peer(const Peer & p) 
			: ip(p.getIp()), portNumber(p.getPortNumber()) {}

		~Peer() {};

		//~Methods-----------------------------------------------------
		/*void setId(std::string newId) {
			id = newId;
		}*/

		/*std::string & getId() const {
			return id;
		}*/

		std::string getIp() const {
			return ip;
		}

		uint16_t getPortNumber() const {
			return portNumber;
		}

		bool operator== (const Peer & p) const {

			return (portNumber == portNumber && ip == p.getIp());
		}

		/*
		const Peer & operator= (const Peer & p) {

			Peer * me = this;

			portNumber = p.getPortNumber();
			ip = p.getIp();

			return *me;
		}*/
};


struct PeerHash {

	std::size_t operator()(const Peer & p) const {

		return (51 + 51 * std::hash<uint16_t>()(p.getPortNumber()) + std::hash<std::string>()(p.getIp()));
	}
};

struct PeerPointerHash {

	std::size_t operator()(const Peer * p) const {

		return (51 + 51 * std::hash<uint16_t>()(p->getPortNumber()) + std::hash<std::string>()(p->getIp()));
	}
};

#endif