#ifndef __NET_IP_WRAPPER_H
#define __NET_IP_WRAPPER_H 1

#include_next <net/ip.h>

#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,1,0)
static inline bool ip_is_fragment(const struct iphdr *iph)
{
	return (iph->frag_off & htons(IP_MF | IP_OFFSET)) != 0;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
#define inet_get_local_port_range_ovs(a, b, c) \
	inet_get_local_port_range(a, b, c)
#else
static inline void inet_get_local_port_range_ovs(struct net *net, int *low,
						 int *high)
{
	(void)(net);
	inet_get_local_port_range(low, high);
}
#endif

#endif
