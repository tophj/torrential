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

//For testing because I'm lazy
/*int main(int argc, char** argv){

    //const char temp[41] = "8C3760CB651C863861FA9ABE2EF70246943C1994";
    uint8_t info_hash[] = {0xdf, 0x79, 0xd5, 0xef, 0xc6, 0x83, 0x4c, 0xfb, 0x31, 0x21, 0x8d, 0xb8, 0x3d, 0x6f, 0xf1, 0xc5, 0x5a, 0xd8, 0x17, 0x9d};
    //info_hash  = convert(temp);
    
    thread_pool pool;
    
    PeerList newPeerList;
    std::vector<std::string> hashpieces;

   TorrentPeerwireProtocol peerwire = TorrentPeerwireProtocol(&pool);

    uint8_t id[] = {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
    
    Peer p("213.112.225.102", 6985);    
    if (SUCCESS == peerwire.connect(&*info_hash, &*id, p)) {
        std::cout << "SUCCESS ACHIEVED!\n";
        if (peerwire.handshake(info_hash, &*id, p, tcpSendMessage, tcpRecvMessage)) {

            std::cout << "HANDSHAKE RECEIVED!!\n";
            uint8_t buffer[1024];
            tcpRecvMessage(buffer, sizeof(buffer), p);
            int32_t length = ntohl((int32_t) *buffer);
            for (int k = 0; k < 4; k++) {
std::cout << "buffer stuffer at " << k << " == " << buffer[k] << "\n";
            }
std::cout << "length == " << length << "\n";
std::cout << "id == " << ((char) buffer[4]) << "\n";
sleep(5000);
            if (buffer[4] == 5) {
             
                peerwire.parseBitfield(buffer, length, p);
            }
        }
    }

    return 0;
}
*/
 * getIPaddresses(){

}

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

    std::cout << "SENDING MESSAGE\n";

    if (Send(p.sockFd, message, messageSize, 0) < 0) {
        
        std::cout << "An error occurred in send message..........\n";
    }
    else{
        
        std::cout << "SEND SUCCESS!!!!!!!!!!!\n";
    }   
}

int tcpRecvMessage(void * message, uint32_t messageSize, const Peer & p) {

    std::cout << "RECEIVING MESSAGE, buffer size == " << messageSize << "\n";

    int res; 
    fd_set myset; 
    struct timeval tv; 
    int receivedBytes = -1;

    if ((receivedBytes = Recv(p.sockFd, message, messageSize, 0)) == -1) {

        if (errno == EAGAIN) { 
                        
            fprintf(stderr, "EAGAIN in Recv() - selecting\n"); 
            
            do { 

                if ((receivedBytes = Recv(p.sockFd, message, messageSize, 0)) > -1) {

                    std::cout << "Receive succeeded!\n";
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

                    fprintf(stderr, "Timeout in select() - Cancelling!\n ERRNO == %s\n", strerror(errno)); 
                    return -1; 
                } 

            } while (1); 
        } 
        else { 

            fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); 
        }
    }
    else {

        std::cout << "RECEIVE SUCCESS!!!!!!!!!!!!\n";
        return receivedBytes;
    }

    return -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//For testing because I'm lazy
/*int main(int argc, char** argv){

    //const char temp[41] = "8C3760CB651C863861FA9ABE2EF70246943C1994";
    uint8_t info_hash[] = {0xdf, 0x79, 0xd5, 0xef, 0xc6, 0x83, 0x4c, 0xfb, 0x31, 0x21, 0x8d, 0xb8, 0x3d, 0x6f, 0xf1, 0xc5, 0x5a, 0xd8, 0x17, 0x9d};
    //info_hash  = convert(temp);
    
    thread_pool pool;
    
    PeerList newPeerList;
    std::vector<std::string> hashpieces;

   TorrentPeerwireProtocol peerwire = TorrentPeerwireProtocol(&pool);

    uint8_t id[] = {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
    
    Peer p("213.112.225.102", 6985);    
    if (SUCCESS == peerwire.connect(&*info_hash, &*id, p)) {
        std::cout << "SUCCESS ACHIEVED!\n";
        if (peerwire.handshake(info_hash, &*id, p, tcpSendMessage, tcpRecvMessage)) {

            std::cout << "HANDSHAKE RECEIVED!!\n";
            //peerwire.download(infoHash);
        }
    }

    return 0;
}
*/

//using namespace libtorrent;
//hashpieces are all the hash files this instantiation of peerwire protocol is reuqired to download
TorrentPeerwireProtocol::TorrentPeerwireProtocol(int pieceLen, iptool * itool ,char* hash ,struct thread_pool* pool ,PeerList newPeerList,std::vector<std::string> pList){

printf("Launching Peerwire...\n");
printf("Searching through peers...\n");
    tool = iptool;
    BIT_TORRENT_ID = std::string("BitTorrent protocol").c_str();
    struct thread_pool* ThePool = pool;
std::cout << "length of BIT_TORRENT_ID == ||" << strlen(BIT_TORRENT_ID) << "||\n";
}

void TorrentPeerwireProtocol::download(uint8_t * infoHash, PeerList & pList, 
                                        std::vector<Piece> & hashPieces,
                                        int pieceLen) {

    while (true) {

        std::vector<Peer> * peers = pList.getPeers();

        for (std::vector<Peer>::iterator it = peers->begin(); it != peers->end(); ++it) {
            
            if ((*it).sockFd == -1) {

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
            
            struct sockaddr_in clientAddress;
            clientAddress.sin_family = AF_INET;
            std::string tempString("172.31.162.103");
            clientAddress.sin_addr.s_addr = inet_pton(AF_INET, tool.getBest().c_str(), &(clientAddress.sin_addr));
            clientAddress.sin_port = htons(p.getPortNumber());
            Bind(p.sockFd, (struct sockaddr *) &clientAddress, sizeof(clientAddress));
            std::cout << "yep, it's been bound....\n";
            

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
    uint8_t buffer[100];
    if (recvMessage(buffer, sizeof(buffer), p)) {
        

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
    length = htonl(length);
    sendMessage(&length, 4, p);
}

//Choke a peer to prevent them from sending information
//<len=0001><id=0>
void TorrentPeerwireProtocol::choke(const Peer & p, SendMessage sendMessage){

    //Construct the message
    uint32_t length = 1;
    uint8_t id = 0;
    
    uint8_t message[5];
    message[0] = htonl(length);
    message[4] = id;
    
    sendMessage(message, 5, p);
}

//Unchoke a peer and startup data xfer
//<len=0001><id=1>
void TorrentPeerwireProtocol::unchoke(const Peer & p, SendMessage sendMessage) {

    //Construct the message
    uint32_t length = 1;
    uint8_t id = 1;
    
    uint8_t message[5];
    message[0] = htonl(length);
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
    message[0] = htonl(length);
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
    message[0] = htonl(length);
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
    
    message[0] = htonl(length);
    message[4] = id;
    message[5] = pieceIndex;

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
    //bits->append((byte & 1) ? "1" : "0");
    bits->append((byte & 1) ? "1" : "0");
    bits->append((byte & 2) ? "1" : "0");
    bits->append((byte & 4) ? "1" : "0");
    bits->append((byte & 8) ? "1" : "0");
    bits->append((byte & 16) ? "1" : "0");
    bits->append((byte & 32) ? "1" : "0");
    bits->append((byte & 64) ? "1" : "0");
    bits->append((byte & 128) ? "1" : "0");
std::cout << *bits << std::endl;
    return bits;
}

void TorrentPeerwireProtocol::parseBitfield(uint8_t * buffer, uint32_t length, Peer & p) {

    uint8_t * payload = &(buffer[5]);
    for (uint32_t i = 0; i < length - 1; i++) {

        std::string * pieces = parseByte(payload[i]);
        for (uint32_t j = 0; j < pieces->size(); ++j) {
            
            if ((*pieces)[j]) {

                Piece * newPiece = new Piece(i + j * 8);
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
    
    message[0] = htonl(length);
    message[4] = id;
    message[5] = index;
    message[9] = begin;
    message[13] = requestedLength;

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
    message[0] = htonl(length);
    message[5] = id;
    message[9] = index;
    message[13] = begin;
    for (uint32_t i = 0; i < blockLength; i++) {

        message[17 + i] = block[i];    
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
  
  	message[0] = htonl(length);
    message[4] = id;
    message[5] = index;
    message[9] = begin;
    message[13] = requestedLength;

    sendMessage(message, 17, p);

    return;
}

void TorrentPeerwireProtocol::recieve(Peer & currentPeer, int pieceLen){

    while(1){

        uint8_t buffer[1024];
        uint8_t id = buffer[4];

        tcpRecvMessage(buffer, sizeof(buffer), currentPeer);

        const char * save = "/torrentialSaveFile";
        

        switch(id){
            case 0: // choke
            {
                printf("Recieved a choke message :( choking in response");
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

                uint8_t * payload = &(buffer[5]);

                Piece * newPiece = new Piece(i + j * 8);
                p.addPiece(*newPiece);
            }
           
            break;


            case 5: 


            break;

            case 6: // request
                {
                printf("Recieved a request message");

            // load the piece from the file and send it using piece()

                //Piece_t piece;

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

                // Open up the file to read
                if((torrentialSaveFile = fopen(save, "r")) == NULL){
                    perror("load_piece: fopen failed :(");
                } //pices

                if(fseek(torrentialSaveFile, index * pieceLen  + begin * requestedLength, SEEK_SET) < 0){
                    perror("load_piece: fseek failed :(");

                }

                // Extract the piece
                
                fread(block,1,requestedLength,load);

                fclose(torrentialSaveFile);

                pthread_mutex_unlock(&uploadMutex);

                //Create the piece message to send

                newLength = 9+requestedLength;


                //Add the message in a queue


                piece(index,begin,block,newLength,currentPeer);

                }
                
            break;

            case 7: // Got a piece, save it to file
                {
                uint32_t blockIndex = ntohl((uint32_t) buffer[5]);
                uint32_t begin = ntohl((uint32_t) buffer[9]);
                uint8_t * block = (uint8_t *) &buffer[13];

                pthread_mutex_lock(&uploadMutex);

                // Open up the file to read
                if((torrentialSaveFile = fopen(save, "wr")) == NULL){
                    perror("load_piece: fopen failed :(");
                }

                if(fseek(torrentialSaveFile, index * pieceLen + begin * (sizeof(block)), SEEK_SET) < 0){
                    perror("load_piece: fseek failed :(");

                }

                // Extract the piece
                
                fwrite(block,1,sizeof(block),torrentialSaveFile);

                fclose(torrentialSaveFile);

                pthread_mutex_unlock(&uploadMutex);

                }
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
}
