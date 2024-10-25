#!/bin/sh

# This file should be called 'settings.sh' and should be copied to the 
# 'rootfs/app/bin' folder under you project folder.
# VERY IMPORTANT: *** THIS FILE MUST HAVE LINUX LINE ENDINGS *** 

# The default application launched (also if key 1 is held) (MANDATORY).
APP1="BebProject"

# The application launched when key 2 is held (OPTIONAL).
# For example:- 
#   APP2="AnotherApp"

# The application launched when key 3 is held (OPTIONAL).
# For example:- 
#   APP3="YetAnotherApp"

# The application launched when key 4 is held (OPTIONAL).
# For example:- 
#   APP4=""

# The application launched when key 5 is held (OPTIONAL).
# For example:- 
#   APP5=""

# Add any extra files to the upgrade package (OPTIONAL).
# Seperated by a space, these filenames should include the path relative to the 
# 'rootfs/app' folder under your project folder 
# For example:- 
#   EXTRAFILES="data/config/extrafile.txt bin/extrafile.txt" 

# Add any extra folders (and all the files they contain) to the upgrade package (OPTIONAL).
# Seperated by a space, these folders should include the path relative to the 
# 'rootfs/app' folder under your project folder 
# For example:- 
#   EXTRAFOLDERS="data/extrafolder anotherextrafolder"

# Specify the size of the 'app' partition on the target hardware (OPTIONAL).
# This value is specified in megabytes and must be in the range 16 to 75.
# The default is 32MB for the 'app' partition (which is mounted READ-ONLY as '/app')
# and 58MB for the 'data' partition (which is mounted READ-WRITE under '/app/data')
# Please note that the total size of the available NAND is approx 90MB and this is shared 
# between the 'app' and 'data' partitions.  
# For example, to set the 'app' partition to 64MB (which leaves 26MB for the 'data' partition):- 
#   APPPARTSIZEMB=64

# Keep any settings data stored in the 'data' partition during the upgrade process (OPTIONAL).
# The default (if this value is not Y or ommitted) is to delete this data.
# For example:-
#   KEEPOLDDATA="Y"

# Should the ethernet networking be enabled on the target? Ethernet will be ONLY be enabled if the 
# variable is set to "Y", it won't be enabled if it is not set or set to any other value.
# If this is enabled, the network settings file netsettings.sh will be automatically included
# in the update package and the ethernet port will be monitored to detect cable plug/unplug events.
#   ETHERNET_ENABLED="Y" 

## WATCHDOG CONTROL ##
# If this variable is set to any value then the watchdog will be disabled otherwise it will be 
# enabled (we highly recommended it is left enabled!)
#   WATCHDOG_OFF="Y"

## DRIVERS ##
# Select which drivers are loaded on the target hardware. Drivers will be ONLY loaded if the 
# variable is set to "Y", it won't be loaded if it is not set or set to any other value.
# USB Ethernet Drivers # 
#   DRIVER_USB_NET_ASIX="Y"
#   DRIVER_USB_NET_DM9601="Y"
#   DRIVER_USB_NET_SMSC75XX="Y"
#   DRIVER_USB_NET_SMSC95XX="Y"
#   DRIVER_USB_NET_MCS7830="Y"
#   DRIVER_USB_NET_AX88179_178A="Y"

# USB Serial Drivers #
#   DRIVER_USB_SERIAL_FTDI_SIO="Y"
#   DRIVER_USB_SERIAL_PL2303="Y"
