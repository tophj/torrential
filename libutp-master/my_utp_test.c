#include "utp.h"
#include <stdio.h>
#include <stdlib.h>

void my_sendto_tester(void * user_data, const byte * p, size_t len, const struct sockaddr * to, socklen_t toLen) {

	printf("hey lookie here, that is how it works.....\n");
	
	
}


int main() {

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("200.125.87.64");
	sin.sin_port = htons(60401);

	printf("do socket creation...\n");
	UTPSocket * socket = UTP_Create(my_sendto_tester, NULL, (struct sockaddr *) &sin, sizeof(sin));

	printf("Connecting to socket......\n");
	UTP_Connect(socket);

	if (UTP_Write(socket, 7)) {
		printf("awwww yeah got them rights\n");
	}
	else {
		printf("slave\n");
	}

	return 1;
}
