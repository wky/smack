#!/bin/bash

#Set permissions on local ephemeral storage,
#so sudo is not needed
sudo chgrp SMACK /mnt/local
sudo chmod g+w /mnt/local

#Change apt-get mirror to local cs.utah.edu mirror (makes it much faster)
sudo sed -i  "s|us.archive.ubuntu.com/ubuntu/|ubuntu.cs.utah.edu/ubuntu/|g" /etc/apt/sources.list


#Install packages
sudo apt-get update
# Packages:
#   htop                       - interactive convenience
#   vim                        - interactive convenience
#   cgroup-bin                 - benchexec (cgroups)
#   cgroup-lite                - benchexec (cgroups)
#   cgmanager                  - benchexec (cgroups)
#   software-properties-common - SMACK buildscript
#   python-daemon              - SMACKBench Server
sudo apt-get install htop vim cgroup-bin cgroup-lite cgmanager software-properties-common python-daemon -y

#Upgrade kernel
sudo apt-get install --install-recommends linux-generic-lts-vivid -y

#And all packages
sudo apt-get upgrade -y
sudo apt-get upgrade -y

#Enable tracking of memory swapping for processes (requires reboot)
sudo sed -i '/GRUB_CMDLINE_LINUX=/ s/^\(.*\)\("\)/\1 swapaccount=1\2/' /etc/default/grub
sudo update-grub

#Calling script must reboot after it finishes its portion!
