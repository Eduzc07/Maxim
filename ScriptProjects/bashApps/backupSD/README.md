# Create an Image from SD Card

Run ``backup.sh`` in order to make an *img* from tge SD card.

As first step we need to know where is mounted our sd card
``lsblk``, in our case **sdb** is the SD card.

```
sda      8:0    0 931.5G  0 disk
├─sda1   8:1    0   512M  0 part /boot/efi
├─sda2   8:2    0 925.2G  0 part /
└─sda3   8:3    0   5.9G  0 part [SWAP]
sdb      8:16   1  59.5G  0 disk
├─sdb1   8:17   1   128M  0 part /media/edu/boot
└─sdb2   8:18   1  59.4G  0 part /media/edu/rootfs
sr0     11:0    1  1024M  0 rom
```

### 1. Check number of cylinders
Once we know which one is the SD card, we can check more in detail, run:
``sudo fdisk -l -u=cylinders /dev/sdb``

```
Disk /dev/sdb: 59.5 GiB, 63864569856 bytes, 124735488 sectors
Geometry: 64 heads, 32 sectors/track, 60906 cylinders
Units: cylinders of 2048 * 512 = 1048576 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x3cedfd53

Device     Boot Start   End Cylinders  Size Id Type
/dev/sdb1           2   129       129  128M  c W95 FAT32 (LBA)
/dev/sdb2         130 60906     60777 59.4G 83 Linux
```
from here can we know that units are *1048576 bytes (~1Mb)*

### 2.  Check how much of space is used
We do not require to make an image of empty space, hence we check how much memory is in used. We can get it running ``df /dev/sdb*``.

```
Filesystem     1K-blocks    Used Available Use% Mounted on
udev             2954600       0   2954600   0% /dev
/dev/sdb1         130798   17984    112814  14% /media/edu/boot
/dev/sdb2       61257764 6234104  55005108  11% /media/edu/rootfs
```

or ``df -h /dev/sdb*`` to understand better:
```
Filesystem      Size  Used Avail Use% Mounted on
udev            2.9G     0  2.9G   0% /dev
/dev/sdb1       128M   18M  111M  14% /media/edu/boot
/dev/sdb2        59G  6.0G   53G  11% /media/edu/rootfs
```

from here can we know that only around **6Gb** is being used.

### 3.  Make an Image

We are going to used **dd** command in order to get a backup only for the first **7168 Mbytes (~7Gb)**, only the used space.

- conv = sync,noerror -> to save 0 if a error happens.
- bs = 1048576 (or 1M) -> cylinders
- count = 7168  -> quantity of Mb to write

#### Method 1 [&#x2713;]
This works, slower but the smallest.
- Slow to get the Image.
- Faster to flash on SD Card.
```
sudo dd status=progress conv=sync,noerror bs=1M count=7168 if=/dev/sdb | xz --best > 24052020_odroid_backup.img.xz
```

#### Method 2 [&#x2713;]
This work, faster and a little bit bigger.
- Fast to get the image.
- Slow to flash on SD Card

```
sudo dd status=progress conv=sync,noerror bs=1M count=7168 if=/dev/sdb | gzip -c --fast | dd of=24052020_odroid_backup.img.gz
```
#### Method 3 [&#x2713;]
This works, slower but smaller.

- Step1: First get an image from SD Card.
```
sudo dd status=progress conv=sync,noerror bs=1M count=7168 if=/dev/sdb | pv | dd of=/home/edu/24052020_odroid_backup.img
```
- Step 2: Once the image has been done, it can be compressed.
```
xz -zvk --best 24052020_odroid_backup.img
xz -zvk 24052020_odroid_backup.img
```

#### Method 4:
Image from all the SD card including empty space **count=60907**.
- Slow
```
sudo dd if=/dev/sdb conv=sync,noerror bs=1048576 count=60907 | pv | dd of=/home/edu/24052020_odroid_backup.img
xz -zvk --best 24052020_odroid_backup.img
```
