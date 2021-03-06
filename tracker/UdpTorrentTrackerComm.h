#ifndef TORRENTIAL_DOWNLOADS_UDP_TORRENT_TRACKER_COMM
#define TORRENTIAL_DOWNLOADS_UDP_TORRENT_TRACKER_COMM

#include "TorrentTrackerComm.h"

/* Object used for connecting to and receiving from a specific tracker server using UDP. */
class UdpTorrentTrackerComm : public TorrentTrackerComm {
	
	public:
		//~Constructors/Destructors----------------------
		/* Constructor taking the tracker address, port number, and file hash. */
		UdpTorrentTrackerComm(const std::string tracker, 
								const uint16_t newPortNumber, 
								const uint8_t newFileHash[20],
								const uint16_t myNewPortNumber);

		/* Constructor taking the tracker address, port number, 
		   file hash, and a time limit to timeout. */
		UdpTorrentTrackerComm(const std::string tracker, 
								const uint16_t newPortNumber, 
								const uint8_t newFileHash[20],
								const uint16_t myNewPortNumber,
								const int newSecondsUntilTimeout);

		/* Destructor. */
		~UdpTorrentTrackerComm();

		//~Methods---------------------------------------
		/* Opens a connection with a tracker by sending initial GET requests. */
		virtual const bool initiateConnection(std::string ipAddress);

		/* Requests peers from the tracker server.
		   Returns a bencoded string ptr that is the response from the tracker.
		   Uses the curEvent for the event parameter.
		   		Returns NULL if the tracker did not respond. */
		virtual const std::vector<Peer * > * requestPeers(const uint64_t amountUploaded, 
															const uint64_t amountDownloaded, 
															const uint64_t amountLeft);

		/* Requests peers from the tracker server.
		   Returns a bencoded string ptr that is the response from the tracker.
		   		Returns NULL if the tracker did not respond. */
		virtual const std::vector<Peer * > * requestPeers(const uint64_t amountUploaded, 
															const uint64_t amountDownloaded, 
															const uint64_t amountLeft, 
															const TrackerEvent event);

	protected:
		//~Methods---------------------------------------

	private:
		//~Data Fields-----------------------------------
		//~Methods---------------------------------------		
};

#endif