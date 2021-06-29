#!/usr/bin/sh

# Build toolkit
gcc -Llib -g src/sgtk.c src/sgcpu.c src/sgcpu_data.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o sgtk

# Build tests
cd tests
  # Build bank test
  cd bank
  ../../sgtk asm test.asm -o ../bin/bank.bin
  cd ..

  # Build fast test
  cd fast
  ../../sgtk asm test.asm -o ../bin/fast.bin
  cd ..

  # Build font test
  cd font
  ../../sgtk asm test.asm -o ../bin/font.bin
  cd ..

  # Build serial test
  cd serial
  ../../sgtk asm test.asm -o ../bin/serial.bin
  cd ..

  # Build video test
  cd video
  ../../sgtk asm test.asm -o ../bin/video.bin
  cd ..
cd ..