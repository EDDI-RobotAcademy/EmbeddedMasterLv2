/*
 * prj_network.c
 *
 *  Created on: 2022. 5. 8.
 *      Author: son
 */
#include "lwip/inet.h"

#include "prj_network.h"

network info;

boolean set_up_local_network(char *src_ip, uint16_t src_port)
{
    uint32_t tmp;
    tmp = inet_addr(src_ip);
    info.src.ip = tmp;
    info.src.port = src_port;

    if(!tmp)
        return false;
    return true;
}

boolean set_up_remote_network(char *dst_ip, uint16_t dst_port)
{
    uint32_t tmp;
    tmp = inet_addr(dst_ip);
    info.dst.ip = tmp;
    info.dst.port = dst_port;

    if(!tmp)
        return false;
    return true;
}

boolean set_up_gate_mask(char *gate_way, char *net_mask)
{
    uint32_t tmp1, tmp2;
    tmp1 = inet_addr(gate_way);
    info.gateway = tmp1;
    tmp2 = inet_addr(net_mask);
    info.netmask = tmp2;

    if(!tmp1 & !tmp2)
        return false;
    return true;
}
