// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 *
 * Author: Guochun Huang <hero.huang@rock-chips.com>
 */

#include "rk628_config.h"

#ifdef RK628_DSI_OUT
static struct rk628_display_mode src_mode = {
	/* .clock = */ 132000,
	/* .hdisplay = */ 1080,
	/* .hsync_start = */ 1080 + 15,
	/* .hsync_end = */ 1080 + 15 + 2,
	/* .htotal = */ 1080 + 15 + 2 + 30,
	/* .vdisplay = */ 1920,
	/* .vsync_start = */ 1920 + 15,
	/* .vsync_end = */ 1920 + 15 + 2,
	/* .vtotal = */ 1920 + 15 + 2 + 15,
	/* .flags = */ DRM_MODE_FLAG_NVSYNC | DRM_MODE_FLAG_NVSYNC,
};

static struct rk628_display_mode dst_mode = {
	/* .clock = */ 132000,
	/* .hdisplay = */ 1080,
	/* .hsync_start = */ 1080 + 15,
	/* .hsync_end = */ 1080 + 15 + 2,
	/* .htotal = */ 1080 + 15 + 2 + 30,
	/* .vdisplay = */ 1920,
	/* .vsync_start = */ 1920 + 15,
	/* .vsync_end = */ 1920 + 15 + 2,
	/* .vtotal = */ 1920 + 15 + 2 + 15,
	/* .flags = */ DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC,
};
#endif

#ifdef RK628_LVDS_OUT
/* for single lvds */
static struct rk628_display_mode src_mode = {
	/* .clock = */ 68000,
	/* .hdisplay = */ 800,
	/* .hsync_start = */ 800 + 30,
	/* .hsync_end = */ 800 + 30 + 4,
	/* .htotal = */ 800 + 30 + 3 + 30,
	/* .vdisplay = */ 1280,
	/* .vsync_start = */ 1280 + 3,
	/* .vsync_end = */ 1280 + 3 + 2,
	/* .vtotal = */ 1280 + 3 + 2 + 3,
	/* .flags = */ DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC,
};

static struct rk628_display_mode dst_mode = {
	/* .clock = */ 68000,
	/* .hdisplay = */ 800,
	/* .hsync_start = */ 800 + 30,
	/* .hsync_end = */ 800 + 30 + 4,
	/* .htotal = */ 800 + 30 + 3 + 30,
	/* .vdisplay = */ 1280,
	/* .vsync_start = */ 1280 + 3,
	/* .vsync_end = */ 1280 + 3 + 2,
	/* .vtotal = */ 1280 + 3 + 2 + 3,
	/* .flags = */ DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC,
};

/* for dual lvds */
#if 0
static struct rk628_display_mode src_mode = {
	/* .clock = */ 148500,
	/* .hdisplay = */ 1920,
	/* .hsync_start = */ 1920 + 120,
	/* .hsync_end = */ 1920 + 120 + 64,
	/* .htotal = */ 1920 + 120 + 64 + 96,
	/* .vdisplay = */ 1080,
	/* .vsync_start = */ 1080 + 33,
	/* .vsync_end = */ 1080 + 33 + 4,
	/* .vtotal = */ 1080 + 33 + 4 + 8,
	/* .flags = */ DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC,
};

static struct rk628_display_mode dst_mode = {
	/* .clock = */ 148500,
	/* .hdisplay = */ 1920,
	/* .hsync_start = */ 1920 + 120,
	/* .hsync_end = */ 1920 + 120 + 64,
	/* .htotal = */ 1920 + 120 + 64 + 96,
	/* .vdisplay = */ 1080,
	/* .vsync_start = */ 1080 + 33,
	/* .vsync_end = */ 1080 + 33 + 4,
	/* .vtotal = */ 1080 + 33 + 4 + 8,
	/* .flags = */ DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC,
};
#endif
#endif

#ifdef RK628_GVI_OUT
static struct rk628_display_mode src_mode = {
	/* .clock = */ 148500,
	/* .hdisplay = */ 1920,
	/* .hsync_start = */ 1920 + 88,
	/* .hsync_end = */ 1920 + 88 + 44,
	/* .htotal = */ 1920 + 88 + 44 + 148,
	/* .vdisplay = */ 1080,
	/* .vsync_start = */ 1080 + 4,
	/* .vsync_end = */ 1080 + 4 + 5,
	/* .vtotal = */ 1080 + 4 + 5 + 36,
	/* .flags = */ DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC,
};

static struct rk628_display_mode dst_mode = {
	/* .clock = */ 594000,
	/* .hdisplay = */ 3840,
	/* .hsync_start = */ 3840 + 176,
	/* .hsync_end = */ 3840 + 176 + 88,
	/* .htotal = */ 3840 + 176 + 88 + 296,
	/* .vdisplay = */ 2160,
	/* .vsync_start = */ 2160 + 8,
	/* .vsync_end = */ 2160 + 8 + 12,
	/* .vtotal = */ 2160 + 8 + 12 + 72,
	/* .flags = */ DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC,
};
#endif

#ifdef RK628_CSI_OUT
static struct rk628_display_mode src_mode = {
	/* .clock = */ 148500,
	/* .hdisplay = */ 1920,
	/* .hsync_start = */ 1920 + 88,
	/* .hsync_end = */ 1920 + 88 + 44,
	/* .htotal = */ 1920 + 88 + 44 + 148,
	/* .vdisplay = */ 1080,
	/* .vsync_start = */ 1080 + 4,
	/* .vsync_end = */ 1080 + 4 + 5,
	/* .vtotal = */ 1080 + 4 + 5 + 36,
	/* .flags = */ DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC,
};

static struct rk628_display_mode dst_mode = {
	/* .clock = */ 148500,
	/* .hdisplay = */ 1920,
	/* .hsync_start = */ 1920 + 88,
	/* .hsync_end = */ 1920 + 88 + 44,
	/* .htotal = */ 1920 + 88 + 44 + 148,
	/* .vdisplay = */ 1080,
	/* .vsync_start = */ 1080 + 4,
	/* .vsync_end = */ 1080 + 4 + 5,
	/* .vtotal = */ 1080 + 4 + 5 + 36,
	/* .flags = */ DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC,
};
#endif

#ifdef RK628_HDMI_OUT
static struct rk628_display_mode src_mode = {
	/* .clock = */ 148500,
	/* .hdisplay = */ 1920,
	/* .hsync_start = */ 1920 + 88,
	/* .hsync_end = */ 1920 + 88 + 44,
	/* .htotal = */ 1920 + 88 + 44 + 148,
	/* .vdisplay = */ 1080,
	/* .vsync_start = */ 1080 + 4,
	/* .vsync_end = */ 1080 + 4 + 5,
	/* .vtotal = */ 1080 + 4 + 5 + 36,
	/* .flags = */ DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC,
};

static struct rk628_display_mode dst_mode = {
	/* .clock = */ 148500,
	/* .hdisplay = */ 1920,
	/* .hsync_start = */ 1920 + 88,
	/* .hsync_end = */ 1920 + 88 + 44,
	/* .htotal = */ 1920 + 88 + 44 + 148,
	/* .vdisplay = */ 1080,
	/* .vsync_start = */ 1080 + 4,
	/* .vsync_end = */ 1080 + 4 + 5,
	/* .vtotal = */ 1080 + 4 + 5 + 36,
	/* .flags = */ DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC,
};
#endif

struct rk628_display_mode *rk628_display_get_src_mode(void)
{
	return &src_mode;
}

struct rk628_display_mode *rk628_display_get_dst_mode(void)
{
	return &dst_mode;
}

void rk628_mode_copy(struct rk628_display_mode *to, struct rk628_display_mode *from)
{
	to->clock = from->clock;
	to->hdisplay = from->hdisplay;
	to->hsync_start = from->hsync_start;
	to->hsync_end = from->hsync_end;
	to->htotal = from->htotal;
	to->vdisplay = from->vdisplay;
	to->vsync_start = from->vsync_start;
	to->vsync_end = from->vsync_end;
	to->vtotal = from->vtotal;
	to->flags = from->flags;
}

/* option config dsi0 */
const static struct rk628_dsi rk628_dsi0 = {
		/* .bpp = */ 24,
		/* .bus_format */ MIPI_DSI_FMT_RGB888,
		/* .slave = */ false,
		/* .master = */ false,
		/* .channel = */ 0,
		/* .lanes */ 4,
		/* .id */ 0,
		/* .mode_flags */ MIPI_DSI_MODE_VIDEO |
				  MIPI_DSI_MODE_VIDEO_BURST |
				  MIPI_DSI_MODE_LPM |
				  MIPI_DSI_MODE_EOT_PACKET,
};

/* option config dsi1 for dual dsi */
const static struct rk628_dsi rk628_dsi1 = {
		/* .bpp = */ 24,
		/* .bus_format */ MIPI_DSI_FMT_RGB888,
		/* .slave = */ false,
		/* .master = */ false,
		/* .channel = */ 0,
		/* .lanes */ 4,
		/* .id */ 1,
		/* .mode_flags */ MIPI_DSI_MODE_VIDEO |
				  MIPI_DSI_MODE_VIDEO_BURST |
				  MIPI_DSI_MODE_LPM |
				  MIPI_DSI_MODE_EOT_PACKET,
};

static struct rk628_gvi rk628_gvi = {
	/* .bus_format = */ MEDIA_BUS_FMT_RGB888_1X24,
	/* .lanes = */ 8,
	/* .division_mode = */ false,
	/* .frm_rst = */ false,
	/* .byte_mode = */ 3,
	/* .color_depth = */ COLOR_DEPTH_RGB_YUV444_24BIT,
};

const struct rk628_dsi *rk628_display_get_dsi0_host(void)
{
	return &rk628_dsi0;
}

const struct rk628_dsi *rk628_display_get_dsi1_host(void)
{
	return &rk628_dsi1;
}

struct rk628_gvi *rk628_display_get_gvi_host(void)
{
	return &rk628_gvi;
}

u32 rk628_lvds_get_link_type(void)
{
	/*
	 * LVDS_SINGLE_LINK
	 * LVDS_DUAL_LINK_ODD_EVEN_PIXELS
	 * LVDS_DUAL_LINK_EVEN_ODD_PIXELS
	 * LVDS_DUAL_LINK_LEFT_RIGHT_PIXELS
	 * LVDS_DUAL_LINK_RIGHT_LEFT_PIXELS
	 */
	return LVDS_SINGLE_LINK;
}

u32 rk628_lvds_get_format(void)
{
	/*
	 * LVDS_FORMAT_VESA_24BIT
	 * LVDS_FORMAT_JEIDA_24BIT
	 * LVDS_FORMAT_JEIDA_18BIT
	 * LVDS_FORMAT_VESA_18BIT
	 */
	return LVDS_FORMAT_JEIDA_24BIT;
}

enum bus_format input_bus_format = BUS_FMT_RGB;
enum bus_format output_bus_format = BUS_FMT_RGB;

void rk628_set_input_bus_format(enum bus_format format)
{
	input_bus_format = format;
}

enum bus_format rk628_get_input_bus_format(void)
{
	return input_bus_format;
}

void rk628_set_output_bus_format(enum bus_format format)
{
	output_bus_format = format;
}

enum bus_format rk628_get_output_bus_format(void)
{
	return output_bus_format;
}
