#include <string>
#include <iostream>
#include "UdpTorrentTrackerComm.h"

int main() {
	
	using namespace std;

	string tracker("");
	const int portNumber = 5;
	string fileHash("");
	
	cout << "Calling UdpTorrentTrackerComm constructor...\n";
	UdpTorrentTrackerComm track(tracker, portNumber, fileHash);
	cout << "UdpTorrentTrackerComm constructor SUCCESS!\n";

	cout << "Calling UdpTorrentTrackerComm initiateConnection...\n";
	if (track.initiateConnection(0, 0, 5000)) {
		cout << "CONNECTION INITIATION SUCCESS!!\n";
	}
	return 0;
}