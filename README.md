Kiosk
--------------------

## Goal

Create the simplest possible low-footprint kiosk app for displaying web pages.

 - Low memory and performance footprint
 - Support for custom stylesheet per domain
 - Simple, readable code


## Dependencies

 - Qt 5.12.1 (haven't checked which version exactly)
 - CMake

## Building

```bash
mkdir build; cd build;
cmake .. -G Ninja
```

## Running

```bash
./kiosk
```

Arguments:

 - `-s --start` Start cycling immediatly after launching.
 - `-f --fullscreen` Launch in fullscreen mode
 - `-u --urls` Path to a file with URLs (one per line) to be loaded and displayed
