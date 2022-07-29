/*
 * prj_network.h
 *
 *  Created on: 2022. 5. 3.
 *      Author: son
 */
#ifndef EDDI_COM_PRJ_ETHERNET_INCLUDE_PRJ_NETWORK_H_
#define EDDI_COM_PRJ_ETHERNET_INCLUDE_PRJ_NETWORK_H_

#include "HL_sys_common.h"

typedef struct net_info net_info;
struct net_info{
    uint32_t ip;
    uint16_t port;
};

typedef struct network network;
struct network{
    net_info dst;
    net_info src;
    uint32_t netmask;
    uint32_t gateway;
};

extern boolean set_up_local_network(char *src_ip, uint16_t src_port);
extern boolean set_up_remote_network(char *dst_ip, uint16_t dst_port);
extern boolean set_up_gate_mask(char *gate_way, char *net_mask);

#endif /* EDDI_COM_PRJ_ETHERNET_INCLUDE_PRJ_NETWORK_H_ */
