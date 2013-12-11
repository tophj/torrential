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
static FILE * torrentialSaveFile;
static pthread_mutex_t uploadMutex = PTHREAD_MUTEX_INITIALIZER;
static bool done = false;
static int totalBytesWritten = 0;
static int fileLength = 0;

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

void * peerSend(void * sendArgsPass) {

    SendArgs * sendArgs = (SendArgs *) sendArgsPass;
    Peer * p = sendArgs->currentPeer;
    uint32_t pieceLen = sendArgs->pieceLen;

    //std::cout<<"ADDED PIECE"<<sendArgs->currentPeer->printPeer();
    std::unordered_set<Piece, PieceHash> pieces;
    //While fileNotDone
    while (!done) {
std::cout << "looping...shit....((((((((((()()(()()()()()()()())()()()()()()()()\n((((((((((()()(()()()()()()()())()()()()()()()()\n((((((((((()()(()()()()()()()())()()()()()()()()\n((((((((((()()(()()()()()()()())()()()()()()()()\n \n";
        //~Request stuff--------------------------------------------
        pieces = p->getPieces();
        if (pieces.size() != 0) {
            
            std::unordered_set<Piece, PieceHash>::iterator it = pieces.begin();
            for (; it != pieces.end(); ++it) {
                
                //split up piece into blocks
                uint32_t blockLength = (pieceLen / 4);
                for (uint32_t i = 0; i < pieceLen; i += blockLength) {
                 
                    //Request piece
                    sendArgs->peerwire.request((*it).getPieceIndex(), 
                        i, 
                        blockLength, 
                        p, 
                        tcpSendMessage);
                }
            }
        }
        else {

            //keep alive
        }

        //~Receive stuff--------------------------------------------
        sleep(1);
    }

    return NULL;
}

void * listenForThem(void * pointerLen) {

    ListenArgs * args = (ListenArgs *) pointerLen;
    int len = args->pieceLen;
    uint32_t i;

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
std::cout << "looping IN LISTEN...shit....NNNNNNNNNNN\n";
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
        struct sockaddr_in address;
        uint32_t addrSize = sizeof(address);
        if(getpeername(new_fd, (struct sockaddr*)&address, &addrSize)!=0){
            printf("ERROR ON LISTEN\n");
        }

        char convDest[1024];
        inet_ntop(AF_INET, &(res->ai_addr), convDest, INET_ADDRSTRLEN);
        std::string theIp(convDest);
        Peer * p = new Peer(theIp, address.sin_port);

        ReceiveArgs * sendReceive = (ReceiveArgs *) malloc(sizeof(ReceiveArgs));
        sendReceive->pieceL = len;
        sendReceive->currentPeer = p;
        //sendReceive->peerwire = *this;

        uint8_t buffer[68];
        int bytesGotten = -1;
        if ((bytesGotten = tcpRecvMessage(buffer, sizeof(buffer), p))) {
            
        }
        else {

            std::cout << "Unhandled error in recvMessage.............\n";
        }

        if (buffer[0] != 19){

            std::cout << "\nReceived incorrect handshake\n";
            continue;
        }

        //Create the Handshake to send
        Handshake h;    
        h.pstrLen = 19;
        for (i = 1; i < 20; i++) {

            h.pstr[i] = args->BIT_TORRENT_ID[i];
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

    return NULL;
}

void tcpSendMessage(const void * message, uint32_t messageSize, const Peer * p) {

    if (Send(p->sockFd, message, messageSize, 0) < 0) {
        
        std::cout << "An error occurred in send message..........$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\n";
    }
}

int tcpRecvMessage(void * message, uint32_t messageSize, const Peer * p) {

    int res; 
    fd_set myset; 
    struct timeval tv; 
    int receivedBytes = -1;

    if ((receivedBytes = Recv(p->sockFd, message, messageSize, 0)) == -1) {

        if (errno == EAGAIN) { 
                        
            fprintf(stderr, "EAGAIN in Recv() - selecting\n"); 
            
            do { 

                if ((receivedBytes = Recv(p->sockFd, message, messageSize, 0)) > -1) {

                    std::cout << "Receive succeeded!\n\n";
                    return receivedBytes;
                }

                //Set timeouts
                tv.tv_sec = 30; 
                tv.tv_usec = 0; 
                
                FD_ZERO(&myset); 
                FD_SET(p->sockFd, &myset); 

                res = Select(p->sockFd + 1, &myset, NULL, NULL, &tv); 

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
TorrentPeerwireProtocol::TorrentPeerwireProtocol(int pieceLen, 
                                                    iptool * theTool, 
                                                    uint8_t * hash, 
                                                    struct thread_pool* thePool, 
                                                    PeerList newPeerList, 
                                                    std::vector<std::string> pList,
                                                    const uint8_t * thePeerId) {

    BIT_TORRENT_ID = std::string("BitTorrent protocol").c_str();
    pool = thePool;
    tool = theTool;
    peerId = thePeerId;

    
}

void TorrentPeerwireProtocol::download(const uint8_t * infoHash, 
                                        PeerList & pList, 
                                        std::vector<Piece> & hashPieces,
                                        int pieceLen) {


    fileLength = 809984;

// std::cout << "pieceLen of file is : " << pieceLen;
// std::cout << "hashpiece size is of file is : " << hashPieces.size();

// std::cout << "Length of file is : " << fileLength;

    std::unordered_set<Peer *, PeerPointerHash> * peers = pList.getPeerSet();

    while (!done) {

std::cout << "zomg!\n";

        for (std::unordered_set<Peer *, PeerPointerHash>::iterator it = peers->begin(); it != peers->end(); ++it) {
            
            Peer * peer = *it;

            if (peer->sockFd == -1) {
std::cout << "\nCONNECTING IN DOWNLOAD\n";
                connect(infoHash, peerId, peer);
std::cout << "\nHANDSHAKING IN DOWNLOAD\n";
                handshake(infoHash, peerId, peer, tcpSendMessage, tcpRecvMessage);

std::cout << "\nSTARTING THREAD FOR SENDING\n";
                SendArgs sendPass(peer, *this, pieceLen);
                thread_pool_submit(pool, peerSend, &sendPass);
std::cout << "\nSTARTING THREAD FOR RECEIVING\n";
                ReceiveArgs receivePass(peer, *this, pieceLen);
                thread_pool_submit(pool, receive, &receivePass);
std::cout << "\nSTARTING THREAD FOR LISTENING\n";

                ListenArgs listenPass(pieceLen, BIT_TORRENT_ID);
                thread_pool_submit(pool, listenForThem, &listenPass);
            }
        }

        sleep(1);

        std::unordered_set<Peer *, PeerPointerHash> * tempPeers = pList.getPeerSet();
        peers->insert(tempPeers->begin(), tempPeers->end());
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
ConnectStatus TorrentPeerwireProtocol::connect(const uint8_t * infoHash,
                                                const uint8_t * peerId, 
                                                Peer * p) {
    
    struct sockaddr * saddr;
    struct addrinfo hints, * ai,  * it;
    char strportnum[25];

    memset(&hints, '\0', sizeof(hints));
    hints.ai_flags = AI_ADDRCONFIG;
    hints.ai_socktype = SOCK_STREAM;

    snprintf(strportnum, 10, "%d", p->getPortNumber());

    GetAddrInfo(p->getIp().c_str(), strportnum, &hints, &ai);

    for (it = ai; it != NULL; it = it->ai_next) {

        if ((p->sockFd = Socket(it->ai_family, it->ai_socktype, it->ai_protocol)) != -1) {

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
            clientAddress.sin_port = htons(p->getPortNumber());
            Bind(p->sockFd, (struct sockaddr *) &clientAddress, sizeof(clientAddress));
            std::cout << "yep, it's been bound....\n";
            */

            // Set non-blocking 
            if( (arg = fcntl(p->sockFd, F_GETFL, NULL)) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno)); 
                return FAIL;
            }
            arg |= O_NONBLOCK; 
            if( fcntl(p->sockFd, F_SETFL, arg) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno)); 
                return FAIL;
            } 

            // Trying to connect with timeout 
            res = Connect(p->sockFd, saddr, sizeof(*saddr)); 
            if (res < 0) { 

                if (errno == EINPROGRESS) { 
                    
                    fprintf(stderr, "EINPROGRESS in connect() - selecting\n"); 
                    
                    do { 

                        //Set timeouts
                        tv.tv_sec = 15; 
                        tv.tv_usec = 0; 
                        
                        FD_ZERO(&myset); 
                        FD_SET(p->sockFd, &myset); 
            
                        res = Select(p->sockFd + 1, NULL, &myset, NULL, &tv); 

                        if (res < 0 && errno != EINTR) { 
                            fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); 
                        } 
                        else if (res > 0) { 
                
                            // Socket selected for write 
                            lon = sizeof(int); 
                            if (getsockopt(p->sockFd, SOL_SOCKET, SO_ERROR, (void *) &valopt, &lon) < 0) { 
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
            if( (arg = fcntl(p->sockFd, F_GETFL, NULL)) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno)); 
            } 

            arg &= (~O_NONBLOCK); 

            if(fcntl(p->sockFd, F_SETFL, arg) < 0) { 
                fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno)); 
            } 

            // I hope that is all 
            //////////////////////////////////////////////////////////////////////////////////
            //////////////////////this shizzzzzzzzzzzzzz................//////////////////////
            //////////////////////////////////////////////////////////////////////////////////
        }
    }

    freeaddrinfo(ai);
    return FAIL;
}

bool TorrentPeerwireProtocol::handshake(const uint8_t * infoHash,
                                        const uint8_t * peerId, 
                                        const Peer * p, 
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

        std::cout << "\nReceived incorrect handshake\n";
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
void TorrentPeerwireProtocol::keepAlive(const Peer * p, SendMessage sendMessage) {

    //Construct the message
    uint32_t length = 0;
    sendMessage(&length, 4, p);
}

//Choke a peer to prevent them from sending information
//<len=0001><id=0>
void TorrentPeerwireProtocol::choke(const Peer * p, SendMessage sendMessage){

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
void TorrentPeerwireProtocol::unchoke(const Peer * p, SendMessage sendMessage) {

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
void TorrentPeerwireProtocol::interested(const Peer * p, SendMessage sendMessage) {
    //Construct the message
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
void TorrentPeerwireProtocol::notInterested(const Peer * p, SendMessage sendMessage) {
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
                                    const Peer * p, SendMessage sendMessage){

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
void TorrentPeerwireProtocol::bitfield(const Peer * p, SendMessage sendMessage){

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

void TorrentPeerwireProtocol::parseBitfield(uint8_t * buffer, uint32_t length, Peer * p, uint32_t pieceLen) {

    uint8_t * payload = &(buffer[5]);
    int numberadded = 0;
    for (uint32_t i = 0; i < length - 1; i++) {

        std::string * pieces = parseByte(payload[i]);
        for (uint32_t j = 0; j < pieces->size(); ++j) {
            
            if ((*pieces)[j]) {
                
                Piece * newPiece = new Piece(i + j * 8, pieceLen);
                p->addPiece(*newPiece);
                numberadded += 1;
                std::cout << "NumberAdded " << p->getPieces().size() << "\n";
            }
        }

        delete pieces;
    }
}

// In the for <len0013><id=6><index><begin><length big endian>
// index : int specifying the zero-based piece index
// begin : int specifying the zero-based byte offset within the piece
// length : int specifying the requested length
void TorrentPeerwireProtocol::request(uint32_t index, 
                                        uint32_t begin, 
                                        uint32_t requestedLength, 
                                        const Peer * p, 
                                        SendMessage sendMessage) {

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
                                    const Peer * p, SendMessage sendMessage) {
  
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
                                        const Peer * p, SendMessage sendMessage) {

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


void * receive(void * receivePeer){
    
    ReceiveArgs * receiveArgs = (ReceiveArgs *) receivePeer;

    // Open up the file to read and write
    // if((torrentialSaveFile = fopen("torrentialSaveFile", "wb")) == NULL){
    //     perror("save_piece: fopen failed :(");
    //     sleep(2000);
    // }

    int numBytesReceived = 0;
    int numBytes = 0;
    uint8_t buffer[1024];
    uint8_t  id ;
    int length;
    Peer * currentPeer = receiveArgs->currentPeer;
    int pieceLen = receiveArgs->pieceL;



    while(!done){
std::cout << "looping..in receive!!!!!!.shit....##################################################################################3\n###########################################################\n#############################################3\n\n";        
        length = 999;
        numBytesReceived = 0;


        while (numBytesReceived < length) {

            numBytes = tcpRecvMessage(buffer+numBytesReceived, sizeof(buffer)-numBytesReceived, currentPeer);
            
            if(numBytesReceived == 0){

                //length = ntohl((uint32_t&) *buffer);//uint32_t(buffer[0]) + uint32_t(buffer[1] << 8)
                         //    + uint32_t(buffer[2] << 16) + uint32_t(buffer[3] << 24);
                
                length = (buffer[3] + (buffer[2] >> 8) + (buffer[1] >> 16) + (buffer[0] >> 24)) ;                

                printf("Length is %d\n", length);

                if((buffer[0] + buffer[1] + buffer[2] + buffer[3] != 0) || (buffer[0] + buffer[1] + buffer[2] + buffer[3] != -1)){
                    printf("changing id to %d \n", buffer[4]);
                    id = 0;
                    memcpy(&id,&buffer[4],sizeof(uint8_t));
                    //id = &idpointer;
                }
                else if(buffer[0] + buffer[1] + buffer[2] + buffer[3] != 0){
                    id = 11;
                }
                else{
                    id = 12;
                }
                
            }

            numBytesReceived += numBytes;        

            printf("total length received == %d length == %d\n", numBytesReceived, length);
        }
        std::cout << "*********************************************************************8id == " << id << "\n";
    
        switch (id) {

            case 0: // choke
            {
                printf("Received a choke message :( choking in response\n");
                currentPeer->peerChoking = true;
                receiveArgs->peerwire.choke(currentPeer, tcpSendMessage);
                currentPeer->amChoking = true;
                break;
            }
            case 1: // unchoke
            {
                printf("Received an unchoke message! Unchoking them in response\n");
                currentPeer->peerChoking = false;
                receiveArgs->peerwire.unchoke(currentPeer,tcpSendMessage);
                currentPeer->amChoking = false;
                break;
            }
            case 2: // interested
            {
                printf("Received an interested message! Updating the peer\n");
                currentPeer->peerInterested = true;
                receiveArgs->peerwire.interested(currentPeer,tcpSendMessage);
                currentPeer->amInterested = true;
                break;
            }
            case 3: // not interested
            {
                printf("Received an uninterested message from peer...story of my life. Updating peer\n");
                currentPeer->peerInterested = false;
                receiveArgs->peerwire.notInterested(currentPeer,tcpSendMessage);
                currentPeer->amInterested = false;
                break;
            }
            case 4: // have
            {
                 //update the peers list to have that piece
                printf("Received a have message, should update the peer's hash pieces list\n");

                uint32_t pieceIndex = buffer[5];

                Piece * piece = new Piece(pieceIndex, pieceLen);
                currentPeer->addPiece(*piece);

                // std::string * pieces = parseByte(piece);
                // for (uint32_t j = 0; j < pieces->size(); ++j) {
            
                //     if ((*pieces)[j]) {

                //         Piece * newPiece = new Piece(j * 8);
                //         currentPeer->addPiece(*newPiece);
                //     }
                // }
                break;
            }
            case 5:
            {
                // uint32_t lengthOfBitfield = ntohl((uint32_t) buffer[3]);
                // std::cout << "LEEEEEEEEEEEEEEEEE " <<lengthOfBitfield;

                std::unordered_set<Piece, PieceHash> pieces = currentPeer->getPieces();
                std::cout << "Amount of peer pieces before " <<pieces.size() << "\n";


                uint32_t lengthOfBitfield = (uint32_t)buffer[3] + ((uint32_t)buffer[2] << 8) + 
                ((uint32_t)buffer[1] << 16) + ((uint32_t)buffer[0] << 24);
                
                std::cout << "Length of the Bitfield " <<lengthOfBitfield << "\n";

                receiveArgs->peerwire.parseBitfield(buffer, lengthOfBitfield, currentPeer,pieceLen);


                pieces = currentPeer->getPieces();
                std::cout << "Amount of peer pieces after " <<pieces.size() << "\n";

                break;
            }
            case 6: // request
            {
                printf("Received a request message \n");
                // load the piece from the file and send it using piece()
                //Piece_t piece;
            
                uint32_t index;          
                uint32_t begin;
                uint32_t requestedLength;

                uint32_t newLength;
                uint8_t * block = 0;

                index = ntohl(buffer[5]);

                begin = ntohl(buffer[9]);

                requestedLength = ntohl(buffer[13]);
               
                pthread_mutex_lock(&uploadMutex);

                // Open up the file to read
                // if((torrentialSaveFile = fopen(save, "r")) == NULL){
                //     perror("load_piece: fopen failed :(");
                // } //pices

                if(fseek(torrentialSaveFile, index * pieceLen  + begin * requestedLength, SEEK_SET) < 0){
                    perror("load_piece: fseek failed :( \n");

                }

                // Extract the piece
                
                fread(block,1,requestedLength,torrentialSaveFile);

                //fclose(torrentialSaveFile);

                pthread_mutex_unlock(&uploadMutex);

                //Create the piece message to send

                newLength = 9 + requestedLength;

                //Add the message in a queue

                receiveArgs->peerwire.piece(index, begin, block, newLength, currentPeer, tcpSendMessage);
                break;
            }
            case 7: // Got a piece, save it to file
            {
                printf("Writing\n");
                 if((torrentialSaveFile = fopen("torrentialSaveFile", "w+")) == NULL){
        perror("save_piece: fopen failed :(");
        sleep(2000);
    }

                std::cout << numBytesReceived << " \n||";
                for (int i = 0; i < numBytesReceived; i++) {

                    printf("%x", buffer[i]);
                }
                std::cout << "||\n";

                uint32_t blockIndex = ntohl((uint32_t) buffer[5]);
                uint32_t begin = ntohl((uint32_t) buffer[9]);
                uint8_t * block = buffer + 13;
                printf("Writing THIS BLOCKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK\n");
                std::cout << numBytesReceived << " \n||";
                for (int i = 0; i < numBytesReceived - 13; i++) {

                    printf("%x", block[i]);
                }
                std::cout << "||\n";

                pthread_mutex_lock(&uploadMutex);
        
                if(fseek(torrentialSaveFile, blockIndex * pieceLen + begin * (sizeof(block)), SEEK_SET) < 0){
                    perror("save_piece: fseek failed :(");
                    sleep(2000);
                }

                // Extract the piece
                int numBytesWritten = 0;
                if ((numBytesWritten = fwrite(block, 1, numBytesReceived-13, torrentialSaveFile)) < 0) {
                    perror("save_piece: fwrite failed :(");
                    sleep(2000);
                }
                std::cout << "numBytesWRitten == " << numBytesWritten << "\n";
                
                totalBytesWritten += numBytesReceived-13;

                if(totalBytesWritten == fileLength){
                    std::cout << ",,!,, DONE THE FILE BITCHES ,,!,," << "\n";
                    done = true;
                }
                fclose(torrentialSaveFile);
                pthread_mutex_unlock(&uploadMutex);

                printf("Done writing\n");



                break;
            }
            case 11: //It's a keep alive. Let's keep alive.
            {
                break;
            }
            case 12: //Id is null :()
            {
                printf("Got a null ID :(\n");

                // while(1){
                //     printf("BROKEN TORRENT, THANKS JOHN \n");
                // }

                break;
            }
            case 8: // cancel
            {
                break;
            }
            default:
            {
                printf("Received some sort of unknown message. Joke's on them, I'm ignoring it.\n");
                printf("#hardtoget\n");
                break;
            }
        }
        sleep(1);
    }

    //fclose(torrentialSaveFile);
    return NULL;
}