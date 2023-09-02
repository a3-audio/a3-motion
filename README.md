<!--
SPDX-FileCopyrightText: 2023 A3 Audio UG (haftungsbeschränkt) <contact@a3-audio.com>

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Setting up a new installation
- flash latest Raspian OS image light
- resize sd-card space with gparted to maximum size
- boot raspberry pi
- define keyboard layout
- add user aaa and set password
- perform sudo apt update && sudo apt upgrade
- perform sudo apt install lightdm vim i3-wm dmenu x11-xserver-utils git build-essentials cmake libserial-dev gpiod libgpiod-dev
- Sudo raspi-config > Interface Options
  - (+) SSH
  - (+) I2C
- Sudo raspi-config > System-Settings > Hostname
  - a3-motion-v02
- sudo vim /etc/hosts
  - make sure hostname is correct
- sudo groupadd autologin
- sudo usermod -aG autologin aaa
- sudo vim /etc/lightdm/lightdm.conf
```
[Seat:*]
xserver-command=X -nocursor
autologin-user=aaa
autologin-session=i3
```
- sudo systemctl set-default graphical.target
- sudo systemctl enable lightdm
- configure autostart at the end of ~/.config/i3/config
  - remove bar {} block
  - rotate display for portrait orientation
  - rotate mouse input coordinates
  - disable screen saver and screen power management
  - start motion controller ui
```
exec --no-startup-id xrandr -o right
exec --no-startup-id xinput set-prop "wch.cn USB2IIC_CTP_CONTROL" --type=float "Coordinate Transformation Matrix" 0 1 0 -1 0 1 0 0 1

exec --no-startup-id xset s off
exec --no-startup-id xset -dpms

exec sleep 1

exec /usr/bin/bash /home/aaa/a3-system/a3motion/a3MotioncontrollerUI/moc.sh --serial_device /dev/ttyACM0 --server_ip "192.168.43.50" --server_port 9000 --encoder_base_port 1337
```
- configure network: vim /etc/dhcpcd.conf
```
# Example static IP configuration:
interface eth0
static ip_address=192.168.43.52/24
static routers=192.168.43.1
static domain_name_servers=192.168.43.1 8.8.8.8
```
- git clone --recursive https://github.com/a3-audio/a3-motion.git
- build a3-motion-ui
  - see a3motion/ui/readme.md

----------------------------------
- set up kernel config (TODO: what was changed compared to stock installation?)
```
#gpu_mem=64
initramfs initramfs-linux.img followkernel
kernel=kernel8.img
arm_64bit=1
disable_overscan=1
dtparam=krnbt=on

#enable sound
dtparam=audio=on
#hdmi_drive=2

#enable vc4
dtoverlay=vc4-kms-v3d
max_framebuffers=2
disable_splash=1

display_rotate=2
```

# Branching in Git

Development of the *upcoming* hardware version and the corresponding software happens on the `main` branch. We keep "release branches" for each successive hardware build to allow for hotfixes. When features are developed on a release branch, e.g. because the upcoming hardware is not ready to be used yet, they have to be ported forward into the main branch.
