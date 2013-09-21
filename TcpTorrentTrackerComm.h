#ifndef TORRENTIAL_DOWNLOADS_TCP_TORRENT_TRACKER_COMM
#define TORRENTIAL_DOWNLOADS_TCP_TORRENT_TRACKER_COMM

#include <string>

class TcpTorrentTrackerComm : TorrentTrackerComm {
	
	public:
		//~Constructors/Destructors----------------------
		TcpTorrentTrackerComm(const std::string tracker);
		TcpTorrentTrackerComm(const std::string tracker, const int newSecondsUntilTimeout);
		~TcpTorrentTrackerComm();

		//~Methods---------------------------------------
		/* Opens a connection with a tracker by sending initial GET requests. */
		void initiateConnection(const int amountUploaded, 
			const int amountDownloaded, const int amountLeft);
		/* Waits for a response from the tracker server. 
		   Times out if time goes over SECONDS_UNTIL_TIMEOUT. */
		const bool waitForResponse();
		/* Closes a connection with a tracker, this cancels any waiting for requests. */
		void closeConnection();

	private:
		//~Data Fields-----------------------------------
		
		//~Methods---------------------------------------

};

#endif