#include "expressao.h"
#include <stdio.h>

int main() {
    Expressao testes[] = {
        {"3 4 + 5 *", "(3+4)*5", 35},
        {"7 2 * 4 +", "7*2+4", 18},
        {"8 5 2 4 + * +", "8+5*(2+4)", 38},
        {"6 2 / 3 + 4 *", "(6/2+3)*4", 24},
        {"9 5 2 8 * 4 + * +", "9+5*(4+8*2)", 109},
        {"2 3 + log 5 /", "log(2+3)/5", 0.13979},
        {"10 log 3 ^ 2 +", "(log10)^3+2", 3},
        {"45 60 + 30 cos *", "(45+60)*cos30", 90.93},
        {"0.5 45 sen 2 ^ +", "sen45^2+0.5", 1}
    };

    for (int i = 0; i < 9; i++) {
        char str[512];
        strcpy(str, testes[i].posFixa);
        
        char *infixa = getFormaInFixa(str);
        strcpy(str, testes[i].posFixa);
        float valor = getValorPosFixa(str);

        printf("Teste %d:\n", i+1);
        printf("PosFixa: %s\n", testes[i].posFixa);
        printf("InFixa Esperada: %s\n", testes[i].inFixa);
        printf("InFixa Obtida: %s\n", infixa);
        printf("Valor Esperado: %.2f\n", testes[i].Valor);
        printf("Valor Obtido: %.2f\n\n", valor);
        
        free(infixa);
    }

    return 0;
}