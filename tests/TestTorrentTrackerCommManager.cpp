#include <iostream>
#include <vector>
#include "../TorrentTrackerCommManager.h"

int main() {
	
	using namespace std;

	vector<string> trackers;

	//UDP tracker
	const uint8_t fileHash[] = {0x99, 0xFE, 0xAE, 0x0A, 0x05, 0xC6, 0xA5, 0xDD, 0x9A, 
								0xF9, 0x39, 0xFF, 0xCE, 0x5C, 0xA9, 0xB0, 0xD1, 0x6F, 
								0x31, 0xB0};

	trackers.push_back("udp://tracker.seedceo.com:2710/announce");
	trackers.push_back("udp://tracker.beeimg.com:6969/announce");
	trackers.push_back("udp://exodus.desync.com:6969/announce");

	cout << "Initializing TorrentTrackerCommManager.....\n";
	TorrentTrackerCommManager man(fileHash, trackers);
	cout << "INITIALIZED!\n";

	cout << "\nRequesting Peers.....\n";
	man.requestPeers(0, 0, 975000000);
	cout << "\nPeers grabbed etc....\n";

	cout << "\nPrinting peers info...\n";
	//Print peer info etc
	cout << "\nPeer info Printed!\n";

	return 0;
}
