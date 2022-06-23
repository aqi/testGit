// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020 Rockchip Electronics Co. Ltd.
 *
 * Author: Wyon Bi <bivvy.bi@rock-chips.com>
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/gpio/consumer.h>
#include <linux/regmap.h>
#include <linux/backlight.h>
#include <linux/pm_runtime.h>

#include "rk628.h"
#include "rk628_cru.h"
#include "rk628_combrxphy.h"
#include "rk628_post_process.h"
#include "rk628_hdmirx.h"
#include "rk628_combtxphy.h"
#include "rk628_dsi.h"
#include "rk628_rgb.h"
#include "rk628_lvds.h"
#include "rk628_gvi.h"
#include "rk628_csi.h"
#include "rk628_hdmitx.h"

static const struct regmap_range rk628_cru_readable_ranges[] = {
	regmap_reg_range(CRU_CPLL_CON0, CRU_CPLL_CON4),
	regmap_reg_range(CRU_GPLL_CON0, CRU_GPLL_CON4),
	regmap_reg_range(CRU_MODE_CON00, CRU_MODE_CON00),
	regmap_reg_range(CRU_CLKSEL_CON00, CRU_CLKSEL_CON21),
	regmap_reg_range(CRU_GATE_CON00, CRU_GATE_CON05),
	regmap_reg_range(CRU_SOFTRST_CON00, CRU_SOFTRST_CON04),
};

static const struct regmap_access_table rk628_cru_readable_table = {
	.yes_ranges     = rk628_cru_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_cru_readable_ranges),
};

static const struct regmap_range rk628_combrxphy_readable_ranges[] = {
	regmap_reg_range(COMBRX_REG(0x6600), COMBRX_REG(0x665b)),
	regmap_reg_range(COMBRX_REG(0x66a0), COMBRX_REG(0x66db)),
	regmap_reg_range(COMBRX_REG(0x66f0), COMBRX_REG(0x66ff)),
	regmap_reg_range(COMBRX_REG(0x6700), COMBRX_REG(0x6790)),
};

static const struct regmap_access_table rk628_combrxphy_readable_table = {
	.yes_ranges     = rk628_combrxphy_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_combrxphy_readable_ranges),
};

static const struct regmap_range rk628_hdmirx_readable_ranges[] = {
	regmap_reg_range(HDMI_RX_HDMI_SETUP_CTRL, HDMI_RX_HDMI_SETUP_CTRL),
	regmap_reg_range(HDMI_RX_HDMI_PCB_CTRL, HDMI_RX_HDMI_PCB_CTRL),
	regmap_reg_range(HDMI_RX_HDMI_MODE_RECOVER, HDMI_RX_HDMI_ERROR_PROTECT),
	regmap_reg_range(HDMI_RX_HDMI_SYNC_CTRL, HDMI_RX_HDMI_CKM_RESULT),
	regmap_reg_range(HDMI_RX_HDMI_RESMPL_CTRL, HDMI_RX_HDMI_RESMPL_CTRL),
	regmap_reg_range(HDMI_RX_HDMI_VM_CFG_CH2, HDMI_RX_HDMI_STS),
	regmap_reg_range(HDMI_RX_HDCP_CTRL, HDMI_RX_HDCP_SETTINGS),
	regmap_reg_range(HDMI_RX_HDCP_KIDX, HDMI_RX_HDCP_KIDX),
	regmap_reg_range(HDMI_RX_HDCP_DBG, HDMI_RX_HDCP_AN0),
	regmap_reg_range(HDMI_RX_HDCP_STS, HDMI_RX_HDCP_STS),
	regmap_reg_range(HDMI_RX_MD_HCTRL1, HDMI_RX_MD_HACT_PX),
	regmap_reg_range(HDMI_RX_MD_VCTRL, HDMI_RX_MD_VSC),
	regmap_reg_range(HDMI_RX_MD_VOL, HDMI_RX_MD_VTL),
	regmap_reg_range(HDMI_RX_MD_IL_POL, HDMI_RX_MD_STS),
	regmap_reg_range(HDMI_RX_AUD_CTRL, HDMI_RX_AUD_CTRL),
	regmap_reg_range(HDMI_RX_AUD_PLL_CTRL, HDMI_RX_AUD_PLL_CTRL),
	regmap_reg_range(HDMI_RX_AUD_CLK_CTRL, HDMI_RX_AUD_CLK_CTRL),
	regmap_reg_range(HDMI_RX_AUD_FIFO_CTRL, HDMI_RX_AUD_FIFO_TH),
	regmap_reg_range(HDMI_RX_AUD_CHEXTR_CTRL, HDMI_RX_AUD_PAO_CTRL),
	regmap_reg_range(HDMI_RX_AUD_FIFO_STS, HDMI_RX_AUD_FIFO_STS),
	regmap_reg_range(HDMI_RX_AUDPLL_GEN_CTS, HDMI_RX_AUDPLL_GEN_N),
	regmap_reg_range(HDMI_RX_PDEC_CTRL, HDMI_RX_PDEC_CTRL),
	regmap_reg_range(HDMI_RX_PDEC_AUDIODET_CTRL, HDMI_RX_PDEC_AUDIODET_CTRL),
	regmap_reg_range(HDMI_RX_PDEC_ERR_FILTER, HDMI_RX_PDEC_ASP_CTRL),
	regmap_reg_range(HDMI_RX_PDEC_STS, HDMI_RX_PDEC_STS),
	regmap_reg_range(HDMI_RX_PDEC_GCP_AVMUTE, HDMI_RX_PDEC_GCP_AVMUTE),
	regmap_reg_range(HDMI_RX_PDEC_ACR_CTS, HDMI_RX_PDEC_ACR_N),
	regmap_reg_range(HDMI_RX_PDEC_AIF_CTRL, HDMI_RX_PDEC_AIF_PB0),
	regmap_reg_range(HDMI_RX_PDEC_AVI_PB, HDMI_RX_PDEC_AVI_PB),
	regmap_reg_range(HDMI_RX_HDMI20_CONTROL, HDMI_RX_CHLOCK_CONFIG),
	regmap_reg_range(HDMI_RX_SCDC_REGS1, HDMI_RX_SCDC_REGS2),
	regmap_reg_range(HDMI_RX_SCDC_WRDATA0, HDMI_RX_SCDC_WRDATA0),
	regmap_reg_range(HDMI_RX_PDEC_ISTS, HDMI_RX_PDEC_IEN),
	regmap_reg_range(HDMI_RX_AUD_FIFO_ISTS, HDMI_RX_AUD_FIFO_IEN),
	regmap_reg_range(HDMI_RX_MD_ISTS, HDMI_RX_MD_IEN),
	regmap_reg_range(HDMI_RX_HDMI_ISTS, HDMI_RX_HDMI_IEN),
	regmap_reg_range(HDMI_RX_DMI_DISABLE_IF, HDMI_RX_DMI_DISABLE_IF),
};

static const struct regmap_access_table rk628_hdmirx_readable_table = {
	.yes_ranges     = rk628_hdmirx_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_hdmirx_readable_ranges),
};

static const struct regmap_range rk628_key_readable_ranges[] = {
        regmap_reg_range(EDID_BASE, EDID_BASE + 0x400),
};

static const struct regmap_access_table rk628_key_readable_table = {
        .yes_ranges     = rk628_key_readable_ranges,
        .n_yes_ranges   = ARRAY_SIZE(rk628_key_readable_ranges),
};

static const struct regmap_range rk628_combtxphy_readable_ranges[] = {
	regmap_reg_range(COMBTXPHY_BASE, COMBTXPHY_CON10),
};

static const struct regmap_access_table rk628_combtxphy_readable_table = {
	.yes_ranges     = rk628_combtxphy_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_combtxphy_readable_ranges),
};

static const struct regmap_range rk628_dsi0_readable_ranges[] = {
	regmap_reg_range(DSI0_BASE, DSI0_BASE + DSI_MAX_REGISTER),
};

static const struct regmap_access_table rk628_dsi0_readable_table = {
	.yes_ranges     = rk628_dsi0_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_dsi0_readable_ranges),
};

static const struct regmap_range rk628_dsi1_readable_ranges[] = {
	regmap_reg_range(DSI1_BASE, DSI1_BASE + DSI_MAX_REGISTER),
};

static const struct regmap_access_table rk628_dsi1_readable_table = {
	.yes_ranges     = rk628_dsi1_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_dsi1_readable_ranges),
};

static const struct regmap_range rk628_gvi_readable_ranges[] = {
	regmap_reg_range(GVI_BASE, GVI_BASE + GVI_COLOR_BAR_VTIMING1),
};

static const struct regmap_access_table rk628_gvi_readable_table = {
	.yes_ranges     = rk628_gvi_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_gvi_readable_ranges),
};

static const struct regmap_range rk628_csi_readable_ranges[] = {
	regmap_reg_range(CSITX_CONFIG_DONE, CSITX_CSITX_VERSION),
	regmap_reg_range(CSITX_SYS_CTRL0_IMD, CSITX_TIMING_HPW_PADDING_NUM),
	regmap_reg_range(CSITX_VOP_PATH_CTRL, CSITX_VOP_PATH_CTRL),
	regmap_reg_range(CSITX_VOP_PATH_PKT_CTRL, CSITX_VOP_PATH_PKT_CTRL),
	regmap_reg_range(CSITX_CSITX_STATUS0, CSITX_LPDT_DATA_IMD),
	regmap_reg_range(CSITX_DPHY_CTRL, CSITX_DPHY_CTRL),
};

static const struct regmap_access_table rk628_csi_readable_table = {
	.yes_ranges     = rk628_csi_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_csi_readable_ranges),
};

static const struct regmap_range rk628_hdmi_volatile_reg_ranges[] = {
	regmap_reg_range(HDMI_SYS_CTRL, HDMI_MAX_REG),
};

static const struct regmap_access_table rk628_hdmi_volatile_regs = {
	.yes_ranges = rk628_hdmi_volatile_reg_ranges,
	.n_yes_ranges = ARRAY_SIZE(rk628_hdmi_volatile_reg_ranges),
};

static const struct regmap_range rk628_gpio0_readable_ranges[] = {
	regmap_reg_range(RK628_GPIO0_BASE, RK628_GPIO0_BASE + GPIO_VER_ID),
};

static const struct regmap_access_table rk628_gpio0_readable_table = {
	.yes_ranges     = rk628_gpio0_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_gpio0_readable_ranges),
};

static const struct regmap_range rk628_gpio1_readable_ranges[] = {
	regmap_reg_range(RK628_GPIO1_BASE, RK628_GPIO1_BASE + GPIO_VER_ID),
};

static const struct regmap_access_table rk628_gpio1_readable_table = {
	.yes_ranges     = rk628_gpio1_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_gpio1_readable_ranges),
};

static const struct regmap_range rk628_gpio2_readable_ranges[] = {
	regmap_reg_range(RK628_GPIO2_BASE, RK628_GPIO2_BASE + GPIO_VER_ID),
};

static const struct regmap_access_table rk628_gpio2_readable_table = {
	.yes_ranges     = rk628_gpio2_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_gpio2_readable_ranges),
};

static const struct regmap_range rk628_gpio3_readable_ranges[] = {
	regmap_reg_range(RK628_GPIO3_BASE, RK628_GPIO3_BASE + GPIO_VER_ID),
};

static const struct regmap_access_table rk628_gpio3_readable_table = {
	.yes_ranges     = rk628_gpio3_readable_ranges,
	.n_yes_ranges   = ARRAY_SIZE(rk628_gpio3_readable_ranges),
};

static const struct regmap_config rk628_regmap_config[RK628_DEV_MAX] = {
	[RK628_DEV_GRF] = {
		.name = "grf",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = GRF_MAX_REGISTER,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
	},
	[RK628_DEV_CRU] = {
		.name = "cru",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = CRU_MAX_REGISTER,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_cru_readable_table,
	},
	[RK628_DEV_COMBRXPHY] = {
		.name = "combrxphy",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = COMBRX_REG(0x6790),
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_combrxphy_readable_table,
	},
	[RK628_DEV_HDMIRX] = {
		.name = "hdmirx",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = HDMI_RX_MAX_REGISTER,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_hdmirx_readable_table,
	},
	[RK628_DEV_ADAPTER] = {
		.name = "adapter",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = KEY_MAX_REGISTER,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_key_readable_table,
	},
	[RK628_DEV_COMBTXPHY] = {
		.name = "combtxphy",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = COMBTXPHY_CON10,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_combtxphy_readable_table,
	},
	[RK628_DEV_DSI0] = {
		.name = "dsi0",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = DSI0_BASE + DSI_MAX_REGISTER,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_dsi0_readable_table,
	},
	[RK628_DEV_DSI1] = {
		.name = "dsi1",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = DSI1_BASE + DSI_MAX_REGISTER,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_dsi1_readable_table,
	},
	[RK628_DEV_GVI] = {
		.name = "gvi",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = GVI_COLOR_BAR_VTIMING1,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_gvi_readable_table,
	},
	[RK628_DEV_CSI] = {
		.name = "csi",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = CSI_MAX_REGISTER,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_csi_readable_table,
	},
	[RK628_DEV_HDMITX] = {
		.name = "hdmi",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = HDMI_MAX_REG,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_hdmi_volatile_regs,
	},
	[RK628_DEV_GPIO0] = {
		.name = "gpio0",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = RK628_GPIO0_BASE + GPIO_VER_ID,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_gpio0_readable_table,
	},
	[RK628_DEV_GPIO1] = {
		.name = "gpio1",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = RK628_GPIO1_BASE + GPIO_VER_ID,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_gpio1_readable_table,
	},
	[RK628_DEV_GPIO2] = {
		.name = "gpio2",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = RK628_GPIO2_BASE + GPIO_VER_ID,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_gpio2_readable_table,
	},
	[RK628_DEV_GPIO3] = {
		.name = "gpio3",
		.reg_bits = 32,
		.val_bits = 32,
		.reg_stride = 4,
		.max_register = RK628_GPIO3_BASE + GPIO_VER_ID,
		.reg_format_endian = REGMAP_ENDIAN_NATIVE,
		.val_format_endian = REGMAP_ENDIAN_NATIVE,
		.rd_table = &rk628_gpio3_readable_table,
	},
};

static void rk628_display_disable(struct rk628 *rk628)
{
	if (!rk628->display_enabled)
		return;

#ifdef RK628_CSI_OUT
	rk628_csi_disable(rk628);
#endif

#ifdef RK628_GVI_OUT
	rk628_gvi_disable(rk628);
#endif

#ifdef RK628_LVDS_OUT
	rk628_lvds_disable(rk628);
#endif

#ifdef RK628_DSI_OUT
	rk628_dsi_disable(rk628);
#endif
	rk628_post_process_disable(rk628);
#ifdef RK628_HDMI_IN
	rk628_hdmirx_disable(rk628);
#endif

	rk628->display_enabled = false;
}

static void rk628_display_enable(struct rk628 *rk628)
{
#ifdef RK628_HDMI_IN
	u8 ret = 0;
#endif

	if (rk628->display_enabled)
		return;

#ifdef RK628_HDMI_IN
	ret = rk628_hdmirx_enable(rk628);
	if ((ret == HDMIRX_PLUGOUT) || (ret & HDMIRX_NOSIGNAL)) {
		rk628_display_disable(rk628);
		return;
	}
#endif

#ifdef RK628_RGB_IN
	rk628_rgb_rx_enable(rk628);
#endif

	rk628_post_process_init(rk628);
	rk628_post_process_enable(rk628);

#ifdef RK628_DSI_OUT
	rk628_dsi_enable(rk628);
#endif

#ifdef RK628_LVDS_OUT
	rk628_lvds_enable(rk628);
#endif

#ifdef RK628_GVI_OUT
	rk628_gvi_enable(rk628);
#endif

#ifdef RK628_CSI_OUT
	rk628_csi_enable(rk628);
#endif

#ifdef RK628_HDMI_OUT
	rk628_hdmitx_enable(rk628);
#endif

	rk628->display_enabled = true;
}

static void rk628_display_work(struct work_struct *work)
{
	u8 ret = 0;
	struct rk628 *rk628 =
		container_of(work, struct rk628, delay_work.work);

#ifdef RK628_HDMI_IN
	ret = rk628_hdmirx_detect(rk628);
	if (!(ret & HDMIRX_CHANGED)) {
		if (!rk628->plugin_det_gpio)
			queue_delayed_work(rk628->monitor_wq, &rk628->delay_work,
					   msecs_to_jiffies(2000));
		else
			rk628_hdmirx_enable_interrupts(rk628, true);
		return;
	}
#endif

	if (ret & HDMIRX_PLUGIN) {
		/* if resolution or input format change, disable first */
		rk628_display_disable(rk628);
		rk628_display_enable(rk628);
	} else if (ret & HDMIRX_PLUGOUT) {
		rk628_display_disable(rk628);
	}

#ifdef RK628_HDMI_IN
	if (!rk628->plugin_det_gpio)
		queue_delayed_work(rk628->monitor_wq, &rk628->delay_work,
				   msecs_to_jiffies(2000));
	else
		rk628_hdmirx_enable_interrupts(rk628, true);
#endif
}

#ifdef RK628_HDMI_IN
static irqreturn_t rk628_hdmirx_plugin_irq(int irq, void *dev_id)
{
	struct rk628 *rk628 = dev_id;

	rk628_hdmirx_enable_interrupts(rk628, false);
	/* clear interrupts */
	rk628_i2c_write(rk628, HDMI_RX_MD_ICLR, 0xffffffff);
	rk628_i2c_write(rk628, HDMI_RX_PDEC_ICLR, 0xffffffff);
	rk628_i2c_write(rk628, GRF_INTR0_CLR_EN, 0x01000100);

	/* control hpd after 50ms */
	schedule_delayed_work(&rk628->delay_work, HZ / 20);

	return IRQ_HANDLED;
}
#endif

static int
rk628_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct device_node *backlight;
	struct rk628 *rk628;
	int i, ret;
#ifdef RK628_HDMI_IN
	int err;
	unsigned long irq_flags;
#endif

	dev_info(dev, "RK628 misc driver version: %s\n", DRIVER_VERSION);

	rk628 = devm_kzalloc(dev, sizeof(*rk628), GFP_KERNEL);
	if (!rk628)
		return -ENOMEM;

	rk628->dev = dev;
	rk628->client = client;
	i2c_set_clientdata(client, rk628);
	rk628->hdmirx_irq = client->irq;

	rk628->soc_24M = devm_clk_get(dev, "soc_24M");
	if (rk628->soc_24M == ERR_PTR(-ENOENT))
		rk628->soc_24M = NULL;

	if (IS_ERR(rk628->soc_24M)) {
		ret = PTR_ERR(rk628->soc_24M);
		dev_err(dev, "Unable to get soc_24M: %d\n", ret);
		return ret;
	}

	clk_prepare_enable(rk628->soc_24M);

	rk628->enable_gpio = devm_gpiod_get_optional(dev, "enable",
						     GPIOD_OUT_LOW);
	if (IS_ERR(rk628->enable_gpio)) {
		ret = PTR_ERR(rk628->enable_gpio);
		dev_err(dev, "failed to request enable GPIO: %d\n", ret);
		return ret;
	}

	rk628->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_LOW);
	if (IS_ERR(rk628->reset_gpio)) {
		ret = PTR_ERR(rk628->reset_gpio);
		dev_err(dev, "failed to request reset GPIO: %d\n", ret);
		return ret;
	}

	rk628->plugin_det_gpio = devm_gpiod_get_optional(dev, "plugin-det",
						    GPIOD_IN);
	if (IS_ERR(rk628->plugin_det_gpio)) {
		dev_err(rk628->dev, "failed to get hdmirx det gpio\n");
		ret = PTR_ERR(rk628->plugin_det_gpio);
		return ret;
	}

	rk628->panel = devm_kzalloc(dev, sizeof(struct panel_simple), GFP_KERNEL);
	if (!rk628->panel)
		return -ENOMEM;

	rk628->panel->enable_gpio = devm_gpiod_get_optional(dev, "panel-enable", GPIOD_OUT_LOW);
	if (IS_ERR(rk628->panel->enable_gpio)) {
		ret = PTR_ERR(rk628->panel->enable_gpio);
		dev_err(dev, "failed to request panel enable GPIO: %d\n", ret);
		return ret;
	}


	backlight = of_parse_phandle(dev->of_node, "panel-backlight", 0);
	if (backlight) {
		rk628->panel->backlight = of_find_backlight_by_node(backlight);
		of_node_put(backlight);

		if (!rk628->panel->backlight) {
			dev_err(dev, "failed to find backlight \n");
			return -EPROBE_DEFER;
		}

	}

	gpiod_set_value(rk628->enable_gpio, 1);
	usleep_range(10000, 11000);
	gpiod_set_value(rk628->reset_gpio, 0);
	usleep_range(10000, 11000);
	gpiod_set_value(rk628->reset_gpio, 1);
	usleep_range(10000, 11000);
	gpiod_set_value(rk628->reset_gpio, 0);
	usleep_range(10000, 11000);

	for (i = 0; i < RK628_DEV_MAX; i++) {
		const struct regmap_config *config = &rk628_regmap_config[i];

		if (!config->name)
			continue;

		rk628->regmap[i] = devm_regmap_init_i2c(client, config);
		if (IS_ERR(rk628->regmap[i])) {
			ret = PTR_ERR(rk628->regmap[i]);
			dev_err(dev, "failed to allocate register map %d: %d\n",
				i, ret);
			return ret;
		}
	}

	/* selete int io function */
	ret = rk628_i2c_write(rk628, GRF_GPIO3AB_SEL_CON, 0x30002000);
	if (ret) {
		dev_err(dev, "failed to access register: %d\n", ret);
		return ret;
	}

	rk628->monitor_wq = alloc_ordered_workqueue("%s",
		WQ_MEM_RECLAIM | WQ_FREEZABLE, "rk628-monitor-wq");
	INIT_DELAYED_WORK(&rk628->delay_work, rk628_display_work);

	rk628_cru_init(rk628);

#ifdef RK628_CSI_OUT
	rk628_csi_init(rk628);
#endif

#ifdef RK628_HDMI_IN
	if (rk628->plugin_det_gpio) {
		rk628->plugin_irq = gpiod_to_irq(rk628->plugin_det_gpio);
		if (rk628->plugin_irq < 0) {
			dev_err(rk628->dev, "failed to get plugin det irq\n");
			err = rk628->plugin_irq;
			return err;
		}

		err = devm_request_threaded_irq(dev, rk628->plugin_irq, NULL,
				rk628_hdmirx_plugin_irq, IRQF_TRIGGER_FALLING |
				IRQF_TRIGGER_RISING | IRQF_ONESHOT, "rk628_hdmirx", rk628);
		if (err) {
			dev_err(rk628->dev, "failed to register plugin det irq (%d)\n", err);
			return err;
		}

		if (rk628->hdmirx_irq) {
			irq_flags = irqd_get_trigger_type(irq_get_irq_data(rk628->hdmirx_irq));
			dev_dbg(rk628->dev, "cfg hdmirx irq, flags: %lu!\n", irq_flags);
			err = devm_request_threaded_irq(dev, rk628->hdmirx_irq, NULL,
					rk628_hdmirx_plugin_irq, irq_flags |
					IRQF_ONESHOT, "rk628", rk628);
			if (err) {
				dev_err(rk628->dev, "request rk628 irq failed! err:%d\n",
						err);
				return err;
			}
			/* hdmirx int en */
			rk628_i2c_write(rk628, GRF_INTR0_EN, 0x01000100);
			queue_delayed_work(rk628->monitor_wq, &rk628->delay_work,
					   msecs_to_jiffies(1000));
		}
	} else {
		rk628_display_enable(rk628);
		queue_delayed_work(rk628->monitor_wq, &rk628->delay_work,
				    msecs_to_jiffies(3000));
	}
#else
	rk628_display_enable(rk628);
#endif

	pm_runtime_enable(dev);

	return 0;
}

static int rk628_i2c_remove(struct i2c_client *client)
{
	struct device *dev = &client->dev;

	pm_runtime_disable(dev);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int rk628_suspend(struct device *dev)
{
	struct rk628 *rk628 = dev_get_drvdata(dev);

	rk628_display_disable(rk628);

	return 0;
}

static int rk628_resume(struct device *dev)
{
	struct rk628 *rk628 = dev_get_drvdata(dev);

	rk628_display_enable(rk628);

	return 0;
}
#endif

static const struct dev_pm_ops rk628_pm_ops = {
#ifdef CONFIG_PM_SLEEP
	.suspend = rk628_suspend,
	.resume = rk628_resume,
#endif
};
static const struct of_device_id rk628_of_match[] = {
	{ .compatible = "rockchip,rk628", },
	{}
};
MODULE_DEVICE_TABLE(of, rk628_of_match);

static const struct i2c_device_id rk628_i2c_id[] = {
	{ "rk628", 0 },
	{}
};
MODULE_DEVICE_TABLE(i2c, rk628_i2c_id);

static struct i2c_driver rk628_i2c_driver = {
	.driver = {
		.name = "rk628",
		.of_match_table = of_match_ptr(rk628_of_match),
		.pm = &rk628_pm_ops,
	},
	.probe = rk628_i2c_probe,
	.remove = rk628_i2c_remove,
	.id_table = rk628_i2c_id,
};
module_i2c_driver(rk628_i2c_driver);

MODULE_AUTHOR("Wyon Bi <bivvy.bi@rock-chips.com>");
MODULE_DESCRIPTION("Rockchip RK628 MFD driver");
MODULE_LICENSE("GPL v2");
