#include <string>
#include <iostream>
#include "../UdpTorrentTrackerComm.h"

int main() {
	
	using namespace std;

	string tracker("tracker.publicbt.com");
	const int portNumber = 80;
	string fileHash("7FBC3A271B116F93CA5C7BCA0A712F20EEF5E0AF");
	
	cout << "Calling UdpTorrentTrackerComm constructor...\n";
	UdpTorrentTrackerComm track(tracker, portNumber, fileHash);
	cout << "UdpTorrentTrackerComm constructor SUCCESS!\n";

	cout << "Calling UdpTorrentTrackerComm initiateConnection...\n";
	if (track.initiateConnection(0, 0, 5000)) {
		cout << "CONNECTION INITIATION SUCCESS!!\n";
	}
	return 0;
}