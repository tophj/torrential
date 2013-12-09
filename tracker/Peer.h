#ifndef TORRENTIAL_DOWNLOADS_PEER
#define TORRENTIAL_DOWNLOADS_PEER

#include <iostream>
#include <string>
#include <unordered_set>

#include "Piece.h"

/* A Peer class that wraps up the fields needed to represent a peer.
   Includes an id, an ip, and a port number. */
class Peer {

	private:
		//~Data Fields---------------------------------------------------
		//The id of the peer.
		//std::string id;

		//The IP address for the Peer.
		std::string ip;

		//The port number for the Peer.
		uint16_t portNumber;

		/* The pieces that this Peer has. */
		//PieceList pieces;
		std::unordered_set<Piece, PieceHash> pieces;

		/* Boolean field indicating if the peerwire protocol has altered this peer. */
		bool peerwireAltered = false;

	public:
		//~Choking Fields-------------------------------------------
		/* Whether or not the remote peer has choked this client. When a peer chokes the client, 
		   it is a notification that no requests will be answered until the client is unchoked. 
		   The client should not attempt to send requests for blocks, 
		   and it should consider all pending (unanswered) requests to be discarded by the remote peer. */
		/* Client is choking the peer */
		bool amChoking = false;

		/* Peer is choking us. */
		bool peerChoking = false;

		/* The socket that we are connected to this peer on. */
		int sockFd;

		//~Interested Fields-------------------------------------------
		/* Whether or not the remote peer is interested in something this client has to offer. 
		This is a notification that the remote peer will begin requesting blocks when the client unchokes them.
		Note: this also implies that the client will also need to keep track of whether or not it is interested in the remote peer, 
		and if it has the remote peer choked or unchoked. So, the real list looks something like this: */
		
		/* Client is interested in the peer. */
		bool amInterested = false;

		/* Peer is interested in the client. */
		bool peerInterested = false;

		//~Constructors/Destructor-------------------------------------
		/* Creates a new Peer with the specified ID, 
		   sets the IP string, and
		   sets the portNumber of the Peer. */
		Peer(const std::string newIp, 
				const uint16_t newPortNumber) 
			: ip(newIp), 
				portNumber(newPortNumber) {

			sockFd = -1;			
		}

		Peer(const Peer & p) 
			: ip(p.getIp()), portNumber(p.getPortNumber()) {

			sockFd = -1;		
		}

		~Peer() {};

		//~Methods-----------------------------------------------------
		/*void setId(std::string newId) {
			id = newId;
		}*/

		/*std::string & getId() const {
			return id;
		}*/

		/* Gets the IP address that this peer resides at. */
		std::string getIp() const {
			return ip;
		}

		/* Gets the port number that this peer is at. */
		uint16_t getPortNumber() const {
			return portNumber;
		}

		/* Adds a piece to this Peer's pieces set. */
		void addPiece(const Piece & piece) {

			pieces.insert(piece);
		}

		/* Gets the pieces set that this peer has. */
		std::unordered_set<Piece, PieceHash> getPieces() {

			return pieces;
		}

		bool operator== (const Peer & p) const {

			return (portNumber == portNumber && ip == p.getIp());
		}

		void printPeer() {

			std::cout << ip << " : " << portNumber << std::endl;
		}

		/* Sets the field peerwire altered field. 
		   Should be called as soon as the peerwire protocol
		   object takes control of this Peer. */
		void setPeerwireAltered() {

			peerwireAltered = true;
		}

		/* Indicates whether the peerwire protocol has altered
		   this peer object. */
		bool getPeerwireAltered() {

			return peerwireAltered;
		}
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