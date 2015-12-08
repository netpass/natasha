#ifndef CORE_H_
#define CORE_H_

#include <rte_ethdev.h>

#ifdef RTE_EXEC_ENV_BAREMETAL
    #define MAIN _main
#else
    #define MAIN main
#endif

/* Configure logging */
#ifdef DEBUG
    #define LOG_LEVEL RTE_LOG_DEBUG
    #define LOG_DEBUG(log_type, fmt, args...) do {	\
        RTE_LOG(DEBUG, log_type, fmt, ##args);		\
    } while (0)
#else
    #define LOG_LEVEL RTE_LOG_INFO
    #define LOG_DEBUG(log_type, fmt, args...) do{} while(0)
#endif

/* Macros for printing using RTE_LOG */
#define RTE_LOGTYPE_APP RTE_LOGTYPE_USER1


/* Application configuration*/
struct app_config_port {
    uint32_t ip;
    int vlan;
};

struct app_config {
    struct app_config_port ports[RTE_MAX_ETHPORTS];
};


/* Workers */
struct queue {
    uint16_t id;
};

struct core {
    struct app_config *app_config;

    struct queue rx_queues[RTE_MAX_ETHPORTS];
    struct queue tx_queues[RTE_MAX_ETHPORTS];
};


int app_config_parse(int argc, char **argv, struct app_config *config);
int arp_handle(struct rte_mbuf *pkt, uint8_t port, struct core *core);

// Utility macros
#define IPv4_FMT            "%i.%i.%i.%i"
#define IPv4_FMTARGS(ip)    ((ip) >> 24) & 0xff,   \
                            ((ip) >> 16) & 0xff,   \
                            ((ip) >>  8) & 0xff,    \
                            ((ip) >>  0) & 0xff

#define MAC_FMT                 "%x%x.%x%x.%x%x"
#define MAC_FMTARGS(ether_addr) (ether_addr).addr_bytes[0], \
                                (ether_addr).addr_bytes[1], \
                                (ether_addr).addr_bytes[2], \
                                (ether_addr).addr_bytes[3], \
                                (ether_addr).addr_bytes[4], \
                                (ether_addr).addr_bytes[5]

#endif