/**
  ******************************************************************************
  * @file    LwIP/LwIP_TCP_Echo_Server/Inc/ethernetif.h 
  * @author  MCD Application Team
  * @brief   Header for ethernetif.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define ETH_TX_DESC_CNT         4U  /* number of Ethernet Tx DMA descriptors */
#define ETH_RX_DESC_CNT         4U  /* number of Ethernet Rx DMA descriptors */

#define ETH_MAC_ADDR0    ((uint8_t)0x00)
#define ETH_MAC_ADDR1    ((uint8_t)0x15)
#define ETH_MAC_ADDR2    ((uint8_t)0x5D)
#define ETH_MAC_ADDR3    ((uint8_t)0xDC)
#define ETH_MAC_ADDR4    ((uint8_t)0x50)
#define ETH_MAC_ADDR5    ((uint8_t)0xE2)

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   ((uint8_t) 192U)
#define IP_ADDR1   ((uint8_t) 168U)
#define IP_ADDR2   ((uint8_t) 0U)
#define IP_ADDR3   ((uint8_t) 10U)

/*NETMASK*/
#define NETMASK_ADDR0   ((uint8_t) 255U)
#define NETMASK_ADDR1   ((uint8_t) 255U)
#define NETMASK_ADDR2   ((uint8_t) 255U)
#define NETMASK_ADDR3   ((uint8_t) 0U)

/*Gateway Address*/
#define GW_ADDR0   ((uint8_t) 192U)
#define GW_ADDR1   ((uint8_t) 168U)
#define GW_ADDR2   ((uint8_t) 0U)
#define GW_ADDR3   ((uint8_t) 1U)

/* Exported types ------------------------------------------------------------*/
err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);
void ethernet_link_check_state(struct netif *netif);


#ifdef __cplusplus
}
#endif

#endif
