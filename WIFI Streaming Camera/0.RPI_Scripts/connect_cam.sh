#!/bin/bash

# Set the WiFi SSID and password
SSID="AMB82-Mini"
PASSWORD="Password"

# Path to the wpa_supplicant configuration file
WPA_CONF_FILE="/etc/wpa_supplicant/wpa_supplicant.conf"

# Add network configuration to wpa_supplicant configuration file
# This part needs to run with root privileges
echo "network={" >> $WPA_CONF_FILE
echo "    ssid=\"$SSID\"" >> $WPA_CONF_FILE
echo "    psk=\"$PASSWORD\"" >> $WPA_CONF_FILE
echo "}" >> $WPA_CONF_FILE

# Reconfigure wpa_supplicant to apply the changes
wpa_cli -i wlan0 reconfigure

# Function to check WiFi connection status and IP address
check_wifi_connection() {
    # Check if the WiFi interface is associated with the specified SSID
    if iwconfig wlan0 | grep -q "ESSID:\"$SSID\""; then
        # Check if an IP address is assigned to wlan0
        if ip addr show wlan0 | grep -q "inet "; then
            return 0  # IP address assigned, connection successful
        fi
    fi
    return 1  # Connection not successful yet
}

# Wait until WiFi connection is established and IP address is assigned
echo "Connecting to WiFi network '$SSID'..."
while ! check_wifi_connection; do
    echo "WiFi connection in progress. Waiting..."
    sleep 5
done

echo "WiFi connected successfully to '$SSID'."
echo "IP address assigned to wlan0: $(ip addr show wlan0 | grep -oP '(?<=inet\s)\d+(\.\d+){3}')"

# Continue with the python script commands
python3 ./vlc_rtsp.py