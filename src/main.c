/* SPDX-License-Identifier: Apache-2.0 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG);

#define DT_DRV_COMPAT test_i2c_device

#define REG_ADDR 0x00

struct test_i2c_device_config {
        struct i2c_dt_spec i2c;
};

static int test_i2c_device_init(const struct device *dev)
{
	const struct test_i2c_device_config *cfg = dev->config;
	int ret;
	uint8_t value;

	LOG_DBG("");

#if 0
	if (!i2c_is_ready_dt(&cfg->i2c)) {
		LOG_ERR("I2C bus device not ready.");
		return -ENODEV;
	}
#endif

	ret = i2c_reg_read_byte_dt(&cfg->i2c, REG_ADDR, &value);
	if (ret) {
		LOG_ERR("i2c_reg_read_byte_dt error: %d", ret);
		return -EIO;
	}

	LOG_INF("reg %02x: %02x", REG_ADDR, value);

	return 0;
}

#define TEST_I2C_DEVICE_DEFINE(n) \
        static const struct test_i2c_device_config test_i2c_device_config_##n = { \
                .i2c = I2C_DT_SPEC_INST_GET(n), \
        }; \
        DEVICE_DT_INST_DEFINE(n, test_i2c_device_init, NULL, \
			      NULL, &test_i2c_device_config_##n, \
			      POST_KERNEL, 1, \
			      NULL);

DT_INST_FOREACH_STATUS_OKAY(TEST_I2C_DEVICE_DEFINE)

int main(void)
{
	LOG_DBG("");

	return 0;
}
