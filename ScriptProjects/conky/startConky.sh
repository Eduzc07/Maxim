#!/bin/bash
process_num=$(ps -ef | grep conky | grep -v "grep" | wc -l)
if [ "$process_num" -gt 1 ]; then
    echo "Conky's already running"
else
    echo "Starting ..."
   {
    killall conky
    sleep 1
    conky --config=$HOME/.conky/conkyrc/conkyrc_default  &
    sleep 10
    cd $HOME/.conky/conky_ddweather
    conky -c $HOME/.conky/conky_ddweather/conky_ddweather &
   # sleep 1
   # conky --config=$HOME/.conky/conky_e2i/conky_e2i  &
   # sleep 1
   # conky -c $HOME/Local_G/2018/Pruebas_2018/revolutionary_clocks/all/conkyrc4 &
   # conky --config=$HOME/Local_G/2018/Pruebas_2018/conky/conkyrc_wordclock &
    } &>/dev/null
    echo "Conkys started"
fi
