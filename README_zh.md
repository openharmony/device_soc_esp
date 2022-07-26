# device_soc_esp

## 介绍

该仓库托管乐鑫科技旗下soc芯片相关代码，不同soc型号，对应不同的开发板及目录名：

| SOC型号       | 对应开发板                                                   | 目录名称        |
| ------------- | ------------------------------------------------------------ | --------------- |
| ESP32U4WDH    | [NiobeU4](https://gitee.com/openharmony-sig/device_board_openvalley/blob/master/niobeu4/README_zh.md) | esp32 |

## 目录框架

```
device/soc/esp
├── esp32                                 # ESP芯片平台目录
├── Kconfig.liteos_m.defconfig            # Kconfig配置
├── Kconfig.liteos_m.series
└── Kconfig.liteos_m.soc
```

## 相关仓

[device_board_openvalley](https://gitee.com/openharmony-sig/device_board_openvalley.git)

[vendor_openvalley](https://gitee.com/openharmony-sig/vendor_openvalley.git)