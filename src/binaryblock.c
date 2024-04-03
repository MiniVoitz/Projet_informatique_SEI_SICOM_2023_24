#include "pyas/all.h"

void print_binaryblock(binaryblock_t bb) {
    if (bb == NULL)
    {
        printf("binaryblock NULL.\n");
        return;
    }

    printf("Operateur: %c\n", bb->operateur);
    printf("Crochet: %d\n", bb->crochet);
    printf("Negation: %d\n", bb->negation);
    printf("Point: %d\n", bb->point);
    printf("Erreur: %d\n", bb->err);

    int i;
    if (bb->array) {
        printf("Contenu du tableau: ");
        for (i = 0; i < 128; i++)
        {
            if((bb->array)[i])
            printf("%d : %c ", i);
        }
        printf("\n");
    } else {
        printf("Aucun tableau associé.\n");
    }
}
