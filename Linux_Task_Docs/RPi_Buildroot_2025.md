# Buildroot for Raspberry Pi 4B

This guide aims to document the full process for configuring and setting up a Buildroot environment for Raspberry Pi 4 with the ability to boot and run applications.

## Equipment Prerequisites

* A Linux system (preferably Ubuntu/Debian).
* A Raspberry Pi 4B board.
* A micro SD card (8GB+ storage).
* A micro SD card reader.
* Stable internet connection.

## Essential Dependencies

* libncurses5-dev (for menuconfig).
* git (to clone buildroot repository).
* build-essential (g++, gcc, and make tools).
* bc (to build scripts).
* libssl-dev (for OpenSSL-based packages).
* bison/flex (lexical parser tools).
* unzip/wget (to fetch source archives).
* cpio (for initializing ramdisk).
* rsync (used for rootfs creation).
* file (detects file types).
* python3 (may be required for certain packages).
* u-boot-tools (Utilities to work with U-Boot bootloader).
* gcc-multilib (Enables 32-bit compilation on 64-bit systems — important for building cross-compilers or 32-bit software).
* libelf-dev (ELF format library needed for tools that inspect or manipulate ELF binaries like kernel modules).
* device-tree-compiler (Compiles Device Tree Source (DTS) files into binaries (.dtb) for kernel booting on embedded systems).
* gawk (GNU version of awk required in many build scripts and Makefiles for text processing).
* libexpat1-dev (XML parsing library often used in build systems or kernel tooling like device tree validation).

On Debian/Ubuntu systems, install them using-
```bash
sudo apt update
sudo apt install -y \
    build-essential \
    git \
    cpio \
    unzip \
    rsync \
    bc \
    python3 \
    file \
    wget \
    libncurses5-dev \
    libssl-dev \
    flex \
    bison \
    gawk \
    libelf-dev \
    libexpat1-dev \
    gcc-multilib \
    libncurses-dev \
    device-tree-compiler \
    u-boot-tools
```


## Install and Setup Steps

#### Note: After each build/make you can run these commands to clear the terminal-
```bash
make clean
make distclean
```


### Step 1. Download and Extract the Latest Stable Buildroot
```bash
#Download the latest stable buildroot
wget https://buildroot.org/downloads/buildroot-2025.02.4.tar.gz
tar -xvzf buildroot-2025.02.4.tar.gz
cd buildroot-2025.02.4
```

Expected Response after download step:
```bash
--2025-06-17 13:28:58--  https://buildroot.org/downloads/buildroot-2025.02.1.tar.gz
Resolving buildroot.org (buildroot.org)... 138.201.205.61
Connecting to buildroot.org (buildroot.org)|138.201.205.61|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: 7649047 (7.3M) [application/x-gtar-compressed]
Saving to: ‘buildroot-2025.02.1.tar.gz’

buildroot-2025.02.1.tar.gz                         100%[================================================================================================================>]   7.29M  1.06MB/s    in 21s     

2025-06-17 13:29:22 (353 KB/s) - ‘buildroot-2025.02.1.tar.gz’ saved [7649047/7649047]
```


### Step 2. Load Raspberry Pi 4B Default Configuration
```bash
make raspberrypi4_defconfig
```

Expected Response:
```bash
mkdir -p /home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config/lxdialog
PKG_CONFIG_PATH="" make CC="/usr/bin/gcc" HOSTCC="/usr/bin/gcc" \
    obj=/home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config -C support/kconfig -f Makefile.br conf
/usr/bin/gcc -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -DCURSES_LOC="<ncurses.h>" -DNCURSES_WIDECHAR=1 -DLOCALE  -I/home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config -DCONFIG_=\"\"  -MM *.c > /home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config/.depend 2>/dev/null || :
/usr/bin/gcc -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -DCURSES_LOC="<ncurses.h>" -DNCURSES_WIDECHAR=1 -DLOCALE  -I/home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config -DCONFIG_=\"\"   -c conf.c -o /home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config/conf.o
/usr/bin/gcc -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -DCURSES_LOC="<ncurses.h>" -DNCURSES_WIDECHAR=1 -DLOCALE  -I/home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config -DCONFIG_=\"\"  -I. -c /home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config/zconf.tab.c -o /home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config/zconf.tab.o
/usr/bin/gcc -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -DCURSES_LOC="<ncurses.h>" -DNCURSES_WIDECHAR=1 -DLOCALE  -I/home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config -DCONFIG_=\"\"   /home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config/conf.o /home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config/zconf.tab.o  -o /home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config/conf
rm /home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/output/build/buildroot-config/zconf.tab.c
#
# configuration written to /home/vayavya/Documents/RPI_project2025/buildroot-2025.02.1/.config
#
```


### Step 3. Customize Using Menuconfig
```bash
make menuconfig
```
Here you enter the menuconfig GUI. To navigate the menuconfig utility, use your keyboard:
* To navigate directionally, use the arrow keys
* To enter a submenu (indicated by --->), press the Enter key
* To go up a level or exit, press Escape twice
* To toggle the state of a binary option, press the space bar
* To select the state of a multiple choice option, press Enter to open a submenu, the arrow keys to navigate the submenu, and press Enter again to select a state
* To get help with an option or menu, press the H key

Customisation is optional. We can enable SSH or set a root password, even edit certain file system parameters.
Important options include the Root Password and Build Options.


### Step 4. Build Everything
This make take over an hour depending on your system.
```bash
make
```


### Step 5. Identify Your Storage Device
Insert your micro SD card then run-
```bash
lsblk
```

Look for a file name such as /dev/sdb
#### WARNING-Using the wrong device will erase your hard drive!


### Step 6. Flash the Image to Your Storage Device
```bash
sudo dd if=output/images/sdcard.img of=/dev/sdX bs=4M status=progress conv=fsync
```
This is how the dd command works here:
- Replace sdX with your SD card device such as sdb, etc. DO NOT ADD A PARTITION BUT THE WHOLE DEVICE! (sdb, not sdb1).
- if is the input file (your buildroot image).
- of is the output device (your SD card).
- bs=4M writes in 4MB blocks.
- status=progress shows write progress.
- conv=fsync ensures that all writes are flushed


### Step 7. Safely Eject Your Storage Device
```bash
sync
sudo eject /dev/sdX
```
Remove your SD card after running the above commands. 


### Step 8. Boot the Raspberry Pi 4B Board
Now, to verify our results-
* Connect your Raspberry Pi 4B board to power, a keyboard, a display, and a mouse.
* Insert the SD card and boot up the board.
* If a password was set, login as root.

We now have a working basic Linux OS with room for customisation on the Raspberry Pi 4B.


## Appendix A (Common Mistakes and Errors)
Here is a list of common errors during the above process, categorized by step:
* Dependency Downloads
  * Not using sudo to install packages.
  * Missing libncurses (without which menuconfig fails).
  * Missing wget, bzip2 (Essential for later steps).

* Step 1
  * Cloning an old branch (may not work for newer systems).
  * Missing extract command (no new dircetory created for cd command).
  * No cd command (cannot complete step 2 in the wrong directory).
  * Network connectivity breaks (Ensure stable network and try again).
 
* Step 2
  * Using incorrect Raspberry Pi board version (Mentioning model 3 instead of 4).
  * Error: No rule to make target 'raspberrypi4_defconfig' (Either an extraction command issue or you are in the wrong directory).

* Step 3
  * Forgetting to enable fs support for certain changed parameters.
  * Missing essential dependencies (see dependency downloads section)
 
* Step 4
  * Build hangs (Low RAM or inadequate system specs).
  * Error: unable to resolve host address (network connection was interrupted, retry with stable connection).
 
* Step 5
  * See warning in Step 5 procedure
 
* Step 6
  * Incorrect input and output file naming (can cause fatal errors and data loss, to ensure correct naming).
 
* Step 7 
  * Do not remove the SD card before running the requisite commands.
 
* Step 8 
  * Ensure the device is powered and the display chosen is functional.


## Appendix B (Buildroot and Raspberry Pi 4B Background)

### Raspberry Pi 4B
It is a cost-effective and versatile platform for programming, and peripheral interfacing. It's features include a/an:
- BCM2835 ARM processor System-on-chip (SoC).
- Video core graphics processing unit (GPU).
- High Definition Multimedia Interface (HDMI).
- General purpose Input Output (GPIO) ports.
- Universal Serial Bus (USB) ports.
- micro SD card slot to load the requisite operating system.
- Ethernet port (exclusive to model B).
- Micro USB port (specifically for power).
- Camera serial interface (CSI).
- Display serial interface (DSI).

What grants this board it's versatility is it's 40 GPIO pins that allow it to utilise various protocols:
1. UART or Universal Asynchronous Receiver Transmitter module.
2. I2C or Inter-Integrated-Circuit module.
3. SPI or Serial Peripheral Interface module.
4. MISO or Master In, Slave Out.
5. MOSI or Master Out, Slave In.
6. SDI or Serial Digital Interface.
7. SCL or Serial Clock LIne.

These features serve as both, quality-of-life factors and essential dependencies for certain project types. For example, feature (1) is an essential module for communications projects, and (2) and (3) are required for inter-board communication and peripheral interfacing respectively.

In addition to it's aforementioned strengths, it also has a large online community and it's use is very well-documented. Those resources can be accessed [here] [https://www.raspberrypi.com/documentation/]. 

It's own custom OS too is serviceable for most basic tasks and projects. Rasbian OS provides a desktop environment, some pre-installed software, and a beginner-friendly experience. It's ideal for learning, and projects that don't require extreme customization. However, as the goal of our project is to update it to a minimal Linux image, we explored buildroot.


### Buildroot
It is a tool that simplifies the process of building, bootable Linux system for embedded devices. It automates the building, configuration, and installation of the kernel, bootloader, and rootfs, making it easier for us to create customized Linux distributions. Buildroot also supports cross-compilation, allowing developers to build on a different architecture than the target device. An essential resource here is the official user manual for it which contains several helpful tips including shell commands and common mistakes. You can access it [here][https://buildroot.org/downloads/manual/manual.html].

It's directory structure is a good way to understand it's overall functionality:
* __output/__ is the root directory for all build outputs. It has-
  1. __build/__ to store source code for all packages. It also holds build processes for essential tools.
  2. __host/__ contains the cross-compilation toolchain and other tools for our host system. It also holds sysroot.
  3. __staging/__ is the symbolic link to the target toolchain sysroot. It enables backwards compatibility.
  4. __target/__ is the almost complete rootfs for the target system barring device files and permissions.
  5. __images/__ holds the generated kernel, bootloader and rootfs images to be deployed to the target device.
* __board/__ is used during out-of-tree builds for specific board configurations such as defconfig and patch files.
* __package/__ holds package definitions and source codes for all buildable software components in the buildroot.
* __external/__ supports out-of-tree builds' custom configs and packages.

In this manner buildroot provides a great deal of control and convenience to the developer and has allowed us to easily setup and upgrade our custom OS for the Raspberry Pi 4B board.




















