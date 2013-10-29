#include <iostream>
#include <string>
#include "../UdpTorrentTrackerComm.h"

int main() {
	
	using namespace std;

	const uint8_t fileHash1[] = {0x99, 0xFE, 0xAE, 0x0A, 0x05, 0xC6, 0xA5, 0xDD, 0x9A, 0xF9, 0x39, 0xFF, 0xCE, 0x5C, 0xA9, 0xB0, 0xD1, 0x6F, 0x31, 0xB0};

	time_t curTime;

	//TPB AFK test1
	string tracker1("31.172.63.226");
	const int portNumber1 = 80;
	int myPortNumber1 = 55555;

	//TPB AFK test2
	string tracker2("tracker.publicbt.com");
	const int portNumber2 = 80;
	int myPortNumber2 = 51414;

	//TPB AFK Test 3
	string tracker3("tracker.seedceo.com");
	const int portNumber3 = 2710;
	int myPortNumber3 = 51416;

	//TPB AFK Test 4
	/*string tracker4("tracker.beeimg.com");
	const int portNumber4 = 6969;
	int myPortNumber4 = 51513;*/

	//TPB Afk Test 5
	string tracker5("exodus.desync.com");
	const int portNumber5 = 6969;
	int myPortNumber5 = 59413;

	//TPB AFK Test 6
	string tracker6("tracker.coppersurfer.tk");
	const int portNumber6 = 6969;
	int myPortNumber6 = 56565;
	
	cout << "TRACKER TEST 1\n";
	cout << "Calling UdpTorrentTrackerComm constructor...\n";
	UdpTorrentTrackerComm track1(tracker1, portNumber1, fileHash1, myPortNumber1);
	cout << "UdpTorrentTrackerComm constructor SUCCESS!\n";

	cout << "Calling UdpTorrentTrackerComm initiateConnection...\n";
	if (track1.initiateConnection()) {
		
		cout << "CONNECTION INITIATION SUCCESS!!\n";

		cout << "Listening for response.....\n";
		const std::vector<Peer * > * peers;
		if ((peers = track1.requestPeers(0, 0, 8240000000, STARTED))) {
		//if ((peers = track.requestPeers(0, 0, 8240000000, NONE))) {			

			cout << "BRILLIANT! TWO WAY COMMUNICATION!!!!\n";
			
			cout << "AWAITING INTERVAL of size: " << track1.getRequestInterval() << " TO REQUEST MORE\n";
			//cout << "getTimeOfLastResponse: ||" << track1.getTimeOfLastResponse() << endl;

			//time(&curTime);
			//cout << "curTime - track1.getTimeOfLastResponse()" << difftime(curTime, track1.getTimeOfLastResponse()) << endl;
			/*while(track1.getRequestInterval() > difftime(track1.getTimeOfLastResponse(), curTime)) {
				cout << "looping....\n";
				cout << "AWAITING INTERVAL of size: " << track1.getRequestInterval() << " TO REQUEST MORE\n";
				cout << "getTimeOfLastResponse: ||" << track1.getTimeOfLastResponse() << endl;

				time(&curTime);
				cout << "clock - track1.getTimeOfLastResponse()" << difftime(curTime, track1.getTimeOfLastResponse()) << endl;				
			}*/
			/*if ((peers = track1.requestPeers(0, 0, 8240000000, NONE))) {

				cout << "SUCCEEDED IN GETTING MOAR!\n";
				sleep(200);
			}*/
		}
		else {

			cout << "FUCKING AWFULLLLLLLLLLLLLLLLLLLLLLLLlll! THERE IS NO TWO WAY COMMUNICATION!!!!!!!!!!\n";
		}
	}

	cout << "\n\n\n#########################################################################################\n";
	cout << "#########################################################################################\n\n\n";

return -66;


	cout << "\n\n\nTRACKER TEST 2\n";
	cout << "Calling UdpTorrentTrackerComm constructor...\n";
	UdpTorrentTrackerComm track2(tracker2, portNumber2, fileHash1, myPortNumber2);
	cout << "UdpTorrentTrackerComm constructor SUCCESS!\n";

	cout << "Calling UdpTorrentTrackerComm initiateConnection...\n";
	if (track2.initiateConnection()) {
		
		cout << "CONNECTION INITIATION SUCCESS!!\n";

		cout << "Listening for response.....\n";
		const std::vector<Peer * > * peers;
		//if ((peers = track.requestPeers(0, 0, 8240000000, STARTED))) {
		if ((peers = track2.requestPeers(0, 0, 8240000000, STARTED))) {
		//if ((peers = track.requestPeers(0, 0, 8240000000, NONE))) {			

			cout << "BRILLIANT! TWO WAY COMMUNICATION!!!!\n";
			delete peers->front();
			delete peers;
		}
	}

	cout << "\n\n\n#########################################################################################\n";
	cout << "#########################################################################################\n\n\n";

	cout << "\n\n\nTRACKER TEST 3\n";
	cout << "Calling UdpTorrentTrackerComm constructor...\n";
	UdpTorrentTrackerComm track3(tracker3, portNumber3, fileHash1, myPortNumber3);
	cout << "UdpTorrentTrackerComm constructor SUCCESS!\n";

	cout << "Calling UdpTorrentTrackerComm initiateConnection...\n";
	if (track3.initiateConnection()) {
		
		cout << "CONNECTION INITIATION SUCCESS!!\n";

		cout << "Listening for response.....\n";
		const std::vector<Peer * > * peers;
		//if ((peers = track.requestPeers(0, 0, 8240000000, STARTED))) {
		if ((peers = track3.requestPeers(0, 0, 8240000000, STARTED))) {
		//if ((peers = track.requestPeers(0, 0, 8240000000, NONE))) {			

			cout << "BRILLIANT! TWO WAY COMMUNICATION!!!!\n";
			delete peers->front();
			delete peers;
		}
	}

	cout << "\n\n\n#########################################################################################\n";
	cout << "#########################################################################################\n\n\n";	

	/*cout << "\n\n\nTRACKER TEST 4\n";
	cout << "Calling UdpTorrentTrackerComm constructor...\n";
	UdpTorrentTrackerComm track4(tracker4, portNumber4, fileHash1, myPortNumber4);
	cout << "UdpTorrentTrackerComm constructor SUCCESS!\n";

	cout << "Calling UdpTorrentTrackerComm initiateConnection...\n";
	if (track4.initiateConnection()) {
		
		cout << "CONNECTION INITIATION SUCCESS!!\n";

		cout << "Listening for response.....\n";
		const std::vector<Peer * > * peers;
		//if ((peers = track.requestPeers(0, 0, 8240000000, STARTED))) {
		if ((peers = track4.requestPeers(0, 0, 8240000000, STARTED))) {
		//if ((peers = track.requestPeers(0, 0, 8240000000, NONE))) {			

			cout << "BRILLIANT! TWO WAY COMMUNICATION!!!!\n";
			delete peers->front();
			delete peers;
		}
	}*/

	cout << "\n\n\n#########################################################################################\n";
	cout << "#########################################################################################\n\n\n";

	cout << "\n\n\nTRACKER TEST 5\n";
	cout << "Calling UdpTorrentTrackerComm constructor...\n";
	UdpTorrentTrackerComm track5(tracker5, portNumber5, fileHash1, myPortNumber5);
	cout << "UdpTorrentTrackerComm constructor SUCCESS!\n";

	cout << "Calling UdpTorrentTrackerComm initiateConnection...\n";
	if (track5.initiateConnection()) {
		
		cout << "CONNECTION INITIATION SUCCESS!!\n";

		cout << "Listening for response.....\n";
		const std::vector<Peer * > * peers;
		//if ((peers = track.requestPeers(0, 0, 8240000000, STARTED))) {
		if ((peers = track5.requestPeers(0, 0, 8240000000, STARTED))) {
		//if ((peers = track.requestPeers(0, 0, 8240000000, NONE))) {			

			cout << "BRILLIANT! TWO WAY COMMUNICATION!!!!\n";
			//delete peers->front();
			//delete peers;
			time(&curTime);
			cout << "AWAITING INTERVAL of size: " << track5.getRequestInterval() << " TO REQUEST MORE\n";
			cout << "curTime - track5.getTimeOfLastResponse()" << difftime(curTime, track5.getTimeOfLastResponse()) << endl;
			while(track5.getRequestInterval() > difftime(curTime, track5.getTimeOfLastResponse())) {}
			peers = track5.requestPeers(0, 0, 8240000000, NONE);
		}
	}

	cout << "\n\n\n#########################################################################################\n";
	cout << "#########################################################################################\n\n\n";

	cout << "\n\n\nTRACKER TEST 6\n";
	cout << "Calling UdpTorrentTrackerComm constructor...\n";
	UdpTorrentTrackerComm track6(tracker6, portNumber6, fileHash1, myPortNumber6);
	cout << "UdpTorrentTrackerComm constructor SUCCESS!\n";

	cout << "Calling UdpTorrentTrackerComm initiateConnection...\n";
	if (track6.initiateConnection()) {
		
		cout << "CONNECTION INITIATION SUCCESS!!\n";

		cout << "Listening for response.....\n";
		const std::vector<Peer * > * peers;
		if ((peers = track6.requestPeers(0, 0, 8240000000, STARTED))) {

			cout << "BRILLIANT! TWO WAY COMMUNICATION!!!!\n";

			cout << "AWAITING INTERVAL of size: " << track6.getRequestInterval() << " TO REQUEST MORE\n";
			time(&curTime);
			cout << "time - track6.getTimeOfLastResponse()" << difftime(curTime, track6.getTimeOfLastResponse()) << endl;
			while(track6.getRequestInterval() > difftime(curTime, track6.getTimeOfLastResponse())) {}
			peers = track6.requestPeers(0, 0, 8240000000, NONE);
		}
	}

	cout << "\n\n\n#########################################################################################\n";
	cout << "#########################################################################################\n\n\n";

	return 0;
}