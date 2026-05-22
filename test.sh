#!/bin/bash
# test.sh - Compile et lance le programme.
# A lancer depuis le dossier bibliocy-main :
#     wsl bash test.sh     (depuis Windows)
#     ./test.sh            (depuis WSL ou Linux)

gcc -Wall -std=c99 -O2 main.c livre.c utilisateur.c emprunt.c -o bibliotech
./bibliotech
