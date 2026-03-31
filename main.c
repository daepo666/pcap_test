#include <pcap.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "my_struct.h"
void usage() {
	printf("syntax: pcap-test <interface>\n");
	printf("sample: pcap-test wlan0\n");
}

typedef struct {
	char* dev_;
} Param;

Param param = {
	.dev_ = NULL
};

bool parse(Param* param, int argc, char* argv[]) {
	if (argc != 2) {
		usage();
		return false;
	}
	param->dev_ = argv[1];
	return true;
}

int main(int argc, char* argv[]) {
	if (!parse(&param, argc, argv))
		return -1;

	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* pcap = pcap_open_live(param.dev_, BUFSIZ, 1, 1000, errbuf);
	if (pcap == NULL) {
		fprintf(stderr, "pcap_open_live(%s) return null - %s\n", param.dev_, errbuf);
		return -1;
	}

	while (true) {
		struct pcap_pkthdr* header;
		const uint8_t* packet;
		int res = pcap_next_ex(pcap, &header, &packet);
		if (res == 0) continue;
		if (res == PCAP_ERROR || res == PCAP_ERROR_BREAK) {
			printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(pcap));
			break;
		}
        tcp_ipv4 my_tcp_ipv4;
        tcp_ipv6 my_tcp_ipv6;
		printf("%u bytes captured\n", header->caplen);
        if((ntohs(*(uint16_t*)&packet[12])) == 0x800 ){ //check ipv4
            translate_ipv4(packet, &my_tcp_ipv4);
        }
        else if((ntohs(*(uint16_t*)&packet[12])) == 0x86dd){
            translate_ipv6(packet, &my_tcp_ipv6);
        }
    }

	pcap_close(pcap);
}
