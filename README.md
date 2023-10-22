# Priority check demo

### 1. checkout v3.4.0, build, no error, fails in runtime

```console
$ git -C ~/zephyrproject/zephyr checkout v3.4.0
$ west update
$ west build -p
$ west flash
```

On the console:
```console
[00:00:00.492,431] <dbg> app: test_i2c_device_init:
[00:00:00.644,195] <err> app: i2c_reg_read_byte_dt error: -5
[00:00:00.644,226] <err> i2c_nrfx_twi: Failed to initialize device: i2c@40003000
```

### 2. checkout v3.5.0, build, error at build time

```console
$ git -C ~/zephyrproject/zephyr checkout v3.5.0
$ west update
$ west build -p
...
ERROR: /soc/i2c@40003000/test-i2c-device@20 POST_KERNEL 1 < /soc/i2c@40003000 POST_KERNEL 3
ninja: build stopped: subcommand failed.
FATAL ERROR: command exited with status 1: /usr/bin/cmake --build build
...
$ ~/zephyrproject/zephyr/scripts/build/check_init_priorities.py -i
...
POST_KERNEL
  __init_enable_logger: enable_logger(NULL)
  __init___device_dts_ord_105: test_i2c_device_init(__device_dts_ord_105)
  __init_malloc_prepare: malloc_prepare(NULL)
  __init___device_dts_ord_104: twi_0_init(__device_dts_ord_104)
...
```

### 3. fix the level, build, works

```diff
--- a/src/main.c
+++ b/src/main.c
@@ -49,7 +49,7 @@ static int test_i2c_device_init(const struct device *dev)
         }; \
         DEVICE_DT_INST_DEFINE(n, test_i2c_device_init, NULL, \
                              NULL, &test_i2c_device_config_##n, \
-                             POST_KERNEL, 1, \
+                             POST_KERNEL, 99, \
                              NULL);

 DT_INST_FOREACH_STATUS_OKAY(TEST_I2C_DEVICE_DEFINE)
```

```console
$ west build
$ west flash
$ west build -t initlevels
...
POST_KERNEL
  __init_enable_logger: enable_logger(NULL)
  __init_malloc_prepare: malloc_prepare(NULL)
  __init___device_dts_ord_104: twi_0_init(__device_dts_ord_104)
  __init___device_dts_ord_105: test_i2c_device_init(__device_dts_ord_105)
...
```

On the console:
```console
[00:00:00.485,809] <dbg> app: test_i2c_device_init:
[00:00:00.486,328] <inf> app: reg 00: ff
```
