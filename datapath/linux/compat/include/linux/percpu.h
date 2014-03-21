#ifndef __LINUX_PERCPU_WRAP_H
#define __LINUX_PERCPU_WRAP_H

#include <linux/version.h>

#include_next <linux/percpu.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
struct dp_stats_percpu;
struct pcpu_tstats;
void ovs_init_per_cpu_dp_stats(struct dp_stats_percpu *);
void ovs_init_per_cpu_t_stats(struct pcpu_tstats *);
#else
#define ovs_init_per_cpu_dp_stats(a) (void)(a)
#define ovs_init_per_cpu_t_stats(a) (void)(a)
#endif /* KERNEL_VERSION(3,13,0) */

#endif /* __LINUX_PERCPU_WRAP_H */
