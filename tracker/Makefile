CC=g++
CFLAGS= -Wall -Werror -g -std=gnu++0x -lpthread

manager_test: SystemFunctionWrappers.cpp TorrentTrackerComm.cpp TcpTorrentTrackerComm.cpp UdpTorrentTrackerComm.cpp TorrentTrackerCommManager.cpp tests/TestTorrentTrackerCommManager.cpp *.h
	$(CC) $(CFLAGS) -o managerTest SystemFunctionWrappers.cpp TorrentTrackerComm.cpp TcpTorrentTrackerComm.cpp UdpTorrentTrackerComm.cpp TorrentTrackerCommManager.cpp tests/TestTorrentTrackerCommManager.cpp

udp_test: SystemFunctionWrappers.cpp TorrentTrackerComm.cpp UdpTorrentTrackerComm.cpp tests/TestUdpTorrentTrackerComm.cpp *.h
	$(CC) $(CFLAGS) -o udpTest SystemFunctionWrappers.cpp TorrentTrackerComm.cpp UdpTorrentTrackerComm.cpp tests/TestUdpTorrentTrackerComm.cpp

peer_list_test: PeerList.h Peer.h tests/TestPeerList.cpp
	$(CC) $(CFLAGS) -o testPeerList tests/TestPeerList.cpp

all: *.cpp *.h
	$(CC) $(CFLAGS) -c *.cpp

clean:
	rm -rf *o udpTest managerTest
