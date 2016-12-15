#!/bin/bash
rm *.hlt
gcc MyBot.c -o MyBot.o
gcc oldbot.c -o oldbot.o
./halite -d "45 45" "./MyBot.o" "./oldbot.o"
