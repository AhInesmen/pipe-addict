#!/bin/sh
# Compile le jeu, sous macOS comme sous Windows (terminal MSYS2 UCRT64).
# À lancer depuis le dossier pipe-addict :   sh compiler.sh
set -e

SOURCES="src/main.c src/jeu.c src/grille.c src/tuyau.c src/fluide.c src/affichage.c"
ALLEGRO="allegro-5 allegro_main-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5"

if [ -n "$MSYSTEM" ]; then # la variable MSYSTEM n'existe que dans les terminaux MSYS2 (Windows)
    COMPILATEUR=gcc
    PROGRAMME=pipe-addict.exe
else
    COMPILATEUR=clang
    PROGRAMME=pipe-addict
fi

$COMPILATEUR -Wall -Wextra $SOURCES -o $PROGRAMME $(pkg-config --cflags --libs $ALLEGRO)
echo "Compilation reussie. Pour jouer : ./$PROGRAMME"
