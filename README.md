# Setting up a new installation

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

- configure network
- set hostname
- create user aaa
- add user aaa to autologin group
- .......
- /etc/lightdm/lightdm.conf 
  - disable graphical cursor for X server
  - autologin with i3 session
```
[Seat:*]
xserver-command=X -nocursor
autologin-user=aaa
autologin-session=i3
```
- configure autostart at the end of ~/.config/i3/config
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

# Branching in Git

Development of the *upcoming* hardware version and the corresponding software happens on the `main` branch. We keep "release branches" for each successive hardware build to allow for hotfixes. When features are developed on a release branch, e.g. because the upcoming hardware is not ready to be used yet, they have to be ported forward into the main branch.
