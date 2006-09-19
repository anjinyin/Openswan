/*
 * RFC2367 PF_KEYv2 Key management API message parser
 * Copyright (C) 1999, 2000, 2001  Richard Guy Briggs.
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * RCSID $Id: pfkey_v2_ext_bits.c,v 1.22 2005/05/11 01:45:31 mcr Exp $
 */

/*
 *		Template from klips/net/ipsec/ipsec/ipsec_parse.c.
 */

char pfkey_v2_ext_bits_c_version[] = "$Id: pfkey_v2_ext_bits.c,v 1.22 2005/05/11 01:45:31 mcr Exp $";

/*
 * Some ugly stuff to allow consistent debugging code for use in the
 * kernel and in user space
*/

#if defined(__KERNEL__) && defined(linux)

# include <linux/kernel.h>  /* for printk */

# include "openswan/ipsec_kversion.h" /* for malloc switch */
# ifdef MALLOC_SLAB
#  include <linux/slab.h> /* kmalloc() */
# else /* MALLOC_SLAB */
#  include <linux/malloc.h> /* kmalloc() */
# endif /* MALLOC_SLAB */
# include <linux/errno.h>  /* error codes */
# include <linux/types.h>  /* size_t */
# include <linux/interrupt.h> /* mark_bh */

# include <linux/netdevice.h>   /* struct device, and other headers */
# include <linux/etherdevice.h> /* eth_type_trans */
# include <linux/ip.h>          /* struct iphdr */ 
# if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
#  include <linux/ipv6.h>
# endif /* defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE) */

#else /* __KERNEL__ */

# include <sys/types.h>
# include <sys/errno.h>
# include <stdio.h>
#endif

#include <openswan.h>
#include <openswan/pfkeyv2.h>
#include <openswan/pfkey.h>

unsigned int extensions_bitmaps[2/*in/out*/][2/*perm/req*/][SADB_EXTENSIONS_MAX] = {

/* INBOUND EXTENSIONS */
{

/* PERMITTED IN */
{
/* SADB_RESERVED */
0
,
/* SADB_GETSPI */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_SPIRANGE
,
/* SADB_UPDATE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_X_EXT_NAT_T_SPORT
| 1<<SADB_X_EXT_NAT_T_DPORT
,
/* SADB_ADD */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_X_EXT_NAT_T_TYPE
| 1<<SADB_X_EXT_NAT_T_SPORT
| 1<<SADB_X_EXT_NAT_T_DPORT
| 1<<SADB_X_EXT_NAT_T_OA
,
/* SADB_DELETE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_GET */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_ACQUIRE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_EXT_PROPOSAL
,
/* SADB_REGISTER */
1<<SADB_EXT_RESERVED
,
/* SADB_EXPIRE */
0
,
/* SADB_FLUSH */
1<<SADB_EXT_RESERVED
,
/* SADB_DUMP */
1<<SADB_EXT_RESERVED
,
/* SADB_X_PROMISC */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_EXT_PROPOSAL
| 1<<SADB_EXT_SUPPORTED_AUTH
| 1<<SADB_EXT_SUPPORTED_ENCRYPT
| 1<<SADB_EXT_SPIRANGE
| 1<<SADB_X_EXT_KMPRIVATE
| 1<<SADB_X_EXT_SATYPE2
| 1<<SADB_X_EXT_SA2
| 1<<SADB_X_EXT_ADDRESS_DST2
,
/* SADB_X_PCHANGE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_EXT_PROPOSAL
| 1<<SADB_EXT_SUPPORTED_AUTH
| 1<<SADB_EXT_SUPPORTED_ENCRYPT
| 1<<SADB_EXT_SPIRANGE
| 1<<SADB_X_EXT_KMPRIVATE
| 1<<SADB_X_EXT_SATYPE2
| 1<<SADB_X_EXT_SA2
| 1<<SADB_X_EXT_ADDRESS_DST2
,
/* SADB_X_GRPSA */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_X_EXT_SATYPE2
| 1<<SADB_X_EXT_SA2
| 1<<SADB_X_EXT_ADDRESS_DST2
,
/* SADB_X_ADDFLOW */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_X_EXT_ADDRESS_SRC_FLOW
| 1<<SADB_X_EXT_ADDRESS_DST_FLOW
| 1<<SADB_X_EXT_ADDRESS_SRC_MASK
| 1<<SADB_X_EXT_ADDRESS_DST_MASK
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_X_EXT_PROTOCOL
,
/* SADB_X_DELFLOW */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_X_EXT_ADDRESS_SRC_FLOW
| 1<<SADB_X_EXT_ADDRESS_DST_FLOW
| 1<<SADB_X_EXT_ADDRESS_SRC_MASK
| 1<<SADB_X_EXT_ADDRESS_DST_MASK
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_X_EXT_PROTOCOL
,
/* SADB_X_DEBUG */
1<<SADB_EXT_RESERVED
| 1<<SADB_X_EXT_DEBUG
,
/* SADB_X_NAT_T_NEW_MAPPING */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_X_EXT_NAT_T_SPORT
| 1<<SADB_X_EXT_NAT_T_DPORT
},

/* REQUIRED IN */
{
/* SADB_RESERVED */
0
,
/* SADB_GETSPI */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_SPIRANGE
,
/* SADB_UPDATE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
/*| 1<<SADB_EXT_KEY_AUTH*/
/*| 1<<SADB_EXT_KEY_ENCRYPT*/
,
/* SADB_ADD */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
/*| 1<<SADB_EXT_KEY_AUTH*/
/*| 1<<SADB_EXT_KEY_ENCRYPT*/
,
/* SADB_DELETE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_GET */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_ACQUIRE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_PROPOSAL
,
/* SADB_REGISTER */
1<<SADB_EXT_RESERVED
,
/* SADB_EXPIRE */
0
,
/* SADB_FLUSH */
1<<SADB_EXT_RESERVED
,
/* SADB_DUMP */
1<<SADB_EXT_RESERVED
,
/* SADB_X_PROMISC */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_EXT_PROPOSAL
| 1<<SADB_EXT_SUPPORTED_AUTH
| 1<<SADB_EXT_SUPPORTED_ENCRYPT
| 1<<SADB_EXT_SPIRANGE
| 1<<SADB_X_EXT_KMPRIVATE
| 1<<SADB_X_EXT_SATYPE2
| 1<<SADB_X_EXT_SA2
| 1<<SADB_X_EXT_ADDRESS_DST2
,
/* SADB_X_PCHANGE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_EXT_PROPOSAL
| 1<<SADB_EXT_SUPPORTED_AUTH
| 1<<SADB_EXT_SUPPORTED_ENCRYPT
| 1<<SADB_EXT_SPIRANGE
| 1<<SADB_X_EXT_KMPRIVATE
| 1<<SADB_X_EXT_SATYPE2
| 1<<SADB_X_EXT_SA2
| 1<<SADB_X_EXT_ADDRESS_DST2
,
/* SADB_X_GRPSA */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_DST
/*| 1<<SADB_X_EXT_SATYPE2*/
/*| 1<<SADB_X_EXT_SA2*/
/*| 1<<SADB_X_EXT_ADDRESS_DST2*/
,
/* SADB_X_ADDFLOW */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_X_EXT_ADDRESS_SRC_FLOW
| 1<<SADB_X_EXT_ADDRESS_DST_FLOW
| 1<<SADB_X_EXT_ADDRESS_SRC_MASK
| 1<<SADB_X_EXT_ADDRESS_DST_MASK
,
/* SADB_X_DELFLOW */
1<<SADB_EXT_RESERVED
/*| 1<<SADB_EXT_SA*/
#if 0 /* SADB_X_CLREROUTE doesn't need all these... */
| 1<<SADB_X_EXT_ADDRESS_SRC_FLOW
| 1<<SADB_X_EXT_ADDRESS_DST_FLOW
| 1<<SADB_X_EXT_ADDRESS_SRC_MASK
| 1<<SADB_X_EXT_ADDRESS_DST_MASK
#endif
,
/* SADB_X_DEBUG */
1<<SADB_EXT_RESERVED
| 1<<SADB_X_EXT_DEBUG
,
/* SADB_X_NAT_T_NEW_MAPPING */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_X_EXT_NAT_T_SPORT
| 1<<SADB_X_EXT_NAT_T_DPORT
}

},

/* OUTBOUND EXTENSIONS */
{

/* PERMITTED OUT */
{
/* SADB_RESERVED */
0
,
/* SADB_GETSPI */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_UPDATE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_X_EXT_NAT_T_SPORT
| 1<<SADB_X_EXT_NAT_T_DPORT
,
/* SADB_ADD */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_X_EXT_NAT_T_TYPE
| 1<<SADB_X_EXT_NAT_T_SPORT
| 1<<SADB_X_EXT_NAT_T_DPORT
| 1<<SADB_X_EXT_NAT_T_OA
,
/* SADB_DELETE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_GET */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_X_EXT_NAT_T_TYPE
| 1<<SADB_X_EXT_NAT_T_SPORT
| 1<<SADB_X_EXT_NAT_T_DPORT
| 1<<SADB_X_EXT_NAT_T_OA
,
/* SADB_ACQUIRE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_EXT_PROPOSAL
,
/* SADB_REGISTER */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SUPPORTED_AUTH
| 1<<SADB_EXT_SUPPORTED_ENCRYPT
,
/* SADB_EXPIRE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_FLUSH */
1<<SADB_EXT_RESERVED
,
/* SADB_DUMP */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_X_EXT_NAT_T_TYPE
| 1<<SADB_X_EXT_NAT_T_SPORT
| 1<<SADB_X_EXT_NAT_T_DPORT
| 1<<SADB_X_EXT_NAT_T_OA
,
/* SADB_X_PROMISC */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_EXT_PROPOSAL
| 1<<SADB_EXT_SUPPORTED_AUTH
| 1<<SADB_EXT_SUPPORTED_ENCRYPT
| 1<<SADB_EXT_SPIRANGE
| 1<<SADB_X_EXT_KMPRIVATE
| 1<<SADB_X_EXT_SATYPE2
| 1<<SADB_X_EXT_SA2
| 1<<SADB_X_EXT_ADDRESS_DST2
,
/* SADB_X_PCHANGE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_EXT_PROPOSAL
| 1<<SADB_EXT_SUPPORTED_AUTH
| 1<<SADB_EXT_SUPPORTED_ENCRYPT
| 1<<SADB_EXT_SPIRANGE
| 1<<SADB_X_EXT_KMPRIVATE
| 1<<SADB_X_EXT_SATYPE2
| 1<<SADB_X_EXT_SA2
| 1<<SADB_X_EXT_ADDRESS_DST2
,
/* SADB_X_GRPSA */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_X_EXT_SATYPE2
| 1<<SADB_X_EXT_SA2
| 1<<SADB_X_EXT_ADDRESS_DST2
,
/* SADB_X_ADDFLOW */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_X_EXT_ADDRESS_SRC_FLOW
| 1<<SADB_X_EXT_ADDRESS_DST_FLOW
| 1<<SADB_X_EXT_ADDRESS_SRC_MASK
| 1<<SADB_X_EXT_ADDRESS_DST_MASK
| 1<<SADB_X_EXT_PROTOCOL
,
/* SADB_X_DELFLOW */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_X_EXT_ADDRESS_SRC_FLOW
| 1<<SADB_X_EXT_ADDRESS_DST_FLOW
| 1<<SADB_X_EXT_ADDRESS_SRC_MASK
| 1<<SADB_X_EXT_ADDRESS_DST_MASK
| 1<<SADB_X_EXT_PROTOCOL
,
/* SADB_X_DEBUG */
1<<SADB_EXT_RESERVED
| 1<<SADB_X_EXT_DEBUG
,
/* SADB_X_NAT_T_NEW_MAPPING */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_X_EXT_NAT_T_SPORT
| 1<<SADB_X_EXT_NAT_T_DPORT
},

/* REQUIRED OUT */
{
/* SADB_RESERVED */
0
,
/* SADB_GETSPI */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_UPDATE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_ADD */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_DELETE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_GET */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
/* | 1<<SADB_EXT_KEY_AUTH */
/* | 1<<SADB_EXT_KEY_ENCRYPT */
,
/* SADB_ACQUIRE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_PROPOSAL
,
/* SADB_REGISTER */
1<<SADB_EXT_RESERVED
/* | 1<<SADB_EXT_SUPPORTED_AUTH
   | 1<<SADB_EXT_SUPPORTED_ENCRYPT */
,
/* SADB_EXPIRE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
/* | 1<<SADB_EXT_LIFETIME_HARD
   | 1<<SADB_EXT_LIFETIME_SOFT */
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_FLUSH */
1<<SADB_EXT_RESERVED
,
/* SADB_DUMP */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
,
/* SADB_X_PROMISC */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_EXT_PROPOSAL
| 1<<SADB_EXT_SUPPORTED_AUTH
| 1<<SADB_EXT_SUPPORTED_ENCRYPT
| 1<<SADB_EXT_SPIRANGE
| 1<<SADB_X_EXT_KMPRIVATE
| 1<<SADB_X_EXT_SATYPE2
| 1<<SADB_X_EXT_SA2
| 1<<SADB_X_EXT_ADDRESS_DST2
,
/* SADB_X_PCHANGE */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_LIFETIME_CURRENT
| 1<<SADB_EXT_LIFETIME_HARD
| 1<<SADB_EXT_LIFETIME_SOFT
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_EXT_ADDRESS_PROXY
| 1<<SADB_EXT_KEY_AUTH
| 1<<SADB_EXT_KEY_ENCRYPT
| 1<<SADB_EXT_IDENTITY_SRC
| 1<<SADB_EXT_IDENTITY_DST
| 1<<SADB_EXT_SENSITIVITY
| 1<<SADB_EXT_PROPOSAL
| 1<<SADB_EXT_SUPPORTED_AUTH
| 1<<SADB_EXT_SUPPORTED_ENCRYPT
| 1<<SADB_EXT_SPIRANGE
| 1<<SADB_X_EXT_KMPRIVATE
| 1<<SADB_X_EXT_SATYPE2
| 1<<SADB_X_EXT_SA2
| 1<<SADB_X_EXT_ADDRESS_DST2
,
/* SADB_X_GRPSA */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_DST
,
/* SADB_X_ADDFLOW */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_X_EXT_ADDRESS_SRC_FLOW
| 1<<SADB_X_EXT_ADDRESS_DST_FLOW
| 1<<SADB_X_EXT_ADDRESS_SRC_MASK
| 1<<SADB_X_EXT_ADDRESS_DST_MASK
,
/* SADB_X_DELFLOW */
1<<SADB_EXT_RESERVED
/*| 1<<SADB_EXT_SA*/
| 1<<SADB_X_EXT_ADDRESS_SRC_FLOW
| 1<<SADB_X_EXT_ADDRESS_DST_FLOW
| 1<<SADB_X_EXT_ADDRESS_SRC_MASK
| 1<<SADB_X_EXT_ADDRESS_DST_MASK
,
/* SADB_X_DEBUG */
1<<SADB_EXT_RESERVED
| 1<<SADB_X_EXT_DEBUG
,
/* SADB_X_NAT_T_NEW_MAPPING */
1<<SADB_EXT_RESERVED
| 1<<SADB_EXT_SA
| 1<<SADB_EXT_ADDRESS_SRC
| 1<<SADB_EXT_ADDRESS_DST
| 1<<SADB_X_EXT_NAT_T_SPORT
| 1<<SADB_X_EXT_NAT_T_DPORT
}
}
};

/*
 * $Log: pfkey_v2_ext_bits.c,v $
 * Revision 1.22  2005/05/11 01:45:31  mcr
 * 	make pfkey.h standalone.
 *
 * Revision 1.21  2004/07/10 07:48:36  mcr
 * Moved from linux/lib/libfreeswan/pfkey_v2_ext_bits.c,v
 *
 * Revision 1.20  2004/03/08 01:59:08  ken
 * freeswan.h -> openswan.h
 *
 * Revision 1.19  2003/12/22 21:38:13  mcr
 * 	removed extraenous #endif.
 *
 * Revision 1.18  2003/12/22 19:34:41  mcr
 * 	added 0.6c NAT-T patch.
 *
 * Revision 1.17  2003/12/10 01:20:19  mcr
 * 	NAT-traversal patches to KLIPS.
 *
 * Revision 1.16  2003/10/31 02:27:12  mcr
 * 	pulled up port-selector patches and sa_id elimination.
 *
 * Revision 1.15.30.1  2003/09/21 13:59:44  mcr
 * 	pre-liminary X.509 patch - does not yet pass tests.
 *
 * Revision 1.15  2002/04/24 07:55:32  mcr
 * 	#include patches and Makefiles for post-reorg compilation.
 *
 * Revision 1.14  2002/04/24 07:36:40  mcr
 * Moved from ./lib/pfkey_v2_ext_bits.c,v
 *
 * Revision 1.13  2002/01/29 22:25:36  rgb
 * Re-add ipsec_kversion.h to keep MALLOC happy.
 *
 * Revision 1.12  2002/01/29 01:59:10  mcr
 * 	removal of kversions.h - sources that needed it now use ipsec_param.h.
 * 	updating of IPv6 structures to match latest in6.h version.
 * 	removed dead code from openswan.h that also duplicated kversions.h
 * 	code.
 *
 * Revision 1.11  2001/10/18 04:45:24  rgb
 * 2.4.9 kernel deprecates linux/malloc.h in favour of linux/slab.h,
 * lib/openswan.h version macros moved to lib/kversions.h.
 * Other compiler directive cleanups.
 *
 * Revision 1.10  2001/09/08 21:13:35  rgb
 * Added pfkey ident extension support for ISAKMPd. (NetCelo)
 *
 * Revision 1.9  2001/06/14 19:35:16  rgb
 * Update copyright date.
 *
 * Revision 1.8  2001/03/26 23:07:36  rgb
 * Remove requirement for auth and enc key from UPDATE.
 *
 * Revision 1.7  2000/09/12 22:35:37  rgb
 * Restructured to remove unused extensions from CLEARFLOW messages.
 *
 * Revision 1.6  2000/09/09 06:39:01  rgb
 * Added comments for clarity.
 *
 * Revision 1.5  2000/06/02 22:54:14  rgb
 * Added Gerhard Gessler's struct sockaddr_storage mods for IPv6 support.
 *
 * Revision 1.4  2000/01/21 06:27:56  rgb
 * Added address cases for eroute flows.
 * Added comments for each message type.
 * Added klipsdebug switching capability.
 * Fixed GRPSA bitfields.
 *
 * Revision 1.3  1999/12/01 22:20:27  rgb
 * Remove requirement for a proxy address in an incoming getspi message.
 *
 * Revision 1.2  1999/11/27 11:57:06  rgb
 * Consolidated the 4 1-d extension bitmap arrays into one 4-d array.
 * Add CVS log entry to bottom of file.
 * Cleaned out unused bits.
 *
 */
