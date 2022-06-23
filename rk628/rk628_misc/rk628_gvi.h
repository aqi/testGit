// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2020 Rockchip Electronics Co. Ltd.
 *
 * Author: Guochun Huang<hero.huang@rock-chips.com>
 */

#ifndef RK628_GVI_H
#define RK628_GVI_H

#include "rk628.h"

#define GVI_BASE					0x80000
#define HOSTREG(x)					((x) + GVI_BASE)
#define GVI_SYS_CTRL0					HOSTREG(0x0000)
#define GVI_SYS_CTRL1					HOSTREG(0x0004)
#define GVI_SYS_CTRL2					HOSTREG(0x0008)
#define GVI_SYS_CTRL3					HOSTREG(0x000c)
#define GVI_VERSION					HOSTREG(0x0010)
#define GVI_SYS_RST					HOSTREG(0x0014)
#define GVI_LINE_FLAG					HOSTREG(0x0018)
#define GVI_STATUS					HOSTREG(0x001c)
#define GVI_PLL_LOCK_TIMEOUT				HOSTREG(0x0030)
#define GVI_HTPDN_TIMEOUT				HOSTREG(0x0034)
#define GVI_LOCKN_TIMEOUT				HOSTREG(0x0038)
#define GVI_WAIT_LOCKN					HOSTREG(0x003C)
#define GVI_WAIT_HTPDN					HOSTREG(0x0040)
#define GVI_INTR_EN					HOSTREG(0x0050)
#define GVI_INTR_CLR					HOSTREG(0x0054)
#define GVI_INTR_RAW_STATUS				HOSTREG(0x0058)
#define GVI_INTR_STATUS					HOSTREG(0x005c)
#define GVI_COLOR_BAR_CTRL				HOSTREG(0x0060)
#define GVI_COLOR_BAR_HTIMING0				HOSTREG(0x0070)
#define GVI_COLOR_BAR_HTIMING1				HOSTREG(0x0074)
#define GVI_COLOR_BAR_VTIMING0				HOSTREG(0x0078)
#define GVI_COLOR_BAR_VTIMING1				HOSTREG(0x007c)

/* SYS_CTRL0 */
#define SYS_CTRL0_GVI_EN				BIT(0)
#define SYS_CTRL0_AUTO_GATING				BIT(1)
#define SYS_CTRL0_FRM_RST_EN				BIT(2)
#define SYS_CTRL0_FRM_RST_MODE				BIT(3)
#define SYS_CTRL0_LANE_NUM_MASK				GENMASK(7, 4)
#define SYS_CTRL0_LANE_NUM(x)				UPDATE(x, 7, 4)
#define SYS_CTRL0_BYTE_MODE_MASK			GENMASK(9, 8)
#define SYS_CTRL0_BYTE_MODE(x)				UPDATE(x, 9, 8)
#define SYS_CTRL0_SECTION_NUM_MASK			GENMASK(11, 10)
#define SYS_CTRL0_SECTION_NUM(x)			UPDATE(x, 11, 10)
#define SYS_CTRL0_CDR_ENDIAN_SWAP			BIT(12)
#define SYS_CTRL0_PACK_BYTE_SWAP			BIT(13)
#define SYS_CTRL0_PACK_ENDIAN_SWAP			BIT(14)
#define SYS_CTRL0_ENC8B10B_ENDIAN_SWAP			BIT(15)
#define SYS_CTRL0_CDR_EN				BIT(16)
#define SYS_CTRL0_ALN_EN				BIT(17)
#define SYS_CTRL0_NOR_EN				BIT(18)
#define SYS_CTRL0_ALN_NOR_MODE				BIT(19)
#define SYS_CTRL0_GVI_MASK				GENMASK(19, 16)
#define SYS_CTRL0_GVI_GN_EN(x)				UPDATE(x, 19, 16)

#define SYS_CTRL0_SCRAMBLER_EN				BIT(20)
#define SYS_CTRL0_ENCODE8B10B_EN			BIT(21)
#define SYS_CTRL0_INIT_RD_EN				BIT(22)
#define SYS_CTRL0_INIT_RD_VALUE				BIT(23)
#define SYS_CTRL0_FORCE_HTPDN_EN			BIT(24)
#define SYS_CTRL0_FORCE_HTPDN_VALUE			BIT(25)
#define SYS_CTRL0_FORCE_PLL_EN				BIT(26)
#define SYS_CTRL0_FORCE_PLL_VALUE			BIT(27)
#define SYS_CTRL0_FORCE_LOCKN_EN			BIT(28)
#define SYS_CTRL0_FORCE_LOCKN_VALUE			BIT(29)

/* SYS_CTRL1 */
#define SYS_CTRL1_COLOR_DEPTH_MASK			GENMASK(3, 0)
#define SYS_CTRL1_COLOR_DEPTH(x)			UPDATE(x, 3, 0)
#define SYS_CTRL1_DUAL_PIXEL_EN				BIT(4)
#define SYS_CTRL1_TIMING_ALIGN_EN			BIT(8)
#define SYS_CTRL1_LANE_ALIGN_EN				BIT(9)

#define SYS_CTRL1_DUAL_PIXEL_SWAP			BIT(12)
#define SYS_CTRL1_RB_SWAP				BIT(13)
#define SYS_CTRL1_YC_SWAP				BIT(14)
#define SYS_CTRL1_WHOLE_FRM_EN				BIT(16)
#define SYS_CTRL1_NOR_PROTECT				BIT(17)
#define SYS_CTRL1_RD_WCNT_UPDATE			BIT(31)

/* SYS_CTRL2 */
#define SYS_CTRL2_AFIFO_READ_THOLD_MASK			GENMASK(7, 0)
#define SYS_CTRL2_AFIFO_READ_THOLD(x)			UPDATE(x, 7, 0)
#define SYS_CTRL2_AFIFO_ALMOST_FULL_THOLD_MASK		GENMASK(23, 16)
#define SYS_CTRL2_AFIFO_ALMOST_FULL_THOLD(x)		UPDATE(x, 23, 16)
#define SYS_CTRL2_AFIFO_ALMOST_EMPTY_THOLD_MASK		GENMASK(31, 24)
#define SYS_CTRL2_AFIFO_ALMOST_EMPTY_THOLD(x)		UPDATE(x, 31, 24)

/* SYS_CTRL3 */
#define SYS_CTRL3_LANE0_SEL_MASK			GENMASK(2, 0)
#define SYS_CTRL3_LANE0_SEL(x)				UPDATE(x, 2, 0)
#define SYS_CTRL3_LANE1_SEL_MASK			GENMASK(6, 4)
#define SYS_CTRL3_LANE1_SEL(x)				UPDATE(x, 6, 4)
#define SYS_CTRL3_LANE2_SEL_MASK			GENMASK(10, 8)
#define SYS_CTRL3_LANE2_SEL(x)				UPDATE(x, 10, 8)
#define SYS_CTRL3_LANE3_SEL_MASK			GENMASK(14, 12)
#define SYS_CTRL3_LANE3_SEL(x)				UPDATE(x, 14, 12)
#define SYS_CTRL3_LANE4_SEL_MASK			GENMASK(18, 16)
#define SYS_CTRL3_LANE4_SEL(x)				UPDATE(x, 18, 16)
#define SYS_CTRL3_LANE5_SEL_MASK			GENMASK(22, 20)
#define SYS_CTRL3_LANE5_SEL(x)				UPDATE(x, 22, 20)
#define SYS_CTRL3_LANE6_SEL_MASK			GENMASK(26, 24)
#define SYS_CTRL3_LANE6_SEL(x)				UPDATE(x, 26, 24)
#define SYS_CTRL3_LANE7_SEL_MASK			GENMASK(30, 28)
#define SYS_CTRL3_LANE7_SEL(x)				UPDATE(x, 30, 28)
/* VERSIION */
#define VERSION_VERSION(x)				UPDATE(x, 31, 0)
/* SYS_RESET*/
#define SYS_RST_SOFT_RST				BIT(0)
/* LINE_FLAG */
#define LINE_FLAG_LANE_FLAG0_MASK			GENMASK(15, 0)
#define LINE_FLAG_LANE_FLAG0(x)				UPDATE(x, 15, 0)
#define LINE_FLAG_LANE_FLAG1_MASK			GENMASK(31, 16)
#define LINE_FLAG_LANE_FLAG1(x)				UPDATE(x, 31, 16)
/* STATUS */
#define STATUS_HTDPN					BIT(4)
#define STATUS_LOCKN					BIT(5)
#define STATUS_PLL_LOCKN				BIT(6)
#define STATUS_AFIFO0_WCNT_MASK				GENMASK(23, 16)
#define STATUS_AFIFO0_WCNT(x)				UPDATE(x, 23, 16)
#define STATUS_AFIFO1_WCNT_MASK				GENMASK(31, 24)
#define STATUS_AFIFO1_WCNT(x)				UPDATE(x, 31, 24)
/* PLL_LTIMEOUT */
#define PLL_LOCK_TIMEOUT_PLL_LOCK_TIME_OUT_MASK		GENMASK(31, 0)
#define PLL_LOCK_TIMEOUT_PLL_LOCK_TIME_OUT(x)		UPDATE(x, 31, 0)
/* HTPDNEOUT */
#define HTPDN_TIMEOUT_HTPDN_TIME_OUT_MASK		GENMASK(31, 0)
#define HTPDN_TIMEOUT_HTPDN_TIME_OUT(x)			UPDATE(x, 31, 0)
/* LOCKNEOUT */
#define LOCKN_TIMEOUT_LOCKN_TIME_OUT_MASK		GENMASK(31, 0)
#define LOCKN_TIMEOUT_LOCKN_TIME_OUT(x)			UPDATE(x, 31, 0)
/* WAIT_LOCKN */
#define WAIT_LOCKN_WAIT_LOCKN_TIME_MASK			GENMASK(30, 0)
#define WAIT_LOCKN_WAIT_LOCKN_TIME(x)			UPDATE(x, 30, 0)
#define WAIT_LOCKN_WAIT_LOCKN_TIME_EN			BIT(31)
/* WAIT_HTPDN */
#define WAIT_HTPDN_WAIT_HTPDN_TIME_MASK			GENMASK(30, 0)
#define WAIT_HTPDN_WAIT_HTPDN_TIME(x)			UPDATE(x, 30, 0)
#define WAIT_HTPDN_WAIT_HTPDN_EN			BIT(31)
/* INTR_EN */
#define INTR_EN_INTR_FRM_ST_EN				BIT(0)
#define INTR_EN_INTR_PLL_LOCK_EN			BIT(1)
#define INTR_EN_INTR_HTPDN_EN				BIT(2)
#define INTR_EN_INTR_LOCKN_EN				BIT(3)
#define INTR_EN_INTR_PLL_TIMEOUT_EN			BIT(4)
#define INTR_EN_INTR_HTPDN_TIMEOUT_EN			BIT(5)
#define INTR_EN_INTR_LOCKN_TIMEOUT_EN			BIT(6)
#define INTR_EN_INTR_LINE_FLAG0_EN			BIT(8)
#define INTR_EN_INTR_LINE_FLAG1_EN			BIT(9)
#define INTR_EN_INTR_AFIFO_OVERFLOW_EN			BIT(10)
#define INTR_EN_INTR_AFIFO_UNDERFLOW_EN			BIT(11)
#define INTR_EN_INTR_PLL_ERR_EN				BIT(12)
#define INTR_EN_INTR_HTPDN_ERR_EN			BIT(13)
#define INTR_EN_INTR_LOCKN_ERR_EN			BIT(14)
/* INTR_CLR*/
#define INTR_CLR_INTR_FRM_ST_CLR			BIT(0)
#define INTR_CLR_INTR_PLL_LOCK_CLR			BIT(1)
#define INTR_CLR_INTR_HTPDN_CLR				BIT(2)
#define INTR_CLR_INTR_LOCKN_CLR				BIT(3)
#define INTR_CLR_INTR_PLL_TIMEOUT_CLR			BIT(4)
#define INTR_CLR_INTR_HTPDN_TIMEOUT_CLR			BIT(5)
#define INTR_CLR_INTR_LOCKN_TIMEOUT_CLR			BIT(6)
#define INTR_CLR_INTR_LINE_FLAG0_CLR			BIT(8)
#define INTR_CLR_INTR_LINE_FLAG1_CLR			BIT(9)
#define INTR_CLR_INTR_AFIFO_OVERFLOW_CLR		BIT(10)
#define INTR_CLR_INTR_AFIFO_UNDERFLOW_CLR		BIT(11)
#define INTR_CLR_INTR_PLL_ERR_CLR			BIT(12)
#define INTR_CLR_INTR_HTPDN_ERR_CLR			BIT(13)
#define INTR_CLR_INTR_LOCKN_ERR_CLR			BIT(14)
/* INTR_RAW_STATUS */
#define INTR_RAW_STATUS_RAW_INTR_FRM_ST			BIT(0)
#define INTR_RAW_STATUS_RAW_INTR_PLL_LOCK		BIT(1)
#define INTR_RAW_STATUS_RAW_INTR_HTPDN			BIT(2)
#define INTR_RAW_STATUS_RAW_INTR_LOCKN			BIT(3)
#define INTR_RAW_STATUS_RAW_INTR_PLL_TIMEOUT		BIT(4)
#define INTR_RAW_STATUS_RAW_INTR_HTPDN_TIMEOUT		BIT(5)
#define INTR_RAW_STATUS_RAW_INTR_LOCKN_TIMEOUT		BIT(6)
#define INTR_RAW_STATUS_RAW_INTR_LINE_FLAG0		BIT(8)
#define INTR_RAW_STATUS_RAW_INTR_LINE_FLAG1		BIT(9)
#define INTR_RAW_STATUS_RAW_INTR_AFIFO_OVERFLOW		BIT(10)
#define INTR_RAW_STATUS_RAW_INTR_AFIFO_UNDERFLOW	BIT(11)
#define INTR_RAW_STATUS_RAW_INTR_PLL_ERR		BIT(12)
#define INTR_RAW_STATUS_RAW_INTR_HTPDN_ERR		BIT(13)
#define INTR_RAW_STATUS_RAW_INTR_LOCKN_ERR		BIT(14)
/* INTR_STATUS */
#define INTR_STATUS_INTR_FRM_ST				BIT(0)
#define INTR_STATUS_INTR_PLL_LOCK			BIT(1)
#define INTR_STATUS_INTR_HTPDN				BIT(2)
#define INTR_STATUS_INTR_LOCKN				BIT(3)
#define INTR_STATUS_INTR_PLL_TIMEOUT			BIT(4)
#define INTR_STATUS_INTR_HTPDN_TIMEOUT			BIT(5)
#define INTR_STATUS_INTR_LOCKN_TIMEOUT			BIT(6)
#define INTR_STATUS_INTR_LINE_FLAG0			BIT(8)
#define INTR_STATUS_INTR_LINE_FLAG1			BIT(9)
#define INTR_STATUS_INTR_AFIFO_OVERFLOW			BIT(10)
#define INTR_STATUS_INTR_AFIFO_UNDERFLOW		BIT(11)
#define INTR_STATUS_INTR_PLL_ERR			BIT(12)
#define INTR_STATUS_INTR_HTPDN_ERR			BIT(13)
#define INTR_STATUS_INTR_LOCKN_ERR			BIT(14)

/* COLOR_BAR_CTRL */
#define COLOR_BAR_EN					BIT(0)

#define COLOR_DEPTH_RGB_YUV444_18BIT			0
#define COLOR_DEPTH_RGB_YUV444_24BIT			1
#define COLOR_DEPTH_RGB_YUV444_30BIT			2
#define COLOR_DEPTH_YUV422_16BIT			8
#define COLOR_DEPTH_YUV422_20BIT			9

void rk628_gvi_enable(struct rk628 *rk628);
void rk628_gvi_disable(struct rk628 *rk628);

#endif
