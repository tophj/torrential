// Basically, given a user look at the tracker to get a list of peers.
// Then, iterate through that list of peers and look to see if they have the 
// specific piece that tbe user needs.
// If they do, then start a new thread that connects / handshakes through FTP to
// download that piece. 
// Keep looking at the pieces that peer has available until you either hit the limit of 
// pieces you can download simultaneously, or hit the download limit.

//Initiates handshaking
//Requests look like <pstr len><pstr><reserved 8bits><info_hash><peer_id>

#include "Peerwire.h"
//File used for loading and saving
FILE * torrentialSaveFile;
pthread_mutex_t uploadMutex=PTHREAD_MUTEX_INITIALIZER;

//For testing
int main(int argc, char** argv){

    //const char temp[41] = "8C3760CB651C863861FA9ABE2EF70246943C1994";
    uint8_t info_hash[] = {0xdf, 0x79, 0xd5, 0xef, 0xc6, 0x83, 0x4c, 0xfb, 0x31, 0x21, 0x8d, 0xb8, 0x3d, 0x6f, 0xf1, 0xc5, 0x5a, 0xd8, 0x17, 0x9d};
    //info_hash  = convert(temp);
    
    thread_pool pool;
    iptool itool;
    PeerList newPeerList;
    std::vector<std::string> pList;
   TorrentPeerwireProtocol peerwire = TorrentPeerwireProtocol(0, &itool, NULL, &pool, newPeerList, pList);

    uint8_t id[] = {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
    
    Peer p("213.112.225.102", 6985);    
    if (SUCCESS == peerwire.connect(&*info_hash, &*id, p)) {
        std::cout << "SUCCESS ACHIEVED!\n";
        if (peerwire.handshake(info_hash, &*id, p, tcpSendMessage, tcpRecvMessage)) {

            std::cout << "HANDSHAKE RECEIVED!!\n";

            std::cout << "\n\nReceiving Bitfield!\n";
            uint8_t buffer[1024];
            int receivedBytes = tcpRecvMessage(buffer, sizeof(buffer), p);
            receivedBytes += tcpRecvMessage(buffer + 5, sizeof(buffer) - 5, p);

            uint32_t * lengthPointer = (uint32_t *) buffer;
            uint32_t length = ntohl(*lengthPointer);

            if (buffer[4] == 5) {
             
                peerwire.parseBitfield(buffer, length, p, 0);
            }
            std::cout << "Received bitfield!\n";
            
            std::cout << "Expressing my interest ;)\n";
            peerwire.interested(p, tcpSendMessage);
            std::cout << "Interest expressed!!!\n\n";

            std::cout << "Unchoking....we will show mercy...\n";
            peerwire.unchoke(p, tcpSendMessage);
            std::cout << "UNCHOKED! HE WILL LIVE!\n\n";

            std::cout << "Requesting some random piece....\n";
            peerwire.request(0, 0, 100, p, tcpSendMessage);
            std::cout << "Requested a piece!\n\n";

            Recieve_t recv(p);
            recv.pieceL = 100;

            peerwire.recieve(&recv);
        }
    }

    return 0;
}

/*
 * getIPaddresses(){

}
*/

uint8_t * convert(const char * str){

    uint8_t * bytes = (uint8_t *) malloc(20);
    int i=0;
    while(i<SHA_DIGEST_LENGTH){
        //printf("%x\n", str[i]& 0xff);
        bytes[i] = str[i] & 0xff;
        i++;
     }
     return bytes;
}

void * peerReceive (void * peer) {

    return NULL;
}

void * peerSend(void * peer) {

    Peer * p = (Peer *) peer;

    //While fileNotDone
    while (true) {

        //~Request stuff--------------------------------------------
        std::unordered_set<Piece, PieceHash> pieces = p->getPieces();
        if (pieces.size() != 0) {

            for (std::unordered_set<Piece, PieceHash>::iterator it = pieces.begin(); it != pieces.end(); ++it) {
                
                //Request piece
            }
        }
        else {

            //keep alive
        }

        //~Receive stuff--------------------------------------------

    }

    return NULL;
}

void tcpSendMessage(const void * message, uint32_t messageSize, const Peer & p) {

    if (Send(p.sockFd, message, messageSize, 0) < 0) {
        
        std::cout << "An error occurred in send message..........$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\n";
    }
}

int tcpRecvMessage(void * message, uint32_t messageSize, const Peer & p) {

    int res; 
    fd_set myset; 
    struct timeval tv; 
    int receivedBytes = -1;

    if ((receivedBytes = Recv(p.sockFd, message, messageSize, 0)) == -1) {

        if (errno == EAGAIN) { 
                        
            fprintf(stderr, "EAGAIN in Recv() - selecting\n"); 
            
            do { 

                if ((receivedBytes = Recv(p.sockFd, message, messageSize, 0)) > -1) {

                    std::cout << "Receive succeeded!\n\n";
                    return receivedBytes;
                }

                //Set timeouts
                tv.tv_sec = 30; 
                tv.tv_usec = 0; 
                
                FD_ZERO(&myset); 
                FD_SET(p.sockFd, &myset); 

                res = Select(p.sockFd + 1, &myset, NULL, NULL, &tv); 

                if (errno == EAGAIN) {

                    std::cout << "Waiting............for john.....\n";
                }
                else if (res < 0 && errno != EINTR) { 

                    fprintf(stderr, "Error receiving %d - %s\n", errno, strerror(errno)); 
                } 
                else { 

                    fprintf(stderr, "Timeout in select() - Cancelling!\n ERRNO == %s\n\n", strerror(errno)); 
                    return -1; 
                } 

            } while (1); 
        } 
        else { 

            fprintf(stderr, "Error connecting %d - %s\n\n", errno, strerror(errno)); 
        }
    }
    else {

        std::cout << "RECEIVE SUCCESS!!!!!!!!!!!!\n\n";
        return receivedBytes;
    }

    return -1;
}

//using namespace libtorrent;
//hashpieces are all the hash files this instantiation of peerwire protocol is reuqired to download
TorrentPeerwireProtocol::TorrentPeerwireProtocol(int pieceLen, iptool * theTool, 
                                                    char* hash, struct thread_pool* thePool, 
                                                    PeerList newPeerList, std::vector<std::string> pList) {

    BIT_TORRENT_ID = std::string("BitTorrent protocol").c_str();
    pool = thePool;
    tool = theTool;
}

void TorrentPeerwireProtocol::download(uint8_t * infoHash, PeerList & pList, 
                                        std::vector<Piece> & hashPieces,
                                        int pieceLen) {


    Recieve_t * sendRecieve = (Recieve_t *) malloc(sizeof(Recieve_t));
    sendRecieve->pieceL = pieceLen;

    while (true) {

        std::vector<Peer> * peers = pList.getPeers();

        for (std::vector<Peer>::iterator it = peers->begin(); it != peers->end(); ++it) {
            
            if ((*it).sockFd == -1) {

                sendRecieve->currentPeer = *it;
                //connect
                //handshake
            }

            //thread_pool_submit(pool, peerSend, NULL);
            //thread_pool_submit(pool, peerReceive, NULL);
        }
    }
}
/*
void TorrentPeerwireProtocol::download(uint8_t * infoHash, PeerList & pList, 
                                        std::vector<Piece> & hashPieces,
                                        int pieceLen) {

    while (true) {

        std::vector<Peer> * peers = pList.getPeers();
        std::vector<Peer>::iterator peerIt = peers->begin();

        for (; peerIt != peers->end(); peerIt++) {

            //If first Connection
            if (peerIt->sockFd == -1) {


                uint8_t id[] = {20,10,20,20,02,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};

                switch (connect(infoHash, &*id, *peerIt)) {

                    case SUCCESS:
                        //Handshake with peer
                        if (handshake(infoHash, &*id, *peerIt, tcpSendMessage, tcpRecvMessage)) {

                            //getBitfield();
                            //on success continue to download as many pieces as possible
                            break;
                        }
                        else {

                            pList.removePeer(*peerIt); 
                            continue;    
                        }
                    case TIMEOUT:
                        pList.removePeer(*peerIt);    
                        continue;
                    case FAIL:
                        pList.removePeer(*peerIt);
                        continue;
                    default:
                        break;
                }
            }

            //request pieces
            std::unordered_set<Piece, PieceHash> pieces = (*peerIt).getPieces();
            if (!pieces.empty()) {

                //iterate over all pieces peer has`
                std::unordered_set<Piece, PieceHash>::iterator pieceIt = pieces.begin();
                for (; pieceIt != pieces.end(); pieceIt++) {

                    request((*pieceIt).getPieceIndex(), 0, (*pieceIt).getPieceSize(), *peerIt, tcpSendMessage);

                    //uint8_t buffer[50];
                    //tcpRecvMessage(buffer, 50, *peerIt);
                    //after message gotten, spin off thread to finish
                    //do something
                    downloadPiece(NULL);
                }
            }            
        }
    }
}
*/
ConnectStatus TorrentPeerwireProtocol::connect(uint8_t * infoHash,
                                                uint8_t * peerId, 
                                                Peer & p) {
    
    struct sockaddr * saddr;
    struct addrinfo hints, * ai,  * it;
    char strportnum[25];

    memset(&hints, '\0', sizeof(hints));
    hints.ai_flags = AI_ADDRCONFIG;
    hints.ai_socktype = SOCK_STREAM;

    snprintf(strportnum, 10, "%d", p.getPortNumber());

    GetAddrInfo(p.getIp().c_str(), strportnum, &hints, &ai);

    for (it = ai; it != NULL; it = it->ai_next) {

        if ((p.sockFd = Socket(it->ai_family, it->ai_socktype, it->ai_protocol)) != -1) {

            saddr = ai->ai_addr;
            saddr->sa_family = AF_INET;

            //////////////////////////////////////////////////////////////////////////////////
            //////////////////////this shizzzzzzzzzzzzzz................/////////////////////
            //////////////////////////////////////////////////////////////////////////////////
            int res; 
            long arg; 
            fd_set myset; 
            struct timeval tv; 
            int valopt; 
            socklen_t lon; 

            //Uncomment to get dual connections working!!!
            /*            
            struct sockaddr_in clientAddress;
            clientAddress.sin_family = AF_INET;
            //std::string tempString("172.31.162.103");
            clientAddress.sin_addr.s_addr = inet_pton(AF_INET, tool->getBest().c_str(), &(clientAddress.sin_addr));
            clientAddress.sin_port = htons(p.getPortNumber());
            Bind(p.sockFd, (struct sockaddr *) &clientAddress, sizeof(clientAddress));
            std::cout << "yep, it's been bound....\n";
            */

            // Set non-blocking 
            if( (arg = fcntl(p.sockFd, F_GETFL, NULL)) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno)); 
                return FAIL;
            }
            arg |= O_NONBLOCK; 
            if( fcntl(p.sockFd, F_SETFL, arg) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno)); 
                return FAIL;
            } 

            // Trying to connect with timeout 
            res = Connect(p.sockFd, saddr, sizeof(*saddr)); 
            if (res < 0) { 

                if (errno == EINPROGRESS) { 
                    
                    fprintf(stderr, "EINPROGRESS in connect() - selecting\n"); 
                    
                    do { 

                        //Set timeouts
                        tv.tv_sec = 15; 
                        tv.tv_usec = 0; 
                        
                        FD_ZERO(&myset); 
                        FD_SET(p.sockFd, &myset); 
            
                        res = Select(p.sockFd + 1, NULL, &myset, NULL, &tv); 

                        if (res < 0 && errno != EINTR) { 
                            fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); 
                        } 
                        else if (res > 0) { 
                
                            // Socket selected for write 
                            lon = sizeof(int); 
                            if (getsockopt(p.sockFd, SOL_SOCKET, SO_ERROR, (void *) &valopt, &lon) < 0) { 
                                fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno)); 
                            } 
                
                            // Check the value returned... 
                            if (valopt) { 
                                fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt)); 
                            } 

                            return SUCCESS;
                        } 
                        else { 
                            fprintf(stderr, "Timeout in select() - Cancelling!\n"); 
                            return TIMEOUT; 
                        } 
                    } while (1); 
                } 
                else { 
                    fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); 
                } 
            } 

            // Set to blocking mode again... 
            if( (arg = fcntl(p.sockFd, F_GETFL, NULL)) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno)); 
            } 

            arg &= (~O_NONBLOCK); 

            if(fcntl(p.sockFd, F_SETFL, arg) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno)); 
            } 
            // I hope that is all 
            //////////////////////////////////////////////////////////////////////////////////
            //////////////////////this shizzzzzzzzzzzzzz................/////////////////////
            //////////////////////////////////////////////////////////////////////////////////

        }
    }

    freeaddrinfo(ai);
    return FAIL;
}
bool TorrentPeerwireProtocol::listenForThem(int len){

    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    GetAddrInfo(NULL, "50200", &hints, &res);

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    Bind(sockfd, res->ai_addr, res->ai_addrlen);
    //HARD
    listen(sockfd, 40);

    addr_size = sizeof their_addr;
    while(1){

        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
        struct sockaddr_in address;
        uint32_t addrSize = sizeof(address);
        if(getpeername(new_fd, (struct sockaddr*)&address, &addrSize)!=0){
            printf("ERROR ON LISTEN\n");
        }

        char convDest[1024];
        inet_ntop(AF_INET, &(res->ai_addr), convDest, INET_ADDRSTRLEN);
        std::string theIp(convDest);
        Peer p = Peer(theIp, address.sin_port);

        Recieve_t * sendRecieve = (Recieve_t *) malloc(sizeof(Recieve_t));
        sendRecieve->pieceL = len;
        sendRecieve->currentPeer = p;

        uint8_t buffer[68];
        int bytesGotten = -1;
        if ((bytesGotten = tcpRecvMessage(buffer, sizeof(buffer), p))) {
            
        }
        else {

            std::cout << "Unhandled error in recvMessage.............\n";
        }

        if (buffer[0] != 19){

            std::cout << "\nRecieved incorrect handshake\n";
            return false;
        }
        uint32_t i;
        //Create the Handshake to send
        Handshake h;    
        h.pstrLen = 19;
        for (i = 1; i < 20; i++) {

            h.pstr[i] = BIT_TORRENT_ID[i];
        }
        for (; i < 28; i++) {
            h.reserved[i] = buffer[i];
        }
        for (; i < 48; i++) {
            h.infoHash[i] = buffer[i];
        }
        for (; i < 68; i++) {
             h.peerId[i] = 20;
        }
               
        //Send the handshake to the peer
        tcpSendMessage(&h, sizeof(h), p);
    }
}

bool TorrentPeerwireProtocol::handshake(uint8_t * infoHash,
                                        uint8_t * peerId, 
                                        const Peer & p, 
                                        SendMessage sendMessage,
                                        RecvMessage recvMessage) {

    const uint32_t handshakeSize = 1 + strlen(BIT_TORRENT_ID) + 8 + 20 + 20;
    
    //Create the Handshake to send
    Handshake h;    
    h.pstrLen = 19;
    for (uint32_t i = 0; i < 19; i++) {

        h.pstr[i] = BIT_TORRENT_ID[i];
    }
    for (uint32_t i = 0; i < 8; i++) {
        h.reserved[i] = 0;
    }
    for (uint32_t i = 0; i < 20; i++) {
        h.infoHash[i] = infoHash[i];
        h.peerId[i] = peerId[i];
    }

    //Send the handshake to the peer
    sendMessage(&h, handshakeSize, p);

    //Receive the handshake in response
    uint8_t buffer[68];
    int bytesGotten = -1;
    if ((bytesGotten = recvMessage(buffer, sizeof(buffer), p))) {
        
    }
    else {

        std::cout << "Unhandled error in recvMessage.............\n";
    }

    if (buffer[0] != 19){

        std::cout << "\nRecieved incorrect handshake\n";
        return false;
    }

    return true;
}

void TorrentPeerwireProtocol::printHandshake(const uint8_t * h) const {

    uint8_t pstrLen = h[0];
    std::cout << std::hex << "pstrLen == ||" << pstrLen << "||\n";

    std::cout << "\n######################\n" << (char*) h << std::endl << "##############################\n";

    for (uint8_t i = 0; i < pstrLen + 1; i++) {

        std::cout << h[i];
    }
    std::cout << "\n----------------------------------------------------\nending print Handshake\n----------------------------------------------------\n";
}

// 0 byte payload message, default timeout for connection is two minutes so send this every 90 seconds
// Spawn a new thread on each connection for this
void TorrentPeerwireProtocol::keepAlive(const Peer & p, SendMessage sendMessage) {

    //Construct the message
    uint32_t length = 0;
    sendMessage(&length, 4, p);
}

//Choke a peer to prevent them from sending information
//<len=0001><id=0>
void TorrentPeerwireProtocol::choke(const Peer & p, SendMessage sendMessage){

    //Construct the message
    uint8_t id = 0;
    
    uint8_t message[5];

    message[0] = 0x0;
    message[1] = 0x0;
    message[2] = 0x0;
    message[3] = 0x1;
    message[4] = id;
    
    sendMessage(message, 5, p);
}

//Unchoke a peer and startup data xfer
//<len=0001><id=1>
void TorrentPeerwireProtocol::unchoke(const Peer & p, SendMessage sendMessage) {

    //Construct the message
    uint8_t id = 1;
    
    uint8_t message[5];

    message[0] = 0x0;
    message[1] = 0x0;
    message[2] = 0x0;
    message[3] = 0x1;
    
    message[4] = id;
    
    sendMessage(message, 5, p);
}

//Fixed-length and has no payload
//<len=0001><id=2>
void TorrentPeerwireProtocol::interested(const Peer & p, SendMessage sendMessage) {
    //Construct the message
    uint32_t length = 1;
    uint8_t id = 2;
    
    uint8_t message[5];

    message[0] = 0x0;
    message[1] = 0x0;
    message[2] = 0x0;
    message[3] = 0x1;

    message[4] = id;
    
    sendMessage(message, 5, p);
}

//Fixed-length and has no payload
//<len=0001><id=3>
void TorrentPeerwireProtocol::notInterested(const Peer & p, SendMessage sendMessage) {
    //Construct the message
    uint32_t length = 1;
    uint8_t id = 3;
    
    uint8_t message[5];
    (uint32_t&)*message = htonl(length);
    message[4] = id;
    
    sendMessage(message, 5, p);
}

//<len=0005><id=4><piece_index>
//fixed length, piece_index is the zero-based index of a piece that has just been downloaded and 
//verified via the hash
void TorrentPeerwireProtocol::have(uint32_t pieceIndex, 
                                    const Peer & p, SendMessage sendMessage){

    uint32_t length = 5;
    uint8_t id = 4;
    uint8_t message[9];
    
    (uint32_t&)*message = htonl(length);
    message[4] = id;
    (uint32_t&) message[5] = htonl(pieceIndex);

    sendMessage(message, 9, p);
}

//<len=0001+X><id=5><bitfield>
//Only sent after the initial handshake, and before any other message is sent
//X here is the length of the bitfield.
//The payload, the bitfield, represents pieces that have been downloaded successfully, 
//The high bit in the first byte represents piece index 0. 
void TorrentPeerwireProtocol::bitfield(const Peer & p, SendMessage sendMessage){

    //uint32_t length = 1;
    //uint8_t id = 5;
    //bitfield

    //sendMessage(message, socket, p);
    return;
}

std::string * TorrentPeerwireProtocol::parseByte(uint8_t byte) {

    std::string * bits = new std::string("");
    bits->append((byte & 1) ? "1" : "0");
    bits->append((byte & 2) ? "1" : "0");
    bits->append((byte & 4) ? "1" : "0");
    bits->append((byte & 8) ? "1" : "0");
    bits->append((byte & 16) ? "1" : "0");
    bits->append((byte & 32) ? "1" : "0");
    bits->append((byte & 64) ? "1" : "0");
    bits->append((byte & 128) ? "1" : "0");

    return bits;
}

void TorrentPeerwireProtocol::parseBitfield(uint8_t * buffer, uint32_t length, Peer & p, uint32_t pieceLen) {

    uint8_t * payload = &(buffer[5]);
    for (uint32_t i = 0; i < length - 1; i++) {

        std::string * pieces = parseByte(payload[i]);
        for (uint32_t j = 0; j < pieces->size(); ++j) {
            
            if ((*pieces)[j]) {

                Piece * newPiece = new Piece(i + j * 8, pieceLen);
                p.addPiece(*newPiece);
            }
        }

        delete pieces;
    }
}

// In the for <len0013><id=6><index><begin><length big endian>
// index : int specifying the zero-based piece index
// begin : int specifying the zero-based byte offset within the piece
// length : int specifying the requested length
void TorrentPeerwireProtocol::request(uint32_t index, uint32_t begin, uint32_t requestedLength, 
                                        const Peer & p, SendMessage sendMessage) {

    uint32_t length = 13;
    uint8_t id = 6;
    uint8_t message[17];

    (uint32_t&) *message = htonl(length);
    message[4] = id;
    (uint32_t&) message[5] = htonl(index);
    (uint32_t&) message[9] = htonl(begin);
    (uint32_t&) message[13] = htonl(requestedLength);

    sendMessage(message, 17, p);
}

//<len=0009+X><id=7><index><begin><block>
//X here is the length of the block of the piece
void TorrentPeerwireProtocol::piece(uint32_t index, uint32_t begin, 
                                    uint8_t * block, uint32_t blockLength, 
                                    const Peer & p, SendMessage sendMessage) {
  
    uint32_t length = 1 + 4 + 4 + blockLength;
    uint8_t id = 7;
    uint8_t * message = new uint8_t[4 + length];
    (uint32_t&)*message = htonl(length);
    message[5] = id;
    (uint32_t&) message[6] = htonl(index);
    (uint32_t&) message[10] = htonl(begin);
    for (uint32_t i = 0; i < blockLength; i++) {

        message[14 + i] = block[i];    
    }
    
    sendMessage(message, length + 4, p);

    return;
}

//<len=0013><id=8><index><begin><length>
void TorrentPeerwireProtocol::cancel(uint32_t index, uint32_t begin, uint32_t requestedLength, 
                                        const Peer & p, SendMessage sendMessage) {

    uint32_t length = 13;
    uint8_t id = 8;
    uint8_t message[17];
  
  	(uint32_t&)*message = htonl(length);
    message[4] = id;
    (uint32_t&) message[5] = htonl(index);
    (uint32_t&) message[9] = htonl(begin);
    (uint32_t&) message[13] = htonl(requestedLength);

    sendMessage(message, 17, p);

    return;
}


void * TorrentPeerwireProtocol::recieve(void * recievePeer){


    const char * save = "~/Desktop/torrentialSaveFile";
    // Open up the file to read and write
    if((torrentialSaveFile = fopen(save, "wr")) == NULL){
        perror("save_piece: fopen failed :(");
    }

    while(1){

        Peer & currentPeer = ((Recieve_t *)recievePeer)->currentPeer;
        int pieceLen = ((Recieve_t *)recievePeer)->pieceL;

        uint8_t buffer[1024];
        int numBytes = tcpRecvMessage(buffer, sizeof(buffer), currentPeer);

        uint8_t id = 0;
        if(buffer[0] + buffer[1] + buffer[2] + buffer[3] != 0){
            id = buffer[4];
        }
        else{
            id = 11;
        }

        std::cout << numBytes << " \n||";
        for (int i = 0; i < numBytes; i++) {

            printf("%x\n", buffer[i]);
        }
        std::cout << "||\n";


        
        
std::cout << "id == " << id << "\n";
    
        switch(id){
            case 0: // choke
            {
                printf("Recieved a choke message :( choking in response\n");
                currentPeer.peerChoking = true;
                choke(currentPeer,tcpSendMessage);
                currentPeer.amChoking = true;
                break;
            }

            case 1: // unchoke
            {
                printf("Recieved an unchoke message! Unchoking them in response");
                currentPeer.peerChoking = false;
                unchoke(currentPeer,tcpSendMessage);
                currentPeer.amChoking = false;
                break;
            }

            case 2: // interested
            {
                printf("Recieved an interested message! Updating the peer");
                currentPeer.peerInterested = true;
                interested(currentPeer,tcpSendMessage);
                currentPeer.amInterested = true;
                break;
            }

            case 3: // not interested
            {
                printf("Recieved an uninterested message from peer...story of my life. Updating peer");
                currentPeer.peerInterested = false;
                notInterested(currentPeer,tcpSendMessage);
                currentPeer.amInterested = false;
                break;
            }

            case 4: // have
            {
                 //update the peers list to have that piece
                printf("Recieved a have message, should update the peer's hash pieces list");

                uint32_t pieceIndex = buffer[5];


                Piece * piece = new Piece(pieceIndex, pieceLen);
                currentPeer.addPiece(*piece);


                // std::string * pieces = parseByte(piece);
                // for (uint32_t j = 0; j < pieces->size(); ++j) {
            
                //     if ((*pieces)[j]) {

                //         Piece * newPiece = new Piece(j * 8);
                //         currentPeer.addPiece(*newPiece);
                //     }
                // }
            }
           
            break;


            case 5: 


            break;

            case 6: // request
                {
                printf("Recieved a request message");

            // load the piece from the file and send it using piece()

                //Piece_t piece;

                //Get the size of the piece we are going to be recieveing
                uint32_t size = uint32_t(buffer[0]) + uint32_t(buffer[1] << 8)
                         + uint32_t(buffer[2] << 16) + uint32_t(buffer[3] << 24);

                size = size - 9;


                uint32_t index;          
                uint32_t begin;
                uint32_t requestedLength;

                uint32_t newLength;
                uint8_t * block = 0;

                index = uint32_t(buffer[5]) + uint32_t(buffer[6] << 8)
                         + uint32_t(buffer[7] << 16) + uint32_t(buffer[8] << 24);

                begin = uint32_t(buffer[9]) + uint32_t(buffer[10] << 8)
                         + uint32_t(buffer[11] << 16) + uint32_t(buffer[12] << 24);

                requestedLength = uint32_t(buffer[13]) + uint32_t(buffer[14] << 8)
                         + uint32_t(buffer[15] << 16) + uint32_t(buffer[16] << 24);
               
                

                pthread_mutex_lock(&uploadMutex);

                // // Open up the file to read
                // if((torrentialSaveFile = fopen(save, "r")) == NULL){
                //     perror("load_piece: fopen failed :(");
                // } //pices

                if(fseek(torrentialSaveFile, index * pieceLen  + begin * requestedLength, SEEK_SET) < 0){
                    perror("load_piece: fseek failed :(");

                }

                // Extract the piece
                
                fread(block,1,requestedLength,torrentialSaveFile);

                //fclose(torrentialSaveFile);

                pthread_mutex_unlock(&uploadMutex);

                //Create the piece message to send

                newLength = 9+requestedLength;


                //Add the message in a queue


                piece(index,begin,block,newLength,currentPeer,tcpSendMessage);

                }
                
            break;

            case 7: // Got a piece, save it to file
                {
                uint32_t blockIndex = ntohl((uint32_t) buffer[5]);
                uint32_t begin = ntohl((uint32_t) buffer[9]);
                uint8_t * block = (uint8_t *) &buffer[13];

                pthread_mutex_lock(&uploadMutex);

                

                if(fseek(torrentialSaveFile, blockIndex * pieceLen + begin * (sizeof(block)), SEEK_SET) < 0){
                    perror("save_piece: fseek failed :(");

                }

                // Extract the piece
                
                fwrite(block,1,sizeof(block),torrentialSaveFile);

                

                pthread_mutex_unlock(&uploadMutex);

                }
                break;

            case 11: //It's a keep alive. Let's keep alive.

            break;

            // case 8: // cancel

            // //send signal???
            // break;
            default:

                printf("Received some sort of unknown message. Joke's on them, I'm ignoring it.\n");
                printf("#hardtoget\n");
                break;
        }
    }

    fclose(torrentialSaveFile);
    return NULL;
}