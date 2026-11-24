#include <stdio.h>
#include "types.h"
#include "tuyau.h"

void verifier(bool ok, const char *nom)
{
    if (ok)
        printf("OK      %s\n", nom);
    else
        printf("ERREUR  %s\n", nom);
}

int main(void)
{
    verifier(opposee(HAUT) == BAS, "opposee(HAUT) = BAS");
    verifier(opposee(GAUCHE) == DROITE, "opposee(GAUCHE) = DROITE");
    verifier(lireConnexion(HORIZONTAL, GAUCHE) == DROITE, "horizontal, entrée à gauche -> sortie à droite");
    verifier(lireConnexion(HORIZONTAL, HAUT) == AUCUNE, "horizontal, entrée en haut -> cul-de-sac");
    verifier(lireConnexion(COUDE_HAUT_DROITE, HAUT) == DROITE, "coude haut-droite, entrée en haut -> sortie à droite");
    verifier(lireConnexion(COUDE_GAUCHE_BAS, GAUCHE) == BAS, "coude gauche-bas, entrée à gauche -> sortie en bas");
    verifier(lireConnexion(BOMBE, BAS) == HAUT, "bombe : tout droit");
    return 0;
}