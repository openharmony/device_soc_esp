# device_espressif

#### 介绍

##### 目录

```
device/soc/espressif
├── esp32_wrover_ie                       # 芯片SOC名称
├── ...                                   # 芯片SOC名称
├── Kconfig.liteos_m.defconfig            # kconfig 默认宏配置
├── Kconfig.liteos_m.series               # esp系列soc配置宏
└── Kconfig.liteos_m.soc                  # soc kconfig配置宏
```

编译环境搭建包含如下几步：
1. 环境搭建
2. 获取源码
3. 源码构建
4. 在开发板中运行镜像

## 1.环境搭建

   1. esp-idf安装

      使用安装指导请参考：(https://docs.espressif.com/projects/esp-idf/zh_CN/release-v4.1/get-started/index.html)

      注：esp-idf安装可以跳过，当前已将生成好的bootloader.bin和partition-table.bin放入device\soc\espressif\esp32_wrover_ie\pack_tools文件夹中。

      注：若跳过安装esp-idf可以按照下列方式安装工具链：

      1.下载官方release的SDK包：https://www.espressif.com/zh-hans/support/download/sdks-demos?keys=&field_type_tid%5B%5D=13

      2.将下载好的SDK包放入linux系统，进入目录执行如下指令：

         ```shell
         unzip esp-idf-v4.3.1.zip
         cd esp-idf-v4.3.1/
         ./install.sh
         . ./export.sh
         ```

      注：本教程使用的工具链版本为gcc version 8.2.0 (crosstool-NG esp-2019r2)或gcc version 8.4.0 (crosstool-NG esp-2021r1)

   2. esptool安装

      ```shell
      python -m pip install esptool
      ```
   3. hb工具安装

      a) 运行如下命令安装hb

         ```
         pip3 install build/lite
         ```
      b) 设置环境变量

         ```
         vim ~/.bashrc
         ```

         将以下命令拷贝到.bashrc文件的最后一行，保存并退出。

         ```
         export PATH=~/.local/bin:$PATH
         ```

         执行如下命令更新环境变量。

         ```
         source ~/.bashrc
         ```
      c) 执行"hb -h"，有打印以下信息即表示安装成功：

         ```
         usage: hb

         OHOS build system

         positional arguments:
         {build,set,env,clean}
            build               Build source code
            set                 OHOS build settings
            env                 Show OHOS build env
            clean               Clean output

         optional arguments:
         -h, --help            show this help message and exit
         ```

   3. 编译镜像

      a) 生成bootloader.bin

         ```shell
         $ cd esp-idf
         $ ./install.sh
         $ ./export.sh
         $ cd examples/get-started/hello_world
         $ make menuconfig
         (关掉 Partition Table -> [*] Generate an MD5 checksum for the partition table 选项)
         $ make bootloader
         ```

      b) 生成分区表

         使用`gen_esp32part.py`脚本将用户自定义csv文件转换为对应bin文件，如：
         ```shell
         $ gen_esp32part.py partitions_user.csv partitions_user.bin --disable-md5sum
         ```

         esp32 csv文件可参考以下配置：

         ```
         nvs,data,nvs,0x9000,24K,
         phy_init,data,phy,0xf000,4K,
         factory,app,factory,0x10000,1M,
         ```

   注：编译镜像可以跳过，当前已将生成好的bootloader.bin和partition-table.bin放入device\soc\espressif\esp32_wrover_ie\pack_tools文件夹中。

## 2.获取源码

   [代码获取](https://gitee.com/openharmony/docs/blob/master/zh-cn/device-dev/get-code/sourcecode-acquire.md)

   提示: 可以使用 `repo` 命令来获取源码。

   下载好源码后通过下列指令下载开发板代码：

   ```shell
      $ repo init -u https://gitee.com/openharmony-sig/manifest.git -m devboard_espressif.xml --no-repo-verify
      $ repo sync -c
      $ repo forall -c 'git lfs pull'
   ```

## 3.源码构建

   1. 执行hb set命令并选择项目`esp32_wrover_ie_demo`。

      ```shell
      $ hb set
      ```

   2. 进入kernel/liteos_m中执行make manuconfig选择Platform -> Chip -> qemu_xtensa_esp32,配置当前开发板。

      ```shell
      $ cd kernel/liteos_m/
      $ make menuconfig
      ```

   3. 执行hb clean && hb build命令构建产生 `OHOS_Image` 的可执行文件。

      ```shell
      $ hb clean && hb build
      ```

   4. 在构建完成之后，对应的可执行文件在主目录下：

      ```
      out/esp32_devkitc_v4/esp32_wrover_ie_demo/
      ```

   5. 在主目录下执行以下命令生成bin文件：

      ```shell
      esptool.py --chip esp32 elf2image --flash_mode dio --flash_freq 80m     \
       --flash_size 4MB -o out/esp32_devkitc_v4/esp32_wrover_ie_demo/OHOS_Image.bin out/esp32_devkitc_v4/esp32_wrover_ie_demo/OHOS_Image
      ```

## 4.在开发板中运行镜像

   1. 下载烧录工具

      烧录工具（Flash下载工具）链接：https://www.espressif.com/zh-hans/support/download/other-tools

   2. 使用烧录工具

      使用参考 https://docs.ai-thinker.com/esp_download 中 “不支持OTA” 的FLASH地址配置方式：

      a) 配置bootloader.bin在地址0x1000，配置partition-table.bin在地址0x8000，配置OHOS_Image.bin在地址0x10000。

      b) 配置SPI SPEED为40MHz，配置SPI MODE为DOUT，配置FLASH SIZE为8Mbit，波特率设置为115200。

      c) 点击START开始下载。

注1：如果没有手动生成bootloader.bin和partition-table.bin，可以直接使用device\soc\espressif\esp32_wrover_ie\pack_tools下的bootloader.bin和partition-table.bin。

注2：烧录用的bin文件需要按照3-4中的方法使用esptool.py工具生成。
