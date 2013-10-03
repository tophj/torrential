#ifndef TORRENTIAL_DOWNLOADS_UDP_TORRENT_TRACKER_COMM
#define TORRENTIAL_DOWNLOADS_UDP_TORRENT_TRACKER_COMM

#include "TorrentTrackerComm.h"

struct ConnectionIdRequest_t {

	uint64_t connectionId;
	uint32_t action;
	uint32_t transactionId;
} typedef ConnectionIdRequest;

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
		/* The ID received from the tracker server used to identify this connection. */
		int64_t connectionId;
		
		//~Methods---------------------------------------
		/* Creates a ConnectionIdRequest object and fills its fields. */
		ConnectionIdRequest * createConnectionIdRequest();
};

#endif