#ifndef __NET_GENERIC_NETLINK_WRAPPER_H
#define __NET_GENERIC_NETLINK_WRAPPER_H 1

#include <linux/version.h>
#include <linux/netlink.h>
#include <net/net_namespace.h>
#include_next <net/genetlink.h>

/*
 * 15e473046cb6e5d18a4d0057e61d76315230382b renames pid to portid
 * the affected structures are
 * netlink_skb_parms::pid -> portid
 * genl_info::snd_pid -> snd_portid
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
#define snd_portid snd_pid
#define portid pid
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,13,0)
#define genl_notify rpl__genl_notify
void genl_notify(struct genl_family *family,
		 struct sk_buff *skb, struct net *net, u32 portid, u32 group,
		 struct nlmsghdr *nlh, gfp_t flags);

#define genl_set_err rpl__genl_set_err
static inline int genl_set_err(struct genl_family *family, struct net *net,
			       u32 portid, u32 group, int code)
{
	struct genl_multicast_group *grp;

	list_for_each_entry(grp, &family->mcast_groups, list) {
		if (group == grp->id)
			break;
	}

	return netlink_set_err(net->genl_sock, portid, grp->id, code);
}

#define genlmsg_multicast_netns rpl__genlmsg_multicast_netns
static inline int genlmsg_multicast_netns(struct genl_family *family,
					  struct net *net, struct sk_buff *skb,
					  u32 portid, unsigned int group, gfp_t flags)
{
	struct genl_multicast_group *grp;

	list_for_each_entry(grp, &family->mcast_groups, list) {
		if (group == grp->id)
			break;
	}

	if (WARN_ON_ONCE(grp == NULL))
		return -EINVAL;
	return nlmsg_multicast(net->genl_sock, skb, portid, grp->id, flags);
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,14,0)
static inline struct sk_buff *genlmsg_new_unicast(size_t payload,
						  struct genl_info *info,
						  gfp_t flags)
{
	return genlmsg_new(payload, flags);
}
#endif

struct genl_family_and_ops_ovs {
	struct genl_family *family;
	struct genl_ops *ops;
	int n_ops;
	struct genl_multicast_group *group;
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
static inline int genl_register_family_ovs(const struct genl_family_and_ops_ovs *f)
{
	f->family->ops = f->ops;
	f->family->n_ops = f->n_ops;
	f->family->mcgrps = f->group;
	f->family->n_mcgrps = f->group ? 1 : 0;

	return genl_register_family(f->family);
}
#else
static inline int genl_register_family_ovs(const struct genl_family_and_ops_ovs *f)
{
	int err;

	err = genl_register_family_with_ops(f->family, f->ops, f->n_ops);
	if (err)
		goto error;

	if (f->group) {
		err = genl_register_mc_group(f->family, f->group);
		if (err)
			goto error;
	}
error:
	return err;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
static inline int netlink_has_listeners_ovs(struct sock *genl_sock,
					    const struct genl_multicast_group *grp,
					    unsigned int id)
{
	/* Unused argument */
	(void)(grp);

	return netlink_has_listeners(genl_sock, id);
}
#else
static inline int netlink_has_listeners_ovs(struct sock *genl_sock,
					    const struct genl_multicast_group *grp,
					    unsigned int id)
{
	/* Unused arguments */
	(void)(id);

	return netlink_has_listeners(genl_sock, grp->id);
}
#endif
#endif /* genetlink.h */
