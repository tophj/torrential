#ifndef TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM_MANAGER
#define TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM_MANAGER

#include <vector>
#include <unordered_set>

#include "../threadpool/threadpool.h"

#include "TcpTorrentTrackerComm.h"
#include "UdpTorrentTrackerComm.h"
#include "PeerList.h"

/* Struct used to pass to the 'addPeerToPeerList' */
struct CallRequestPeersParams_t {

	uint64_t amountUploaded;
	uint64_t amountDownloaded;
	uint64_t amountLeft;
	TorrentTrackerComm * tracker;
	std::vector<TorrentTrackerComm *> * trackers;
	PeerList * peerList;
} typedef CallRequestPeersParams;

class TorrentTrackerCommManager {
	
	public:
		//~Constructors/Destructors----------------------
		/* Set the trackers into the trackers vector. 
		   Initializes communication variables for this side of
		   the connection. */
		TorrentTrackerCommManager(struct thread_pool * theThreadPool,
									PeerList & newPeerList,
									uint8_t newFileHash[20], 
									std::vector<std::string> & newTrackers);

		/* Destructor. */
		~TorrentTrackerCommManager();

		//~Methods---------------------------------------		
		/* Request that this request peers from all trackers that have returned data or haven't been queried. 
		   Places results into the PeersPool variable. Uses the TorrentTrackerComm objects current event. */
		void requestPeers(const uint64_t amountUploaded, 
							const uint64_t amountDownloaded, 
							const uint64_t amountLeft);

		/* Adds a tracker url or ip to the trackers list.
		   Returns true if the tracker was added, false if it wasn't. */
		const bool addTracker(const std::string & newTracker);

		/* Removes a tracker denoted 
		   Returns true if the tracker was removed, false if it wasn't. */
		const bool removeTracker(const std::string & deleteTracker);

	private:
		//~Data Fields-----------------------------------
		/* A list of TorrentTrackerComm objects that communicate with torrent trackers. */
		std::vector<TorrentTrackerComm *> trackers;

		/* A hash set to ensure unique port numbers are used for communication with each tracker. */
		std::unordered_set<uint16_t> portSet;

		/* PeerList object where all peers are placed. */
		PeerList & peerList;

		/* The file hash of the torrent being managed. */
		uint8_t fileHash[20];

		/* The thread pool that functions will be passed to. */
		struct thread_pool * threadPool;

		//~Methods---------------------------------------			
		/* Takes a string that is either an IP address or a web address and has information
		   about whether the tracker denoted by the IP or address uses TCP or UDP. Creates
		   the corresponding TorrentTrackerComm object and returns it. NOTE: IPv6 addresses
		   are currently ignored and result in NULL being returned. */
		TorrentTrackerComm * makeTorrentTrackerComm(const std::string & newTracker) const;

		/* Generates a port number that is not already in use by this program that is above 50000. */
		const uint16_t generatePortNumber() const;
};


#endif