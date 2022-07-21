# NIOBEU4

# 简介

## 开发板概述

Niobeu4是基于乐鑫ESP32U4WDH芯片推出的物联网设备开发套件，集成2.4GHz Wifi和蓝牙双模，具有超高的射频性能、稳定性、通用性和可靠性，以及超低的功耗，适用于各种应用场景；

# 目录

```
device/soc/esp/esp32
├── components                                 # ESP32组件目录
│   ├── bootloader_support                     # bootloader组件
│   ├── bt                                     # 蓝牙组件
│   ├── esp_wifi                               # wifi组件
│   ├── lwip                                   # lwip组件
│   ├── spi_flash                              # spi组件
│   ├── tcpip_adapter                          # tcpip适配组件
│   ├── vfs
│   ├── ...
├── Kconfig.liteos_m.defconfig.esp32            # esp32 kconfig 默认宏配置
├── Kconfig.liteos_m.series                     # esp32_wrover_ie系列soc配置宏
└── Kconfig.liteos_m.soc                        # soc kconfig配置宏
```

# 说明

适配鸿蒙编译集成bsp的目录

# 相关仓

* [device/soc/esp](https://gitee.com/openharmony-sig/device_soc_esp)

