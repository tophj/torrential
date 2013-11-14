// Basically, given a user look at the tracker to get a list of peers.
// Then, iterate through that list of peers and look to see if they have the 
// specific piece that tbe user needs.
// If they do, then start a new thread that connects / handshakes through FTP to
// download that piece. 
// Keep looking at the pieces that peer has available until you either hit the limit of 
// pieces you can download simultaneously, or hit the download limit.

//Initiates handshaking
//Requests look like <pstr len><pstr><reserved 8bits><info_hash><peer_id>

#include "peerwire.h"

uint8_t * convert(const char * str){

    uint8_t * bytes = (uint8_t *) malloc(20);
    int i=0;
    while(i<SHA_DIGEST_LENGTH){
        //printf("%x\n", str[i]& 0xff);
        bytes[i] = str[i] & 0xff;
        //printf("%x\n", bytes[i]);
        i++;
     }
     return bytes;
}

//For testing because I'm lazy
int main(int argc, char** argv){


    const char temp[41] = "8C3760CB651C863861FA9ABE2EF70246943C1994";
    uint8_t * info_hash;
    info_hash  = convert(temp);
    
    thread_pool pool;
    
    PeerList newPeerList;
    std::vector<std::string> hashpieces;

    TorrentPeerwireProtocol peerwire = TorrentPeerwireProtocol(info_hash, &pool, newPeerList, hashpieces);

    peerwire.handshake();

    return 0;
}



//using namespace libtorrent;
//hashpieces are all the hash files this instantiation of peerwire protocol is reuqired to download
TorrentPeerwireProtocol::TorrentPeerwireProtocol(uint8_t * info_hash, struct thread_pool *pool,
                                                  PeerList & pList, std::vector<std::string> & hashpieces){

    printf("Launching Peerwire...\n");
    printf("Searching through peers...\n");

    infoHash = info_hash;

    uint8_t id[] = {20,10,20,20,02,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
    BIT_TORRENT_ID = std::string("BitTorrent protocol").c_str();
std::cout << "length of BIT_TORRENT_ID == ||" << strlen(BIT_TORRENT_ID) << "||\n";

    peerId = id;

    connectToPeer(info_hash, peerId, "", 4, hashpieces);
}

void TorrentPeerwireProtocol::connectToPeer(uint8_t* info_hash,
                                            uint8_t* peer_id, 
                                            const std::string host,
                                            int port,
                                            std::vector<std::string> hashpieces) {

    #define HOST "24.85.80.212"
    #define PORT 46928
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    struct sockaddr * saddr;
    struct addrinfo hints, * ai,  * it;
    char strportnum[25];

    memset(&hints, '\0', sizeof(hints));
    hints.ai_flags = AI_ADDRCONFIG;
    hints.ai_socktype = SOCK_STREAM;

    snprintf(strportnum, 10, "%d", PORT);

    GetAddrInfo(HOST, strportnum, &hints, &ai);

    for (it = ai; it != NULL; it = it->ai_next) {

        std::cout << "looping over results of Getaddrinfo,.....\n";
        std::cout << "SOCKET-ING!\n";
        if ((sockFd = Socket(it->ai_family, it->ai_socktype, it->ai_protocol)) != -1) {
            std::cout << "SOCKETED!\n";

            saddr = ai->ai_addr;
            saddr->sa_family = AF_INET;

            //////////////////////////////////////////////////////////////////////////////////
            //////////////////////this shizzzzzzzzzzzzzz................/////////////////////
            //////////////////////////////////////////////////////////////////////////////////
            int res; 
            //struct sockaddr_in addr; 
            long arg; 
            fd_set myset; 
            struct timeval tv; 
            int valopt; 
            socklen_t lon; 

            // Set non-blocking 
            if( (arg = fcntl(sockFd, F_GETFL, NULL)) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno)); 
                exit(0); 
            } 
            arg |= O_NONBLOCK; 
            if( fcntl(sockFd, F_SETFL, arg) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno)); 
                exit(0); 
            } 

            // Trying to connect with timeout 
            res = Connect(sockFd, saddr, sizeof(*saddr)); 
            if (res < 0) { 

                if (errno == EINPROGRESS) { 
                    
                    fprintf(stderr, "EINPROGRESS in connect() - selecting\n"); 
                    
                    do { 

                        //Set timeouts
                        tv.tv_sec = 15; 
                        tv.tv_usec = 0; 
                        
                        FD_ZERO(&myset); 
                        FD_SET(sockFd, &myset); 
            
                        res = Select(sockFd + 1, NULL, &myset, NULL, &tv); 

                        if (res < 0 && errno != EINTR) { 
                            fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); 
                            exit(0); 
                        } 
                        else if (res > 0) { 
                
                            // Socket selected for write 
                            lon = sizeof(int); 
                            if (getsockopt(sockFd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) { 
                                fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno)); 
                                exit(0); 
                            } 
                
                            // Check the value returned... 
                            if (valopt) { 
                                fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt)); 
                                exit(0); 
                            } 
                            break; 
                        } 
                        else { 
                            fprintf(stderr, "Timeout in select() - Cancelling!\n"); 
                            exit(0); 
                        } 
                    } while (1); 
                } 
                else { 
                    fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); 
                    exit(0); 
                } 
            } 
            // Set to blocking mode again... 
            if( (arg = fcntl(sockFd, F_GETFL, NULL)) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno)); 
                exit(0); 
            } 

            arg &= (~O_NONBLOCK); 

            if(fcntl(sockFd, F_SETFL, arg) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno)); 
                exit(0); 
            } 
            // I hope that is all 
            //////////////////////////////////////////////////////////////////////////////////
            //////////////////////this shizzzzzzzzzzzzzz................/////////////////////
            //////////////////////////////////////////////////////////////////////////////////

        }
    }
    std::cout << "I MADE IT\n";
    freeaddrinfo(ai);

    ///////////////////////////////////////////////////////////////////////////////////////////////////


}

void TorrentPeerwireProtocol::handshake() {

    printf("Got in handshake, all the networking is good!##############################################\n");
    const int handshakeSize = 1 + strlen(BIT_TORRENT_ID) + 8 + 20 + 20;
    
    //Create the Handshake to send
    Handshake h;    
    h.pstrLen = (char) strlen(BIT_TORRENT_ID);
    h.pstr = BIT_TORRENT_ID;
    for (int i = 0; i < 20; i++) {
        h.reserved[i] = 0;
        h.infoHash[i] = infoHash[i];
        h.peerId[i] = peerId[i];
    }



    //Print our the handshake being sent
    std::cout << "PRINTING MY DAMNED HANDSHAKE####################################################\n";
    std::cout << "pstrLen == \n";
    std::cout << std::hex << h.pstrLen << std::endl;
    std::cout << "\npstr == \n";
    std::cout << h.pstr << std::endl;
    std::cout << "\nreserved == \n";
    for (int i = 0; i < 20; i++) {
        std::cout << h.reserved[i];
    }
    std::cout << "\ninfoHash == \n";
    for (int i = 0; i < 20; i++) {
        std::cout << infoHash[i];
    }
    std::cout << "\npeerId == \n";
    for (int i = 0; i < 20; i++) {
        std::cout << peerId[i];
    }
    std::cout << "\n##############################################################################\n";



    std::cout << "Sending.............................\n";
    sendMessage(&h, handshakeSize);

    std::cout << "Receiving...........................\n";
    //Receive the handshake in response
    uint8_t buffer[1024];
    if (Recv(sockFd, buffer, sizeof(buffer), 0) == -1) {
        std::cout << "Receive failure....\n";
        exit(1);
    }
    std::cout << "RECEIVED!\n\n";
    printHandshake(buffer);

    if (buffer[0] != 0){
        std::cout << "\nRecieved incorrect handshake\n";
    }


}

void TorrentPeerwireProtocol::printHandshake(const uint8_t * h) const {

    uint8_t pstrLen = h[0];
    std::cout << (char*) h << std::endl << "##############################\n\n\n";

    for (uint8_t i = 1; i < pstrLen + 1; i++) {

        std::cout << h[i] << std::endl;
    }
    std::cout << "----------------------------------------------------\nending print Handshake\n----------------------------------------------------\n";
}


void TorrentPeerwireProtocol::sendMessage(const void * message, uint32_t messageSize) {

    std::cout << "SENDING MESSAGE\n";

    if (Send(sockFd, message, messageSize, 0) < 0) {
        exit(1);
    }
    else{
        std::cout << "SEND SUCCESS!!!!!!!!!!!\n";
    }   
}

// 0 byte payload message, default timeout for connection is two minutes so send this every 90 seconds
// Spawn a new thread on each connection for this
void TorrentPeerwireProtocol::keepAlive(){

    //Construct the message
    uint32_t length = 0;
    length = htonl(length);
    sendMessage(&length, 4);
}

//Choke a peer to prevent them from sending information
//<len=0001><id=0>
void TorrentPeerwireProtocol::choke(){

    //Construct the message
    uint32_t length = 1;
    uint8_t id = 0;
    
    uint8_t message[5];
    message[0] = htonl(length);
    message[4] = id;
    
    sendMessage(message, 5);
}

//Unchoke a peer and startup data xfer
//<len=0001><id=1>
void TorrentPeerwireProtocol::unchoke() {

    //Construct the message
    uint32_t length = 1;
    uint8_t id = 1;
    
    uint8_t message[5];
    message[0] = htonl(length);
    message[4] = id;
    
    sendMessage(message, 5);
}

//Fixed-length and has no payload
//<len=0001><id=2>
void TorrentPeerwireProtocol::interested() {
    //Construct the message
    uint32_t length = 1;
    uint8_t id = 2;
    
    uint8_t message[5];
    message[0] = htonl(length);
    message[4] = id;
    
    sendMessage(message, 5);
}

//Fixed-length and has no payload
//<len=0001><id=3>
void TorrentPeerwireProtocol::nonInterested() {
    //Construct the message
    uint32_t length = 1;
    uint8_t id = 3;
    
    uint8_t message[5];
    message[0] = htonl(length);
    message[4] = id;
    
    sendMessage(message, 5);
}

//<len=0005><id=4><piece_index>
//fixed length, piece_index is the zero-based index of a piece that has just been downloaded and 
//verified via the hash
void TorrentPeerwireProtocol::have(uint32_t pieceIndex){

    uint32_t length = 5;
    uint8_t id = 4;
    uint8_t message[9];
    
    message[0] = htonl(length);
    message[4] = id;
    message[5] = pieceIndex;

    sendMessage(message, 9);
}

//<len=0001+X><id=5><bitfield>
//Only sent after the initial handshake, and before any other message is sent
//X here is the length of the bitfield.
//The payload, the bitfield, represents pieces that have been downloaded successfully, 
//The high bit in the first byte represents piece index 0. 
void TorrentPeerwireProtocol::bitfield(){

    //uint32_t length = 1;
    //uint8_t id = 5;
    //bitfield

    //sendMessage(message, socket);
    return;
}

// In the for <len0013><id=6><index><begin><length big endian>
// index : int specifying the zero-based piece index
// begin : int specifying the zero-based byte offset within the piece
// length : int specifying the requested length
void TorrentPeerwireProtocol::request(uint32_t index, uint32_t begin, uint32_t requestedLength) {

    uint32_t length = 13;
    uint8_t id = 6;
    uint8_t message[17];
    
    message[0] = htonl(length);
    message[4] = id;
    message[5] = index;
    message[9] = begin;
    message[13] = requestedLength;

    sendMessage(message, 17);
}

//<len=0009+X><id=7><index><begin><block>
//X here is the length of the block of the piece
void TorrentPeerwireProtocol::piece(uint32_t index, uint32_t begin, uint8_t block, uint32_t blockLength) {
  
    uint32_t length = 13;
    uint8_t id = 7;
    uint8_t * message = new uint8_t[4 + 1 + 4 + 4 + blockLength];

    sendMessage(message, 20);

    return;
}

//<len=0013><id=8><index><begin><length>
void TorrentPeerwireProtocol::cancel(uint32_t index, uint32_t begin, uint32_t requestedLength) {

    uint32_t length = 13;
    uint8_t id = 8;
    uint8_t message[17];
  
  	message[0] = htonl(length);
    message[4] = id;
    message[5] = index;
    message[9] = begin;
    message[13] = requestedLength;

    sendMessage(message, 17);

    return;
}

