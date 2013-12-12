#include "../Peerwire.h"

//For testing
int main(int argc, char** argv){

    //const char temp[41] = "8C3760CB651C863861FA9ABE2EF70246943C1994";
    
    uint8_t info_hash[] = {0x8e,0x0f,0xaf,0x6e,0x57,0x91,0xaa,0x5f,0xec,0x20,0xa1,0x3e,0x08,0xd7,0x3e,0x80,0x8f,0xc0,0x32,0x42};
    //uint8_t info_hash[] = {0xdf, 0x79, 0xd5, 0xef, 0xc6, 0x83, 0x4c, 0xfb, 0x31, 0x21, 0x8d, 0xb8, 0x3d, 0x6f, 0xf1, 0xc5, 0x5a, 0xd8, 0x17, 0x9d};
    uint8_t id[] = {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
    //uint32_t pieceLen = 51200;
    uint32_t pieceLen = 4096;
    //51200 worked

    //info_hash  = convert(temp);
    
    thread_pool * pool = thread_pool_new(4);
    iptool itool;
    PeerList newPeerList;
    std::vector<std::string> pList;
    std::vector<Piece> pieceList;

    
    TorrentPeerwireProtocol peerwire = TorrentPeerwireProtocol(pieceLen, &itool, &*info_hash, pool, newPeerList, pList, id);

    //Peer p("213.112.225.102", 6985);   
    Peer p("70.49.197.10",500); 
    newPeerList.addPeer(p);
    std::cout << "\n\n=====Download=====\n";
    //Test for actual procedure
    peerwire.download(info_hash, newPeerList, pieceList, pieceLen);
    /*
    if (SUCCESS == peerwire.connect(&*info_hash, &*id, p)) {
        std::cout << "SUCCESS ACHIEVED!\n";
        if (peerwire.handshake(info_hash, &*id, p, tcpSendMessage, tcpRecvMessage)) {

            std::cout << "HANDSHAKE RECEIVED!!\n";

            //Test for 1 thread
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

            Receive_t recv(p, peerwire, 100);

            receive(&recv);
        }
    }
    */

    return 0;
}