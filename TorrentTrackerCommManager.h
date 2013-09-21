#ifndef TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM_MANAGER
#define TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM_MANAGER

#include <string>

class TorrentTrackerCommManager {
	
	public:
		//~Constructors/Destructors----------------------
		TorrentTrackerComm();
		/* Set the trackers into the trackers vector. 
		   Initializes communication variables for this side of
		   the connection. */
		TorrentTrackerComm(vector<std::string> newTrackers);
		~TorrentTrackerComm();

		//~Methods---------------------------------------		
		/* Request that this request peers from all trackers that have returned data or haven't been queried. 
		   Places results into the PeersPool variable. */
		std::vector<Peer> requestPeers();
		/* Adds a tracker url or ip to the trackers list.
		   Returns true if the tracker was added, false if it wasn't. */
		bool addTracker(std::string newTracker);
		/* Removes a tracker denoted 
		   Returns true if the tracker was removed, false if it wasn't.*/
		bool removeTracker(std::string deleteTracker);
		
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
		const TorrentTrackerComm const * makeTorrentTrackerComm(std::string);
};


#endif