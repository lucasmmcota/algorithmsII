#include <stdlib.h>
#include <limits.h>

#include "funcionarios.h"
#include "particoes.h"

#include "manipulacao_arquivos.h"

void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc)
{
    rewind(arq); // posiciona cursor no inicio do arquivo
    int reg = 0;

    while (reg != nFunc)
    {
        // le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(arq))
        {
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(arq);
            //imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i >= M)
                break;
        }

        // ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M)
            M = i;

        // faz ordenacao
        for (int j = 1; j < M; j++)
        {
            TFunc *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod))
            {
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        // cria arquivo de particao e faz gravacao
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        printf("\n%s\n", nome_particao);
        FILE *p;
        if ((p = fopen(nome_particao, "wb+")) == NULL)
            printf("Erro criar arquivo de saida\n");
        else
        {
            for (int i = 0; i < M; i++)
            {
                fseek(p, (i) * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[i], p);
                imprime_funcionario(v[i]);
            }
            fclose(p);
        }
        for(int jj = 0; jj<M; jj++)
            free(v[jj]);
    }
}

TFunc *recupera_funcionario(FILE *arq, int *totalLidos)
{
    fseek(arq, *totalLidos * tamanho_registro(), SEEK_SET);
    TFunc* func = le_funcionario(arq);
    (*totalLidos)++;
    return func;
}

void atualiza_nome_particao(Lista *nomes, int *numeroNomes)
{
    if(nomes->prox == NULL)
    {
        char* novoNome = malloc(5 * sizeof(char));
        (*numeroNomes)++;
        sprintf(novoNome, "p%d.dat", *numeroNomes);
        nomes->prox = cria(novoNome, NULL);
    }
}

int buscar_menor(TFunc *v[], int tam)
{
    int menor = 0;
    for (int i = 1; i < tam; i++)
    {
        if(v[menor]->cod > v[i]->cod)
            menor = i;
    }
    return menor;
}

void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n)
{
    FILE* reservatorio = fopen("reservatorio.dat", "w+"); // reserva-se um espaço para abrigar os registros congelados
    TFunc** v = malloc(M * sizeof(TFunc*));
    char* nome;
    Lista* nomes = nome_arquivos_saida;
    int noVetor = 0, totalLidos = 0, noReservatorio = 0, naParticaoAtual = 0; // contadores

    FILE* particaoAtual = NULL;
    while(totalLidos < nFunc)  // repete ate ler todos os funcionarios
    {

        while(noVetor < M)  // le M registros do arquivo para a memoria
        {
            v[noVetor] = recupera_funcionario(arq, &totalLidos);
            noVetor++;
        }

        // seleciona, no array em memoria, o registro com a menor chave
        int menor = buscar_menor(v, noVetor); // retorna o index do funcionario com a menor chave no vetor

        if(particaoAtual == NULL)   // abre uma nova particao
        {
            nome = nomes->nome;
            particaoAtual = fopen(nome, "w+");
            atualiza_nome_particao(nomes, &n);
            nomes = nomes->prox;
        }
        if(naParticaoAtual == M)
        {
            imprime_particao(particaoAtual, nome, M);
            fclose(particaoAtual);
            particaoAtual = NULL;
            naParticaoAtual = 0;
            continue;
        }
        // Grava o registro r na partição de saída atual
        fseek(particaoAtual, naParticaoAtual * tamanho_registro(), SEEK_SET);
        salva_funcionario(v[menor], particaoAtual);
        naParticaoAtual++;

        if(!(totalLidos < nFunc))
        {
            for (int i = menor; i < noVetor - 1; i++)
                v[i] = v[i + 1];
            noVetor--;
            break;
        }
        // substitui o registro r pelo proximo registro do arquivo de entrada.
        int code = v[menor]->cod;
        v[menor] = recupera_funcionario(arq, &totalLidos);
        menor = buscar_menor(v, noVetor);

        if(v[menor]->cod < code)  // Caso a chave do proximo registro seja menor que a chave recém gravada, gravá-lo no reservatório e substituir, no array em memória, o registro r pelo próximo registro do arquivo de entrada
        {
            fseek(reservatorio, noReservatorio * tamanho_registro(), SEEK_SET);
            salva_funcionario(v[menor], reservatorio);
            noReservatorio++;
            if(!(totalLidos < nFunc))
            {
                for (int i = menor; i < noVetor - 1; i++)
                    v[i] = v[i + 1];
                noVetor--;
                break;
            }
            v[menor] = recupera_funcionario(arq, &totalLidos); //substitiui o registro r pelo proximo registro do arquivo de entrada
        }
        if(noReservatorio == n)  // caso o reservatorio esteja cheio
        {
            imprime_particao(particaoAtual, nome, M);
            fclose(particaoAtual); // fechar a particao de saida atual

            nome = nomes->nome;
            particaoAtual = fopen(nome, "w+");;
            atualiza_nome_particao(nomes, &n);
            nomes = nomes->prox;

            int j = 0;
            while(noVetor > 0)
            {
                fseek(particaoAtual, j * tamanho_registro(), SEEK_SET);
                int lower = buscar_menor(v, noVetor);
                salva_funcionario(v[lower], particaoAtual);
                for (int i = lower; i < noVetor - 1; i++)
                    v[i] = v[i + 1];
                noVetor--;
                j++;
            }

            for (int i = 0; i < noVetor; i++)
                free(v[i]);

            noVetor = 0;
            for (int i = 0; i < noReservatorio; i++)  // copia os registros do reservatorio para o array em memoria
            {
                fseek(reservatorio, i * tamanho_registro(), SEEK_SET);
                v[i] = le_funcionario(reservatorio);
                noVetor++;
            }
            fclose(reservatorio);
            reservatorio = fopen("reservatorio.dat", "w+");
            noReservatorio = 0; // limpando o reservatorio
            imprime_particao(particaoAtual, nome, M);
            fclose(particaoAtual);
            particaoAtual = NULL;
            naParticaoAtual = 0;
        }
    }
    if(particaoAtual)
    {
        imprime_particao(particaoAtual, nome, M);
        fclose(particaoAtual);
        particaoAtual = NULL;
        naParticaoAtual = 0;
        nome = nomes->nome;
        particaoAtual = fopen(nome, "w+");;
        atualiza_nome_particao(nomes, &n);
        nomes = nomes->prox;
    }
    if(noVetor > 0 && noReservatorio > 0)
    {
        if(noReservatorio > 0)
        {
            for (int i = 0; i < noReservatorio; i++)  // copia o que sobrou no resevatorio para o vetor pela ultima vez.
            {
                fseek(reservatorio, i * tamanho_registro(), SEEK_SET);
                v[noVetor] = le_funcionario(reservatorio);
                noVetor++;
                fseek(particaoAtual, naParticaoAtual * tamanho_registro(), SEEK_SET);
                naParticaoAtual++;
            }
            fclose(reservatorio);
            reservatorio = fopen("reservatorio.dat", "w+");// limpa reservatorio
            naParticaoAtual = 0;
            noReservatorio = 0;
        }

        if(noVetor > 0)
        {
            int j = 0;
            while(noVetor > 0)
            {
                fseek(particaoAtual, j * tamanho_registro(), SEEK_SET);
                int lower = buscar_menor(v, noVetor);
                salva_funcionario(v[lower], particaoAtual);
                for (int i = lower; i < noVetor - 1; i++)
                {
                    v[i] = v[i + 1];
                }
                noVetor--;
                j++;
            }
        }
    }
    imprime_particao(particaoAtual, nome, M);
    imprime_particao(reservatorio, "reservatorio.dat", n);
    fclose(particaoAtual);
    fclose(reservatorio);
    free(v);
}

void imprime_particao(FILE *arq, char *nome, int tam)
{
    printf("\n%s:\n", nome);
    imprime_base_dados_funcionarios(arq, tam);
}
