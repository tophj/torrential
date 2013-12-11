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
		std::vector<Peer *> peerList;

		/* Set holding all of the Peer objects. */
		std::unordered_set<Peer *, PeerPointerHash> peerSet;

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

		std::unordered_set<Peer *, PeerPointerHash> * getPeerSet() {

			pthread_rwlock_rdlock(&lock);

			std::unordered_set<Peer *, PeerPointerHash> * returnPeers = new std::unordered_set<Peer *, PeerPointerHash>(peerSet);

			pthread_rwlock_unlock(&lock);

			return returnPeers;
		}

		/* Get a vector of Peer objects. */
		std::vector<Peer *> * getPeers() {

			pthread_rwlock_rdlock(&lock);

			std::vector<Peer *> * returnPeers = new std::vector<Peer *>(peerList);

			pthread_rwlock_unlock(&lock);

			return returnPeers;
		}

		/* Adds a peer to the PeerList. */
		void addPeer(Peer * peer) {

			peerList.push_back(peer);
			peerSet.insert(peer);
		}

		/* Adds a vector of Peer objects to the PeerList. */
		void addPeers(std::vector<Peer *> newPeers) {

			pthread_rwlock_wrlock(&lock);

			std::vector<Peer *>::iterator it;
			for (it = newPeers.begin(); it != newPeers.end(); ++it) {

				Peer * p = *it;

				//If we don't already have that peer
				if (peerSet.find(p) == peerSet.end()) {

					//add him
					peerList.push_back(p);
					peerSet.insert(p);
				}
			}

			pthread_rwlock_unlock(&lock);
		}

		void addHashPieceToPeer(const Peer * p, Piece & piece) {

			pthread_rwlock_wrlock(&lock);

			std::vector<Peer *>::iterator it = peerList.begin();
			for(; it != peerList.end(); it++) {

				if (**it == *p) {
					(**it).addPiece(piece);
					return;
				}
			}

			pthread_rwlock_unlock(&lock);
		}

		/* Removes the passed in peer from the list. */
		bool removePeer(Peer * p) {

			pthread_rwlock_wrlock(&lock);

			std::vector<Peer *>::iterator it = peerList.begin();
			for (; it != peerList.end(); it++) {

				//found peer
				if (**it == *p) {
					peerList.erase(it);
					peerSet.erase(p);
					
					pthread_rwlock_unlock(&lock);

					return true;
				}
			}

			pthread_rwlock_unlock(&lock);

			return false;
		}

		void printList() {

			std::cout << "Printing PeerList...\n[";

			std::vector<Peer *>::iterator it;
			for (it = peerList.begin(); it != peerList.end(); it++) {

				Peer * p = *it;
				if (it != peerList.begin()) {

					std::cout << ",";
				}

				p->printPeer();
			}
			std::cout << "]\n\n";
		}
};

#endif