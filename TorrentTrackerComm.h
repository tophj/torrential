#ifndef TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM
#define TORRENTIAL_DOWNLOADS_TORRENT_TRACKER_COMM

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

/* Struct used to send a request for peers to the tracker server. */
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

/* Struct that wraps up a 32-bit ip address and a 16-bit port number. */
struct PeerAddress_t {

	uint32_t ipAddress;
	uint16_t tcpPort;
} typedef PeerAddress;

/* Struct used in receipt of a request for peers from the tracker server. */
struct PeerResponse_t {

	uint32_t action;
	uint32_t transactionId;
	uint32_t interval;
	uint32_t leechers;
	uint32_t seeders;
	PeerAddress * peers;
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
		/* Constructor taking the tracker address, port number, and file hash. */
		TorrentTrackerComm(const std::string tracker, 
							const uint16_t newPortNumber, 
							const std::string newFileHash);

		/* Constructor taking the tracker address, port number, 
		   file hash, and a time limit to timeout. */
		TorrentTrackerComm(const std::string tracker, 
							const uint16_t newPortNumber, 
							const std::string newFileHash,
							const int newSecondsUntilTimeout);

		/* Destructor. */
		virtual ~TorrentTrackerComm();

		//~Methods---------------------------------------
		/* Opens a connection with a tracker, handles handshake step. 
		   Returns false if an error occurred, true otherwise. */
		virtual const bool initiateConnection() = 0;

		/* Requests peers from the tracker server.
		   Returns a bencoded string ptr that is the response from the tracker.
		   		Returns NULL if the tracker did not respond. */
		virtual const std::string * requestPeers(const uint64_t amountUploaded, 
										const uint64_t amountDownloaded, 
										const uint64_t amountLeft, 
										const TrackerEvent event) = 0;

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
		uint16_t portNumber;

		/* The socket between the tracker server and this connection that is currently active.
		   This is equal to -1 when there is not active socket. */
		int activeSocket;

		/* 20 byte SHA-1 hash of the file that peers are needed for. */
		std::string fileHash;

		/* The peer id for the current connection. */
		uint32_t * transactionId;

		/* The ID received from the tracker server used to identify this connection. */
		uint64_t connectionId;

		/* ID identifying the peer, specifically, the peer client. Seems like TD is appropriate, and it isn't taken! */
		uint32_t peerId;

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
};

#endif