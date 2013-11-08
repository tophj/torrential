#include <iostream>
#include <vector>
#include "../TorrentTrackerCommManager.h"
#include "../PeerList.h"

int main() {
	
	using namespace std;

	vector<string> trackers;

	//UDP tracker
	const uint8_t fileHash[] = {0x79, 0x81, 0x60, 0x60, 0xEA, 0x56, 
								0xD5, 0x6F, 0x2A, 0x21, 0x48, 0xCD,
							 	0x45, 0x70, 0x55, 0x11, 0x07, 0x9F, 
							 	0x9B, 0xCA};
	//TPB AFK test2
	trackers.push_back("udp://tracker.publicbt.com:80/announce");
	trackers.push_back("udp://tracker.publicbt.com:80/announce");
	trackers.push_back("udp://tracker.openbittorrent.com:80/announce");
	trackers.push_back("udp://tracker.istole.it:80/announce");
	trackers.push_back("udp://tracker.seedceo.com:2710/announce");
	trackers.push_back("udp://fr33domtracker.h33t.com:3310/announce");

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
