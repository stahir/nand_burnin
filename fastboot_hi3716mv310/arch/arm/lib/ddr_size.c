/******************************************************************************
 *    COPYRIGHT (C) 2012 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2012-12-22
 *
******************************************************************************/

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/sizes.h>
#include <asm/arch/platform.h>
#include <version.h>

#define _16M        0x1000000
/*****************************************************************************/
/* Use this feature, Dcache should be disable */
unsigned int get_ddr_size(void)
{
#define TO_UINT32(_p)   (*(volatile unsigned int *)(_p))

	static unsigned int ddr_size = 0;
	volatile unsigned char *memskip;
	volatile unsigned char *membase = (unsigned char *)MEM_BASE_DDR;
	unsigned int orgin = TO_UINT32(membase);
	unsigned int rd_origin = 0, rd_verify = 0;

	if (ddr_size)
		return ddr_size;

	for (memskip = membase + _16M;
	     memskip <= membase + get_max_ddr_size();
	     memskip += _16M) {

		TO_UINT32(membase) = 0xA9A9A9A9;
		rd_origin = TO_UINT32(memskip);

		TO_UINT32(membase) = 0x53535352;
		rd_verify = TO_UINT32(memskip);

		if (rd_origin != rd_verify) {
			ddr_size = (unsigned int)(memskip - membase);
			break;
		}

		if (_HI3798CV100A == get_chipid()
			|| _HI3798CV100 == get_chipid()
			|| _HI3796CV100 == get_chipid()) {
			if ((unsigned int)(memskip - membase) >= (SZ_2G - SZ_16M)) {
				ddr_size = SZ_2G;
				break;
			}
		}
	}

	/* restore membase value. */
	TO_UINT32(membase) = orgin;
	return ddr_size;
}

