#!/usr/bin/sh

gcc -Llib -O4 src/sgtk.c src/sgcpu.c src/sgcpu_data.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o sgtk