#!/usr/bin/env bash

PID=""
exe=""
for p in $(pgrep -f 'Minecraft.Windows.exe'); do
    cand=$(readlink -f "/proc/$p/exe" 2>/dev/null)
    case "$cand" in
        *wine*) PID=$p; exe=$cand; break ;;
    esac
done
[ -n "$PID" ] || { echo "game process not found"; exit 1; }

while IFS= read -r -d '' kv; do
    if [[ "$kv" =~ ^[A-Za-z_][A-Za-z0-9_]*= ]]; then
        export "$kv"
    fi
done < "/proc/$PID/environ"

WINE_BIN="${exe%-preloader}"
ROOT=/home/emi/CLionProjects/CppSystemsTest
SHARE=/mnt/data/bedrock_on_linux/inject

mkdir -p "$SHARE"
cp -f "$ROOT/cmake-build-debug/injector.exe" "$ROOT/cmake-build-debug/CppSystemsTest.dll" "$SHARE/"
DLL_WIN='Z:\mnt\data\bedrock_on_linux\inject\CppSystemsTest.dll'

"$WINE_BIN" "$SHARE/injector.exe" Minecraft.Windows "$DLL_WIN"
echo "injector exit code: $?"