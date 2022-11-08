# device_soc_gigadevice

## 介绍

基于兆易创新芯片开发的soc部分，主要存放gd32f450芯片的sdk和适配代码。目前已添加GD32F4系列的芯片，开发不同的device/board时，可共用该仓库代码进行开发。board和soc关系如下所示：

|                            开发板名称                            |   Soc型号   |      应用领域      | 设备互联类型 |
| :---------------------------------------------------------------: | :----------: | :----------------: | :----------: |
| [KHDVK-450A](https://gitee.com/openharmony-sig/device_board_kaihong) | GD32F450ZKT6 | 图形、工业、物联网 |  有线以太网  |

### soc介绍

GD32F450ZKT6系列MCU采用Arm® Cortex®-M4内核，处理器主频高达200MHz，可支持算法复杂度更高的嵌入式应用，具备更快速的实时处理能力，并拥有业界领先的大容量存储优势。GD32F450ZKT6具有丰富的外设资源特性，可提供多达4个USART和4个UART，3个I2C，6个SPI，2个I2S，2个CAN2.0B，1个SDIO接口，1个10/100M以太网控制器，并支持USB2.0 FS和HS通信，还配备了3个采样率高达2.6M SPS的12位高速ADC和2个12位DAC。此外KHDVK-450A集成了TFT LCD控制器和硬件图形加速器IPA，以实现液晶驱动并显著提升显示图像显示画质。

## 目录

```
device/soc/gigadevice
├── gd32f4xx                                #GD32F4系列芯片   
│    ├── sdk                                #sdk代码目录
│    └── ...
├── Kconfig.liteos_m.defconfig              #Kconfig默认配置宏
├── Kconfig.liteos_m.series                 #gd系列soc配置宏
├──	Kconfig.liteos_m.soc                    #soc Kconfig配置宏
└── ...
```

## 代码获取与固件烧录

代码获取流程与固件烧录实践操作见如下参考链接：

- [KHDVK-450A开发板实践指南](https://gitee.com/openharmony-sig/vendor_kaihong/blob/master/khdvk_450a/README_zh.md)

## 相关仓

- [device_board_kaihong](https://gitee.com/openharmony-sig/device_board_kaihong)
- [vendor_kaihong](https://gitee.com/openharmony-sig/vendor_kaihong)
