#ifndef TORRENTIAL_DOWNLOADS_UDP_TORRENT_TRACKER_COMM
#define TORRENTIAL_DOWNLOADS_UDP_TORRENT_TRACKER_COMM

#include "TorrentTrackerComm.h"

class UdpTorrentTrackerComm : public TorrentTrackerComm {
	
	public:
		//~Constructors/Destructors----------------------
		/* Constructor taking the tracker address, port number, and file hash. */
		UdpTorrentTrackerComm(const std::string tracker, 
							const int newPortNumber, 
							const std::string newFileHash);

		/* Constructor taking the tracker address, port number, 
		   file hash, and a time limit to timeout. */
		UdpTorrentTrackerComm(const std::string tracker, 
							const int newPortNumber, 
							const std::string newFileHash,
							const int newSecondsUntilTimeout);

		/* Destructor. */
		~UdpTorrentTrackerComm();

		//~Methods---------------------------------------
		/* Opens a connection with a tracker by sending initial GET requests. */
		virtual const bool initiateConnection(const int amountUploaded, 
												const int amountDownloaded, 
												const int amountLeft);

		/* Waits for a response from the tracker server. 
		   Times out if time goes over SECONDS_UNTIL_TIMEOUT. */
		virtual const bool waitForResponse() const;

	protected:
		//~Methods---------------------------------------

	private:
		//~Data Fields-----------------------------------
		
		//~Methods---------------------------------------
};

#endif