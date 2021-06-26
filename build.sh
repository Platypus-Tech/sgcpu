#!/usr/bin/sh

gcc -Llib -g src/sgtk.c src/sgcpu.c src/sgcpu_data.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o sgtk