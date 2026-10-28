#!/bin/bash
# Evitar ejecuciones duplicadas
if pgrep -x "conky" > /dev/null; then
    exit 0
fi

# Esperar a que GNOME y los monitores completen su inicializacion inicial
sleep 10

# Esperar a que el servidor grafico (Xwayland) este completamente listo y respondiendo
for i in {1..20}; do
    if xset q >/dev/null 2>&1; then
        break
    fi
    sleep 1
done

# Verificar nuevamente si ya existe una instancia
if pgrep -x "conky" > /dev/null; then
    exit 0
fi

# Lanzar Conky completamente desacoplado
setsid /usr/bin/conky --config="$HOME/.conky/conkyrc/conkyrc_default" >/dev/null 2>&1 &
