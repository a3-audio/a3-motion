# A³ Motion

The 4-channel motion sampler of [A³ Audio](https://github.com/a3-audio/a3-system).
It records movements on a touchscreen sphere and plays them back in time with
the beat, steering each channel's position on A³ Core over OSC.

This repository holds the panel firmware (`firmware/`, ESP32-S3) and the PCB and
housing designs (`hardware/`). The touchscreen app is
[a3-motion-ui](https://github.com/a3-audio/a3-motion-ui), wired in here as the
`ui` submodule.

**Documentation: https://a3-audio.github.io/a3-doc/**

- [A³ Motion user guide](https://a3-audio.github.io/a3-doc/user/a3motion.html)
- [Configuration](https://a3-audio.github.io/a3-doc/configuration/moc.html),
  [Assembly](https://a3-audio.github.io/a3-doc/assembly/moc.html) and
  [Development](https://a3-audio.github.io/a3-doc/development/moc.html)

## Firmware

A PlatformIO project for the `esp32-s3-devkitc-1-n16r8`. Run from `firmware/`:

```bash
pio run                    # build
pio run -t upload          # flash
pio device monitor         # serial monitor, 115200 baud
pio test -e native         # host-side unit tests
python3 -m unittest test_host
```

Upload and monitor need no port. They find the panel by its USB ID, the CH343
bridge `1A86:55D3` (`build.hwids` in `boards/esp32-s3-devkitc-1-n16r8.json`).

`host.py` polls the firmware over serial and prints the decoded buttons,
encoders and pots (`python3 host.py`; needs `pyserial`). Its docstring lists
the options and describes the binary poll-frame protocol. Use it to tell a
firmware fault from a fault in the UI.

## The UI computer

A Raspberry Pi runs the UI. It logs the user `aaa` into an i3 session through
LightDM autologin. The i3 config, the X rules for the touchscreen and the user
service are in the UI repository, under
[`platform_config/`](https://github.com/a3-audio/a3-motion-ui/tree/main/platform_config).

## License

REUSE-compliant: the licenses are in `LICENSES/`, which file has which is in
`.reuse/dep5`.
