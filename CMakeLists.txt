# Compilation avec CMake (CLion, VS Code...), sous macOS ou Windows (MSYS2 UCRT64).
#   cmake -S . -B build
#   cmake --build build
cmake_minimum_required(VERSION 3.16)
project(pipe_addict C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED IMPORTED_TARGET
    allegro-5 allegro_main-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5)

# Le jeu
add_executable(pipe-addict src/main.c src/jeu.c src/grille.c src/tuyau.c src/fluide.c src/affichage.c)
target_link_libraries(pipe-addict PRIVATE PkgConfig::ALLEGRO)
target_compile_options(pipe-addict PRIVATE -Wall -Wextra)

# La police doit se trouver à côté du programme : on copie assets/ dans le dossier de compilation
add_custom_command(TARGET pipe-addict POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:pipe-addict>/assets)

# Les tests en console, sans Allegro
add_executable(test_fluide tests/test_fluide.c src/fluide.c src/grille.c src/tuyau.c)
target_include_directories(test_fluide PRIVATE src)

add_executable(jeu_console tests/jeu_console.c src/fluide.c src/grille.c src/tuyau.c)
target_include_directories(jeu_console PRIVATE src)
