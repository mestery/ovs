#include <net/genetlink.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,13,0)
void genl_notify(struct genl_family *family,
		 struct sk_buff *skb, struct net *net, u32 portid, u32 group,
		 struct nlmsghdr *nlh, gfp_t flags)
{
	struct sock *sk = net->genl_sock;
	int report = 0;
	struct genl_multicast_group *grp;
	int i = 0;

	if (nlh)
		report = nlmsg_report(nlh);

	list_for_each_entry(grp, &family->mcast_groups, list) {
		if (group == grp->id)
			break;
		i++;
	}

	if (WARN_ON_ONCE(grp == NULL))
		return;
	nlmsg_notify(sk, skb, portid, grp->id, report, flags);
}
#endif /* kernel version < 3.13.0 */
