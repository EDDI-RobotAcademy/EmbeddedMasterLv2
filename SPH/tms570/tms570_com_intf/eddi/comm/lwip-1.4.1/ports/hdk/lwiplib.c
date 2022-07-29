/**
*  \file lwiplib.c
*
*  \brief lwip related initializations
*/
/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
*/

/*
** Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
** ALL RIGHTS RESERVED
*/

/*
** lwIP Compile Time Options for HDK.
*/
#include <comm/lwip-1.4.1/ports/hdk/include/lwiplib.h>
#include <comm/lwip-1.4.1/ports/hdk/include/lwiplib.h>
#include <comm/lwip-1.4.1/ports/hdk/locator.c>
#include <comm/lwip-1.4.1/ports/hdk/netif/hdkif.c>
#include <comm/lwip-1.4.1/ports/hdk/perf.c>
#include <comm/lwip-1.4.1/ports/hdk/sys_arch.c>
#include <comm/lwip-1.4.1/src/api/api_lib.c>
#include <comm/lwip-1.4.1/src/api/api_msg.c>
#include <comm/lwip-1.4.1/src/api/err.c>
#include <comm/lwip-1.4.1/src/api/netbuf.c>
#include <comm/lwip-1.4.1/src/api/netdb.c>
#include <comm/lwip-1.4.1/src/api/netifapi.c>
#include <comm/lwip-1.4.1/src/api/sockets.c>
#include <comm/lwip-1.4.1/src/api/tcpip.c>
#include <comm/lwip-1.4.1/src/core/dhcp.c>
#include <comm/lwip-1.4.1/src/core/dns.c>
#include <comm/lwip-1.4.1/src/core/init.c>
#include <comm/lwip-1.4.1/src/core/ipv4/autoip.c>
#include <comm/lwip-1.4.1/src/core/ipv4/icmp.c>
#include <comm/lwip-1.4.1/src/core/ipv4/igmp.c>
#include <comm/lwip-1.4.1/src/core/ipv4/inet.c>
#include <comm/lwip-1.4.1/src/core/ipv4/inet_chksum.c>
#include <comm/lwip-1.4.1/src/core/ipv4/ip.c>
#include <comm/lwip-1.4.1/src/core/ipv4/ip_addr.c>
#include <comm/lwip-1.4.1/src/core/ipv4/ip_frag.c>
#include <comm/lwip-1.4.1/src/core/mem.c>
#include <comm/lwip-1.4.1/src/core/memp.c>
#include <comm/lwip-1.4.1/src/core/netif.c>
#include <comm/lwip-1.4.1/src/core/pbuf.c>
#include <comm/lwip-1.4.1/src/core/raw.c>
#include <comm/lwip-1.4.1/src/core/snmp/asn1_dec.c>
#include <comm/lwip-1.4.1/src/core/snmp/asn1_enc.c>
#include <comm/lwip-1.4.1/src/core/snmp/mib_structs.c>
#include <comm/lwip-1.4.1/src/core/snmp/mib2.c>
#include <comm/lwip-1.4.1/src/core/snmp/msg_in.c>
#include <comm/lwip-1.4.1/src/core/snmp/msg_out.c>
#include <comm/lwip-1.4.1/src/core/stats.c>
#include <comm/lwip-1.4.1/src/core/sys.c>
#include <comm/lwip-1.4.1/src/core/tcp.c>
#include <comm/lwip-1.4.1/src/core/tcp_in.c>
#include <comm/lwip-1.4.1/src/core/tcp_out.c>
#include <comm/lwip-1.4.1/src/core/udp.c>
#include <comm/lwip-1.4.1/src/netif/etharp.c>
#include <comm/lwip-1.4.1/src/netif/loopif.c>
#include <comm/lwip-1.4.1/src/netif/ppp/auth.c>
#include <comm/lwip-1.4.1/src/netif/ppp/chap.c>
#include <comm/lwip-1.4.1/src/netif/ppp/chpms.c>
#include <comm/lwip-1.4.1/src/netif/ppp/fsm.c>
#include <comm/lwip-1.4.1/src/netif/ppp/ipcp.c>
#include <comm/lwip-1.4.1/src/netif/ppp/lcp.c>
#include <comm/lwip-1.4.1/src/netif/ppp/magic.c>
#include <comm/lwip-1.4.1/src/netif/ppp/md5.c>
#include <comm/lwip-1.4.1/src/netif/ppp/pap.c>
#include <comm/lwip-1.4.1/src/netif/ppp/ppp.c>
#include <comm/lwip-1.4.1/src/netif/ppp/ppp_oe.c>
#include <comm/lwip-1.4.1/src/netif/ppp/randm.c>
#include <comm/lwip-1.4.1/src/netif/ppp/vj.c>

/******************************************************************************
**                       INTERNAL VARIABLE DEFINITIONS
******************************************************************************/
/*
** The lwIP network interface structure for the HDK Ethernet MAC.
*/
static struct netif hdkNetIF[MAX_EMAC_INSTANCE];

/******************************************************************************
**                          FUNCTION DEFINITIONS
******************************************************************************/
/**
 *
 * \brief Initializes the lwIP TCP/IP stack.
 *
 * \param instNum  The instance index of the EMAC module
 * \param macArray Pointer to the MAC Address
 * \param ipAddr   The IP address to be used
 * \param netMask  The network mask to be used
 * \param gwAddr   The Gateway address to be used
 * \param ipMode   The IP Address Mode.
 *        ipMode can take the following values\n
 *             IPADDR_USE_STATIC - force static IP addressing to be used \n
 *             IPADDR_USE_DHCP - force DHCP with fallback to Link Local \n
 *             IPADDR_USE_AUTOIP - force  Link Local only
 *
 * This function performs initialization of the lwIP TCP/IP stack for the
 * HDK EMAC, including DHCP and/or AutoIP, as configured.
 *
 * \return IP Address.
*/
 unsigned int lwIPInit(unsigned int instNum, unsigned char *macArray,
                       unsigned int ipAddr, unsigned int netMask,
                       unsigned int gwAddr, unsigned int ipMode)
 {
     struct ip_addr ip_addr;
     struct ip_addr net_mask;
     struct ip_addr gw_addr;
     volatile unsigned char *state;
     unsigned int *ipAddrPtr;
     volatile unsigned int cnt = 0x3FFFFFFF;

     lwip_init();

     /* Setup the network address values. */
     if(ipMode == IPADDR_USE_STATIC)
     {
         ip_addr.addr = htonl(ipAddr);
         net_mask.addr = htonl(netMask);
         gw_addr.addr = htonl(gwAddr);
     }
     else
     {
         ip_addr.addr = 0;
         net_mask.addr = 0;
         gw_addr.addr = 0;
     }

     hdkif_macaddrset(instNum, macArray);

     /*
     ** Create, configure and add the Ethernet controller interface with
     ** default settings.  ip_input should be used to send packets directly to
     ** the stack. The lwIP will internaly call the hdkif_init function.
     */
     if(NULL ==
        netif_add(&hdkNetIF[instNum], &ip_addr, &net_mask, &gw_addr, &instNum,
               hdkif_init, ip_input))
     {
         return 0;
     };

     netif_set_default(&hdkNetIF[instNum]);

     /* Start DHCP, if enabled. */
 #if LWIP_DHCP
 if(ipMode == IPADDR_USE_DHCP)
 {
 	unsigned int dhcp_flag = 0;
 	unsigned int dhcp_tries = 5;
 	unsigned int count;
 	unsigned int delay;
 	while(dhcp_tries--)
 	{
 	    dhcp_start(&hdkNetIF[instNum]);
 	    count = 10;
 	/* Check for DHCP completion for 'count' number of times, each for the given delay. */
 		while(count--)
 		{
 			delay = 0x8FFFFU;
 			while(delay--);
 			state = &(hdkNetIF[instNum].dhcp->state);
 			if(DHCP_BOUND == *state)
 			{
 				dhcp_flag = 1;
 			    ipAddrPtr = (unsigned int*)&(hdkNetIF[instNum].ip_addr);
 			    return (*ipAddrPtr);
 			}
 		}
 	}
 	/* In case of DHCP failure, return 0. */
 	if(dhcp_flag == 0)
 		return 0;
 }
 #endif

     /* Start AutoIP, if enabled and DHCP is not. */
 #if LWIP_AUTOIP
     if(ipMode == IPADDR_USE_AUTOIP)
     {
         autoip_start(&hdkNetIF[instNum]);
     }
 #endif

     if((ipMode == IPADDR_USE_STATIC)
     	       ||(ipMode == IPADDR_USE_AUTOIP))
     {
        /* Bring the interface up */
        netif_set_up(&hdkNetIF[instNum]);
     }

     ipAddrPtr = (unsigned int*)&(hdkNetIF[instNum].ip_addr);

     return (*ipAddrPtr);
 }

/*
 * \brief   Checks if the ethernet link is up
 *
 * \param   instNum  The instance number of EMAC module
 *
 * \return  Interface status.
*/
unsigned int lwIPNetIfStatusGet(unsigned int instNum)
{

    return (hdkif_netif_status(&hdkNetIF[instNum]));
}

/*
 * \brief   Checks if the ethernet link is up
 *
 * \param   instNum  The instance number of EMAC module
 *
 * \return  The link status.
*/
unsigned int lwIPLinkStatusGet(unsigned int instNum)
{
    return (hdkif_link_status(&hdkNetIF[instNum]));
}

/**
 * \brief   Interrupt handler for Receive Interrupt. Directly calls the
 *          HDK interface receive interrupt handler.
 *
 * \param   instNum  The instance number of EMAC module for which receive
 *                   interrupt happened
 *
 * \return  None.
*/
void lwIPRxIntHandler(unsigned int instNum)
{
    hdkif_rx_inthandler(&hdkNetIF[instNum]);
}

/**
 * \brief   Interrupt handler for Transmit Interrupt. Directly calls the
 *          HDK interface transmit interrupt handler.
 *
 * \param   instNum  The instance number of EMAC module for which transmit
 *                   interrupt happened
 *
 * \return  None.
*/
void lwIPTxIntHandler(unsigned int instNum)
{
    hdkif_tx_inthandler(&hdkNetIF[instNum]);
}

/***************************** End Of File ***********************************/















