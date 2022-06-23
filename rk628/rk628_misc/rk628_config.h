// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 *
 * Author: Guochun Huang <hero.huang@rock-chips.com>
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "rk628.h"

#define DRM_MODE_FLAG_PHSYNC                    (1<<0)
#define DRM_MODE_FLAG_NHSYNC                    (1<<1)
#define DRM_MODE_FLAG_PVSYNC                    (1<<2)
#define DRM_MODE_FLAG_NVSYNC                    (1<<3)

struct rk628_display_mode {
	int clock; /* in kHz */
	int hdisplay;
	int hsync_start;
	int hsync_end;
	int htotal;
	int vdisplay;
	int vsync_start;
	int vsync_end;
	int vtotal;
	unsigned int flags;
};

struct rk628_display_mode *rk628_display_get_src_mode(void);
struct rk628_display_mode *rk628_display_get_dst_mode(void);
void rk628_mode_copy(struct rk628_display_mode *to, struct rk628_display_mode *from);

/* for mipi-dsi */
#define MIPI_DSI_MODE_VIDEO             BIT(0)
#define MIPI_DSI_MODE_VIDEO_BURST       BIT(1)
#define MIPI_DSI_MODE_VIDEO_SYNC_PULSE  BIT(2)
#define MIPI_DSI_MODE_VIDEO_HFP         BIT(5)
#define MIPI_DSI_MODE_VIDEO_HBP         BIT(6)
#define MIPI_DSI_MODE_EOT_PACKET        BIT(9)
#define MIPI_DSI_CLOCK_NON_CONTINUOUS   BIT(10)
#define MIPI_DSI_MODE_LPM               BIT(11)

/* mipi-dsi format */
#define		MIPI_DSI_FMT_RGB888 	0
#define		MIPI_DSI_FMT_RGB666		1
#define		MIPI_DSI_FMT_RGB666_PACKED		2
#define		MIPI_DSI_FMT_RGB565		3


struct rk628_dsi {
	int bpp; /* 24/18/16*/
	uint8_t  bus_format;
	bool slave;
	bool master;
	uint8_t channel;
	uint8_t  lanes;
	uint8_t  id; /* 0:dsi0 1:dsi1 */
	unsigned long mode_flags;
};

struct rk628_gvi {
	uint8_t bus_format;
	uint8_t lanes;
	bool division_mode;
	bool frm_rst;
	u8 byte_mode;
	u16 color_depth;
};

/* for lvds */
#define LVDS_FORMAT_VESA_24BIT		0
#define LVDS_FORMAT_JEIDA_24BIT		1
#define LVDS_FORMAT_JEIDA_18BIT		2
#define LVDS_FORMAT_VESA_18BIT		3

#define LVDS_SINGLE_LINK			0
#define LVDS_DUAL_LINK_ODD_EVEN_PIXELS		1
#define LVDS_DUAL_LINK_EVEN_ODD_PIXELS		2
#define LVDS_DUAL_LINK_LEFT_RIGHT_PIXELS	3
#define LVDS_DUAL_LINK_RIGHT_LEFT_PIXELS	4

/* for gvi */
#define COLOR_DEPTH_RGB_YUV444_18BIT			0
#define COLOR_DEPTH_RGB_YUV444_24BIT			1
#define COLOR_DEPTH_RGB_YUV444_30BIT			2
#define COLOR_DEPTH_YUV422_16BIT			8
#define COLOR_DEPTH_YUV422_20BIT			9

#ifndef MEDIA_BUS_FMT_RGB666_1X18
#define MEDIA_BUS_FMT_RGB666_1X18               0x09
#endif
#ifndef MEDIA_BUS_FMT_RGB888_1X24
#define MEDIA_BUS_FMT_RGB888_1X24               0x0a
#endif
#ifndef MEDIA_BUS_FMT_RGB101010_1X30
#define MEDIA_BUS_FMT_RGB101010_1X30            0x18
#endif
#ifndef MEDIA_BUS_FMT_YUYV8_1X16
#define MEDIA_BUS_FMT_YUYV8_1X16                0x11
#endif
#ifndef MEDIA_BUS_FMT_YUYV10_1X20
#define MEDIA_BUS_FMT_YUYV10_1X20               0x0d
#endif

enum bus_format {
	BUS_FMT_RGB = 0,
	BUS_FMT_YUV422 = 1,
	BUS_FMT_YUV444 = 2,
	BUS_FMT_YUV420 = 3,
	BUS_FMT_UNKNOWN,
};

const struct rk628_dsi *rk628_display_get_dsi0_host(void);
const struct rk628_dsi *rk628_display_get_dsi1_host(void);
struct rk628_gvi *rk628_display_get_gvi_host(void);

u32 rk628_lvds_get_link_type(void);
u32 rk628_lvds_get_format(void);

void rk628_set_input_bus_format(enum bus_format format);
enum bus_format rk628_get_input_bus_format(void);
void rk628_set_output_bus_format(enum bus_format format);
enum bus_format rk628_get_output_bus_format(void);

#endif

