#ifndef TORRENTIAL_NETWORK_MANAGER_H
#define TORRENTIAL_NETWORK_MANAGER_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <errno.h>
#include <vector>
#include <string>
#include "../peerwire/Peerwire.h"
#include "../threadpool/threadpool.h"
#include "bt_lib.h"
#include "bt_setup.h"
#include "bencode.h"
#include "../tracker/TorrentTrackerCommManager.h"
#include "../tracker/PeerList.h"

char * create_infohash(char* file);
void info_hash(char* dict, char *id);
uint8_t * convert(char* str);
void printBytes(char* str);
void pieceByPiece(char* f, char* pieces);
#endif