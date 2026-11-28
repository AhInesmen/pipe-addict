# Pipe-Addict : le bon tuyau

Projet ING3 (ECE Paris, 2026-2027), groupe DU01, écrit en C avec la bibliothèque graphique Allegro 5. Le jeu fonctionne sous macOS et sous Windows.

Équipe : Ines Nyinawabatoni, Romain Condamine, Yasmine Maimouna Yoda, Andy Erwin Fieni.

## Le jeu

Une grille cachée de 6 × 6 cases contient des morceaux de tuyau et des malus. Le joueur doit construire un chemin entre l'entrée, à gauche, et la sortie, à droite.

- Un clic sur une case cachée « ? » la révèle. Un second clic échange son tuyau avec celui de la case swap.
- 5 secondes après le début de la partie, le fluide part de l'entrée et suit la logique des tuyaux, en remplissant chaque tuyau petit à petit, à vitesse constante (voir Réglages).
- Le joueur gagne si le fluide atteint la sortie. Il perd si le fluide tombe dans un cul-de-sac (un tuyau sans ouverture du bon côté, ou le bord de la grille) ou si le temps limite est écoulé.
- **Décharge** (rond jaune) : le fluide la traverse tout droit et va deux fois plus vite à partir de là.
- **Bombe** (rond noir) : le fluide la traverse tout droit. Toutes les cases qu'il n'a pas remplies se recachent, et il repart 5 secondes plus tard.
- Les malus ne peuvent pas être échangés.

## Commandes

| Commande                           | Action                                                                                                                        |
| ---------------------------------- | ----------------------------------------------------------------------------------------------------------------------------- |
| Accueil                            | Taper son nom, choisir le niveau (Facile, Moyen ou Difficile : 2, 4 ou 6 malus), puis cliquer sur Jouer ou appuyer sur Entrée |
| Bouton **Comment jouer** (accueil) | Écran des règles, avec les tuyaux et les malus dessinés ; Retour ou Échap pour revenir                                        |
| Clic gauche sur une case           | Révéler la case, puis l'échanger avec la case swap                                                                            |
| Bouton **Nouveau**                 | Nouvelle grille, nouvelles entrée et sortie                                                                                   |
| Bouton **Refaire**                 | Même grille et mêmes entrée et sortie, avec tout recaché                                                                      |
| Bouton **Terminer**                | Le fluide va instantanément au bout de son chemin                                                                             |
| Bouton **Menu**                    | Retour à l'accueil, pour changer de nom ou de niveau                                                                          |
| Bouton **Règles**                  | Revoir les règles pendant ou après une partie ; le chrono s'arrête pendant la lecture                                         |
| Espace                             | Mode test : afficher ou recacher toute la grille                                                                              |
| P                                  | Mettre le jeu en pause, ou le relancer                                                                                        |

## Installation (une seule fois)

**macOS**, avec [Homebrew](https://brew.sh) :

```
brew install allegro pkg-config
```

**Windows**, avec [MSYS2](https://www.msys2.org) :

1. Téléchargez et installez MSYS2, en gardant le dossier proposé (`C:\msys64`).
2. Dans le menu Démarrer, ouvrez le terminal **MSYS2 UCRT64**. Attention : pas « MSYS2 MSYS » ni « MINGW64 ».
3. Installez le compilateur, Allegro et pkg-config :

```
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-allegro mingw-w64-ucrt-x86_64-pkgconf git
```

## Compiler et lancer

Placez-vous dans le dossier `pipe-addict`. Dans le terminal MSYS2, le disque `C:` s'appelle `/c`. Par exemple :

```
cd /c/Users/VotreNom/Documents/pipe-addict
```

Puis, **sur Mac comme sur Windows** :

```
sh compiler.sh
```

Le script choisit tout seul `clang` sur Mac ou `gcc` sur Windows. Ensuite, lancez le jeu avec `./pipe-addict` sur Mac, ou `./pipe-addict.exe` sur Windows.

Le jeu se place dans son propre dossier au démarrage. Il trouve donc toujours sa police, dans `assets/fonts/`, même lancé depuis un autre dossier.

## Version Windows à jouer sans MSYS2

Pour jouer sur un PC où MSYS2 n'est pas installé (par exemple le jour de la soutenance), lancez dans le terminal MSYS2 UCRT64 :

```
sh paquet-windows.sh
```

Le script crée un dossier **`Pipe-Addict-Windows`** qui contient le jeu, les DLL d'Allegro dont il a besoin et la police. Copiez ce dossier, ou compressez-le en .zip, sur n'importe quel PC Windows, puis double-cliquez sur `pipe-addict.exe`.

## Avec CodeBlocks

Ouvrez `pipe-addict.cbp`. Le projet contient les fichiers de `src/`, et l'édition de liens avec Allegro (`allegro`, `allegro_main`, `allegro_primitives`, `allegro_font`, `allegro_ttf`) est déjà réglée. Allegro 5 doit être installé et connu de CodeBlocks. Sous Windows, avec MSYS2, réglez le compilateur de CodeBlocks sur celui de MSYS2 : _Settings > Compiler > Toolchain executables_, dossier `C:\msys64\ucrt64`. Le jeu se lance depuis le dossier du projet, où il trouve `assets/`.

## Avec CMake (optionnel : CLion, VS Code)

```
cmake -S . -B build
cmake --build build
```

Le jeu et les tests sont créés dans `build/`, et `assets/` y est copié automatiquement. Sous Windows, installez d'abord CMake et Ninja dans MSYS2 avec `pacman -S mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja`, puis utilisez `cmake -S . -B build -G Ninja`.

## Tests en console (sans Allegro)

```
gcc -Wall -Wextra -Isrc tests/test_fluide.c src/fluide.c src/grille.c src/tuyau.c -o test_fluide && ./test_fluide
gcc -Wall -Wextra -Isrc tests/jeu_console.c src/fluide.c src/grille.c src/tuyau.c -o jeu_console && ./jeu_console
```

`test_fluide` vérifie l'écoulement du fluide sur des grilles écrites à la main : victoires, culs-de-sac, coudes, malus traversés et retour dans une case déjà remplie. `jeu_console` est la première version du jeu, jouable dans le terminal.

## Organisation du code

| Fichier              | Rôle                                                                                                                              |
| -------------------- | --------------------------------------------------------------------------------------------------------------------------------- |
| `src/types.h`        | Structures et énumérations communes : `Direction`, `Contenu`, `Case`, `Grille`, `Fluide`, `Partie`                                |
| `src/tuyau.c/.h`     | Table des connexions (par quel côté le fluide ressort de chaque tuyau), directions opposées, décalages                            |
| `src/grille.c/.h`    | Remplissage au hasard avec les malus, révéler, échanger avec la case swap, recacher les cases non remplies                        |
| `src/fluide.c/.h`    | Écoulement du fluide case par case, victoire, cul-de-sac, effets de la décharge et de la bombe                                    |
| `src/jeu.c/.h`       | Nouvelle partie, Refaire, Terminer, temps limite                                                                                  |
| `src/affichage.c/.h` | Dessin de l'accueil, de l'écran des règles, de la grille, des tuyaux, des textes et des boutons, et détection des boutons cliqués |
| `src/main.c`         | Fenêtre Allegro, chronomètre (60 tics par seconde) et boucle d'événements (souris, clavier, chrono)                               |
| `compiler.sh`        | Compilation sur Mac ou Windows (MSYS2)                                                                                            |
| `paquet-windows.sh`  | Dossier jouable sous Windows sans MSYS2                                                                                           |
| `CMakeLists.txt`     | Compilation avec CMake (optionnel)                                                                                                |
| `pipe-addict.cbp`    | Projet CodeBlocks                                                                                                                 |
| `tests/`             | Tests en console                                                                                                                  |
| `assets/fonts/`      | Police DejaVu Sans, sous licence libre (voir `LICENCE-DejaVu.txt`)                                                                |

## Réglages

| Constante      | Fichier    | Valeur                                                                        |
| -------------- | ---------- | ----------------------------------------------------------------------------- |
| `DELAI_DEPART` | `fluide.h` | 5 s avant le départ du fluide (imposé par le sujet)                           |
| `DELAI_CASE`   | `fluide.h` | 10 s par case                                                                 |
| `PAUSE_BOMBE`  | `fluide.h` | 5 s d'arrêt après une bombe                                                   |
| `TEMPS_LIMITE` | `jeu.h`    | `DELAI_DEPART + 30 × DELAI_CASE`, soit 305 s                                  |
| `MODE_MEMOIRE` | `main.c`   | `true` : quand on révèle une case, les autres cases non remplies se recachent |

## Ajouts et choix par rapport au sujet

- **Temps limite** : la partie est perdue au bout de `TEMPS_LIMITE` secondes (305 s avec les réglages actuels).
- **Mode mémoire** (`MODE_MEMOIRE`) : c'est une option de difficulté. Avec `false`, on retrouve la règle du sujet, où les cases révélées restent visibles.
- **Mode test** (Espace) et **pause** (P) : pour tester le jeu et faire la démonstration.
- **Nom du joueur, niveau, numéro de la partie et nombre de victoires**, affichés à gauche de la grille.
- **Écran « Comment jouer »** : il est accessible depuis l'accueil et pendant la partie (bouton Règles).
- **La case swap est à droite de la grille**, et non en bas à gauche comme sur la figure du sujet.

## Utilisation de l'IA

Une partie du code a été écrite avec l'aide de Claude (Anthropic) : explications pas à pas, corrections, et code fourni pour l'affichage graphique et les fonctionnalités finales. Chaque fichier `.c` indique en en-tête le pourcentage approximatif d'utilisation de l'IA et des exemples de prompts. Le détail figure dans `docs/journal-ia.md`.
