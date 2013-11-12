#include <iostream>
#include <string>
#include "../TcpTorrentTrackerComm.h"

int main() {
	
	using namespace std;

	//Hash for ubuntu iso
	const uint8_t fileHash1[] = {0x2d, 0x11, 0xca, 0xa6, 0x80, 0xe0, 0xf4, 0xa5, 
		0x58, 0xd3, 0x6d, 0x61, 0x8d, 0x8d, 0x9d, 0x5b, 0xd4, 0xc6, 0x83, 0x58};

	//TPB AFK test1
	string tracker1("torrent.fedoraproject.org");
	const int portNumber1 = 6969;
	int myPortNumber1 = 55555;
	
	cout << "TRACKER TEST 1\n";
	cout << "Calling UdpTorrentTrackerComm constructor...\n";
	TcpTorrentTrackerComm track1(tracker1, portNumber1, fileHash1, myPortNumber1);
	cout << "UdpTorrentTrackerComm constructor SUCCESS!\n";

	cout << "Calling UdpTorrentTrackerComm initiateConnection...\n";
	if (track1.initiateConnection()) {
		
		cout << "CONNECTION INITIATION SUCCESS!!\n";

		cout << "Listening for response.....\n";
		const std::vector<Peer * > * peers;
		if ((peers = track1.requestPeers(0, 0, 8240000000, STARTED))) {

			cout << "BRILLIANT! TWO WAY COMMUNICATION!!!!\n";
			
			cout << "AWAITING INTERVAL of size: " << track1.getRequestInterval() << " TO REQUEST MORE\n";
		}
		else {

			cout << "FUCKING AWFULLLLLLLLLLLLLLLLLLLLLLLLlll! THERE IS NO TWO WAY COMMUNICATION!!!!!!!!!!\n";
		}
	}

	cout << "\n\n\n#########################################################################################\n";
	cout << "#########################################################################################\n\n\n";

	return 0;
}