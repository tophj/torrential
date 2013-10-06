#include <string>
#include <iostream>
#include "../UdpTorrentTrackerComm.h"

int main() {
	
	using namespace std;

	//TPB AFK test1
	//string tracker("tracker.publicbt.com");
	//string tracker("31.172.63.226");
	//string fileHash("189FFEF88725D7668AB08C78CBBCCA202EBAA208");

	//TPB AFK test2
	string tracker("tracker.publicbt.com");
	//string tracker("31.172.63.226");
	string fileHash("99FEAE0A05C6A5DD9AF939FFCE5CA9B0D16F31B0");

	//Blue Caprice test...whatever the fuck that is
	//string tracker("tracker.publicbt.com");
	//string fileHash("7FBC3A271B116F93CA5C7BCA0A712F20EEF5E0AF");



	const int portNumber = 80;
	
	cout << "Calling UdpTorrentTrackerComm constructor...\n";
	UdpTorrentTrackerComm track(tracker, portNumber, fileHash);
	cout << "UdpTorrentTrackerComm constructor SUCCESS!\n";

	cout << "Calling UdpTorrentTrackerComm initiateConnection...\n";
	if (track.initiateConnection()) {
		
		cout << "CONNECTION INITIATION SUCCESS!!\n";

		cout << "Listening for response.....\n";
		if (track.requestPeers(0, 0, 8240000000, STARTED)) {

			cout << "BRILLIANT! TWO WAY COMMUNICATION!!!!\n";
		}
	}

	return 0;
}