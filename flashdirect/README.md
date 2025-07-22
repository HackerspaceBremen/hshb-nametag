# README

To create a **backup** or install an **update** of the current firmware of your tag you can use the shellscript in this folder.

## INSTALLATION

You need to install the software `avrdude` which is necessary to communicate with your badge.

### macOS install

use `brew install avrdude` to install it

### Linux install

use `apt-get install avrdude` to install it

## USAGE

Execute the script by changing directory `cd flashdirect` then typing

```bash
./hshb_tag_update.sh
```

The script is designed to run under bash-shells. It will ask questions before it is doing anything and denies to be run under root access level.

## USB SERIAL CONNECTION

You need to know where your USB serial device is located.

### macOS serial device

To find out which usb serial device your tag is connected to you can either call `ls -lFt /dev/tty*` to get a list of active devices. This looks like this:

```bash
$ ls -lFt /dev/tty*
crw--w----  1 helge  tty    0x10000001 Jul 22 22:59 /dev/ttys001
crw--w----  1 root   tty    0x10000157 Jul 22 22:55 /dev/ttys343
crw-rw-rw-  1 root   wheel   0x9000006 Jul 22 22:46 /dev/tty.usbserial-314410
crw--w----  1 helge  tty    0x10000156 Jul 22 21:41 /dev/ttys342
crw--w----  1 helge  tty    0x1000000f Jul 22 21:41 /dev/ttys015
crw--w----  1 root   tty    0x10000155 Jul 22 21:31 /dev/ttys341
crw--w----  1 root   tty    0x10000154 Jul 22 21:26 /dev/ttys340
crw--w----  1 root   tty    0x10000153 Jul 22 21:26 /dev/ttys339
```

Or you can use `ls /dev/tty* | grep usb` to get exactly those devices connected via USB. This will return e.g.:

```bash
$ ls /dev/tty* | grep usb
/dev/tty.usbserial-314410
```

### Linux serial device

Enter the following to find out possible paths to your serial device.

```bash
$ ls -lFt /sys/class/tty/ttyUSB*
lrwxrwxrwx 1 root root 0 2012-03-28 20:43 /sys/class/tty/ttyUSB0 -> ../../devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1.4/2-1.4:1.0/ttyUSB0/tty/ttyUSB0/
lrwxrwxrwx 1 root root 0 2012-03-28 20:44 /sys/class/tty/ttyUSB1 -> ../../devices/pci0000:00/0000:00:1d.0/usb2/2-1/2-1.3/2-1.3:1.0/ttyUSB1/tty/ttyUSB1/
```
