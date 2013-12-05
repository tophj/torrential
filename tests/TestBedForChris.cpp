#include <iostream>
#include <vector>
#include "../TorrentTrackerCommManager.h"
#include "../PeerList.h"

int main() {
	
	using namespace std;

	vector<string> trackers;

	//UDP tracker

uint8_t fileHash[] = {0xdf, 0x79, 0xd5, 0xef, 0xc6, 0x83, 0x4c, 0xfb, 0x31, 0x21, 0x8d, 0xb8, 0x3d, 0x6f, 0xf1, 0xc5, 0x5a, 0xd8, 0x17, 0x9d};

	//TPB AFK test2
	trackers.push_back("udp://tracker.publicbt.com:80/announce");

	cout << "Initializing TorrentTrackerCommManager.....\n";
	PeerList p;
	TorrentTrackerCommManager man(p, fileHash, trackers);
	cout << "INITIALIZED!\n";

	cout << "\nRequesting Peers.....\n";
	man.requestPeers(0, 0, 975000000);
	cout << "\nPeers grabbed etc....\n";

	cout << "\nPrinting peers info...\n";
	p.printList();
	cout << "\nPeer info Printed!\n";

	return 0;
}
