#ifndef BWT_LITE_H_
#define BWT_LITE_H_

#include <stdint.h>
#include "port.h"

typedef struct {
	uint32_t seq_len, bwt_size, n_occ;
	uint32_t primary;
	uint32_t *bwt, *occ, *sa, L2[5];
	uint32_t cnt_table[256];
} bwtl_t;

#define bwtl_B0(b, k) ((b)->bwt[(k)>>4]>>((~(k)&0xf)<<1)&3)

#ifdef __cplusplus
extern "C" {
#endif

	bwtl_t *bwtl_seq2bwtl(int len, const uint8_t *seq);
	myinline uint32_t bwtl_occ(const bwtl_t *bwt, uint32_t k, uint8_t c);
	myinline void bwtl_occ4(const bwtl_t *bwt, uint32_t k, uint32_t cnt[4])
	{
		uint32_t x, b;
		if (k == (uint32_t)(-1)) {
			memset(cnt, 0, 16);
			return;
		}
		if (k >= bwt->primary) --k; // because $ is not in bwt
		memcpy(cnt, bwt->occ + (k>>4<<2), 16);
		b = bwt->bwt[k>>4] & ~((1U<<((~k&15)<<1)) - 1);
		x = bwt->cnt_table[b&0xff] + bwt->cnt_table[b>>8&0xff]
			+ bwt->cnt_table[b>>16&0xff] + bwt->cnt_table[b>>24];
		x -= 15 - (k&15);
		cnt[0] += x&0xff; cnt[1] += x>>8&0xff; cnt[2] += x>>16&0xff; cnt[3] += x>>24;
	}
	myinline void bwtl_2occ4(const bwtl_t *bwt, uint32_t k, uint32_t l, uint32_t cntk[4], uint32_t cntl[4])
	{
		bwtl_occ4(bwt, k, cntk);
		bwtl_occ4(bwt, l, cntl);
	}
	void bwtl_destroy(bwtl_t *bwt);

#ifdef __cplusplus
}
#endif

#endif
