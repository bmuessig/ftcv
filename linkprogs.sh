#!/bin/bash
rm -f /tmp/ftcvf0 /tmp/ftcvf1
mkfifo /tmp/ftcvf0 /tmp/ftcvf1
./ftcv.elf send ftcv.o > /tmp/ftcvf0 < /tmp/ftcvf1 &
./ftcv.elf receive test < /tmp/ftcvf0 > /tmp/ftcvf1

