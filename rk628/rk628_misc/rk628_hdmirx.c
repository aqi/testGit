/* SPDX-License-Identifier: BSD-3-Clause
 * Andy Yan <andy.yan@rock-chisp.com>
 *
 * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd\
 */
#include <linux/gpio/consumer.h>
#include <linux/of_device.h>

#include "rk628.h"
#include "rk628_combrxphy.h"
#include "rk628_config.h"
#include "rk628_hdmirx.h"

#define POLL_INTERVAL_MS			1000
#define MODETCLK_CNT_NUM			1000
#define MODETCLK_HZ				49500000
#define RXPHY_CFG_MAX_TIMES			3

u8 debug = 4;

static u8 edid_init_data[] = {
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
	0x49, 0x78, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
	0x12, 0x1E, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78,
	0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27,
	0x12, 0x48, 0x4C, 0x00, 0x00, 0x00, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A,
	0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
	0x45, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E,
	0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x50,
	0x72, 0x6F, 0x6A, 0x65, 0x63, 0x74, 0x6F, 0x72,
	0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
	0x00, 0x14, 0x78, 0x01, 0xFF, 0x1D, 0x00, 0x0A,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x18,

	0x02, 0x03, 0x13, 0x71, 0x40, 0x23, 0x09, 0x07,
	0x01, 0x83, 0x01, 0x00, 0x00, 0x65, 0x03, 0x0C,
	0x00, 0x10, 0x00, 0x02, 0x3A, 0x80, 0x18, 0x71,
	0x38, 0x2D, 0x40, 0x58, 0x2C, 0x45, 0x00, 0x20,
	0xC2, 0x31, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17,

};

struct rk628_hdmi_mode {
	u32 hdisplay;
	u32 hstart;
	u32 hend;
	u32 htotal;
	u32 vdisplay;
	u32 vstart;
	u32 vend;
	u32 vtotal;
	u32 clock;
	unsigned int flags;
};

struct rk628_hdmirx {
	bool plugin;
	bool res_change;
	struct rk628_hdmi_mode mode;
	u32 input_format;
	u32 fs_audio;
	bool audio_present;
	bool hpd_output_inverted;
	bool src_mode_4K_yuv420;
};

const static struct rk628_hdmi_mode supported_modes[] = {
	{.hdisplay = 3840, .vdisplay = 2160,},
	{.hdisplay = 1920, .vdisplay = 1080,},
	{.hdisplay = 1280, .vdisplay = 720,},
	{.hdisplay = 540,  .vdisplay = 2560,},
	{.hdisplay = 720,  .vdisplay = 576,},
	{.hdisplay = 720,  .vdisplay = 480,},
};

static void rk628_hdmirx_ctrl_enable(struct rk628 *rk628)
{

	rk628_i2c_update_bits(rk628, GRF_SYSTEM_CON0,
	     SW_INPUT_MODE_MASK,
	     SW_INPUT_MODE(INPUT_MODE_HDMI));

	rk628_i2c_write(rk628, HDMI_RX_HDMI20_CONTROL, 0x10001f10);
	rk628_i2c_write(rk628, HDMI_RX_HDMI_MODE_RECOVER, 0x00000021);
	rk628_i2c_write(rk628, HDMI_RX_PDEC_CTRL, 0xbfff8011);
	rk628_i2c_write(rk628, HDMI_RX_PDEC_ASP_CTRL, 0x00000040);
	rk628_i2c_write(rk628, HDMI_RX_HDMI_RESMPL_CTRL, 0x00000001);
	rk628_i2c_write(rk628, HDMI_RX_HDMI_SYNC_CTRL, 0x00000014);
	rk628_i2c_write(rk628, HDMI_RX_PDEC_ERR_FILTER, 0x00000008);
	rk628_i2c_write(rk628, HDMI_RX_SCDC_I2CCONFIG, 0x01000000);
	rk628_i2c_write(rk628, HDMI_RX_SCDC_CONFIG, 0x00000001);
	rk628_i2c_write(rk628, HDMI_RX_SCDC_WRDATA0, 0xabcdef01);
	rk628_i2c_write(rk628, HDMI_RX_CHLOCK_CONFIG, 0x0030c15c);
	rk628_i2c_write(rk628, HDMI_RX_HDMI_ERROR_PROTECT, 0x000d0c98);
	rk628_i2c_write(rk628, HDMI_RX_MD_HCTRL1, 0x00000010);
	rk628_i2c_write(rk628, HDMI_RX_MD_HCTRL2, 0x00001738);
	rk628_i2c_write(rk628, HDMI_RX_MD_VCTRL, 0x00000002);
	rk628_i2c_write(rk628, HDMI_RX_MD_VTH, 0x0000073a);
	rk628_i2c_write(rk628, HDMI_RX_MD_IL_POL, 0x00000004);
	rk628_i2c_write(rk628, HDMI_RX_PDEC_ACRM_CTRL, 0x00000000);
	rk628_i2c_write(rk628, HDMI_RX_HDMI_DCM_CTRL, 0x00040414);
	rk628_i2c_write(rk628, HDMI_RX_HDMI_CKM_EVLTM, 0x00103e70);
	rk628_i2c_write(rk628, HDMI_RX_HDMI_CKM_F, 0x0c1c0b54);
	rk628_i2c_write(rk628, HDMI_RX_HDMI_RESMPL_CTRL, 0x00000001);

	rk628_i2c_update_bits(rk628, HDMI_RX_HDCP_SETTINGS,
	     HDMI_RESERVED_MASK |
	     FAST_I2C_MASK |
	     ONE_DOT_ONE_MASK |
	     FAST_REAUTH_MASK,
	     HDMI_RESERVED(1) |
	     FAST_I2C(0) |
	     ONE_DOT_ONE(1) |
	     FAST_REAUTH(1));
}

static void rk628_hdmirx_video_unmute(struct rk628 *rk628, u8 unmute)
{
	rk628_i2c_update_bits(rk628, HDMI_RX_DMI_DISABLE_IF, VID_ENABLE_MASK, VID_ENABLE(unmute));
}

static void rk628_hdmirx_hpd_ctrl(struct rk628 *rk628, bool en)
{
	u8 en_level, set_level;
	struct rk628_hdmirx *hdmirx = rk628->hdmirx;

	dev_dbg(rk628->dev, "%s: %sable, hpd invert:%d\n", __func__,
			en ? "en" : "dis", hdmirx->hpd_output_inverted);
	en_level = hdmirx->hpd_output_inverted ? 0 : 1;
	set_level = en ? en_level : !en_level;
	rk628_i2c_update_bits(rk628, HDMI_RX_HDMI_SETUP_CTRL,
			HOT_PLUG_DETECT_MASK, HOT_PLUG_DETECT(set_level));
}

static void rk628_hdmirx_disable_edid(struct rk628 *rk628)
{
	rk628_hdmirx_hpd_ctrl(rk628, false);
	rk628_hdmirx_video_unmute(rk628, 0);
}

static void rk628_hdmirx_enable_edid(struct rk628 *rk628)
{
	rk628_hdmirx_hpd_ctrl(rk628, true);
}

static int tx_5v_power_present(struct rk628 *rk628)
{
	bool ret;
	int val, i, cnt;

	/* Direct Mode */
	if (!rk628->plugin_det_gpio)
		return 1;

	cnt = 0;
	for (i = 0; i < 5; i++) {
		val = gpiod_get_value(rk628->plugin_det_gpio);
		if (val > 0)
			cnt++;
		usleep_range(500, 600);
	}

	ret = (cnt >= 3) ? 1 : 0;
	dev_dbg(rk628->dev, "%s: %d\n", __func__, ret);

	return ret;
}

static int rk628_hdmirx_init_edid(struct rk628 *rk628)
{
	struct rk628_display_mode *src_mode;
	struct rk628_hdmirx *hdmirx = rk628->hdmirx;
	u32 val;
	u8 csum = 0;
	int i, base, j;

	src_mode = rk628_display_get_src_mode();
	for (j = 0, base = 0x36; j < 2; j++) {
		csum = 0;
		/* clock-frequency */
		edid_init_data[base + 1]  = ((src_mode->clock / 10) & 0xff00) >> 8;
		edid_init_data[base]  = (src_mode->clock / 10) & 0xff;
		/* hactive low 8 bits */
		edid_init_data[base + 2]  = src_mode->hdisplay & 0xff;

		/* hblanking low 8 bits */
		val = src_mode->htotal - src_mode->hdisplay;
		edid_init_data[base + 3] =  val & 0xff;

		/* hactive high 4 bits & hblanking low 4 bits */
		edid_init_data[base + 4] =  ((src_mode->hdisplay & 0xf00) >> 4) + ((val & 0xf00) >> 8);

		/* vactive low 8 bits */
		edid_init_data[base + 5] =  src_mode->vdisplay & 0xff;

		/* vblanking low 8 bits */
		val = src_mode->vtotal - src_mode->vdisplay;
		edid_init_data[base + 6] =  val & 0xff;

		/* vactive high 4 bits & vblanking low 4 bits */
		edid_init_data[base + 7] =  ((src_mode->vdisplay & 0xf00) >> 4) + ((val & 0xf00) >> 8);

		/* hsync pulse offset low 8 bits */
		val = src_mode->hsync_start - src_mode->hdisplay;
		edid_init_data[base + 8] =  val & 0xff;

		/* hsync pulse width low 8 bits */
		val = src_mode->hsync_end - src_mode->hsync_start;
		edid_init_data[base + 9] =  val & 0xff;

		/* vsync pulse offset low 4 bits & vsync pulse width low 4 bits */
		val = ((src_mode->vsync_start - src_mode->vdisplay) & 0xf) << 4;
		edid_init_data[base + 10] =  val;
		edid_init_data[base + 10] += (src_mode->vsync_end - src_mode->vsync_start) & 0xf;

		/* 6~7bits:hsync pulse offset;
		 * 4~6bits:hsync pulse width;
		 * 2~3bits:vsync pulse offset;
		 * 0~1bits:vsync pulse width
		 * */
		edid_init_data[base + 11] = ((src_mode->hsync_start - src_mode->hdisplay) & 0x300) >> 2;
		edid_init_data[base + 11] += ((src_mode->hsync_end - src_mode->hsync_start) & 0x700) >> 4;
		edid_init_data[base + 11] += ((src_mode->vsync_start - src_mode->vdisplay) & 0x30) >> 2;
		edid_init_data[base + 11] += ((src_mode->vsync_end - src_mode->vsync_start) & 0x30) >> 4;

		edid_init_data[base + 17]  = 0x18;
		if (src_mode->flags & DRM_MODE_FLAG_PHSYNC)
			edid_init_data[base + 17] |= 0x2;

		if (src_mode->flags & DRM_MODE_FLAG_PVSYNC)
			edid_init_data[base + 17] |= 0x4;

		if (hdmirx->src_mode_4K_yuv420 && src_mode->clock == 594000) {
			edid_init_data[0x80 + 0x2] = 0x16;
			edid_init_data[0x80 + 0x13] = 0xe2;
			edid_init_data[0x80 + 0x14] = 0x0E;
			edid_init_data[0x80 + 0x15] = 0x61;
			base += (0x5d + 0x3);
		} else {
			base += 0x5d;
		}

		for (i = 0; i < 127; i++)
			csum += edid_init_data[i + j * 128];

		edid_init_data[127 +  j * 128] = (u8)0 - csum;
	}

	return 0;
}

static int rk628_hdmirx_set_edid(struct rk628 *rk628)
{
	int i;
	u32 val;
	u16 edid_len;

	rk628_hdmirx_disable_edid(rk628);

	/* edid access by apb when write, i2c slave addr: 0x0 */
	rk628_i2c_update_bits(rk628, GRF_SYSTEM_CON0,
	     SW_ADAPTER_I2CSLADR_MASK |
	     SW_EDID_MODE_MASK,
	     SW_ADAPTER_I2CSLADR(0) |
	     SW_EDID_MODE(1));

	rk628_hdmirx_init_edid(rk628);

	edid_len = ARRAY_SIZE(edid_init_data);
	for (i = 0; i < edid_len; i++) {
		rk628_i2c_write(rk628, EDID_BASE + i * 4, edid_init_data[i]);
	}

	/* read out for debug */
	if (debug >= 3) {
		printk("====== Read EDID: ======\n");
		for (i = 0; i < edid_len; i++) {
			rk628_i2c_read(rk628, EDID_BASE + i * 4, &val);
			printk("0x%02x ", val);
			if ((i + 1) % 8 == 0)
				printk("\n");
		}
		printk("============\n");
	}

	/* edid access by RX's i2c, i2c slave addr: 0x0 */
	rk628_i2c_update_bits(rk628, GRF_SYSTEM_CON0,
	     SW_ADAPTER_I2CSLADR_MASK |
	     SW_EDID_MODE_MASK,
	     SW_ADAPTER_I2CSLADR(0) |
	     SW_EDID_MODE(0));

	mdelay(1);

	return 0;
}

static bool rk628_rcv_supported_res(struct rk628 *rk628,
				    u32 width, u32 height,
				    u32 *index)
{
	u32 i;

	for (i = 0; i < ARRAY_SIZE(supported_modes); i++) {
		if ((supported_modes[i].hdisplay == width) &&
		    (supported_modes[i].vdisplay == height)) {
			break;
		}
	}
	if (i == ARRAY_SIZE(supported_modes)) {
		printk("do not support res wxh: %dx%d\n", width, height);
		return false;
	} else {
		*index = i;
		return true;
	}
}

static int rk628_hdmirx_phy_power_on(struct rk628 *rk628, int f)
{
	int ret;
	static bool rxphy_pwron = false;

	if (rxphy_pwron) {
		printk("rxphy already power on, power off!\n");
		ret = rk628_combrxphy_power_off(rk628);
		if (ret)
			printk("hdmi rxphy power off failed!\n");
		else
			rxphy_pwron = false;
	}

	udelay(1000);
	if (rxphy_pwron == false) {
		ret = rk628_combrxphy_power_on(rk628, f);
		if (ret) {
			rxphy_pwron = false;
			printk("hdmi rxphy power on failed\n");
		} else {
			rxphy_pwron = true;
			printk("hdmi rxphy power on success\n");
		}
	}

	pr_info("%s:rxphy_pwron=%d\n", __func__, rxphy_pwron);
	return ret;
}

static void rk628_hdmirx_get_timing(struct rk628 *rk628)
{
	u32 hact, vact, htotal, vtotal, fps, status;
	u32 val;
	u32 modetclk_cnt_hs, modetclk_cnt_vs, hs, vs;
	u32 hofs_pix, hbp, hfp, vbp, vfp;
	u32 tmds_clk, tmdsclk_cnt;
	u64 tmp_data;
	u32 interlaced;
	u32 width, height, hfrontporch, hsync, hbackporch, vfrontporch, vsync, vbackporch;
	unsigned long long pixelclock;
	unsigned long flags = 0;
	struct rk628_hdmirx *hdmirx = rk628->hdmirx;

	rk628_i2c_read(rk628, HDMI_RX_SCDC_REGS1, &val);
	status = val;

	rk628_i2c_read(rk628, HDMI_RX_MD_STS, &val);
	interlaced = val & ILACE_STS ? 1 : 0;

	rk628_i2c_read(rk628, HDMI_RX_MD_HACT_PX, &val);
	hact = val & 0xffff;
	rk628_i2c_read(rk628, HDMI_RX_MD_VAL, &val);
	vact = val & 0xffff;
	rk628_i2c_read(rk628, HDMI_RX_MD_HT1, &val);
	htotal = (val >> 16) & 0xffff;
	rk628_i2c_read(rk628, HDMI_RX_MD_VTL, &val);
	vtotal = val & 0xffff;
	rk628_i2c_read(rk628, HDMI_RX_MD_HT1, &val);
	hofs_pix = val & 0xffff;
	rk628_i2c_read(rk628, HDMI_RX_MD_VOL, &val);
	vbp = (val & 0xffff) + 1;

	rk628_i2c_read(rk628, HDMI_RX_HDMI_CKM_RESULT, &val);
	tmdsclk_cnt = val & 0xffff;
	tmp_data = tmdsclk_cnt;
	tmp_data = ((tmp_data * MODETCLK_HZ) + MODETCLK_CNT_NUM / 2);
	do_div(tmp_data, MODETCLK_CNT_NUM);
	tmds_clk = tmp_data;
	if (!(htotal && vtotal)) {
		printk("timing err, htotal:%d, vtotal:%d\n", htotal, vtotal);
		return;
	}
	fps = (tmds_clk + (htotal * vtotal) / 2) / (htotal * vtotal);

	rk628_i2c_read(rk628, HDMI_RX_MD_HT0, &val);
	modetclk_cnt_hs = val & 0xffff;
	hs = (tmdsclk_cnt * modetclk_cnt_hs + MODETCLK_CNT_NUM / 2) /
		MODETCLK_CNT_NUM;

	rk628_i2c_read(rk628, HDMI_RX_MD_VSC, &val);
	modetclk_cnt_vs = val & 0xffff;
	vs = (tmdsclk_cnt * modetclk_cnt_vs + MODETCLK_CNT_NUM / 2) /
		MODETCLK_CNT_NUM;
	vs = (vs + htotal / 2) / htotal;

	rk628_i2c_read(rk628, HDMI_RX_HDMI_STS, &val);
	if (val & BIT(8))
		flags |= DRM_MODE_FLAG_PHSYNC;
	else
		flags |= DRM_MODE_FLAG_NHSYNC;
	if (val & BIT(9))
		flags |= DRM_MODE_FLAG_PVSYNC;
	else
		flags |= DRM_MODE_FLAG_NVSYNC;

	if ((hofs_pix < hs) || (htotal < (hact + hofs_pix)) ||
	    (vtotal < (vact + vs + vbp))) {
	    printk("timing err, total:%dx%d, act:%dx%d, hofs:%d, "
		   "hs:%d, vs:%d, vbp:%d\n", htotal, vtotal, hact,
		   vact, hofs_pix, hs, vs, vbp);
		return;
	}
	hbp = hofs_pix - hs;
	hfp = htotal - hact - hofs_pix;
	vfp = vtotal - vact - vs - vbp;

	printk("cnt_num:%d, tmds_cnt:%d, hs_cnt:%d, vs_cnt:%d,"
	       " hofs:%d\n", MODETCLK_CNT_NUM, tmdsclk_cnt,
	       modetclk_cnt_hs, modetclk_cnt_vs, hofs_pix);

	width = hact;
	height = vact;
	hfrontporch = hfp;
	hsync = hs;
	hbackporch = hbp;
	vfrontporch = vfp;
	vsync = vs;
	vbackporch = vbp;
	pixelclock = htotal * vtotal * fps;

	if (interlaced == 1) {
		height *= 2;
		vsync = vsync + 1;
		pixelclock /= 2;
	}

	hdmirx->mode.clock = tmds_clk / 1000;
	hdmirx->mode.hdisplay = hact;
	hdmirx->mode.hstart = hdmirx->mode.hdisplay + hfrontporch;
	hdmirx->mode.hend = hdmirx->mode.hstart + hsync;
	hdmirx->mode.htotal = hdmirx->mode.hend + hbackporch;

	hdmirx->mode.vdisplay = vact;
	hdmirx->mode.vstart = hdmirx->mode.vdisplay + vfrontporch;
	hdmirx->mode.vend = hdmirx->mode.vstart + vsync;
	hdmirx->mode.vtotal = hdmirx->mode.vend + vbackporch;
	hdmirx->mode.flags = flags;

	printk("SCDC_REGS1:%#x, act:%dx%d, total:%dx%d, fps:%d,"
	       " pixclk:%llu\n", status, hact, vact, htotal, vtotal,
	       fps, pixelclock);
	printk("hfp:%d, hs:%d, hbp:%d, vfp:%d, vs:%d, vbp:%d,"
	       " interlace:%d\n", hfrontporch, hsync,
	       hbackporch, vfrontporch, vsync,
	       vbackporch, interlaced);
}

static int rk628_hdmirx_phy_setup(struct rk628 *rk628)
{
	u32 i, cnt, val;
	u32 width, height, frame_width, frame_height, status;
	u32 index;
	struct rk628_display_mode *src_mode;
	struct rk628_hdmirx *hdmirx = rk628->hdmirx;
	int f;
	struct rk628_display_mode *dst_mode;

	/* Bit31 is used to distinguish HDMI cable mode and direct connection
	* mode in the rk628_combrxphy driver.
	* Bit31: 0 -direct connection mode;
	*    1 -cable mode;
	* The cable mode is to know the input clock frequency through cdr_mode
	* in the rk628_combrxphy driver, and the cable mode supports up to
	* 297M, so 297M is passed uniformly here.
	*/
	f = (297000 | BIT(31));
	dst_mode = rk628_display_get_dst_mode();
	/*
	 * force 594m mode to yuv420 format.
	 * bit30 is used to indicate whether it is yuv420 format.
	 */
	if (hdmirx->src_mode_4K_yuv420 && dst_mode->clock == 594000)
		f |= BIT(30);

	for ( i = 0; i < RXPHY_CFG_MAX_TIMES; i++) {
		rk628_hdmirx_phy_power_on(rk628, f);
		cnt = 0;

		do {
			cnt++;
			udelay(2000);
			rk628_i2c_read(rk628, HDMI_RX_MD_HACT_PX, &val);
			width = val & 0xffff;
			rk628_i2c_read(rk628, HDMI_RX_MD_HT1, &val);
			frame_width = (val >> 16) & 0xffff;

			rk628_i2c_read(rk628, HDMI_RX_MD_VAL, &val);
			height = val & 0xffff;
			rk628_i2c_read(rk628, HDMI_RX_MD_VTL, &val);
			frame_height = val & 0xffff;

			rk628_i2c_read(rk628, HDMI_RX_SCDC_REGS1, &val);
			status = val;

			printk("%s read wxh:%dx%d, total:%dx%d,"
			       " SCDC_REGS1:%#x, cnt:%d\n", __func__,
			       width, height, frame_width,
			       frame_height, status, cnt);

			if (hdmirx->src_mode_4K_yuv420 && dst_mode->clock == 594000)
				width *= 2;

			if (cnt >= 15)
				break;
		}while(((status & 0xfff) != 0xf00) ||
			(!rk628_rcv_supported_res(rk628, width, height, &index)));

		if (((status & 0xfff) != 0xf00) ||
		    (!rk628_rcv_supported_res(rk628, width, height, &index))) {
			printk("%s hdmi rxphy lock failed, retry:%d\n",
			       __func__, i);
			continue;
		} else {
			rk628_hdmirx_get_timing(rk628);

			src_mode = rk628_display_get_src_mode();
			src_mode->clock = hdmirx->mode.clock;
			src_mode->hdisplay = hdmirx->mode.hdisplay;
			src_mode->hsync_start = hdmirx->mode.hstart;
			src_mode->hsync_end = hdmirx->mode.hend;
			src_mode->htotal = hdmirx->mode.htotal;

			src_mode->vdisplay = hdmirx->mode.vdisplay;
			src_mode->vsync_start = hdmirx->mode.vstart;
			src_mode->vsync_end = hdmirx->mode.vend;
			src_mode->vtotal = hdmirx->mode.vtotal;
			src_mode->flags = hdmirx->mode.flags;
			if (hdmirx->src_mode_4K_yuv420 && dst_mode->clock == 594000) {
				rk628_mode_copy(src_mode, dst_mode);
				src_mode->flags = DRM_MODE_FLAG_PHSYNC|DRM_MODE_FLAG_PVSYNC;
			}

			break;
		}
	}

	if (i == RXPHY_CFG_MAX_TIMES)
		return -1;

	return 0;
}

static u32 rk628_hdmirx_get_input_format(struct rk628 *rk628)
{
	u32 val, format, avi_pb = 0;
	u8 i;
	u8 cnt = 0, max_cnt = 2;
	struct rk628_hdmirx *hdmirx = rk628->hdmirx;

	rk628_i2c_read(rk628, HDMI_RX_PDEC_ISTS, &val);
	if (val & AVI_RCV_ISTS) {
		for (i = 0; i < 100; i++) {
			rk628_i2c_read(rk628, HDMI_RX_PDEC_AVI_PB, &format);
			dev_dbg(rk628->dev, "%s PDEC_AVI_PB:%#x\n", __func__, format);
			if (format && format == avi_pb) {
				if (++cnt >= max_cnt)
					break;
			} else {
				cnt = 0;
				avi_pb = format;
			}
			msleep(30);
		}
		format  = (avi_pb & VIDEO_FORMAT) >> 5;
		switch (format) {
		case 0:
			hdmirx->input_format = BUS_FMT_RGB;
			break;
		case 1:
			hdmirx->input_format = BUS_FMT_YUV422;
			break;
		case 2:
			hdmirx->input_format = BUS_FMT_YUV444;
			break;
		case 3:
			hdmirx->input_format = BUS_FMT_YUV420;
			break;
		default:
			hdmirx->input_format = BUS_FMT_RGB;
			break;
		}
		rk628_i2c_write(rk628, HDMI_RX_PDEC_ICLR, AVI_RCV_ISTS);
	}

	return hdmirx->input_format;
}

static int rk628_check_signal(struct rk628 *rk628)
{
	u32 hact, vact, val;

	rk628_i2c_read(rk628, HDMI_RX_MD_HACT_PX, &val);
	hact = val & 0xffff;
	rk628_i2c_read(rk628, HDMI_RX_MD_VAL, &val);
	vact = val & 0xffff;

	if (!hact || !vact) {
		printk("no signal\n");
		return 0;
	}

	return 1;
}

static bool rk628_hdmirx_status_change(struct rk628 *rk628)
{
	u32 hact, vact, val;
	struct rk628_hdmirx *hdmirx = rk628->hdmirx;

	rk628_i2c_read(rk628, HDMI_RX_MD_HACT_PX, &val);
	hact = val & 0xffff;
	rk628_i2c_read(rk628, HDMI_RX_MD_VAL, &val);
	vact = val & 0xffff;
	if (!rk628->plugin_det_gpio && !hact && !vact)
		return true;

	if (hact != hdmirx->mode.hdisplay || vact != hdmirx->mode.vdisplay) {
		printk("new: hdisplay=%d, vdisplay=%d\n", hact, vact);
		printk("old: hdisplay=%d, vdisplay=%d\n", hdmirx->mode.hdisplay, hdmirx->mode.vdisplay);
		return true;
	}

	rk628_hdmirx_get_input_format(rk628);
	if (hdmirx->input_format != rk628_get_input_bus_format())
		return true;

	return false;
}

static int rk628_hdmirx_init(struct rk628 *rk628)
{
	struct rk628_hdmirx *hdmirx;
	struct device *dev = rk628->dev;

	hdmirx = devm_kzalloc(rk628->dev, sizeof(*hdmirx), GFP_KERNEL);
	if (!hdmirx)
		return -ENOMEM;
	rk628->hdmirx = hdmirx;

	hdmirx->hpd_output_inverted = of_property_read_bool(dev->of_node,
		"hpd-output-inverted");

	hdmirx->src_mode_4K_yuv420 = of_property_read_bool(dev->of_node,
		"src-mode-4k-yuv420");

	/* HDMIRX IOMUX */
	rk628_i2c_write(rk628, GRF_GPIO1AB_SEL_CON, HIWORD_UPDATE(0x7, 10, 8));
	//i2s pinctrl
	rk628_i2c_write(rk628, GRF_GPIO0AB_SEL_CON, 0x155c155c);

	/* if GVI and HDMITX OUT, HDMIRX missing signal */
	rk628_i2c_update_bits(rk628, GRF_SYSTEM_CON0, SW_OUTPUT_MODE_MASK, SW_OUTPUT_MODE(OUTPUT_MODE_RGB));
	rk628_i2c_update_bits(rk628, GRF_SYSTEM_CON0, SW_INPUT_MODE_MASK, SW_INPUT_MODE(INPUT_MODE_HDMI));
	rk628_hdmirx_set_edid(rk628);
	/* clear avi rcv interrupt */
	rk628_i2c_write(rk628, HDMI_RX_PDEC_ICLR, AVI_RCV_ISTS);

	printk("hdmirx driver version: %s\n", DRIVER_VERSION);

	return 0;
}

void rk628_hdmirx_enable_interrupts(struct rk628 *rk628, bool en)
{
	u32 pdec_ien, md_ien;
	u32 pdec_mask = 0, md_mask = 0;

	pdec_mask |= AVI_RCV_ENSET;
	md_mask = VACT_LIN_ENSET | HACT_PIX_ENSET | HS_CLK_ENSET;
	dev_dbg(rk628->dev, "%s: %sable\n", __func__, en ? "en" : "dis");
	/* clr irq */
	rk628_i2c_write(rk628, HDMI_RX_MD_ICLR, md_mask);
	if (en) {
		rk628_i2c_write(rk628, HDMI_RX_MD_IEN_SET, md_mask);
	} else {
		rk628_i2c_write(rk628, HDMI_RX_MD_IEN_CLR, md_mask);
		rk628_i2c_write(rk628, HDMI_RX_AUD_FIFO_IEN_CLR, 0x1f);
	}
	usleep_range(5000, 5000);
	rk628_i2c_read(rk628, HDMI_RX_MD_IEN, &md_ien);
	rk628_i2c_read(rk628, HDMI_RX_PDEC_IEN, &pdec_ien);
	dev_dbg(rk628->dev, "%s MD_IEN:%#x, PDEC_IEN:%#x\n", __func__, md_ien, pdec_ien);
}

int rk628_hdmirx_enable(struct rk628 *rk628)
{
	int ret;
	struct rk628_hdmirx *hdmirx;

	if (!rk628->hdmirx) {
		ret = rk628_hdmirx_init(rk628);
		if (ret < 0)
			return HDMIRX_PLUGOUT;
	}

	hdmirx = rk628->hdmirx;
	if (tx_5v_power_present(rk628)) {
		hdmirx->plugin = true;
		rk628_hdmirx_enable_edid(rk628);
		rk628_hdmirx_ctrl_enable(rk628);
		rk628_hdmirx_phy_setup(rk628);
		rk628_hdmirx_get_input_format(rk628);
		rk628_set_input_bus_format(hdmirx->input_format);
		printk("hdmirx plug in\n");
		printk("input: %d, output: %d\n", hdmirx->input_format, rk628_get_output_bus_format());
		if (!rk628_check_signal(rk628)) {
			return HDMIRX_PLUGIN | HDMIRX_NOSIGNAL;
		} else {
			rk628_hdmirx_video_unmute(rk628, 1);
			return HDMIRX_PLUGIN;
		}
	}

	hdmirx->plugin = false;
	rk628_hdmirx_disable_edid(rk628);
	rk628_i2c_update_bits(rk628, GRF_SYSTEM_CON0, SW_I2S_DATA_OEN_MASK, SW_I2S_DATA_OEN(1));

	return HDMIRX_PLUGOUT;
}

void rk628_hdmirx_disable(struct rk628 *rk628)
{
	int ret;
	struct rk628_hdmirx *hdmirx;

	if (!rk628->hdmirx) {
		ret = rk628_hdmirx_init(rk628);
		if (ret < 0)
			return;
	}

	hdmirx = rk628->hdmirx;
	if (!tx_5v_power_present(rk628)) {
		hdmirx->plugin = false;
		rk628_hdmirx_disable_edid(rk628);
		rk628_i2c_update_bits(rk628, GRF_SYSTEM_CON0, SW_I2S_DATA_OEN_MASK, SW_I2S_DATA_OEN(1));
		printk("hdmirx plug out\n");
	}
}

int rk628_hdmirx_detect(struct rk628 *rk628)
{
	u32 ret = 0;
	struct rk628_hdmirx *hdmirx;

	if (!rk628->hdmirx) {
		ret = rk628_hdmirx_init(rk628);
		if (ret < 0)
			return HDMIRX_PLUGOUT;
	}
	hdmirx = rk628->hdmirx;

	if (tx_5v_power_present(rk628)) {
		ret |= HDMIRX_PLUGIN;
		if (!hdmirx->plugin)
			ret |= HDMIRX_CHANGED;
		if (rk628_hdmirx_status_change(rk628))
			ret |= HDMIRX_CHANGED;
		hdmirx->plugin = true;
	} else {
		ret |= HDMIRX_PLUGOUT;
		if (hdmirx->plugin)
			ret |= HDMIRX_CHANGED;
		hdmirx->plugin = false;
	}

	return ret;
}
