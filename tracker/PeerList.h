#ifndef TORRENTIAL_DOWNLOADS_PEER_LIST_H
#define TORRENTIAL_DOWNLOADS_PEER_LIST_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <pthread.h>

#include "Peer.h"

class PeerList {
	
	private:

		/* List holding all of the Peer objects. */
		std::vector<Peer> peerList;

		/* Set holding all of the Peer objects. */
		std::unordered_set<Peer, PeerHash> peerSet;

		/* Read/write lock used to prevent writes during reads. */
		pthread_rwlock_t lock;

	public:

		/* Constructor */
		PeerList() {

			lock = PTHREAD_RWLOCK_INITIALIZER;
		}

		/* Destructor */
		~PeerList() {

		}

		/* Get a vector of Peer objects. */
		std::vector<Peer> * getPeers() {

			return new std::vector<Peer>(peerList);
		}

		/* Adds a vector of Peer objects to the PeerList. */
		void addPeers(std::vector<Peer *> newPeers) {

			pthread_rwlock_rdlock(&lock);

			std::vector<Peer *>::iterator it;
			for (it = newPeers.begin(); it != newPeers.end(); ++it) {

				Peer p = **it;

				//If we don't already have that peer
				if (peerSet.find(p) == peerSet.end()) {

					//add him
					peerList.push_back(p);
					peerSet.insert(p);
				}
			}

			pthread_rwlock_unlock(&lock);
		}

		/* Removes the peer at the passed in index of the list. */
		bool removePeer(int index) {

			pthread_rwlock_wrlock(&lock);

			if (index > -1 && index < (int)peerList.size()) {

				Peer p = peerList[index];
				peerSet.erase(p);
				peerList.erase(peerList.begin() + index);

				pthread_rwlock_unlock(&lock);

				return true;
			}

			pthread_rwlock_unlock(&lock);

			return false;
		}

		void printList() {

			std::cout << "Printing PeerList...\n[";

			std::vector<Peer>::iterator it;
			for (it = peerList.begin(); it != peerList.end(); it++) {

				Peer p = *it;
				if (it != peerList.begin()) {

					std::cout << ",";
				}

				p.printPeer();
			}
			std::cout << "]\n\n";
		}
};

#endif