#!/bin/bash
case "$1" in
    cpu_temp)
        for n in /sys/class/hwmon/hwmon*/name; do
            if [ -f "$n" ] && [ "$(< "$n")" = "coretemp" ]; then
                d="${n%/name}"
                if [ -f "$d/temp1_input" ]; then
                    echo $(( $(< "$d/temp1_input") / 1000 ))
                    exit 0
                fi
            fi
        done
        echo "N/A"
        ;;
    nvme_temp)
        for n in /sys/class/hwmon/hwmon*/name; do
            if [ -f "$n" ] && [ "$(< "$n")" = "nvme" ]; then
                d="${n%/name}"
                if [ -f "$d/temp1_input" ]; then
                    echo $(( $(< "$d/temp1_input") / 1000 ))
                    exit 0
                fi
            fi
        done
        echo "N/A"
        ;;
    *)
        echo "N/A"
        ;;
esac
