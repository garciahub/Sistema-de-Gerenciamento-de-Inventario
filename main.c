#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct item {
    char itemNome[51];
    int itemQuantidade;
} item;

typedef struct categoria {
    char categoriasNome[51];
    int itensCategoriasQuantidade;
} categoria;

typedef struct dadosPrograma{
    categoria* listaCategorias;
    item** matrizItens;
    int categoriasCadastradasQuantidade;
} dadosPrograma;

void limparTela() {
    system("cls");
}

void liberarMemoria(dadosPrograma dados) {
    
    for (int i = 0; i < dados.categoriasCadastradasQuantidade; i++) {
        free(dados.matrizItens[i]);
    }
    
    free(dados.matrizItens);
    free(dados.listaCategorias);

}

void verificarAlocacaoMemoria(void *ponteiro, dadosPrograma dados) {
    if (ponteiro == NULL) {
        puts("Erro na alocação de memória.\n");
        liberarMemoria(dados);
        exit(EXIT_FAILURE);
    }
}

void tratandoEntradaScanf(int retorno, int *variavel) {
    while (retorno != 1 || *variavel < 0) {
        printf("Entrada inválida. Digite novamente: ");
        while (getchar() != '\n');
        retorno = scanf("%d", variavel);
    }
    fgetc(stdin);
}

int visualizarCategorias(dadosPrograma dados) {
    int categoriaEscolhida, retorno;
    
    puts("Categorias cadastradas:\n");
    for (int i = 0; i < dados.categoriasCadastradasQuantidade; i++) {
        printf("%d - %s\n", i + 1, dados.listaCategorias[i].categoriasNome);
    }
    printf("%d - Voltar para o menu anterior\n\n", dados.categoriasCadastradasQuantidade + 1);
    printf("Digite o número da categoria para escolhe-la: ");

    while(1) {
        retorno = scanf("%d", &categoriaEscolhida);
        tratandoEntradaScanf(retorno, &categoriaEscolhida);
        if (categoriaEscolhida >= 1 && categoriaEscolhida <= dados.categoriasCadastradasQuantidade + 1) {
            categoriaEscolhida -= 1;
            return categoriaEscolhida;
        }
        
        printf("Entrada inválida. Digite novamente: ");
    }
}

int visualizaItens(dadosPrograma dados, int categoriaEscolhida) {
    int itemEscolhido, retorno;
    
    puts("Itens cadastrados:\n");
    for (int i = 0; i < dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade; i++) {
        printf("item %d:\n"
               "Nome: %s\n"
               "Quantidade: %d\n\n",
                i + 1, 
                dados.matrizItens[categoriaEscolhida][i].itemNome,
                dados.matrizItens[categoriaEscolhida][i].itemQuantidade
        );  
    }

    printf("Digite o número do item para escolhe-lo: ");
    while(1) {
        retorno = scanf("%d", &itemEscolhido);
        tratandoEntradaScanf(retorno, &itemEscolhido);
        if (itemEscolhido >= 1 && itemEscolhido <= dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade) {
            itemEscolhido -= 1;
            return itemEscolhido;
        }

        printf("Entrada inválida. Digite novamente: ");
    }
}

dadosPrograma cadastrarCategoria(dadosPrograma dados) {
    limparTela();
    int quantidadeCategoriasNovas, retorno, novoTamanho;

    printf("Digite a quantidade de categorias que serão criadas: ");
    retorno = scanf("%d", &quantidadeCategoriasNovas);
    tratandoEntradaScanf(retorno, &quantidadeCategoriasNovas);

    if (dados.categoriasCadastradasQuantidade == 0) {
        dados.listaCategorias = (categoria*) malloc(quantidadeCategoriasNovas * sizeof(categoria));
        dados.matrizItens = (item**) malloc(quantidadeCategoriasNovas * sizeof(item*));      
    } else {
        novoTamanho = (dados.categoriasCadastradasQuantidade + quantidadeCategoriasNovas);
        dados.listaCategorias = (categoria*) realloc(dados.listaCategorias, novoTamanho * sizeof(categoria));
        dados.matrizItens = (item**) realloc(dados.matrizItens, novoTamanho * sizeof(item*));
    }
    verificarAlocacaoMemoria(dados.listaCategorias, dados);
    verificarAlocacaoMemoria(dados.matrizItens, dados);
    
    for (int i = dados.categoriasCadastradasQuantidade; i < (dados.categoriasCadastradasQuantidade + quantidadeCategoriasNovas); i++) {
        printf("Digite o nome da %dº categoria: ", i + 1);
        fgets(dados.listaCategorias[i].categoriasNome, 51, stdin);
        dados.listaCategorias[i].categoriasNome[strlen(dados.listaCategorias[i].categoriasNome) - 1] = 0;
        dados.listaCategorias[i].itensCategoriasQuantidade = 0;   
    }
    dados.categoriasCadastradasQuantidade += quantidadeCategoriasNovas;

    limparTela();
    puts("Categoria(s) cadastrada(s).\n");
    return dados;
}

dadosPrograma cadastrarItens(dadosPrograma dados) {  
    if (dados.categoriasCadastradasQuantidade != 0) {
        limparTela();
        int categoriaEscolhida, quantidadeProdutosNovos, retorno, novoTamanho;

        categoriaEscolhida = visualizarCategorias(dados);
        if(categoriaEscolhida == (dados.categoriasCadastradasQuantidade)) {
            limparTela();
            return dados;
        }
   
        limparTela();
        printf("Digite a quantidade de itens que serão adicionados: ");
        retorno = scanf("%d", &quantidadeProdutosNovos);
        tratandoEntradaScanf(retorno, &quantidadeProdutosNovos);

        if (dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade == 0) {
            dados.matrizItens[categoriaEscolhida] = (item*) malloc(quantidadeProdutosNovos * sizeof(item));
        } else {
            novoTamanho = (dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade + quantidadeProdutosNovos);
            dados.matrizItens[categoriaEscolhida] = (item*) realloc(dados.matrizItens[categoriaEscolhida], novoTamanho * sizeof(item));
        }
        verificarAlocacaoMemoria(dados.matrizItens[categoriaEscolhida], dados);

        for (int j = dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade; j < (dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade + quantidadeProdutosNovos); j++) {
            printf("Digite o nome para o %dº item: ", j + 1);
            fgets(dados.matrizItens[categoriaEscolhida][j].itemNome, 51, stdin);
            dados.matrizItens[categoriaEscolhida][j].itemNome[strlen(dados.matrizItens[categoriaEscolhida][j].itemNome) - 1] = 0;

            printf("Digite a quantidade para o %dº item: ", j + 1);
            retorno = scanf("%d", &(dados.matrizItens[categoriaEscolhida][j].itemQuantidade));
            tratandoEntradaScanf(retorno, &(dados.matrizItens[categoriaEscolhida][j].itemQuantidade));
            
        }   
        dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade += quantidadeProdutosNovos;

        limparTela();
        puts("Item(s) cadastrado(s).\n");
        return dados;
    }
    limparTela();
    puts("Nenhuma categoria cadastrada ainda.\n");
    return dados;
}

dadosPrograma descadastrarCategorias(dadosPrograma dados) {
    if (dados.categoriasCadastradasQuantidade != 0) {  
        limparTela();
        int categoriaEscolhida = visualizarCategorias(dados);

        if(categoriaEscolhida == (dados.categoriasCadastradasQuantidade)) {
            limparTela();
            return dados;
        }
                        
        if (dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade != 0) {
            free(dados.matrizItens[categoriaEscolhida]);
        }
        dados.matrizItens[categoriaEscolhida] = dados.matrizItens[dados.categoriasCadastradasQuantidade - 1];
        dados.matrizItens = (item**)realloc(dados.matrizItens, (dados.categoriasCadastradasQuantidade - 1) * sizeof(item*));
                            
        dados.listaCategorias[categoriaEscolhida] = dados.listaCategorias[dados.categoriasCadastradasQuantidade - 1];
        dados.listaCategorias = (categoria*)realloc(dados.listaCategorias, (dados.categoriasCadastradasQuantidade - 1) * sizeof(categoria));
        dados.categoriasCadastradasQuantidade -= 1;
                            
        limparTela();
        puts("Categoria descadastrada.\n");
        return dados;             
    }
    limparTela();
    puts("Nenhuma categoria cadastrada ainda.\n");
    return dados; 
}

dadosPrograma descadastrarItens (dadosPrograma dados) {
    if (dados.categoriasCadastradasQuantidade != 0) {
        int categoriaEscolhida, itemEscolhido;

        limparTela();
        categoriaEscolhida = visualizarCategorias(dados);
        if(categoriaEscolhida == (dados.categoriasCadastradasQuantidade)) {
            limparTela();
            return dados;
        }
        limparTela();
                                
        if (dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade == 0) {
            limparTela();
            puts("Nenhum item foi encontrado nessa categoria.\n");
            return dados;
        }       

        itemEscolhido = visualizaItens(dados, categoriaEscolhida);
        limparTela();

        dados.matrizItens[categoriaEscolhida][itemEscolhido] = dados.matrizItens[categoriaEscolhida][dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade - 1];
        dados.matrizItens[categoriaEscolhida] = (item*)realloc(dados.matrizItens[categoriaEscolhida], (dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade - 1) * sizeof(item));
        dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade -= 1;
        
        limparTela();
        puts("Item(s) descadastrado(s).\n");
        return dados;
    }
    limparTela();
    puts("Nenhuma categoria cadastrada ainda.\n");
    return dados; 
}

dadosPrograma adicionarRetirarContagemItens(dadosPrograma dados) {
    if(dados.categoriasCadastradasQuantidade != 0) {
        limparTela();
        int opcaoMenu, retorno, categoriaEscolhida, itemEscolhido, contagem;

        while (1) {
            puts("1 - Adicionar uma contagem de itens\n"
                 "2 - Retirar uma contagem de itens\n"
                 "3 - Voltar para o menu anterior\n"
            );

            printf("Digite o número da categoria para escolhe-la: ");
            retorno = scanf("%d", &opcaoMenu);
            tratandoEntradaScanf(retorno, &opcaoMenu);
            limparTela();

            switch (opcaoMenu) {
                case 1:
                    categoriaEscolhida = visualizarCategorias(dados);
                    if(categoriaEscolhida == (dados.categoriasCadastradasQuantidade)) {
                        limparTela();
                        break;
                    }
                    
                    if (dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade != 0) {
                        limparTela();
                        itemEscolhido = visualizaItens(dados, categoriaEscolhida);
                        printf("Digite a contagem que você quer adicionar em %s: ", dados.matrizItens[categoriaEscolhida][itemEscolhido].itemNome);
                        retorno = scanf("%d", &contagem);
                        tratandoEntradaScanf(retorno, &contagem);

                        dados.matrizItens[categoriaEscolhida][itemEscolhido].itemQuantidade += contagem;
                        
                        limparTela();
                        puts("Contagem adicionada.\n");
                        break;
                    }
                    limparTela();
                    puts("Nenhum Item adicionado nessa categoria.\n");
                break;

                case 2:
                    categoriaEscolhida = visualizarCategorias(dados);
                    if(categoriaEscolhida == (dados.categoriasCadastradasQuantidade)) {
                        limparTela();
                        break;
                    }
                    
                    if (dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade != 0) {
                        limparTela();
                        itemEscolhido = visualizaItens(dados, categoriaEscolhida);
                        printf("Digite a contagem que você quer retirar em %s: ", dados.matrizItens[categoriaEscolhida][itemEscolhido].itemNome);
                        retorno = scanf("%d", &contagem);
                        tratandoEntradaScanf(retorno, &contagem);

                        dados.matrizItens[categoriaEscolhida][itemEscolhido].itemQuantidade -= contagem;
                        if (dados.matrizItens[categoriaEscolhida][itemEscolhido].itemQuantidade < 0) {
                            limparTela();
                            dados.matrizItens[categoriaEscolhida][itemEscolhido].itemQuantidade += contagem;
                            puts("Não foi possível completar a ação. Houve uma tentativa de retirar mais itens do que está registrado.\n");
                            break;
                        } 
                        
                        limparTela();
                        puts("Contagem retirada.\n");
                        break;
                    }
                    limparTela();
                    puts("Nenhum Item adicionado nessa categoria.\n");
                break;

                case 3:
                    limparTela();
                    return dados;
                break;

                default:
                break;
            }
        }
    }
    limparTela();
    puts("Nenhuma categoria cadastrada ainda.\n");
    return dados;
}

void visualizarCategoriasItens(dadosPrograma dados) {
    if (dados.categoriasCadastradasQuantidade != 0) {
        while (1) {
            int categoriaEscolhida = visualizarCategorias(dados);
            if(categoriaEscolhida == (dados.categoriasCadastradasQuantidade)) {
                limparTela();
                return;
            }
            limparTela();

            if (dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade != 0) {
                puts("Itens cadastrados:\n");
                for (int i = 0; i < dados.listaCategorias[categoriaEscolhida].itensCategoriasQuantidade; i++) {
                    printf("Item %d:\n"
                           "Nome: %s\n"
                           "Quantidade: %d\n\n",
                            i + 1, 
                            dados.matrizItens[categoriaEscolhida][i].itemNome,
                            dados.matrizItens[categoriaEscolhida][i].itemQuantidade
                    );  
                }
                system("pause");
                limparTela();
            } else {
                puts("Essa categoria não possui itens ainda.\n");
            }
        }
    }
    limparTela();
    puts("Nenhuma categoria cadastrada ainda.\n");
    return;
}

dadosPrograma dados;

int main() {
    int loop1 = 1, opcoesMenu1, retorno;

    while(loop1) {
        int loop2 = 1, opcoesMenu2;

        puts("1 - Cadastrar\\descadastrar uma categoria\\item\n"
             "2 - Adicionar\\retirar contagem de itens\n"
             "3 - Visualizar categorias\\itens\n"
             "4 - Finalizar o programa\n"
        );
        
        printf("Escolha uma opção: ");
        retorno = scanf("%d", &opcoesMenu1);
        tratandoEntradaScanf(retorno, &opcoesMenu1);
        limparTela();

        switch (opcoesMenu1) {    
            case 1:
                while (loop2) { 
                    puts("1 - Cadastrar nova categoria\n"
                         "2 - Cadastrar item(s) em uma categoria\n"
                         "3 - Descadastrar uma categoria\n"
                         "4 - Descadastrar um item\n"
                         "5 - Voltar para o menu principal\n"
                    );
                    
                    printf("Escolha uma opção: ");
                    retorno = scanf("%d", &opcoesMenu2);
                    tratandoEntradaScanf(retorno, &opcoesMenu2);

                    switch (opcoesMenu2) {
                        case 1:  
                            dados = cadastrarCategoria(dados);
                        break;

                        case 2:
                            dados = cadastrarItens(dados);
                        break;

                        case 3:
                            dados = descadastrarCategorias(dados);
                        break;

                        case 4:
                            dados = descadastrarItens(dados);
                        break;

                        case 5:
                            limparTela();
                            loop2 = 0;
                            break;
                        break;

                        default:
                            limparTela();
                        break;
                    }
                }
            break;
            
            case 2:
                dados = adicionarRetirarContagemItens(dados);
            break;

            case 3:
                visualizarCategoriasItens(dados);
            break;

            case 4:
                liberarMemoria(dados);
                loop1 = 0;
                break;
            break;

            default:
            break;
        }
    }
    return 0;
}