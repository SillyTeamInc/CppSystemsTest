#!/usr/bin/env bash
# this builds a patched gdbserver that fixes the issue i had with request_interrupt in gdb 17.2
# by removing a single fucking minus sign from the kill call in gdbserver/linux-low.cc
# i am not kidding. it caused all of my issues i spent like 4 straight hours on this single thing
# anyway i can debug on linux now :3
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/" && pwd)"
PREFIX="$ROOT/.gdbserver"
WORK="$PREFIX/work"
FORCE=0

for arg in "$@"; do
    case "$arg" in
        --force) FORCE=1 ;;
        *) echo "unknown argument: $arg"; exit 2 ;;
    esac
done

if [ -x "$PREFIX/bin/gdbserver" ] && [ "$FORCE" -eq 0 ]; then
    echo "this is the existing $PREFIX/bin/gdbserver (use --force to rebuild)"
    "$PREFIX/bin/gdbserver" --version | head -n1
    exit 0
fi

missing=()
for tool in gcc g++ make curl tar xz; do
    command -v "$tool" >/dev/null 2>&1 || missing+=("$tool")
done
if [ "${#missing[@]}" -gt 0 ]; then
    echo "missing tools: ${missing[*]}"
    exit 1
fi

if [ -z "${GDB_VERSION:-}" ] && command -v gdbserver >/dev/null 2>&1; then
    GDB_VERSION="$(gdbserver --version | head -n1 | grep -oE '[0-9]+\.[0-9]+(\.[0-9]+)?' | head -n1 || true)"
fi
if [ -z "${GDB_VERSION:-}" ]; then
    echo "could not detect the installed gdbserver version, set GDB_VERSION to the version to build"
    exit 1
fi

mkdir -p "$WORK"

TARBALL="gdb-$GDB_VERSION.tar.xz"
cd "$WORK"

if [ ! -f "$TARBALL" ]; then
    echo "downloading gdb $GDB_VERSION"
    if ! curl -fsSLO "https://ftpmirror.gnu.org/gnu/gdb/$TARBALL" &&
       ! curl -fsSLO "https://ftp.gnu.org/gnu/gdb/$TARBALL"; then
        echo "download failed for $TARBALL"
        exit 1
    fi
fi

rm -rf "gdb-$GDB_VERSION"
echo "extracting"
tar xf "$TARBALL"
cd "gdb-$GDB_VERSION"

SRC="gdbserver/linux-low.cc"
echo "patching $SRC"
sed -i -E 's/kill \(-signal_pid, SIGINT\)/kill (signal_pid, SIGINT)/' "$SRC"
if grep -q -E 'kill \(-signal_pid, SIGINT\)' "$SRC" ||
   ! grep -q -E 'kill \(signal_pid, SIGINT\)' "$SRC"; then
    echo "patch didnt, request_interrupt looks different in gdb $GDB_VERSION"
    exit 1
fi

mkdir build
cd build
echo "configuring"
../configure \
    --prefix="$PREFIX" \
    --disable-gdb --enable-gdbserver \
    --disable-binutils --disable-gas --disable-ld --disable-gold \
    --disable-gprof --disable-sim --disable-werror
echo "building"
make -j"$(nproc)" MAKEINFO=true all-gdbserver
echo "installing"
make MAKEINFO=true install-gdbserver

cd "$ROOT"
if [ "${KEEP_WORK:-0}" != "1" ]; then
    rm -rf "$WORK"
fi

echo "patched gdbserver $PREFIX/bin/gdbserver"
"$PREFIX/bin/gdbserver" --version | head -n1