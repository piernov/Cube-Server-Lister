#!/bin/sh

mkdir -p m4

echo "Running intltoolize..."
intltoolize --force --copy --automake || return 1

echo "Running aclocal..."
aclocal || return 1

echo "Running libtoolize..."
libtoolize || return 1

echo "Running autoheader..."
autoheader || return 1

echo "Running autoconf..."
autoconf || return 1

echo "Running automake..."
automake --add-missing || return 1

echo "Running configure..."
./configure "$@"
