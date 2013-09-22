#ifndef TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM_MANAGER
#define TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM_MANAGER

#include <vector>

#include "TorrentTrackerComm.h"
#include "TcpTorrentTrackerComm.h"
#include "UdpTorrentTrackerComm.h"

class TorrentTrackerCommManager {
	
	public:
		//~Constructors/Destructors----------------------
		/* Initializes a TorrentTrackerCommManager with an empty list of trackers. */
		TorrentTrackerCommManager();

		/* Set the trackers into the trackers vector. 
		   Initializes communication variables for this side of
		   the connection. */
		TorrentTrackerCommManager(const std::string newFileHash, 
									const std::vector<std::string> newTrackers);

		/* Destructor. */
		~TorrentTrackerCommManager();

		//~Methods---------------------------------------		
		/* Request that this request peers from all trackers that have returned data or haven't been queried. 
		   Places results into the PeersPool variable. */
		void requestPeers() const;

		/* Adds a tracker url or ip to the trackers list.
		   Returns true if the tracker was added, false if it wasn't. */
		const bool addTracker(const std::string newFileHash, 
								const std::string newTracker);

		/* Removes a tracker denoted 
		   Returns true if the tracker was removed, false if it wasn't. */
		const bool removeTracker(const std::string newFileHash, 
									const std::string deleteTracker);
		
	private:
		//~Data Fields-----------------------------------
		/* A list of TorrentTrackerComm objects that communicate with torrent trackers. */
		std::vector<TorrentTrackerComm> trackers;
		//POOL OF PEERS THAT PEER OBJECTS ARE PUT IN

		//~Methods---------------------------------------			
		/* Takes a string that is either an IP address or a web address and has information
		   about whether the tracker denoted by the IP or address uses TCP or UDP. Creates
		   the corresponding TorrentTrackerComm object and returns it. NOTE: IPv6 addresses
		   are currently ignored and result in NULL being returned. */
		const TorrentTrackerComm const * makeTorrentTrackerComm(const std::string newFileHash, 
																const std::string newTracker) const;
};


#endif