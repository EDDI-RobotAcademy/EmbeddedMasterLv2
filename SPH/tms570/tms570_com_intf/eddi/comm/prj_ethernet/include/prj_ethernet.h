/*
 * prj_udp.h
 *
 *  Created on: 2022. 4. 26.
 *      Author: son
 */

#ifndef EDDI_COM_PRJ_ETHERNET_INCLUDE_PRJ_ETHERNET_H_
#define EDDI_COM_PRJ_ETHERNET_INCLUDE_PRJ_ETHERNET_H_

#include "prj_network.h"
#include "prj_protocol.h"

#define BROADCAST 1
#define UNICAST 0

#define PRJ_UDP 1
#define PRJ_TCP 0

#define CLIENT 0
#define SERVER 1

#define UDPTEST 1

#if PRJ_UDP
extern boolean udp_socket_handler(void);
extern void udp_tx(protocol_request_packt **pkt);
#else
#endif

#endif /* EDDI_COM_PRJ_ETHERNET_INCLUDE_PRJ_ETHERNET_H_ */
