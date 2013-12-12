#include "../Peerwire.h"

//For testing
int main(int argc, char** argv){

    //const char temp[41] = "8C3760CB651C863861FA9ABE2EF70246943C1994";
    
    //uint8_t info_hash[] ={0x75, 0xE1, 0x1C, 0xB6, 0x4B, 0xD2, 0x96, 0x2B, 0x6A, 0x6F, 0x2F, 0xE6, 0xB3, 0x44, 0xCC, 0xAD, 0x70, 0x42, 0x60, 0x7B};
    uint8_t info_hash[] = {0xdf, 0x79, 0xd5, 0xef, 0xc6, 0x83, 0x4c, 0xfb, 0x31, 0x21, 0x8d, 0xb8, 0x3d, 0x6f, 0xf1, 0xc5, 0x5a, 0xd8, 0x17, 0x9d};
    uint8_t id[] = {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
    uint32_t pieceLen = 51200;
    //uint32_t pieceLen = 102400;
    //51200 worked

    //info_hash  = convert(temp);
    
    thread_pool * pool = thread_pool_new(4);
    iptool itool;
    PeerList newPeerList;
    std::vector<std::string> pList;
    std::vector<Piece> pieceList;

    
    TorrentPeerwireProtocol peerwire = TorrentPeerwireProtocol(pieceLen, &itool, &*info_hash, pool, newPeerList, pList, id);

    Peer p("213.112.225.102", 6985);   
    //Peer p("31.51.127.176",25305); 
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