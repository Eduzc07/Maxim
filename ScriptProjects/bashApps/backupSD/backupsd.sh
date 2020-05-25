#!/bin/bash
echo '##### Bash to make a Img from SD card.'
echo ' '
# Display all the devices connected
echo '---------------------------------------------------------------'
sudo fdisk -l | grep 'Disk /dev/sd*'
echo '---------------------------------------------------------------'
echo ' '

# Request which one is the SD card
read -p "Which one is the SD Card (ex. sdb): " device
device="/dev/$device"
echo ' '
echo "Device \"${device}\" will be cloned into an img."
echo -e "\n"

#Request device name
read -p "Which is the name of the SoC (ex. odroid): " name
# Get output file name
FILE="$HOME/$(date +%d%m%Y)_${name}_backup.img"
echo ' '
echo 'The name of the outputfile will be:'
echo "\"${FILE}\""
echo -e "\n"

# Get the number of blocks
echo '--------------------------------------------'
echo '-------- Get the number of Cylinders -------'
echo '--------------------------------------------'
sudo fdisk -l -u=cylinders ${device}
echo '--------------------------------------------'
cil=$(sudo fdisk -l -u=cylinders ${device} | grep Units | awk '{print $8}'i)
cil=$(expr $cil / 1024) #kbytes
cil=$(expr $cil / 1024) #Mbytes
echo ' '
echo "Number of Cylinders: [$cil Mb]"
echo -e "\n"

# Get the used memory
echo '--------------------------------------------'
echo '----------- Get the used Memory ------------'
echo '--------------------------------------------'
df -m --total ${device}*
echo '--------------------------------------------'
echo ' '
MEM=$(df -m --total ${device}* | grep total | awk '{print $2}')
echo "Memory: [$MEM Mbytes]"
used=$(df -m --total ${device}* | grep total | awk '{print $3}')
echo "Used Memory: [$used Mbytes]"
echo ''

# Add some empty space.
size=$(expr $used + 512) #Mbytes

# Run the complet line to get the image.
echo 'Running the command:'
echo "sudo dd status=progress conv=sync,noerror bs=${cil}M count=$size if=$device | xz --best > ${FILE}.xz"
# Main line
sudo dd status=progress conv=sync,noerror bs=${cil}M count=$size if=$device | xz --best > ${FILE}.xz

# Method with gzip
#echo "sudo dd status=progress conv=sync,noerror bs=${cil}M count=$size if=$device | gzip -c --fast | dd of=${FILE}.gz"
# Second option
#sudo dd status=progress conv=sync,noerror bs=${cil}M count=$size if=$device | gzip -c --fast | dd of=${FILE}.gz

# Close
echo ''
echo "Image has been made successfully!"
echo 'Finished!'
