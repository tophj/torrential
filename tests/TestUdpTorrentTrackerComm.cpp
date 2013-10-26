#include <iostream>
#include <string>
#include "../UdpTorrentTrackerComm.h"

int main() {
	
	using namespace std;

	const string fileHash1 = "99FEAE0A05C6A5DD9AF939FFCE5CA9B0D16F31B0";

	time_t curTime;

	//TPB AFK test1
	string tracker1("31.172.63.226");
	const int portNumber1 = 80;
	int myPortNumber1 = 51413;

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

	//Blue Caprice test...whatever the fuck that is
	//string tracker6("tracker.publicbt.com");
	//string fileHash2("7FBC3A271B116F93CA5C7BCA0A712F20EEF5E0AF");

	
	cout << "TRACKER TEST 1\n";
	cout << "Calling UdpTorrentTrackerComm constructor...\n";
	UdpTorrentTrackerComm track1(tracker1, portNumber1, fileHash1, myPortNumber1);
	cout << "UdpTorrentTrackerComm constructor SUCCESS!\n";

	cout << "Calling UdpTorrentTrackerComm initiateConnection...\n";
	if (track1.initiateConnection()) {
		
		cout << "CONNECTION INITIATION SUCCESS!!\n";

		cout << "Listening for response.....\n";
		const std::vector<Peer * > * peers;
		//if ((peers = track.requestPeers(0, 0, 8240000000, STARTED))) {
		if ((peers = track1.requestPeers(0, 0, 8240000000, STARTED))) {
		//if ((peers = track.requestPeers(0, 0, 8240000000, NONE))) {			

			cout << "BRILLIANT! TWO WAY COMMUNICATION!!!!\n";
			
			cout << "AWAITING INTERVAL of size: " << track1.getRequestInterval() << " TO REQUEST MORE\n";
			cout << "getTimeOfLastResponse: ||" << track1.getTimeOfLastResponse() << endl;

			time(&curTime);
			cout << "clock - track1.getTimeOfLastResponse()" << difftime(curTime, track1.getTimeOfLastResponse()) << endl;
			while(track1.getRequestInterval() > difftime(track1.getTimeOfLastResponse(), curTime)) {
				cout << "looping....\n";
				cout << "AWAITING INTERVAL of size: " << track1.getRequestInterval() << " TO REQUEST MORE\n";
				cout << "getTimeOfLastResponse: ||" << track1.getTimeOfLastResponse() << endl;

				time(&curTime);
				cout << "clock - track1.getTimeOfLastResponse()" << difftime(curTime, track1.getTimeOfLastResponse()) << endl;				
			}
			if ((peers = track1.requestPeers(0, 0, 8240000000, NONE))) {

				cout << "SUCCEEDED IN GETTING MOAR!\n";
				sleep(200);
			}
		}
		else {

			cout << "FUCKING AWFULLLLLLLLLLLLLLLLLLLLLLLLlll! THERE IS NO TWO WAY COMMUNICATION!!!!!!!!!!\n";
		}
	}

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
			cout << "AWAITING INTERVAL of size: " << track5.getRequestInterval() << " TO REQUEST MORE\n";
			cout << "clock - track5.getTimeOfLastResponse()" << (clock() - track5.getTimeOfLastResponse()) << endl;
			while(track5.getRequestInterval() > (clock() - track5.getTimeOfLastResponse())) {}
			peers = track5.requestPeers(0, 0, 8240000000, NONE);
		}
	}



	return 0;
}