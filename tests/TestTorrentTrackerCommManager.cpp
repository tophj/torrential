#include <iostream>
#include <vector>
#include "../TorrentTrackerCommManager.h"

int main() {
	
	using namespace std;

	vector<string> trackers;

	//UDP tracker
	const string fileHash = "99FEAE0A05C6A5DD9AF939FFCE5CA9B0D16F31B0";

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
