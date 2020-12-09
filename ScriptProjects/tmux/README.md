## Tmux

Copy themes:
```
cp -r themes/ ~/.config/tmux/
```

Move config-file to home:
```
cp tmux.conf ~/.tmux.conf
```

tmux key: Backtick ( ` ) | Ctrl-B 
tmux command: key + : 

### Session > Window > Panel

Setup 256Color for konsole-tmux-vim:
- .bashrc: alias tmux='TERM=xterm-256color tmux -2'
- .tmux.conf: set -g default-terminal "screen-256color" 
- .vimrc: set t_Co=256 

#### Session
- tmux new -s name => Create 
- tmux switch -t name => Switch 
- tmux attach -t name => Attach 
- tmux kill-session -t name => Kill 
- key + $ => Rename 
- key + d => Dettach 
- tmux ls => List 

#### Command
- key + $ rename current session 
- key + , rename current window 
- key + . moving current window 
- key + & kill current window 
- key + x kill current pane 

#### Window
- key + c => add 
- :new-window -n winName => add with name 
- :kill-window -t winNumber => close 
- key + n/p => next/previous 
- key + & => kill 
- swap-w(indow) -t idx => swap 
- move-window -t idx => move 
- key + '/w idx (or :select-window -t) => jump 

#### Pane
- key + % => slit vertical 
- key + - => slit horizontal 
- key + Alt-1/2 => rearrange horizontal/vertical 
- key -r + H/J/K/L/Arrows (-r repeat pressed: hold Ctrl) => resize 
- key + q => show index: display-p(anes) 
- key + ' => Enter index for going to pane greater than  9 
- swap 
- swap-p(anes) -t idx 
- key + { or } : swap-pane -U/D 
