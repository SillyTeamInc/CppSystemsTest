#!/usr/bin/env bash
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GAME_PROCESS="${GAME_PROCESS:-Minecraft.Windows}"

if [ -z "${BUILD_DIR:-}" ]; then
    newest="$(ls -t "$ROOT"/cmake-build-*/CppSystemsTest.dll "$ROOT"/build/CppSystemsTest.dll 2>/dev/null | head -n1)"
    [ -n "$newest" ] && BUILD_DIR="$(dirname "$newest")"
fi
if [ -z "${BUILD_DIR:-}" ] || [ ! -f "$BUILD_DIR/CppSystemsTest.dll" ]; then
    echo "CppSystemsTest.dll not found"
    exit 1
fi
if [ ! -f "$BUILD_DIR/injector.exe" ]; then
    echo "injector.exe was not found"
    exit 1
fi

PID=""
exe=""
for p in $(pgrep -f "$GAME_PROCESS.exe"); do
    cand="$(readlink -f "/proc/$p/exe" 2>/dev/null)"
    case "$cand" in
        *wine*) PID="$p"; exe="$cand"; break ;;
    esac
done
[ -n "$PID" ] || { echo "game process not found"; exit 1; }

while IFS= read -r -d '' kv; do
    if [[ "$kv" =~ ^[A-Za-z_][A-Za-z0-9_]*= ]]; then
        export "$kv"
    fi
done < "/proc/$PID/environ"

if [ -z "${WINEPREFIX:-}" ] || [ ! -d "$WINEPREFIX/drive_c" ]; then
    echo "could not find the game wineprefix (got: '${WINEPREFIX:-}')"
    exit 1
fi

WINE_BIN="${exe%-preloader}"
if [ ! -x "$WINE_BIN" ]; then
    echo "wine binary not runnable: $WINE_BIN"
    exit 1
fi

SHARE="$WINEPREFIX/drive_c/cppsystems"
DLL_WIN='C:\cppsystems\CppSystemsTest.dll'

mkdir -p "$SHARE"
cp -f "$BUILD_DIR/injector.exe" "$BUILD_DIR/CppSystemsTest.dll" "$SHARE/"

echo "build_dir=$BUILD_DIR, wine_bin=$WINE_BIN, share=$SHARE, dll_win=$DLL_WIN"

"$WINE_BIN" "$SHARE/injector.exe" "$GAME_PROCESS" "$DLL_WIN"
rc=$?
echo "injector exit code: $rc"
exit $rc

