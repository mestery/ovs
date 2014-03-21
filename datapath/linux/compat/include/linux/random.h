#ifndef __LINUX_RANDOM_WRAP_H
#define __LINUX_RANDOM_WRAP_H

#include <linux/version.h>

#include_next <linux/random.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,6,12)
#define prandom_u32 random32
#endif /* KERNEL_VERSION(3,13,0) */

#endif /* __LINUX_RANDOM_WRAP_H */
