#!/bin/bash
#Compiler optimization flags used
#-no-interleaving=default -fp-relaxed -fpc 
aoc -march=emulator -legacy-emulator device/autorun_kernel.cl  -o bin/autorun_kernel.aocx   -board=de5a_net_ddr4

