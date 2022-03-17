

# Pre-install

0. DISABLE FAST STARTUP AND HIBERNATION

https://www.tenforums.com/tutorials/4189-turn-off-fast-startup-windows-10-a.html

https://docs.microsoft.com/en-us/troubleshoot/windows-client/deployment/disable-and-re-enable-hibernation


1. Disable secure boot

1. Partition drive

right click C: > Shrink volume > enter amount of space to shrink in md > shrink


2. Download Arch Linux x86_64 .iso


3. Etcher > select image > select usb > flash!


4. Boot from usb

Change advanced startup options > restart now > use a device > usb device name > Arch Linux archiso x86_64 UEFI CD


# Install

5. Verify the boot mode

$ ls /sys/firmware/efi/efivars/  (should get output)


7. Connect to Internet

$ ip link  (check for network interface)

$ rfkill list          (check if wifi blocked)
$ rfkill unblock wifi  (unblock soft-blocked)

$ iwctl
> device list
> static device scan
> static device get-networks
> static device connect \SSID\  (will prompt for password)

$ ping reissenzahn.com


8. Configure Date and Time

$ timedatectl set-ntp true
$ timedatectl status  (check date is correct)


9. Configure root and swap partitions

$ cfdisk

Move to free space > New > XXG > Move to free space > New > XXG > Type > Linux swap > Write > Quit

(take note of new partition paths and EFI System path)


10. Format partitions

$ mkfs.ext4 /dev/sda5   (root partition name)
$ mkswap /dev/sda6      (swap partition name)

$ mount /dev/sda5 /mnt  (mount root filesystem)
$ swapon /dev/sda6      (enable swap)

$ mkdir /mnt/efi            (mount EFI)
$ mount /dev/sda2 /mnt/efi  (EFI system partition name) 


11. Install essential packages

$ pacstrap /mnt base linux linux-firmware base-devel

??? base-devel


12. Generate fstab

$ genfstab -U /mnt >> /mnt/etc/fstab


13. Switch to root

$ arch-chroot /mnt   (change root into new system)
$ ls


14. Set timezone

$ ln -sf /usr/share/zoneinfo/(tab to look for zone) /etc/localtime
$ hwclock --systohc  (sync hardware clock)


15. Update package repo

$ pacman -Sy nano


16. Set locale

$ nano /etc/locale.gen

en_US.UTF-8 UTF-8  (uncomment + save changes)

$ locale-gen

$ nano /etc/locale.conf

LANG=en_US.UTF-8  (save changes)


17. Hostname file

$ nano /etc/hostname

magi   (save changes)


$ nano /etc/hosts

127.0.0.1  \TAB   localhost
::1        \TAB   localhost
127.0.1.1  \TAB   magi.localdomain  \TAB  magi


18. Setup WiFi packages

$ pacman -S networkmanager

OR?

$ pacman -Syyu wpa_supplicant wifi-menu dialog


19. Default user

$ useradd -G wheel -m rei

$ passwd rei
New password:
Retype password:

$ ls /home/rei


20. Install boot loader

$ pacman -S grub efibootmgr   (install grub)
$ pacman -S os-prober         (detect windows os)
$ grub-install --target=x86_64-efi --efi-directory=/efi/ --bootloader-id=GRUB
$ grub-mkconfig -o /boot/grub/grub.cfg

Found Windows Boot Manager  (yay)


21. Set root user

$ passwd
New password:
Retype password:


22. Reboot

$ exit
$ reboot

(should show GRUB boot loader; check both installs)


23. Configure network manager

$ cd /etc/netctl
$ cp examples/ethernet-dhcp ./custom-dhcp-profile
$ nano custom-dhcp-profile

SETUP

$ ip link

??? Confusing things


# Post Install

1. Setup Wifi with NetworkManager

$ systemctl start NetworkManager.service
$ systemctl status NetworkManager.service
$ nmtui


3. Setup sudo

$ su (switch to root)
$ nano /etc/sudoers

%wheel ALL=(ALL) ALL  (uncomment)

https://www.youtube.com/watch?v=nSHOb8YU9Gw


2. Nvidea PRIME

$ 

https://www.youtube.com/watch?v=jncc3QL8RWI




4. i3


5. bashrc



V bottom of .profile ???

if [[ "$(tty)" = "/dev/tty1" ]]; then
  pgrep i3 || startx
fi


6. Wallpaper

https://www.youtube.com/watch?v=lvLExb1SUzM



# Resources

https://www.youtube.com/watch?v=LGhifbn6088&feature=youtu.be

https://gist.github.com/ppartarr/175aa0c3416daf3baacde17f442f80e1

https://medium.com/@mudrii/arch-linux-installation-on-hw-with-i3-windows-manager-part-2-x-window-system-and-i3-installation-86735e55a0a0


https://www.youtube.com/watch?v=lvLExb1SUzM

