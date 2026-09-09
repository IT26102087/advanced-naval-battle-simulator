# Advanced Naval Battle Simulator

SE1012 Programming Methodology - Take Home Assignment

## What it does
Simulates a stationary Battleship (B) engaging multiple stationary Escort ships (E)
using projectile motion physics. Battleship's attack range is a full circle
(any angle 0-90 degrees); each escort ship's range is a ring, since its
vertical firing angle is restricted per its type (Table 1).

## Features implemented
- Part 1-A: single-hit-kill battle simulation
- Part 1-B: battleship moves to k random points; gun jam variant included
- Part 1-C: cumulative partial damage based on each escort type's impact power
- Part 2-A: battleship reload time + closest-first targeting strategy
- Full menu system (Setup, run each Part, Instructions, Statistics)
- All results saved to text files, viewable via the Statistics menu

## How to build and run
gcc -Wall -Iinclude -o naval-sim src/main.c src/physics.c src/battlefield.c src/attack.c src/simulation.c src/simulation1b.c src/simulation2a.c src/fileio.c src/menu.c -lm
./naval-sim

## File structure
- `include/` - header files (struct definitions, function declarations)
- `src/` - implementation files
- `common.h` - Battleship/Escortship structs, EscortType, EscortSpec
- `physics.c` - projectile motion equations (range, time of flight, distance)
- `battlefield.c` - Table 1 data, random battlefield generation
- `attack.c` - attack range logic (circle for B, ring for E)
- `simulation.c` - unified Part 1-A/1-C battle resolution
- `simulation1b.c` - moving battleship simulations
- `simulation2a.c` - reload time + targeting strategy
- `fileio.c` - saving initial/final conditions and battle results
- `menu.c` - interactive menu system

## Design notes
- Battleship always fires at 45 degrees, since that requires the minimum
  shell speed to reach any given range - a simple, explainable targeting choice.
- Part 1-A and Part 1-C use the SAME battle engine (`runBattle`); the only
  difference is whether escort impact power is forced to 1.0 (one-hit-kill)
  or left at Table 1 values (cumulative damage).
