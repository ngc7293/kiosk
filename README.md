# Kiosk

## Goal

Create the simplest possible low-footprint kiosk app for displaying web pages.

- Low memory and performance footprint (runnable on a PI)
- Support for custom stylesheet per domain
- Clean, simple, readable code

## Dependencies

- Qt 5.14.1 (haven't checked which version exactly)
- [nlohmann::json](https://github.com/nlohmann/json) 3.7.0
- CMake

For the web remote:

- Python 3
- Flask

## Building

```bash
mkdir build; cd build;
cmake .. -G Ninja
ninja
```

## Running

```bash
# Main application
build/kiosk

# Flask remote
cd flask;
export FLASK_HOST="<your ip>"
./app.py
```

Arguments:

- `-s --start` Start cycling immediatly after launching.
- `-f --fullscreen` Launch in fullscreen mode
- `-u --urls` Path to a file with URLs (one per line) to be loaded and displayed
- `--ipc` Enable the IPC remote control

## IPC Commands

If the `--ipc` flag is passed, the application will listen on `/tmp/kiosk` for
JSON request in a RPC-like format:

```json
{
    "operation": "<operation name>",
    "paramters": []
}
```

Note that the `parameters` element is mandatory for all operations, even if it
is empty.

The following operations are currently supported:

- `get_state`: returns the current state of the kiosk (autoplay, current, playlist)
- `set_current`: change the view to the n-th item in the playlist. Takes one int parameter.
- `set_autoplay`: starts/stop automatic cycling. Takes one bool parameter.
