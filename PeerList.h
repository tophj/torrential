#ifndef TORRENTIAL_DOWNLOADS_PEER_LIST_H
#define TORRENTIAL_DOWNLOADS_PEER_LIST_H

#include <vector>
#include <unordered_set>

#include "Peer.h"

class PeerList {
	
	private:

		/* List holding all of the Peer objects. */
		std::vector<Peer> peerList;

		/* Set holding all of the Peer objects. */
		std::unordered_set<Peer, PeerHash> peerSet;

	public:

		/* Constructor */
		PeerList() {


		}

		/* Destructor */
		~PeerList() {

		}

		/* Get a vector of Peer objects. */
		std::vector<Peer> & getPeers() {

			return peerList;
		}

		/* Adds a vector of Peer objects to the PeerList. */
		void addPeers(std::vector<Peer *> newPeers) {

			std::vector<Peer *>::iterator it;
			for (it = newPeers.begin(); it != newPeers.end(); ++it) {

				/*Peer & peerRef*/ Peer p = **it;
				//Peer * p = new Peer(peerRef);

				//If we don't already have that peer
				if (peerSet.find(p) != peerSet.end()) {

					//add him
					peerList.push_back(p);
					peerSet.insert(p);
				}
			}
		}

		/* Removes the peer at the passed in index of the list. */
		bool removePeer(int index) {

			if (index > -1 && index < (int)peerList.size()) {

				Peer p = peerList[index];
				peerSet.erase(p);
				peerList.erase(peerList.begin() + index);

				return true;
			}

			return false;
		}
};

#endif