#!/bin/bash
CACHE="/dev/shm/conky_gpu_cache"
CACHE_TMP="/dev/shm/conky_gpu_cache.$$.tmp"
NOW=$(date +%s)
FILE_TIME=$(stat -c %Y "$CACHE" 2>/dev/null || echo 0)

if [ ! -f "$CACHE" ] || [ $((NOW - FILE_TIME)) -ge 2 ]; then
    if nvidia-smi --query-gpu=name,utilization.gpu,clocks.gr,utilization.memory,temperature.gpu,fan.speed,power.draw,memory.used,memory.total --format=csv,noheader,nounits > "$CACHE_TMP" 2>/dev/null; then
        mv -f "$CACHE_TMP" "$CACHE" 2>/dev/null
    else
        rm -f "$CACHE_TMP" 2>/dev/null
    fi
fi

IFS=',' read -r NAME UTIL FREQ MEM TEMP FAN POWER MEM_USED MEM_TOTAL < "$CACHE"

case "$1" in
    name)      echo "${NAME# }" ;;
    util)      echo "${UTIL# }" ;;
    freq)      echo "${FREQ# }" ;;
    mem)       echo "${MEM# }" ;;
    temp)      echo "${TEMP# }" ;;
    fan)       echo "${FAN# }" ;;
    power)     printf "%.0f\n" "${POWER# }" 2>/dev/null || echo "${POWER# }" ;;
    mem_used)  echo "${MEM_USED# }" ;;
    mem_total) echo "${MEM_TOTAL# }" ;;
    *)         echo "${UTIL# }" ;;
esac
