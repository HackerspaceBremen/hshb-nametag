# HSHB Nametag

This repositoy contains the source code for the HSHB Nametag, a PCB designed in the [Hackerspace Bremen e.V.](https://www.hackerspace-bremen.de).

## Building

Building and flashing is done via a Makefile. Install avr-gcc and avr-binutils for your operating system or use Microchip Studio.

```shell
$ make -j all BOARD=1
```

Replace `BOARD` with your board no (sticker on the back side of the board, required for calibration data).

## Flashing

Flashing is also done via the Makefile. Install [avrdude](https://github.com/avrdudes/avrdude/) for your operating system.

```shell
$ make program_flash
```

You can provide `SERIAL_PORT` if you require a port different from `/dev/ttyUSB0` like this:

```shell
$ make program_flash SERIAL_PORT=COM1
```

## Serial console

The Makefile also provides a convenience target for opening a serial console:

```shell
$ make console
```
