#!/bin/bash
# Lanseaza cinematograful intr-un terminal nou separat.
# Detecteaza automat terminalul disponibil pe sistem.

DIR="$(cd "$(dirname "$0")" && pwd)"

if command -v gnome-terminal &>/dev/null; then
    gnome-terminal -- bash -c "cd '$DIR' && ./cinematograf; exec bash"
elif command -v xterm &>/dev/null; then
    xterm -fa 'Monospace' -fs 12 -bg black -fg white \
          -title "CineMax" -e bash -c "cd '$DIR' && ./cinematograf; exec bash" &
elif command -v konsole &>/dev/null; then
    konsole --workdir "$DIR" -e bash -c "./cinematograf; exec bash" &
elif command -v xfce4-terminal &>/dev/null; then
    xfce4-terminal --working-directory="$DIR" -e "bash -c './cinematograf; exec bash'" &
else
    echo "Nu am gasit un terminal grafic. Rulare in terminalul curent:"
    cd "$DIR" && ./cinematograf
fi
