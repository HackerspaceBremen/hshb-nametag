# HSHB Nametag

This repositoy contains the source code for the HSHB Nametag, a PCB designed in the [Hackerspace Bremen e.V.](https://www.hackerspace-bremen.de).

## Prerequisites

Building and flashing is done with [PlatformIO](https://platformio.org/).

## Building

Open the project folder in a PlatformIO IDE (VS Codium, etc.) and build it. If you're using the `pio` binary simply run

```shell
$ pio run
```

## Flashing

Flash the project by using the appropriate button in your PlatformIO IDE. If you're using the `pio` binary run

```shell
$ pio run -t upload
```

## Serial console

A serial console can be opened by running

```shell
$ pio device monitor
```
