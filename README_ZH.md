# device_soc_esp

#### 介绍

 在该目录下托管湖南开鸿智谷数字产业发展有限公司基于ESP32 Niobe系列开发板(niobeu4)的驱动代码

| 目录名称                                        | 开发板型号      | SOC型号       |
| ----------------------------------------------- | --------------- | ------------- |
| [esp32](esp32/README_zh.md)                     | niobeu4         | ESP32U4WDH    |
| [esp32_wrover_ie](esp32_wrover_ie/readme_zh.md) | ESP32_WROVER_IE | ESP32-D0WD-V3 |

#### 代码框架

```
device/soc/esp
├── esp32_wrover_ie                       # ESP32芯片平台
├── esp32                                 # ESP32芯片平台
├── Kconfig.liteos_m.defconfig            # kconfig 默认宏配置
├── Kconfig.liteos_m.series               # esp系列soc配置宏
└── Kconfig.liteos_m.soc                  # soc kconfig配置宏
```

#### 编译流程

在[device_board_openvalle](https://gitee.com/openharmony-sig/device_board_openvalley/blob/master/niobeu4/README_zh.md) 仓库下进行详细介绍。

#### 相关仓库

[device_board_openvalle](https://gitee.com/openharmony-sig/device_board_openvalley)

[vendor_openvalle](https://gitee.com/openharmony-sig/vendor_openvalley)