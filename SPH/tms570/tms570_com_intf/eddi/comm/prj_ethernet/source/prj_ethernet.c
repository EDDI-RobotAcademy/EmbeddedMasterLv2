/*
 * prj_udp.c
 *
 *  Created on: 2022. 4. 26.
 *      Author: son
 */
#include <stdio.h>
#include <stdlib.h>

#include "HL_sys_common.h"

#include "lwiplib.h"
#include "lwip/inet.h"

#include "prj_ethernet.h"

#if PRJ_UDP
struct udp_pcb *upcb;
#else
//TCP PCB 추가
#endif

boolean udp_socket_handler(void);
void udp_tx(protocol_request_packt **pkt);
void udp_rx_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port);

#if UDPTEST
uint8 dst_addr[4] = { 192, 168, 1, 15 };
uint8 src_addr[4] = { 192, 168, 1, 7 };
struct ip_addr src_ip;
struct ip_addr dst_ip;
#else
extern network info;
#endif

boolean udp_socket_handler(void)
{
    upcb = udp_new();

    if(!upcb)
        return false;
#if UDPTEST
    src_ip.addr = htonl(*(uint32_t *)src_addr);
    dst_ip.addr = htonl(*(uint32_t *)dst_addr);
    udp_bind(upcb, IP_ADDR_ANY, 1500);
    udp_recv(upcb, udp_rx_callback, NULL);
#else
#if BROADCAST
    udp_bind(upcb, IP_ADDR_ANY, info.src.port);
    udp_connect(upcb, IP_ADDR_ANY, info.dst.port);
    udp_recv(upcb, udp_rx_callback, NULL);
#else
    udp_bind(pcb, &info->src.ip, info->src.port);
    udp_connect(pcb, &info->dst.ip, info->dst.port);
#endif
#endif
    return true;
}

void udp_tx(protocol_request_packt **pkt)
{
    uint32_t data_num;
    uint32_t i;
    if(!(*pkt))
        //error flag 추가
        return;
    struct pbuf *txbuf;

    data_num = ((*pkt)->total_length - sizeof(protocol_request_packt))/sizeof(uint32_t);
    protocol_request_packt *tmp_pkt;

    tmp_pkt = (protocol_request_packt *)malloc((*pkt)->total_length);

    memcpy(tmp_pkt, (*pkt), (*pkt)->total_length);

    tmp_pkt->total_length = htonl(tmp_pkt->total_length);
    tmp_pkt->target_command = htonl(tmp_pkt->target_command);
    tmp_pkt->session_id = htonl(tmp_pkt->session_id);
    tmp_pkt->sub_command = htonl(tmp_pkt->sub_command);

    for(i = 0; i < data_num; i++)
        tmp_pkt->data[i] = htonl(tmp_pkt->data[i]);

    txbuf = pbuf_alloc(PBUF_TRANSPORT, (*pkt)->total_length, PBUF_RAM);

    memcpy(txbuf->payload, tmp_pkt, (*pkt)->total_length);

#if UDPTEST
    udp_sendto(upcb, txbuf, IP_ADDR_BROADCAST, 1500);
#else
    udp_sendto(upcb, txbuf, IP_ADDR_BROADCAST, info.dst.port);
#endif
    free(tmp_pkt);
    pbuf_free(txbuf);
}

void udp_rx_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port)
{
#if UDPTEST
    struct pbuf *tmp;
    if(!p)
    {
        //에러 flag추가
        return;
    }

    tmp = pbuf_alloc(PBUF_TRANSPORT, p->tot_len, PBUF_RAM);
    pbuf_take(p, tmp, p->tot_len);

    udp_connect(pcb, IP_ADDR_ANY, port);
    udp_sendto(pcb, tmp, IP_ADDR_BROADCAST, port);

    pbuf_free(tmp);
    pbuf_free(p);
    udp_disconnect(pcb);
#else
#endif
}
