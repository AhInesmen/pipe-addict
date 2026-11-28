#!/bin/sh
# Fabrique le dossier « Pipe-Addict-Windows », jouable sur n'importe quel PC Windows
# SANS installer MSYS2 : le jeu, les DLL d'Allegro dont il a besoin, et la police.
# À lancer dans le terminal MSYS2 UCRT64, depuis le dossier pipe-addict :   sh paquet-windows.sh
set -e

if [ -z "$MSYSTEM" ]; then
    echo "Ce script se lance dans le terminal MSYS2 UCRT64, sous Windows."
    exit 1
fi

DOSSIER=Pipe-Addict-Windows

echo "1. Compilation du jeu..."
gcc -O2 src/main.c src/jeu.c src/grille.c src/tuyau.c src/fluide.c src/affichage.c -o pipe-addict.exe \
    $(pkg-config --cflags --libs allegro-5 allegro_main-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5)

echo "2. Preparation du dossier $DOSSIER..."
rm -rf "$DOSSIER"
mkdir -p "$DOSSIER"
cp pipe-addict.exe "$DOSSIER/"
cp -r assets "$DOSSIER/"

echo "3. Copie des DLL utilisees par le jeu (Allegro et ses dependances)..."
# ldd liste les DLL dont le programme a besoin ; on garde celles de MSYS2, pas celles de Windows
ldd pipe-addict.exe | grep -i -E '/(ucrt64|mingw64)/' | awk '{print $3}' | while read -r dll; do
    cp "$dll" "$DOSSIER/"
done

echo "Termine : copiez le dossier $DOSSIER sur n'importe quel PC Windows,"
echo "puis double-cliquez sur pipe-addict.exe."
