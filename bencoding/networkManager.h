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
#include "../peerwire/peerwire.h"
#include "../threadpool/threadpool.h"
//#include "../threadpool/list.h"
#include "bt_lib.h"
#include "bt_setup.h"
#include "bencode.h"
#include "../tracker/TorrentTrackerCommManager.h"
#include "../tracker/PeerList.h"


void convert(char* str, uint8_t bytes[]);
#endif