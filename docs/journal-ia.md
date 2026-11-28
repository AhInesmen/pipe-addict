# Journal d'utilisation de l'IA

Outils utilisés : Claude (Anthropic) pour la conception, l'organisation, le PowerPoint et le code ; Gemini (Google) pour l'image du graphe des commits du livrable de mi-parcours.

Chaque fichier `.c` indique aussi, en en-tête, le pourcentage approximatif d'utilisation de l'IA et des exemples de prompts.

## Conception et organisation

| Prompt (exact ou résumé fidèle)                                                                                        | Ce qu'on en a gardé                                                                                       |
| ---------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| « j'ai un projet à faire en C, voici les règles du jeu, aide moi à bien le créer, sans code! » + sujet et notes joints | Types de tuyaux, structures de données, table des connexions, algorithme d'écoulement, états de la partie |
| « conseille moi comment m'organiser », « comment je partage le travail pour 4 personnes »                              | Planning, répartition en 4 lots                                                                           |
| « à quoi doit ressembler mon repository sur git? »                                                                     | Arborescence du dépôt, branches, conventions de commit, .gitignore                                        |

## PowerPoint de mi-parcours

| Prompt (exact ou résumé fidèle)                                                                                              | Ce qu'on en a gardé                                                                          |
| ---------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- |
| « envoie moi un template pour les slides à rendre », « met à jour les incohérences et fais bien les dessins d'organigramme » | Modèle du livrable de mi-parcours, organigrammes redessinés                                  |
| « envoie moi les fichiers de code pour que je puisse remplir cette slide, pour générer la grille en rapide »                 | Prototype Allegro utilisé uniquement pour la capture de la grille du livrable de mi-parcours |

## Code

| Prompt (exact ou résumé fidèle)                                                                                         | Ce qu'on en a gardé                                                                                                                                                                                               |
| ----------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| « je souhaite un peu repartir à zero, écrire les fonctions une à une pas à pas »                                        | Code écrit pas à pas par l'équipe, avec les explications et corrections de Claude : `types.h`, remplissage au hasard avec malus, table des connexions et ses tests, révéler et échanger (version console)         |
| « guide moi pour l'étape 6, sans me donner le code », puis demandes de correction                                       | `initialiserFluide` et `passerCaseSuivante` écrites par l'équipe, corrigées avec Claude (copie au lieu d'un pointeur, `!` manquant) ; phase 2 du `main` console et `tests/test_fluide.c` fournis par Claude       |
| « passons à la prochaine étape » (fenêtre graphique)                                                                    | Code Allegro fourni et expliqué par Claude : fenêtre, dessin de la grille et des tuyaux, clics souris, chronomètre, avance automatique du fluide                                                                  |
| « je n'arrive toujours pas à gagner », « le jeu choisi lui meme où passer d'une case à l'autre »                        | Explication des règles d'écoulement ; mode test (Espace) et pause (P) fournis par Claude                                                                                                                          |
| « pourquoi quand le fluide au deuxième malus, il ne va pas à la case à droite », capture d'un plantage                  | Affichage des seuls bras empruntés par le fluide ; correction d'un champ non initialisé (`coteEntree`) qui faisait planter le jeu                                                                                 |
| Liste de fonctionnalités de l'équipe (temps, nom, compteur de parties, « ? », recacher les cases) + « suivre le sujet » | Code fourni par Claude, intégré et testé par l'équipe : police et textes, « ? », temps limite, boutons Nouveau, Refaire, Terminer et Menu, compteurs, décharge et bombe, mode mémoire, accueil (pseudo et niveau) |
| « un affichage du menu de comment se déroule le jeu », « Refait le text en utilisant de l'impératif »                   | Écran « Comment jouer » fourni par Claude                                                                                                                                                                         |
| « je pense qu'il faut un temps avant le décompte du fluide dans la case »                                               | Remplissage progressif du fluide dans chaque tuyau, fourni par Claude                                                                                                                                             |
| « pouvoir revoir les règles du jeu pendant la partie […] et quand une partie est finie »                                | Bouton Règles dans le jeu, chrono arrêté pendant la lecture, fourni par Claude                                                                                                                                    |
| « refaisons la dernière étape pour pouvoir utiliser le projet sur windows »                                             | `compiler.sh`, `paquet-windows.sh`, `CMakeLists.txt`, projet CodeBlocks, README Windows, fournis par Claude                                                                                                       |

## Rendu final

| Prompt (exact ou résumé fidèle)                                                                                    | Ce qu'on en a gardé                                                                                              |
| ------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------------- |
| « verifies pour moi si je rempli toutes les cases de la grille » (grille d'évaluation et consignes du rendu final) | Vérification par rapport à la grille ; en-têtes « utilisation de l'IA » au début de chaque fichier `.c` ; README |

## Estimation globale

- Conception : 30%
- Code : environ 60 % (détail par fichier dans l'en-tête de chaque fichier `.c`)
- PowerPoint : 60%
