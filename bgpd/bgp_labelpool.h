// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * BGP Label Pool - Manage label chunk allocations from zebra asynchronously
 *
 * Copyright (C) 2018 LabN Consulting, L.L.C.
 */

#ifndef _FRR_BGP_LABELPOOL_H
#define _FRR_BGP_LABELPOOL_H

#include <zebra.h>

#include "mpls.h"

/*
 * Types used in bgp_lp_get for debug tracking; add more as needed
 */
#define LP_TYPE_VRF	0x00000001
#define LP_TYPE_BGP_LU	0x00000002

PREDECL_LIST(lp_fifo);

struct labelpool {
	struct skiplist		*ledger;	/* all requests */
	struct skiplist		*inuse;		/* individual labels */
	struct list		*chunks;	/* granted by zebra */
	struct lp_fifo_head	requests;	/* blocked on zebra */
	struct work_queue	*callback_q;
	uint32_t		pending_count;	/* requested from zebra */
	uint32_t reconnect_count;		/* zebra reconnections */
	uint32_t next_chunksize;		/* request this many labels */
};

extern void bgp_lp_init(struct event_loop *master, struct labelpool *pool);
extern void bgp_lp_finish(void);
extern void bgp_lp_get(int type, void *labelid,
	int (*cbfunc)(mpls_label_t label, void *labelid, bool allocated));
extern void bgp_lp_release(int type, void *labelid, mpls_label_t label);
extern void bgp_lp_event_chunk(uint8_t keep, uint32_t first, uint32_t last);
extern void bgp_lp_event_zebra_down(void);
extern void bgp_lp_event_zebra_up(void);
extern void bgp_lp_vty_init(void);

#endif /* _FRR_BGP_LABELPOOL_H */
