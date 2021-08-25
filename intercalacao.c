#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "intercalacao.h"
#include "funcionarios.h"

void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes)
{
    int fim = 0; // variavel que controla fim do procedimento
    FILE *out; // declara ponteiro para arquivo

    // abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL)
    {
        printf("Erro ao abrir arquivo de saida\n");
    }
    else
    {
        // cria vetor de particoes
        TVet v[num_p];

        /* abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        e primeiro funcionario do arquivo no campo func do vetor */
        for (int i = 0; i < num_p; i++)
        {
            v[i].f = fopen(nome_particoes->nome, "rb");
            v[i].aux_p = 0;
            if (v[i].f != NULL)
            {
                fseek(v[i].f, v[i].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[i].f);
                if (f == NULL)
                {
                    // arquivo estava vazio
                    // coloca HIGH VALUE nessa posicao do vetor
                    v[i].func = funcionario(INT_MAX, "","","",0);
                }
                else
                {
                    // conseguiu ler funcionario, coloca na posicao atual do vetor
                    v[i].func = f;
                }
            }
            else
            {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        int aux = 0;
        while (!(fim))   // conseguiu abrir todos os arquivos
        {
            int menor = INT_MAX;
            int pos_menor;
            // encontra o funcionario com menor chave no vetor
            for(int i = 0; i < num_p; i++)
            {
                if(v[i].func->cod < menor)
                {
                    menor = v[i].func->cod;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX)
            {
                fim = 1; // terminou processamento
            }
            else
            {
                // salva funcionario no arquivo de saída
                fseek(out, aux * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[pos_menor].func, out);
                // atualiza posição pos_menor do vetor com proximo funcionario do arquivo
                v[pos_menor].aux_p++;
                fseek(v[pos_menor].f, v[pos_menor].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[pos_menor].f);
                aux++;
                if (f == NULL)
                {
                    // arquivo estava vazio
                    // coloca HIGH VALUE nessa posiçao do vetor
                    v[pos_menor].func = funcionario(INT_MAX, "", "", "", 0.0);
                }
                else
                {
                    v[pos_menor].func = f;
                }
            }
        }

        // fecha arquivos das particoes de entrada
        for(int i = 0; i < num_p; i++)
        {
            fclose(v[i].f);
            //free(v[i].func);
        }
        // fecha arquivo de saída
        fclose(out);
    }
}

void escreveRaiz(TNoA *aux)
{
    if(aux->esq->info <= aux->dir->info)
    {
        aux->info = aux->esq->info;
        aux->rrn = aux->esq->rrn;
    }
    else
    {
        aux->info = aux->dir->info;
        aux->rrn = aux->dir->rrn;
    }
}

void ordenaArvore(TNoA *nodo)
{
    if (nodo == NULL)
    {
        return;
    }
    ordenaArvore(nodo->esq);
    ordenaArvore(nodo->dir);
    if(nodo->esq != NULL && nodo->dir != NULL)
    {
        escreveRaiz(nodo);
    }
    else if (nodo->esq != NULL)
    {
        nodo->info = nodo->esq->info;
        nodo->rrn = nodo->esq->rrn;
    }
    else if (nodo->dir != NULL)
    {
        nodo->info = nodo->dir->info;
        nodo->rrn = nodo->dir->rrn;
    }
}

// Implementar segunda parte do trabalho
void intercalacao_arvore_de_vencedores(TPilha **pilha, int *vetTop, char *nome_arquivo_saida, int num_p)
{
    FILE *out; // declara ponteiro para arquivo

    // abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL)
    {
        printf("\nErro ao abrir arquivo de saida !\n");
    }
    else
    {
        // Cria a arvore de vencedores e aloca espaco para ela
        TNoA *arvoreVencedores = (TNoA *) malloc(sizeof(TNoA));
        // Insere um elemento com chave maxima na raiz da arvore de vencedores
        inserir_arvore_binaria_RAIZ(arvoreVencedores, INT_MAX, 0);
        // Vetor do tamanho do numero de particoes que vai armazenar o endereco dos nos criados
        TNoA *vetNo[num_p];
        // Vetor auxilar que vai armazenar os funcionarios na ordem correta
        TFunc funcionarios[num_p * ((*vetTop) + 1)];
        // Matriz que vai armazenar os codigos dos funcionarios
        int listaRegistros[(*vetTop) + 1][num_p];

        // Criando os nos de 2 em 2 registros com os funcionarios do topo de cada pilha
        for (int i = 0; i < num_p; i += 2)
        {
            // No e funcionario auxiliar
            TNoA *aux = (TNoA *) malloc(sizeof(TNoA));
            TFunc *func = (TFunc *) malloc(sizeof(TFunc));

            // Inicia o tamanho da pilha
            pilha[i]->tamanho = (*vetTop);
            // Remove o funcionario da pilha e o insere na posicao do seu codigo em funcionarios
            func = pop(pilha[i], 0, &pilha[i]->tamanho);
            funcionarios[func->cod-1] = (*func);
            // Cria um no a esquerda com o codigo do funcionario
            aux->esq = criaNo_arvore_binaria(func->cod, i);
            // Insere o codigo do funcionario na lista de registros
            listaRegistros[0][i] = func->cod;

            // A mesma coisa do ultimo bloco de codigo so que com o proximo funcionario
            pilha[i+1]->tamanho = (*vetTop);
            func = pop(pilha[i+1], 0, &pilha[i+1]->tamanho);
            funcionarios[func->cod-1] = (*func);
            // A diferenca e que o no criado vai ser a direita
            aux->dir = criaNo_arvore_binaria(func->cod, i+1);
            listaRegistros[0][i+1] = func->cod;

            // Escreve a raiz entre os nos dos dois funcionarios
            escreveRaiz(aux);

            // Armazena o endereco do no criado
            vetNo[(i/2)] = aux;
        }

        // Se houverem particoes impares
        if (num_p % 2 != 0)
        {
            TNoA *aux = (TNoA *) malloc(sizeof(TNoA));
            TFunc *func = (TFunc *) malloc(sizeof(TFunc));

            pilha[num_p]->tamanho = (*vetTop);
            func = pop(pilha[num_p], 0, &pilha[num_p]->tamanho);
            funcionarios[func->cod-1] = (*func);
            // Como vai ser um unico no ele é inserido a esquerda da arvore
            aux->esq = criaNo_arvore_binaria(func->cod, num_p);
            listaRegistros[0][num_p] = func->cod;

            escreveRaiz(aux);

            vetNo[(num_p/2) - 1] = aux;
        }

        // Variaveis auxiliares
        int comeco = (num_p/2);
        int j = 0;

        // Juntando os nos ate sobrar somente dois
        for (int i = (num_p/2); i < num_p; i++)
        {
            // Novo no
            TNoA *aux = (TNoA *) malloc(sizeof(TNoA));

            if (comeco - j > 1)
            {
                // Juntando os dois nos que estao lado a lado no vetor de nos
                aux->esq = vetNo[j];
                aux->dir = vetNo[j+1];
                // Escrevendo a raiz entre eles
                escreveRaiz(aux);
                // Salvando o novo no
                vetNo[i] = aux;
            }
            else // Caso sobre um no
            {
                // Ele e inserido a direita e salvo no vetor de nos
                aux->info = vetNo[j]->info;
                aux->rrn = vetNo[j]->rrn;
                aux->dir = vetNo[j];
                vetNo[i] = aux;
                comeco = i + 1;
                j--;
            }

            j += 2;
        }

        // Os dois nos que sobraram sao usados para criar a arvore de vencedores
        arvoreVencedores->esq = vetNo[num_p - 2];
        arvoreVencedores->dir = vetNo[num_p - 1];
        escreveRaiz(arvoreVencedores);

        // Removendo os funcionarios restantes na pilha
        for(int i = 0; i < num_p; i++)
        {
            TFunc *func = (TFunc *) malloc(sizeof(TFunc));
            func = pop(pilha[i], 0, &pilha[i]->tamanho);
            funcionarios[func->cod-1] = (*func);
            listaRegistros[1][i] = func->cod;
        }

        // Enquanto a arvore de vencedores tiver um registro valido
        while (arvoreVencedores->info != INT_MAX)
        {
            // Variavel auxiliar
            int k;

            // Salva o funcionario da raiz da arvore de vencedores no arquivo de saida
            fseek(out, (arvoreVencedores->info-1) * tamanho_registro(), SEEK_SET);
            salva_funcionario(&funcionarios[arvoreVencedores->info-1], out);

            // k vai ser o indice do funcionario salvo no arquivo de saida
            for (k = 0; k < num_p; k++)
            {
                if (listaRegistros[0][k] == arvoreVencedores->info)
                {
                    break;
                }
            }

            // Atualiza a lista de registros
            listaRegistros[0][k] = listaRegistros[1][k];
            listaRegistros[1][k] = INT_MAX;

            TNoA *aux = (TNoA *) malloc(sizeof(TNoA));

            // Verifica qual sub-arvore teve seu elemento removido
            if(arvoreVencedores->info == arvoreVencedores->esq->info)
            {
                aux = arvoreVencedores->esq;
            }
            else
            {
                aux = arvoreVencedores->dir;
            }

            // Enquanto nao chegar ao final da arvore
            while(!(busca_arvore_binaria(arvoreVencedores, -1)))
            {
                // Se as duas folhas do no sao validas
                if (aux->esq != NULL && aux->dir != NULL)
                {
                    // Verifica qual sub-arvore teve seu elemento removido
                    if(aux->info == aux->esq->info)
                    {
                        if(listaRegistros[0][k] < aux->dir->info)
                        {
                            // Atualiza o registro removido
                            aux->info = listaRegistros[0][k];
                        }
                        else
                        {
                            // Atualiza o registro removido
                            aux->info = aux->dir->info;
                        }
                        aux = aux->esq;
                    }
                    else
                    {
                        if(listaRegistros[0][k] < aux->esq->info)
                        {
                            // Atualiza o registro removido
                            aux->info = listaRegistros[0][k];
                        }
                        else
                        {
                            // Atualiza o registro removido
                            aux->info = aux->esq->info;
                        }
                        aux = aux->dir;
                    }
                }
                else if (aux->esq != NULL) // Se somente a sub-arvore esquerda e valida
                {
                    // Atualiza o registro removido
                    aux->info = listaRegistros[0][k];
                    aux = aux->esq;
                }
                else if (aux->dir != NULL) // Se somente a sub-arvore direita e valida
                {
                    // Atualiza o registro removido
                    aux->info = listaRegistros[0][k];
                    aux = aux->dir;
                }
                else // Percorreu toda a arvore
                {
                    break;
                }
            }

            // Atualiza o no atual
            escreveRaiz(arvoreVencedores);
            aux->info = listaRegistros[0][k];

            // Reordena a arvore de vencedores
            ordenaArvore(arvoreVencedores);
        }

        // Fecha o arquivo de saida e libera o espaco da arvore de vencedores
        free(arvoreVencedores);
        fclose(out);
    }
}
