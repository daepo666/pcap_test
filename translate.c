#include "my_struct.h"
#include <stdio.h>
#include <stdint.h>
#include<arpa/inet.h>
void
void translate_ipv4(uint8_t *pointer, tcp_ipv4 *my_tcp_ipv4);
void translate_ipv6(uint8_t *pointer, tcp_ipv6 *my_tcp_ipv6);

void translate_ipv4(uint8_t *pointer, tcp_ipv4 *my_tcp_ipv4){

    for (int i = 0 ; i<6; i++){
        my_tcp_ipv4->eth.dst_MAC[i] = pointer[i];
        my_tcp_ipv4->eth.src_MAC[i] = pointer[i+6];
    }
    my_tcp_ipv4->eth.ethernet_type = ntohs(*((uint16_t *)&pointer[12]));
    
    uint8_t * ip_ptr = &pointer[14];
    my_tcp_ipv4->ipv4.version = (ip_ptr[0] >> 4) & 0xf;
    my_tcp_ipv4->ipv4.ihl     = ip_ptr[0] & 0xf;
    my_tcp_ipv4->ipv4.protocol = ip_ptr[9];
    my_tcp_ipv4->ipv4.src_ip = *((uint32_t *)&ip_ptr[12]);
    my_tcp_ipv4->ipv4.dst_ip = *((uint32_t *)&ip_ptr[16]);

    uint32_t ip_len = (my_tcp_ipv4->ipv4.ihl <<2);
    uint8_t *tcp_ptr = ip_ptr + ip_len;
    my_tcp_ipv4->tcp.src_port = ntohs(*((uint16_t *)&tcp_ptr[0]));
    my_tcp_ipv4->tcp.dst_port = ntohs(*((uint16_t *)&tcp_ptr[2]));
    my_tcp_ipv4->tcp.data_offset = (tcp_ptr[12] >> 4) & 0xf;

}

void translate_ipv6(uint8_t *pointer, tcp_ipv6 *my_tcp_ipv6){

    for (int i = 0 ; i<6; i++){
        my_tcp_ipv6->eth.dst_MAC[i] = pointer[i];
        my_tcp_ipv6->eth.src_MAC[i] = pointer[i+6];
    }
    my_tcp_ipv6->eth.ethernet_type = ntohs(*((uint16_t *)&pointer[12]));
    
    uint8_t * ip_ptr = &pointer[14];
    my_tcp_ipv6->ipv6.version = (ip_ptr[0] >> 4) & 0xf;
    my_tcp_ipv6->ipv6.next_header = ip_ptr[6];
    
    for (int i = 0; i < 16; i++) {
        my_tcp_ipv6->ipv6.src_ip6[i] = ip_ptr[i+8];
        my_tcp_ipv6->ipv6.dst_ip6[i] = ip_ptr[i+24];
    }

    uint8_t *tcp_ptr = ip_ptr + 40;
    my_tcp_ipv6->tcp.src_port = ntohs(*((uint16_t *)&tcp_ptr[0]));
    my_tcp_ipv6->tcp.dst_port = ntohs(*((uint16_t *)&tcp_ptr[2]));
    my_tcp_ipv6->tcp.data_offset = (tcp_ptr[12] >> 4) & 0xf;

}