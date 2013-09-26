#ifndef TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM
#define TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM

#include <string>
#include <ctime>
#include <cstdlib>
#include <string.h>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "SystemFunctionWrappers.h"

class TorrentTrackerComm {
	
	public:
		//~Constructors/Destructors----------------------
		/* Constructor taking the tracker address, port number, and file hash. */
		TorrentTrackerComm(const std::string tracker, 
							const int newPortNumber, 
							const std::string newFileHash);

		/* Constructor taking the tracker address, port number, 
		   file hash, and a time limit to timeout. */
		TorrentTrackerComm(const std::string tracker, 
							const int newPortNumber, 
							const std::string newFileHash,
							const int newSecondsUntilTimeout);

		/* Destructor. */
		virtual ~TorrentTrackerComm();

		//~Methods---------------------------------------
		/* Opens a connection with a tracker by sending initial GET requests. */
		virtual const bool initiateConnection(const int amountUploaded, 
												const int amountDownloaded, 
												const int amountLeft) 
											= 0;

		/* Waits for a response from the tracker server. 
		   Times out if time goes over SECONDS_UNTIL_TIMEOUT. */
		virtual const bool waitForResponse() const = 0;

	protected:
		//~Data Fields-----------------------------------
		/* If no 'newSecondsUntilTimeout' is provided to the class then it defaults to this value. */
		static const int DEFAULT_SECONDS_UNTIL_TIMEOUT = 60;

		/* Holds the system time that a request from this method was made. */
		clock_t timeRequestSent;

		/* Value indicating how long to wait before considering a connection timed out.*/
		int SECONDS_UNTIL_TIMEOUT;

		/* The IP address of the tracker to connect to. */
		const std::string * trackerAddress;

		/* The hostname of the tracker server. */
		const std::string * trackerHostname;

		/* The port number to connect to the tracker on. */
		int portNumber;

		/* 20 byte SHA-1 hash of the file that peers are needed for. */
		std::string fileHash;

		/* The peer id for the current connection. */
		std::string * peerId;

		//~Methods---------------------------------------
		/* Takes a string a checks if the string is a valid IPv4 address.
		   Returns true if the string is a valid IPv4 address, false otherwise. */
		const bool isIp4Address(const std::string theString) const;

		/* Takes a string a checks if the string is a valid IPv6 address.
		   Returns true if the string is a valid IPv6 address, false otherwise. */
		const bool isIp6Address(const std::string theString) const;

		/* Generates a random 20 character string that is to be used as 
		   the peer id for a connection with a tracker server. 
		   Saves the new peerId into the peerId field. */
		void generatePeerId();

		/* Returns true if the connection timed out, false otherwise. */
		const bool isTimedOut() const;

		/* Takes the amountUploaded, amountDownloaded, and amountLeft for a file
		   and creates a tracker request string from it. 
		   Returns this as a std::string */
		std::string * createTrackerRequest(const int amountUploaded, 
													const int amountDownloaded,
													const int amountLeft);
};

#endif