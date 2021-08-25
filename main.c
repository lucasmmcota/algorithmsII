#include <stdlib.h>

#include "ordenacao_busca.h"
#include "lista.h"
#include "particoes.h"
#include "intercalacao.h"
#include "arvore_binaria.h"

/*
    Arcabouço para desenvolvimento do trabalho da disciplina de AEDSII 2020-2, utilizando os exemplos dos livros
    de Schildt, H. C Completo e Total. Ed. McGraw-Hill e de Ferraz, I. N. Programação com Arquivos. Editora Manole Ltda
    sobre o problema de funcionários e algoritmos.
*/

int menu()
{
    printf("\n\n**************MENU**************\n\nBASE DE DADOS\n");

    printf("\nDigite 1 para utilizar a base de dados funcionario de teste.");
    printf("\nDigite 2 se desejar imprimir a base de dados funcionario.");
    printf("\nDigite 3 se desejar imprimir o indice primario da base de funcionarios.");

    printf("\n\nINTERPOLACAO\n");
    printf("\nDigite 4 para utilizar a estrutura de interpolacao teste.");
    printf("\nDigite 5 para gerar particoes ordenadas utilizando o metodo de classificacao interna.");
    printf("\nDigite 6 para gerar particoes ordenadas utilizando o metodo de selecao natural");
    printf("\nDigite 7 para criar uma pilha com o conteudo das particoes geradas.");
    printf("\nDigite 8 para intercalar as particoes ordenadas utilizando o metodo de intercalacao basico.");
    printf("\nDigite 9 para intercalar as particoes ordenadas utilizando o metodo de arvore binaria de vencedores.");

    printf("\n\nINSERTION SORT, ARVORE BINARIA E ARVORE B\n");
    printf("\nDigite 10 para criar arvores binarias em memoria interna e externa.");
    printf("\nDigite 11 para ordenar a base de dados utilizando o metodo insertion sort em disco. ");
    printf("\nDigite 12 para criar Arvore B+ de prefixo simples. ");

    printf("\n\nDigite -1 para sair.");

    int op;
    printf("\n\nDigite a sua opcao: ");
    scanf("%d", &op);

    return op;
}

int main()
{
    /* criando um arquivo lógico para abir para leitura e escrita o arquivo binário */
    FILE* arq = fopen("base_funcionarios.dat", "wb+");
    if(arq == NULL)
    {
        printf("\nArquivo base_funcionarios.dat não encontrado.\n");
        return 1;
    }

    /* criando um arquivo lógico para abir para leitura e escrita do arquivo de indice primario ordenado */
    FILE* arqIndicePrimario = fopen("IndicePrimarioFunc.dat", "wb+");
    if(arqIndicePrimario == NULL)
    {
        printf("\nArquivo IndicePrimarioFunc.dat não encontrado.\n");
        return 1;
    }

    /* criando um arquivo lógico para abir para leitura e escrita do arquivo de indice primario não ordenado */
    FILE* arqIndicePrimarioNaoOrdenado = fopen("IndicePrimarioFuncNaoOrdenado.dat", "wb+");
    if(arqIndicePrimarioNaoOrdenado == NULL)
    {
        printf("Arquivo IndicePrimarioFuncNaoOrdenado.dat não encontrado.\n");
        return 1;
    }

    /* criando um arquivo lógico para abir para leitura e escrita da árvore binária externa */
    FILE* arqArvoreBinariaExterna = fopen("ArvoreBinariaExternaFunc.dat", "wb+");
    if(arqArvoreBinariaExterna == NULL)
    {
        printf("Arquivo ArvoreBinariaExternaFunc.dat não encontrado.\n");
        return 1;
    }

    Lista *nomes = NULL;
    TPilha **vetPilhas;
    int nFunc, nParticoes, nElementos;
    int crioubase = 0, crioulistadenomes = 0, criouparticoes = 0, crioupilhas = 0;
    int op = menu();

    /* ponteiro para salvar o início da lista de nomes */
    Lista *prox;
    int *vetTopo;

    while(op != -1)
    {
        switch (op)
        {
        case (1):
            // criando a base de dados de funcionários teste e o arquivo de índice primário
            nFunc = 20;
            cria_base_dados_funcionarios_teste(arq, arqIndicePrimario, arqIndicePrimarioNaoOrdenado, nFunc);
            crioubase = 1;
            break;

        case (2):
            // imprime a base de dados dos funcionários
            if(crioubase)
            {
                imprime_base_dados_funcionarios(arq, nFunc);
            }
            else
            {
                printf("\nBase de dados inexistente para ser impressa, crie utilizando a opcao 1 do menu!!!\n");
            }
            break;

        case (3):
            // imprime o arquivo de índices primários
            if(crioubase)
            {
                printf("\nArquivo ordenado\n ch:rrn\n");
                imprime_indice_primario_funcionarios(arqIndicePrimario,nFunc);

                printf("\nArquivo nao ordenado\n ch:rrn\n");
                imprime_indice_primario_funcionarios(arqIndicePrimarioNaoOrdenado,nFunc);
            }
            else
            {
                printf("\nIndice primario inexistente para ser impresso, sera criado apos a definicao da base de dados utilizando a opcao 1 do menu!!!\n");
            }
            break;

        case (4):

            if(crioubase)
            {
                nParticoes = 10;
                nElementos = 2;
                printf("\n\nA estrutura de interpolacao sera composta por 10 arquivos de no maximo 2 elementos.\n");
                nomes = cria("p1.dat", cria("p2.dat", cria("p3.dat", cria("p4.dat", cria("p5.dat",  cria("p6.dat",  cria("p7.dat",  cria("p8.dat",  cria("p9.dat",  cria("p10.dat", NULL))))))))));
                crioulistadenomes = 1;
                imprime(nomes);
                break;
            }
            else
            {
                printf("\nA base de dados nao foi criada, utilize a opcao 1 para criar !\n\n");
            }

        case (5):
            if(crioulistadenomes && crioubase)
            {
                // cria as particões que contém a base de dados de funcionários usando o método classificação interna
                printf("\n\nGerando particoes, utilizando o metodo de classificacao interna.\n");
                classificacao_interna(arq, nomes, nElementos, nFunc);

                // salvando o ponteiro para o início da lista de nomes
                prox = nomes;
                criouparticoes = 1;
            }
            else
            {
                if(crioubase == 0)
                {
                    printf("\nA base de dados nao foi criada, utilize a opcao 1 para criar !\n\n");
                }
                else
                {
                    printf("\nLista de nomes de arquivos inexistente para gerar particoes, crie a estrutura de interpolacao utilizando a opcao 4 do menu!!!\n");
                }
            }
            break;

        case (6):
            if(crioulistadenomes)
            {
                printf("\n\nPRIMEIRA PARTE DO TRABALHO");

                printf("\n\nGerando particoes, utilizando o metodo selecao natural.\n");
                selecao_natural(arq, nomes, nElementos, nFunc, 1);

                // salvando o ponteiro para o início da lista de nomes
                prox = nomes;
                criouparticoes = 1;
            }
            else
            {
                printf("\nLista de nomes de arquivos inexistente para gerar particoes, crie a estrutura de interpolacao utilizando a opcao  4 do menu!!!\n");
            }
            break;

        case (7):
            if(criouparticoes)
            {
                // criando pilha com o conteúdo das partições geradas
                vetPilhas = (TPilha **) malloc(sizeof(TPilha *) * (nParticoes));
                vetTopo = (int *) malloc(sizeof(int) * (nParticoes));

                for(int p = 0 ; p < nParticoes ; p++)
                {
                    // alocando a memória para salvar uma pilha para cada particao
                    vetPilhas[p] = (TPilha *) malloc(sizeof(TPilha) * (nElementos));

                    // abrindo primeira partição para leitura
                    FILE *part = fopen(nomes->nome, "rb+");
                    printf("Particao %s \n", nomes->nome);
                    nomes = nomes->prox;

                    // imprimindo os funcionários gravados nas partições
                    imprime_base_dados_funcionarios(part, nElementos);

                    // inicializando os topos das pilhas
                    vetTopo[p] = -1;

                    // criando pilha através da leitura da partição
                    cria_pilha_particao(vetPilhas[p], part, nElementos, &vetTopo[p]);

                    // fechando a partição
                    fclose(part);
                }
                crioupilhas = 1;
                nomes = prox;
            }
            else
            {
                printf("\nParticoes nao foram criadas, crie as particoes utilizando as opcoes 5 ou 6 do menu!!!\n");
            }
            break;

        case (8):
            if(criouparticoes)
            {
                printf("\n\nIntercalando as particoes, utilizando o metodo de intercalacao basico.\n");
                intercalacao_basico("arquivo_intercalado.dat", nParticoes, nomes);

                // imprimindo o arquivo intercalado
                FILE *fi = fopen("arquivo_intercalado.dat","rb+");
                imprime_base_dados_funcionarios(fi, nFunc);
                fclose(fi);
            }
            else
            {
                printf("\nParticoes nao foram criadas, crie as particoes utilizando as opcoes 5 ou 6 do menu!!!\n");
            }
            break;

        case (9):
            printf("\nMetodo a ser implementado na parte II do trabalho.\n");

            if(criouparticoes)
            {
                if(crioupilhas)
                {
                    printf("\n\nIntercalando as particoes, utilizando o metodo de intercalacao arvore binaria de vencedores.\n");
                    intercalacao_arvore_de_vencedores(vetPilhas, vetTopo, "arquivo_intercalado.dat", nParticoes);

                    // imprimindo o arquivo intercalado
                    FILE *fi = fopen("arquivo_intercalado.dat","rb+");
                    imprime_base_dados_funcionarios(fi, nFunc);
                    fclose(fi);
                }
                else
                {
                    printf("\nAs pilhas das particoes geradas nao foram criadas, crie as pilhas utilizando as opcoes 7!!!\n");
                }
            }
            else
            {
                printf("\nParticoes nao foram criadas, crie as particoes utilizando as opcoes 5 ou 6 do menu!!!\n");
            }

            break;

        case (10):
            // cria árvore binária em memória utilizando o arquivo de índice primário ordenado
            if(crioubase)
            {
                printf("\nArvore binaria em memoria pelo arquivo de  indice primario ordenado\n <nopai <noesq><nodireita> >\n");
                cria_arvore_binaria_indice_primario(arqIndicePrimario,nFunc);
                printf("\nArvore binaria em memoria pelo arquivo de indice primario nao ordenado\n <nopai <noesq><nodireita> >\n");
                cria_arvore_binaria_indice_primario(arqIndicePrimarioNaoOrdenado,nFunc);
                printf("\nArvore binaria externa pelo arquivo de base de dados do funcionario\n <nopai <noesq><nodireita> >\n");
                criar_arvore_binaria_externa(arq,arqArvoreBinariaExterna);
            }
            else
            {
                printf("\nArquivo de indice primario inexistente para a criacao da arvore binaria, crie utilizando a opcao 1 do menu!!!\n");
            }
            break;

        case (11):
            // ordenando a base de dados utilizando ordenação direta no arquivo com o método inseriton sort
            if(crioubase)
            {
                insertion_sort_disco(arq, nFunc);
                imprime_base_dados_funcionarios(arq, nFunc);

                // procurando por um funcionário na base de dados usando o método de busca binária
                int busca_codigo;
                printf("\n\nInforme o codigo do funcionario procurado (digite -1 caso nao queira procurar um funcionario): ");
                scanf("%d", &busca_codigo);
                if(busca_codigo != -1)
                {
                    TFunc* func = busca_binaria(busca_codigo, arq, nFunc);
                    if (func != NULL)
                    {
                        imprime_funcionario(func);
                    }
                    else
                    {
                        printf("\nFuncionario nao encontrado na base de dados!");
                    }
                    free(func);
                }
            }
            else
            {
                printf("\nBase de dados inexistente para ser ordenada, crie utilizando a opcao 1 do menu!!!\n");
            }
            break;

        case (12):
            printf("\nMetodo a ser implementado na parte III do trabalho.\n");
            if(crioubase)
            {
                // criar_arvore_B() PARTE III
            }
            else
            {
                printf("\nBase de dados inexistente para ser ordenada, crie utilizando a opcao 1 do menu!!!\n");
            }
            break;

        default:
            printf("\nOpcao invalida!");
        }
        op = menu();
    }

    while(nomes)
    {
        prox = nomes->prox;
        free(nomes);
        nomes = prox;
    }

    for(int p = 0; p <nParticoes; p++)
    {
        free(vetPilhas[p]);
    }

    free(vetPilhas);

    fclose(arqIndicePrimario);
    fclose(arqIndicePrimarioNaoOrdenado);
    fclose(arqArvoreBinariaExterna);
    fclose(arq);

    return 0;
}
