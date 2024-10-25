#!/bin/sh

# This file should be called 'netsettings.sh' and should be copied to the 
# 'rootfs/app/bin' folder under you project folder.
# VERY IMPORTANT: *** THIS FILE MUST HAVE LINUX LINE ENDINGS *** 

## NETWORK SETTINGS ##
# These will only have any effect if you have ETHERNET_ENABLED="Y" in your settings.sh!

# The hostname to use
NET_HOSTNAME="canvu"

# The mode of the network interfaces. Can be either "dhcp" or "static"
NET_MODE="dhcp"
#NET_MODE="static"

# If NET_MODE is "static", you need to set the following NET_STATIC_xxx variables
# They are not required for "dhcp" mode
if [ "${NET_MODE}" == "static" ]; then
    # Static IP Address
    NET_STATIC_IP="192.168.1.1"
    # Static Subnet Mask
    NET_STATIC_SUBNET="255.255.255.0"
    # Static Default Gateway
    NET_STATIC_GATEWAY="192.168.1.254"
    # Static DNS Servers - Use Googles public ones
    NET_STATIC_DNS1="8.8.8.8"
    # NET_STATIC_DNS2 is optional
    NET_STATIC_DNS2="8.8.4.4"
fi

## NTP SETTINGS ##
# Enable NTP Time Setting? If this value is "Y" then NTP will be enabled
#NTP_ENABLED="Y"
# NTP Server Address (domain or IP)
NTP_SERVER="uk.pool.ntp.org"

## SSH SETTINGS ##
# Enable SSH on port 22? If this value is "Y" then NTP will be enabled
SSH_ENABLED="Y"
