# pebble-celeste

Celeste watchface for Pebble. Displays the time over a mountain bitmap using a custom font.

## Building

Requires the [Pebble SDK](https://developer.repebble.com/sdk/) to be installed and available in `PATH`.

```sh
# Compile for all platforms
pebble build

# Run in emulator (replace 'gabbro' with target platform)
pebble install --emulator gabbro

# Stream logs from device/emulator
pebble logs
```

## Architecture

All watchface logic is in `src/c/main.c`. The app follows the standard Pebble lifecycle:

```
main() → init() → app_event_loop() → deinit()
```

A `tick_handler` subscribed to `MINUTE_UNIT` updates the time display every minute.

## Resources

Declared in `package.json` under `pebble.resources.media`, referenced in C as `RESOURCE_ID_*`:

Source artwork (GIMP `.xcf` projects) lives in `assets/` and is not compiled into the watchface.

## Target Platforms

- `aplite`
- `basalt`
- `chalk`
- `diorite`
- `emery`
- `flint`
- `gabbro`
