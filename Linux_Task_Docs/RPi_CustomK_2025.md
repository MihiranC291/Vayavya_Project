# Bring Up Process For A Custom Kernel on Raspberry Pi 4B

The aim of this guide is to outline the bring up process for a custom kernel on a Raspberry Pi 4 board. It shall utilize the concepts of cross-compilation, and menuconfig adjustment.

## Procedure

### Step 1: Dependency Installation
The following dependencies are essential-
* bison
* flex 
* libssl-dev
* make
* bc 
* libc6-dev
* libncurses5-dev

You can run the following command to set them up.

```bash
sudo apt install bc bison flex libssl-dev make libc6-dev libncurses5-dev
```


### Step 2: Cross Compilation Toolchain
Download the corresponding toolchain for your device architecture. 

```bash
sudo apt install crossbuild-essential-arm64
```


### Step 3: Download and Extract Linux Source
```bash
# Download the latest stable kernel from kernel.org
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.x.tar.xz
tar xvf linux-6.x.tar.xz
cd linux-6.x
```
Where x is the latest version you find on the site.


### Step 4: Build Configuration
For the Raspberry Pi 4, run the following commands

```bash
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- defconfig -j$(nproc)
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- menuconfig -j$(nproc)
```
This will generate the default ARM 64 bit configuration, and then open the menuconfig UI. 
To navigate the menuconfig utility, use your keyboard:
* To navigate directionally, use the arrow keys
* To enter a submenu (indicated by --->), press the Enter key
* To go up a level or exit, press Escape twice
* To toggle the state of a binary option, press the space bar
* To select the state of a multiple choice option, press Enter to open a submenu, the arrow keys to navigate the submenu, and press Enter again to select a state
* To get help with an option or menu, press the H key

After a brief compilation, menuconfig presents a list of submenus containing all the options you can configure.
Once you’re done making changes, press Escape until you’re prompted to save your new configuration. By default, this saves to the .config file. You can save and load configurations by copying this file.

### Step 5: Build
For our 64-bit system run this command

```bash
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j$(nproc)
```

This may take extra time depending on your system.


### Step 7: Mount Boot Media
Insert your boot media and run 

```bash
lsblk
```

Find directories such as sdb->sdb1, sdb2. Then mount them with these commands

```bash
mkdir mnt
mkdir mnt/boot
mkdir mnt/root
sudo mount /dev/sdb1 mnt/boot
sudo mount /dev/sdb2 mnt/root
```


### Step 8: Install 
```bash
sudo env PATH=$PATH make -j$(nproc) ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- INSTALL_MOD_PATH=mnt/root modules_install
```


### Step 9: Backup And Umnount
Run the following commands to create a backup image of the current kernel, install the fresh kernel image, overlays, README, and unmount the partitions

```bash
sudo cp mnt/boot/$KERNEL.img mnt/boot/$KERNEL-backup.img
sudo cp arch/arm64/boot/Image mnt/boot/$KERNEL.img
sudo cp arch/arm64/boot/dts/broadcom/*.dtb mnt/boot/
sudo cp arch/arm64/boot/dts/overlays/*.dtb* mnt/boot/overlays/
sudo cp arch/arm64/boot/dts/overlays/README mnt/boot/overlays/
sudo umount mnt/boot
sudo umount mnt/root
```


#### You Have Now Successfully Created Your Custom Kernel!


## Concepts and Background

__Cross Compilation__-
- Compilers are the tool used to translate high-level programming language to low-level programming language.
- A cross compiler is a compiler capable of creating executable code for a platform other than the one on which the compiler is running.

__menuconfig__-
- It is a user-friendly interface that allows customization of the kconfig.
- __WARNING__: It needs to be handles very carefully. Small changes in size parameters and other key options may cause compatibility issues and break the config.


