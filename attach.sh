#!/usr/bin/env bash
PID=""
for p in $(pgrep -f 'Minecraft.Windows.exe'); do
    cand=$(readlink -f "/proc/$p/exe" 2>/dev/null)
    case "$cand" in *wine*) PID=$p; break ;; esac
done
[ -n "$PID" ] || { echo "game process not found"; exit 1; }
echo "attaching to $PID"

if [ ! -f "$HOME/.gdbinit" ]; then
    cat > "$HOME/.gdbinit" <<'EOF'
set exec-file-mismatch off
set print object off
set mi-async on
handle SIGUSR1 SIGUSR2 SIGPIPE nostop noprint pass
handle SIGSEGV nostop noprint pass
EOF
fi

echo "if you have issues with breakpoints on the client, try injecting first"

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GDBSERVER="$ROOT/.gdbserver/bin/gdbserver"
if [ ! -x "$GDBSERVER" ]; then
    echo "patched gdbserver not found, please run build_gdbserver.sh first, goober!!"
    GDBSERVER="$(command -v gdbserver)"
fi

setsid nohup "$GDBSERVER" --attach :12346 "$PID" > /tmp/gdbserver.log 2>&1 &
sleep 1