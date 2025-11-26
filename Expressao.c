#include "expressao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define PI 3.14159265358979323846

typedef struct {
    char dados[512][512];
    int topo;
} PilhaString;

typedef struct {
    float dados[512];
    int topo;
} PilhaFloat;

void initPilhaString(PilhaString *p) {
    p->topo = -1;
}

void pushString(PilhaString *p, char *str) {
    strcpy(p->dados[++p->topo], str);
}

char *popString(PilhaString *p) {
    return p->dados[p->topo--];
}

void initPilhaFloat(PilhaFloat *p) {
    p->topo = -1;
}

void pushFloat(PilhaFloat *p, float val) {
    p->dados[++p->topo] = val;
}

float popFloat(PilhaFloat *p) {
    return p->dados[p->topo--];
}

int isOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^');
}

int precedencia(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': case '%': return 2;
        case '^': return 3;
        default: return 0;
    }
}

int isFunction(const char *token) {
    return (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || 
            strcmp(token, "tg") == 0 || strcmp(token, "log") == 0 || 
            strcmp(token, "raiz") == 0);
}

char *getFormaInFixa(char *Str) {
    PilhaString pilha;
    initPilhaString(&pilha);
    char *token = strtok(Str, " ");
    char *result = NULL;

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            pushString(&pilha, token);
        } else if (isFunction(token)) {
            if (pilha.topo < 0) return NULL;
            char *a = popString(&pilha);
            char *temp = (char *)malloc(512);
            sprintf(temp, "%s(%s)", token, a);
            pushString(&pilha, temp);
            free(temp);
        } else if (isOperador(token[0])) {
            if (pilha.topo < 1) return NULL;
            char *b = popString(&pilha);
            char *a = popString(&pilha);
            char *temp = (char *)malloc(512);
            if (precedencia(token[0]) > 1) {
                sprintf(temp, "(%s)%c(%s)", a, token[0], b);
            } else {
                sprintf(temp, "%s%c%s", a, token[0], b);
            }
            pushString(&pilha, temp);
            free(temp);
        } else {
            return NULL;
        }
        token = strtok(NULL, " ");
    }

    if (pilha.topo != 0) return NULL;
    result = (char *)malloc(strlen(pilha.dados[0]) + 1);
    strcpy(result, pilha.dados[0]);
    return result;
}

float getValorPosFixa(char *StrPosFixa) {
    PilhaFloat pilha;
    initPilhaFloat(&pilha);
    char *token = strtok(StrPosFixa, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            pushFloat(&pilha, atof(token));
        } else if (isOperador(token[0])) {
            if (pilha.topo < 1) return 0;
            float b = popFloat(&pilha);
            float a = popFloat(&pilha);
            switch (token[0]) {
                case '+': pushFloat(&pilha, a + b); break;
                case '-': pushFloat(&pilha, a - b); break;
                case '*': pushFloat(&pilha, a * b); break;
                case '/': 
                    if (b == 0) return 0;
                    pushFloat(&pilha, a / b); 
                    break;
                case '%': 
                    if (b == 0) return 0;
                    pushFloat(&pilha, (int)a % (int)b); 
                    break;
                case '^': pushFloat(&pilha, pow(a, b)); break;
            }
        } else if (isFunction(token)) {
            if (pilha.topo < 0) return 0;
            float a = popFloat(&pilha);
            if (strcmp(token, "sen") == 0) {
                pushFloat(&pilha, sin(a * PI / 180.0));
            } else if (strcmp(token, "cos") == 0) {
                pushFloat(&pilha, cos(a * PI / 180.0));
            } else if (strcmp(token, "tg") == 0) {
                pushFloat(&pilha, tan(a * PI / 180.0));
            } else if (strcmp(token, "log") == 0) {
                if (a <= 0) return 0;
                pushFloat(&pilha, log10(a));
            } else if (strcmp(token, "raiz") == 0) {
                if (a < 0) return 0;
                pushFloat(&pilha, sqrt(a));
            }
        } else {
            return 0;
        }
        token = strtok(NULL, " ");
    }

    if (pilha.topo != 0) return 0;
    return pilha.dados[0];
}