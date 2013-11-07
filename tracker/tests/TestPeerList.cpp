#include "../PeerList.h"
#include "../Peer.h"

#include <vector>

#define NUM_THREADS 8

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

using namespace std;

void * addPeers(void * args) {

	static int portNum = 333;

	cout << "Adding IPs with portNum: " << portNum << endl;

	PeerList * pList = (PeerList *) args;

	Peer p1("192.168.1.1", portNum);
	Peer p2("98.162.7.7", portNum);
	Peer p3("192.168.1.1", portNum);
	Peer p4("98.162.7.7", portNum);

	pthread_mutex_lock(&mutex);

	vector<Peer *> peers;
	peers.push_back(&p1);
	peers.push_back(&p2);
	peers.push_back(&p3);
	peers.push_back(&p4);

	pList->addPeers(peers);

	pthread_mutex_unlock(&mutex);

	portNum += 111;

	return NULL;
}

void * removePeers(void * args) {

	static int portNum = 222;

	PeerList * pList = (PeerList *) args;

	cout << "Removing IPs with portNum: " << portNum << endl;

	Peer p1("192.168.1.1", portNum);
	Peer p2("98.162.7.7", portNum);
	Peer p3("192.168.1.1", portNum);
	Peer p4("98.162.7.7", portNum);

	vector<Peer> * curPeers = pList->getPeers();

	int counter = 0;
	vector<Peer>::iterator it;
	for (it = curPeers->begin(); it != curPeers->end(); it++, counter++) {

		if (*it == p1 || *it == p2 || *it == p3 || *it == p4) {

			pList->removePeer(counter);
		}
	}

	portNum += 111;

	return NULL;
}

int main() {

	PeerList pList;
	Peer p1("192.168.1.1", 222);
	Peer p2("98.162.7.7", 222);
	Peer p3("192.168.1.1", 222);
	Peer p4("98.162.7.7", 222);

	vector<Peer *> peers;
	peers.push_back(&p1);
	peers.push_back(&p2);
	peers.push_back(&p3);
	peers.push_back(&p4);

	pList.addPeers(peers);

	pthread_t threads[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; i++) {

		if (i % 2 == 0) {
			
			pthread_create(&threads[i], NULL, addPeers, &pList);
		}
		else {

			pthread_create(&threads[i], NULL, removePeers, &pList);
		}
	}

	for (int i = 0; i < NUM_THREADS; i++) {
	
		pthread_join(threads[i], NULL);
	}

	pList.printList();

	return 1;
}