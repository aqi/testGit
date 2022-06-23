// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2020 Rockchip Electronics Co. Ltd.
 *
 * Author: Guochun Huang <hero.huang@rock-chips.com>
 */

#include "rk628.h"
#include "panel.h"

void rk628_rgb_decoder_enable(struct rk628 *rk628)
{
		/* config sw_input_mode RGB */
	rk628_i2c_update_bits(rk628, GRF_SYSTEM_CON0, SW_INPUT_MODE_MASK,
			      SW_INPUT_MODE(INPUT_MODE_RGB));

	/* pinctrl for vop pin */
	rk628_i2c_write(rk628, GRF_GPIO2AB_SEL_CON, 0xffffffff);
	rk628_i2c_write(rk628, GRF_GPIO2C_SEL_CON, 0xffff5555);
	rk628_i2c_write(rk628, GRF_GPIO3AB_SEL_CON, 0x10b010b);

	/* rk628: modify IO drive strength for RGB */
	rk628_i2c_write(rk628, GRF_GPIO2A_D0_CON, 0xffff1111);
	rk628_i2c_write(rk628, GRF_GPIO2A_D1_CON, 0xffff1111);
	rk628_i2c_write(rk628, GRF_GPIO2B_D0_CON, 0xffff1111);
	rk628_i2c_write(rk628, GRF_GPIO2B_D1_CON, 0xffff1111);
	rk628_i2c_write(rk628, GRF_GPIO2C_D0_CON, 0xffff1111);
	rk628_i2c_write(rk628, GRF_GPIO2C_D1_CON, 0xffff1111);
	rk628_i2c_write(rk628, GRF_GPIO3A_D0_CON, 0xffff1011);
	rk628_i2c_write(rk628, GRF_GPIO3B_D_CON, 0x10001);
}

void rk628_rgb_encoder_enable(struct rk628 *rk628)
{
	rk628_i2c_update_bits(rk628, GRF_SYSTEM_CON0,
			      SW_BT_DATA_OEN_MASK | SW_OUTPUT_MODE_MASK,
			      SW_OUTPUT_MODE(OUTPUT_MODE_RGB));
	rk628_i2c_update_bits(rk628, GRF_POST_PROC_CON, SW_DCLK_OUT_INV_EN,
			      SW_DCLK_OUT_INV_EN);

	panel_pre_enable(rk628);
	panel_enable(rk628);
}

void rk628_rgb_encoder_disable(struct rk628 *rk628)
{
	panel_disable(rk628);
}


void rk628_rgb_rx_enable(struct rk628 *rk628)
{
		/* chose encoder rgb or bt1120 */
		rk628_rgb_decoder_enable(rk628);
		//rk628_bt1120_rx_enable();
}

void rk628_rgb_tx_enable(struct rk628 *rk628)
{
		/* chose encoder rgb or bt1120 */
		rk628_rgb_encoder_enable(rk628);
		//rk628_bt1120_encoder_enable();

		panel_pre_enable(rk628);
		panel_enable(rk628);
}

void rk628_rgb_tx_disable(struct rk628 *rk628)
{
		panel_disable(rk628);
}
