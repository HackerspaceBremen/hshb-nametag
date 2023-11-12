# HSHB Nametag

This repositoy contains the source code for the HSHB Nametag, a PCB designed in the [Hackerspace Bremen e.V.](https://www.hackerspace-bremen.de).

## Building

Building and flashing is done via a Makefile. Install avr-gcc and avr-binutils for your operating system or use Microchip Studio.

```shell
$ make -j all
```

## Flashing

Flashing is also done via the Makefile. Install [avrdude](https://github.com/avrdudes/avrdude/) for your operating system.

```shell
$ make program_flash
```
