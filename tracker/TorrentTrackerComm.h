#ifndef TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM
#define TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM

#include <vector>
#include <stdint.h>
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

#include "Peer.h"
#include "SystemFunctionWrappers.h"

/* Struct used to send a request for a connectionId to the tracker server.*/
struct ConnectionIdRequest_t {

	uint64_t connectionId;
	uint32_t action;
	uint32_t transactionId;
} typedef ConnectionIdRequest;

/* Struct used in receipt of a request for a connectionId from the tracker server. */
struct ConnectionIdResponse_t {

	uint32_t action;
	uint32_t transactionId;
	uint64_t connectionId;
} typedef ConnectionIdResponse;

/* Struct used to send a request for peers to the tracker server. 98 bytes. */
struct PeerRequest_t {

	uint64_t connectionId;
	uint32_t action;
	uint32_t transactionId;
	uint8_t fileHash[20];
	uint8_t peerId[20];
	uint64_t amountDownloaded;
	uint64_t amountLeft;
	uint64_t amountUploaded;
	uint32_t event;
	uint32_t ipAddress;
	uint32_t key;
	int32_t numWant;
	uint16_t portNumber;
} typedef PeerRequest;

/* Struct used in receipt of a request for peers from the tracker server.
   At least 20 bytes + 6 bytes per PeerAddress (leechers + seeders). */
struct PeerResponse_t {

	int32_t action;
	int32_t transactionId;
	int32_t interval;
	int32_t leechers;
	int32_t seeders;
	uint8_t sources[1000];
} typedef PeerResponse;

/* Struct used in receipt of a request for peers from the tracker server
	in the case that an error occurs in retrieving the peers. */
struct PeerResponseError_t {

	uint32_t action;
	uint32_t transactionId;
	uint8_t errorString[];
} typedef PeerResponseError;

/* Enumeration with fields to announce an action to the tracker server. */
enum TrackerAction {CONNECT = 0, ANNOUNCE, SCRAPE, ERROR};

/* Enumeration with fields to announce an event to the tracker server.*/
enum TrackerEvent {NONE = 0, COMPLETED, STARTED, STOPPED};

/* Abstract base class to define an object that communicates with a torrent tracker server.
   The protocol used is ambiguous. */
class TorrentTrackerComm {
	
	public:
		//~Constructors/Destructors----------------------
		/* Constructor taking the tracker address, port number of the server, and file hash. */
		TorrentTrackerComm(const std::string tracker, 
							const uint16_t newPortNumber, 
							const uint8_t newFileHash[20],
							const uint16_t myNewPortNumber);

		/* Constructor taking the tracker address, port number of the server, 
		   file hash, and a time limit to timeout. */
		TorrentTrackerComm(const std::string tracker, 
							const uint16_t newPortNumber, 
							const uint8_t newFileHash[20],
							const uint16_t myNewPortNumber,
							const int newSecondsUntilTimeout);

		/* Destructor. */
		virtual ~TorrentTrackerComm();

		//~Methods---------------------------------------
		/* Opens a connection with a tracker, handles handshake step. 
		   Returns false if an error occurred, true otherwise. */
		virtual const bool initiateConnection() = 0;

		/* Requests peers from the tracker server.
		   Returns a bencoded string ptr that is the response from the tracker.
		   Uses the curEvent for the event parameter.
		   		Returns NULL if the tracker did not respond. */
		virtual const std::vector<Peer * > * requestPeers(const uint64_t amountUploaded, 
															const uint64_t amountDownloaded, 
															const uint64_t amountLeft) = 0;

		/* Requests peers from the tracker server.
		   Returns a bencoded string ptr that is the response from the tracker.
		   		Returns NULL if the tracker did not respond. */
		virtual const std::vector<Peer * > * requestPeers(const uint64_t amountUploaded, 
															const uint64_t amountDownloaded, 
															const uint64_t amountLeft, 
															const TrackerEvent event) = 0;
		
		/* Getter for the requestInterval time which indicates how long to wait before
		   requesting new data from this tracker.
		   The requestInterval field is in seconds. */
		const uint32_t getRequestInterval() const;

		/* Getter for the timeOfLastResponse field which indicates when the last response
			was received from the tracker server.
			*/
		const clock_t getTimeOfLastResponse() const;

		/* Prints out all fields in this object! */
		const void printTorrentTrackerComm() const;

		//~Data Fields------------------------------------------
		/* The hostname of the tracker server. */
		const std::string * trackerHostname;

		/* TrackerEvent which was the last event and de facto current event for this TrackerComm object. */
		TrackerEvent curEvent;

		bool operator== (TorrentTrackerComm & t);

		std::string getTrackerAddress();

		uint16_t getServerPortNumber();

	protected:
		//~Data Fields-----------------------------------
		/* If no 'newSecondsUntilTimeout' is provided to the class then it defaults to this value. */
		static const int DEFAULT_SECONDS_UNTIL_TIMEOUT = 5;

		/* Holds the system time that a request from this method was made. */
		clock_t timeRequestSent;

		/* Holds the time that the last response was received. Used to ensure querying
			doesn't occur faster than interval seconds. */
		clock_t timeOfLastResponse;

		/* Value indicating how long to wait before considering a connection timed out.*/
		int SECONDS_UNTIL_TIMEOUT;

		/* The IP address of the tracker to connect to. */
		std::string * trackerAddress;

		/* The port number to connect to the tracker on. The server's port number. */
		uint16_t serverPortNumber;

		/* The port number to use on my end to connect to the server. My port number. */
		uint16_t myPortNumber;

		/* The socket between the tracker server and this connection that is currently active.
		   This is equal to -1 when there is not active socket. */
		int activeSocket;

		/* 20 byte SHA-1 hash of the file that peers are needed for. */
		uint8_t fileHash[20];

		/* The peer id for the current connection. */
		uint32_t * transactionId;

		/* The ID received from the tracker server used to identify this connection. */
		uint64_t connectionId;

		/* ID identifying the peer, specifically, the peer client. Seems like TD is appropriate, and it isn't taken! */
		uint32_t peerId;

		/* Value obtained from tracker server indicating how long to wait before querying it again. */
		uint32_t requestInterval;

		/* The server address that this is communicating with. */
		struct sockaddr_in serverAddress;

		/* The address assigned to this client. */
		struct sockaddr_in clientAddress;
		
		//~Methods---------------------------------------
		/* Takes a string a checks if the string is a valid IPv4 address.
		   Returns true if the string is a valid IPv4 address, false otherwise. */
		const bool isIp4Address(const std::string theString) const;

		/* Takes a string a checks if the string is a valid IPv6 address.
		   Returns true if the string is a valid IPv6 address, false otherwise. */
		const bool isIp6Address(const std::string theString) const;

		/* Generates a random 20 character string that is to be used as 
		   the peer id for a connection with a tracker server. 
		   Saves the new transactionId into the transactionId field. */
		void generateTransactionId();

		/* Returns true if the connection timed out, false otherwise. */
		const bool isTimedOut() const;

		/* Checks to see if the requestInterval has been passed. That is, if its alright to
		    request more data from the tracker. */
		const bool isIntervalPassed() const;

		/* Takes the amountUploaded, amountDownloaded, and amountLeft for a file
		   and creates a tracker request string from it. 
		   Returns this as a std::string */
		std::string * createTrackerRequest(const int amountUploaded, 
											const int amountDownloaded,
											const int amountLeft) const;

		/* Creates a ConnectionIdRequest object and fills its fields. */
		ConnectionIdRequest * createConnectionIdRequest();

		/* Takes a pointer to a PeerResponse struct and prints its fields out. */
		void printPeerResponse(const PeerResponse * response);

		/* Takes a pointer to the ConnectionIdResponse struct and prints its fields out. */
		void printConnectionIdResponse(const ConnectionIdResponse * response);

		/* Takes a pointer to some memory that you think might represent a PeerResponse Error.
		   Prints out the memory, if it is a PeerResponseError, then the output should
		   be meaningful. */
		void printPeerResponseError(const void * response);

		/* Takes a pointer to a PeerResponse struct and parses the peers returned
		   into peer objects, which are placed into a vector and returned. */
		std::vector<Peer * > * parseAnnounceResponse(const PeerResponse * response);
};

#endif