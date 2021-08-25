#include <stdlib.h>
#include <math.h>

#include "manipulacao_arquivos.h"
#include "funcionarios.h"

int tamanho_arquivo(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registro());
    return tam;
}
