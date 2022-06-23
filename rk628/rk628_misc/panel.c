
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020 Rockchip Electronics Co. Ltd.
 *
 * Author: Guochun Huang <hero.huang@rock-chips.com>
 */

#include <linux/gpio/consumer.h>
#include <linux/backlight.h>

#include "panel.h"

void panel_pre_enable(struct rk628 *rk628)
{
	if (rk628->panel->enable_gpio) {
		gpiod_set_value(rk628->panel->enable_gpio, 0);
		mdelay(120);
		gpiod_set_value(rk628->panel->enable_gpio, 1);
		mdelay(120);
	}

	if (rk628->panel->reset_gpio) {
		gpiod_set_value(rk628->panel->reset_gpio, 0);
		mdelay(120);
		gpiod_set_value(rk628->panel->reset_gpio, 1);
		mdelay(120);
		gpiod_set_value(rk628->panel->reset_gpio, 0);
		mdelay(120);
	}

}

void panel_enable(struct rk628 *rk628)
{
	if (rk628->panel->backlight)
		backlight_enable(rk628->panel->backlight);
}

void panel_disable(struct rk628 *rk628)
{
	if (rk628->panel->backlight)
		backlight_disable(rk628->panel->backlight);



	if (rk628->panel->reset_gpio) {
		gpiod_set_value(rk628->panel->reset_gpio, 1);
		mdelay(120);
	}

	if (rk628->panel->enable_gpio) {
		gpiod_set_value(rk628->panel->enable_gpio, 0);
		mdelay(120);
	}
}
