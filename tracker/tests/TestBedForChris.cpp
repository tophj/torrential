#include <iostream>
#include <vector>
#include "../TorrentTrackerCommManager.h"
#include "../PeerList.h"
#include "../../threadpool/threadpool.h"

int main() {
	
	using namespace std;

	vector<string> trackers;

	//UDP tracker


	uint8_t fileHash[] = {0x8C, 0x37, 0x60, 0xCB, 0x65, 0x1C, 0x86, 0x38, 
							0x61, 0xFA, 0x9A, 0xBE, 0x2E, 0xF7, 0x02, 0x46, 
							0x94, 0x3C, 0x19, 0x94};

	//TPB AFK test2
	trackers.push_back("http://torrent.fedoraproject.org:6969/announce");

	cout << "Initializing TorrentTrackerCommManager.....\n";
	PeerList p;
	struct thread_pool * pool = thread_pool_new(3);
	TorrentTrackerCommManager man(pool, p, fileHash, trackers);
	cout << "INITIALIZED!\n";

	cout << "\nRequesting Peers.....\n";
	man.requestPeers(0, 0, 975000000);
	cout << "\nPeers grabbed etc....\n";

	cout << "\nPrinting peers info...\n";
	p.printList();
	cout << "\nPeer info Printed!\n";

	return 0;
}
