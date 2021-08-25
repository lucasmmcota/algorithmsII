#ifndef INTERCALACAO_H
#define INTERCALACAO_H

#include "lista.h"
#include "pilha.h"
#include "arvore_binaria.h"

/* estrutura para armazenar o vetor de partições */
typedef struct vetor
{
    TFunc *func;
    int aux_p;
    FILE *f;
} TVet;

/* Algoritmo de intercalacao de particoes básico, recebe
o nome do arquivo de saída, o número de partições a serem intercaladas
e uma lista com os nomes das partições de entrada a serem lidas. */
void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes);

/* Algoritmo de intercalacao de particoes usando arvore de vencedores a ser implementado no trabalho,
 fiquem a vontade para modificar a estrutura dos parâmetros, somente uma sugestão */
void intercalacao_arvore_de_vencedores(TPilha **pilha, int *vetTop, char *nome_arquivo_saida, int num_p);

/* Funcao que escreve a menor chave dos nos folha na raiz */
void escreveRaiz(TNoA *aux);

/* Funcao que ordena a arvore binaria de vencedores, sendo a raiz a menor chave */
void ordenaArvore(TNoA *nodo);

#endif
