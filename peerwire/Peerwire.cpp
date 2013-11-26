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

void * downloadPiece(void * functionPointers) {

    SendRecvFuncs * funcs = (SendRecvFuncs *) functionPointers;

    //download the stuff

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

void tcpRecvMessage(void * message, uint32_t messageSize, const Peer & p) {

    std::cout << "RECEIVING MESSAGE\n";

    if (Recv(p.sockFd, message, messageSize, 0)) {
        
        std::cout << "An error occurred in recv message.......\n";
    }
    else {

        std::cout << "RECEIVE SUCCESS!!!!!!!!!!!!\n";
    }
}

void udpSendMessage(const void * message, uint32_t messageSize, const Peer & p) {

    //Setup server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(p.getPortNumber());
    inet_pton(AF_INET, p.getIp().c_str(), &(serverAddress.sin_addr));

    if (SendTo(p.sockFd, message, messageSize, 0, (struct sockaddr *) &serverAddress, sizeof(serverAddress))) {

        std::cout << "An error occurred in udpSendMessage...........\n";
    }
    else {

        std::cout << "udpSendMessage SUCCESS!!!!!!!!!!!!!!!!!!!\n";
    }
}

void udpRecvMessage(void * message, uint32_t messageSize, const Peer & p) {

    //Setup server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(p.getPortNumber());
    inet_pton(AF_INET, p.getIp().c_str(), &(serverAddress.sin_addr));

    uint32_t serverAddressLength = sizeof(serverAddress);

    if (RecvFrom(p.sockFd, message, messageSize, 0, (struct sockaddr *) &serverAddress, &serverAddressLength)) {

        std::cout << "An error occurred in udpRecvMessage......................\n";
    }
    else {

        std::cout << "udpRecvMessage SUCCESS!!!!!!!!!!!!!!!!!!!!!!!!!\n";
    }
}

//For testing because I'm lazy
int main(int argc, char** argv){

    const char temp[41] = "8C3760CB651C863861FA9ABE2EF70246943C1994";
    uint8_t * info_hash;
    info_hash  = convert(temp);
    
    thread_pool pool;
    
    PeerList newPeerList;
    std::vector<std::string> hashpieces;

    TorrentPeerwireProtocol peerwire = TorrentPeerwireProtocol(&pool);

    uint8_t id[] = {20,10,20,20,02,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
    
    Peer p("213.118.160.153", 13369);    
    //void connect(uint8_t * info_hash, uint8_t * peer_id, Peer & p);
    peerwire.connect(&*info_hash, &*id, p);
    peerwire.handshake(&*id, p, udpSendMessage, udpRecvMessage);

    return 0;
}

//using namespace libtorrent;
//hashpieces are all the hash files this instantiation of peerwire protocol is reuqired to download
TorrentPeerwireProtocol::TorrentPeerwireProtocol(struct thread_pool * pool){

printf("Launching Peerwire...\n");
printf("Searching through peers...\n");

    BIT_TORRENT_ID = std::string("BitTorrent protocol").c_str();
std::cout << "length of BIT_TORRENT_ID == ||" << strlen(BIT_TORRENT_ID) << "||\n";
}

void TorrentPeerwireProtocol::download(uint8_t * infoHash, PeerList & pList, 
                                        std::vector<Piece> & hashPieces) {

    while (true) {

        std::vector<Peer> * peers = pList.getPeers();
        std::vector<Peer>::iterator peerIt = peers->begin();

        for (; peerIt != peers->end(); peerIt++) {

            SendRecvFuncs * funcs = new SendRecvFuncs;

            //If first Connection
            if (peerIt->sockFd == -1) {

                uint8_t id[] = {20,10,20,20,02,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};

                switch (connect(infoHash, &*id, *peerIt)) {

                    case TIMEOUT:
                        //If the socket was setup....
                        if (peerIt->sockFd != -1) {
                            
                            (*peerIt).tcpPeer = false;
                        }
                        else {

                            pList.removePeer(*peerIt);    
                        }
                        break;
                    case FAIL:
                        pList.removePeer(*peerIt);
                        continue;
                        break;
                    default:
                        break;
                }

                //Set appropriate functions to be used depending on tcp or udp
                if ((*peerIt).tcpPeer) {

                    funcs->sendMessage = tcpSendMessage;
                    funcs->recvMessage = tcpRecvMessage;
                }
                else {

                    funcs->sendMessage = udpSendMessage;
                    funcs->recvMessage = udpRecvMessage;
                }
            }
            else {
             
                //Set appropriate functions to be used depending on tcp or udp
                if ((*peerIt).tcpPeer) {

                    funcs->sendMessage = tcpSendMessage;
                    funcs->recvMessage = tcpRecvMessage;
                }
                else {

                    funcs->sendMessage = udpSendMessage;
                    funcs->recvMessage = udpRecvMessage;
                }
            }

            //Refresh connection if necessary
            if ((*peerIt).tcpPeer) {

                //test connection to see if still up

                //if not, connect

                //if fail, remove peer and continue
            }
            else {

                //test connection to see if still up

                //if not handshake

                //if fail, remove peer and continue
            }

            //request pieces
            std::unordered_set<Piece, PieceHash> pieces = (*peerIt).getPieces();
            if (!pieces.empty()) {

                //iterate over all pieces peer has`
                std::unordered_set<Piece, PieceHash>::iterator pieceIt = pieces.begin();
                for (; pieceIt != pieces.end(); pieceIt++) {

                    request((*pieceIt).getPieceIndex(), 0, (*pieceIt).getPieceSize(), *peerIt, funcs->sendMessage);

                    uint8_t buffer[50];
                    funcs->recvMessage(buffer, 50, *peerIt);
                    //after message gotten, spin off thread to finish
                    downloadPiece(funcs);
                }
            }            
        }
    }
}

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

        std::cout << "looping over results of Getaddrinfo,.....\n";
        std::cout << "SOCKET-ING with socket ## == ||" << p.sockFd << "||\n";
std::cout << "ai_socktype == " << it->ai_socktype << "\n SOCK_STREAM == " << SOCK_STREAM << "\nDATA_GRAM" << SOCK_DGRAM << std::endl;
        if ((p.sockFd = Socket(it->ai_family, it->ai_socktype, it->ai_protocol)) != -1) {
            std::cout << "SOCKETED!";
            std::cout << "with socket ## == ||" << p.sockFd << "||\n";

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

void TorrentPeerwireProtocol::handshake(uint8_t * peerId, 
                                        const Peer & p, 
                                        SendMessage sendMessage,
                                        RecvMessage recvMessage) {

    printf("Got in handshake, all the networking is good!##############################################\n");
    const uint32_t handshakeSize = 1 + strlen(BIT_TORRENT_ID) + 8 + 20 + 20;
    
    //Create the Handshake to send
    Handshake h;    
    h.pstrLen = (char) strlen(BIT_TORRENT_ID);
    h.pstr = BIT_TORRENT_ID;
    for (uint32_t i = 0; i < 20; i++) {
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
    sendMessage(&h, handshakeSize, p);

    std::cout << "Receiving...........................\n";
    //Receive the handshake in response
    uint8_t buffer[1024];
    recvMessage(buffer, sizeof(buffer), p);
    std::cout << "RECEIVED!\n\n";
    printHandshake(buffer);

    if (buffer[0] != 19){
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

void TorrentPeerwireProtocol::upload(Peer & currentPeer){

    while(1){

        uint8_t buffer[1024];
        uint8_t id = buffer[4];

        //recvMessage(buffer,sizeof(buffer),currentPeer);

        const char * save = "/torrentialSaveFile";
        FILE * load;

        switch(id){
            case 0: // choke

                currentPeer.peerChoking = true;
                //choke(currentPeer);
                currentPeer.amChoking = true;
                break;


            case 1: // unchoke

                currentPeer.peerChoking = false;
                //unchoke(currentPeer);
                currentPeer.amChoking = false;
                break;

            case 2: // interested

                currentPeer.peerInterested = true;
                //interested(currentPeer);
                currentPeer.amInterested = true;
                break;

            case 3: // not interested

                currentPeer.peerInterested = false;
                //notInterested(currentPeer);
                currentPeer.amInterested = false;
                break;


            case 4: // have

            //update the peers list to have that piece
            break;


            case 5: // bitfield... lol
            break;

            case 6: // request

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
               
                


                // Open up the file to read
                if((load = fopen(save, "r")) == NULL){
                    perror("load_piece: fopen failed :(");
                }

                if(fseek(load, index * requestedLength + begin, SEEK_SET) < 0){
                    perror("load_piece: fseek failed :(");

                }

                // Extract the piece
                
                fread(block,1,requestedLength,load);

                fclose(load);

                //Create the piece message to send

                newLength = 9+requestedLength;

                //piece(index,begin,block,newLength,currentPeer);

            break;

            // case 7: // piece
            
            // //download 
            // //Ethan gets to do this
            // break;

            // case 8: // cancel

            // //send signal???
            // break;
        }
    }
}