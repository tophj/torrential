#include "TorrentTrackerCommManager.h"
#include <iostream>
#include <vector>

int main() {
	
	std::vector<std::string> trackers;
	//TCP tracker
	trackers.push_back("");
	//UDP tracker
	trackers.push_back("");
	TorrentTrackerCommManager man(trackers);
	man.requestPeers();

	return 0;
}