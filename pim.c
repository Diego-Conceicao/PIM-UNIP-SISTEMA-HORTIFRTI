#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_PRODUTOS 100
#define MAX_FORNECEDORES 50
#define TAM_NOME 50

// Estruturas
typedef struct {
    int id;
    char nome[TAM_NOME];
    float preco;
    float quantidade;
} Produto;

typedef struct {
    int id;
    char nome[TAM_NOME];
    char cnpj[20];
    char produtosFornecidos[TAM_NOME];
} Fornecedor;

// Variáveis globais
Produto produtos[MAX_PRODUTOS];
Fornecedor fornecedores[MAX_FORNECEDORES];
int totalProdutos = 0;
int totalFornecedores = 0;
float caixa = 0.0;

// Funções de persistência
void salvarDados(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Erro ao abrir arquivo para salvar dados");
        return;
    }

    // Grava dados do caixa e totais
    fwrite(&caixa, sizeof(float), 1, file);
    fwrite(&totalProdutos, sizeof(int), 1, file);
    fwrite(produtos, sizeof(Produto), totalProdutos, file);
    fwrite(&totalFornecedores, sizeof(int), 1, file);
    fwrite(fornecedores, sizeof(Fornecedor), totalFornecedores, file);

    fclose(file);
    printf("Dados salvos com sucesso!\n");
}

void carregarDados(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        // Arquivo não existe, começando do zero
        printf("Nenhum dado encontrado. Iniciando novo sistema.\n");
        return;
    }

    // Lê dados do caixa e totais
    fread(&caixa, sizeof(float), 1, file);
    fread(&totalProdutos, sizeof(int), 1, file);
    fread(produtos, sizeof(Produto), totalProdutos, file);
    fread(&totalFornecedores, sizeof(int), 1, file);
    fread(fornecedores, sizeof(Fornecedor), totalFornecedores, file);

    fclose(file);
    printf("Dados carregados com sucesso!\n");
}

// Funções utilitárias
int idProdutoExiste(int id) {
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].id == id) {
            return i;
        }
    }
    return -1;
}

int idFornecedorExiste(int id) {
    for (int i = 0; i < totalFornecedores; i++) {
        if (fornecedores[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Módulo de Produtos
void adicionarProduto() {
    if (totalProdutos >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido!\n");
        return;
    }

    Produto p;

    do {
        printf("ID do produto (único): ");
        scanf("%d", &p.id);

        if (idProdutoExiste(p.id) != -1) {
            printf("Erro: Este ID já está em uso. Tente outro.\n");
        }
    } while (idProdutoExiste(p.id) != -1);

    printf("Nome do produto: ");
    scanf(" %[^\n]", p.nome);
    printf("Preço do produto: ");
    scanf("%f", &p.preco);
    printf("Quantidade em KG do produto: ");
    scanf("%f", &p.quantidade);

    produtos[totalProdutos++] = p;
    printf("Produto cadastrado com sucesso!\n\n");
}

void listarProdutos() {
    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("Lista de Produtos:\n");
    for (int i = 0; i < totalProdutos; i++) {
        printf("ID: %d | Nome: %s | Preço: R$%.2f | Quantidade: %2.f\n",
               produtos[i].id, produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }
    printf("\n");
}

void ajustarEstoque() {
    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado para ajustar o estoque.\n");
        return;
    }

    int id,  opcao;
    float quantidade;
    printf("Digite o ID do produto para ajustar o estoque: ");
    scanf("%d", &id);

    int indice = idProdutoExiste(id);
    if (indice == -1) {
        printf("Produto não encontrado.\n");
        return;
    }

    printf("Produto selecionado: %s | Estoque atual: %2.f\n", produtos[indice].nome, produtos[indice].quantidade);
    printf("1. Adicionar ao estoque\n2. Remover do estoque\nEscolha uma opção: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        printf("Quantidade a adicionar: ");
        scanf("%f", &quantidade);
        produtos[indice].quantidade += quantidade;
        printf("Estoque atualizado: %2.f\n", produtos[indice].quantidade);
    } else if (opcao == 2) {
        printf("Quantidade a remover: ");
        scanf("%f", &quantidade);
        if (quantidade > produtos[indice].quantidade) {
            printf("Erro: Quantidade insuficiente.\n");
        } else {
            produtos[indice].quantidade -= quantidade;
            printf("Estoque atualizado: %2.f\n", produtos[indice].quantidade);
        }
    } else {
        printf("Opção inválida.\n");
    }
}

// Módulo de Fornecedores
void adicionarFornecedor() {
    if (totalFornecedores >= MAX_FORNECEDORES) {
        printf("Limite de fornecedores atingido!\n");
        return;
    }

    Fornecedor f;

    do {
        printf("ID do fornecedor (único): ");
        scanf("%d", &f.id);

        if (idFornecedorExiste(f.id) != -1) {
            printf("Erro: Este ID já está em uso. Tente outro.\n");
        }
    } while (idFornecedorExiste(f.id) != -1);

    printf("Nome do fornecedor: ");
    scanf(" %[^\n]", f.nome);
    printf("CNPJ do fornecedor: ");
    scanf(" %[^\n]", f.cnpj);
    printf("Produtos fornecidos: ");
    scanf(" %[^\n]", f.produtosFornecidos);

    fornecedores[totalFornecedores++] = f;
    printf("Fornecedor cadastrado com sucesso!\n\n");
}

void listarFornecedores() {
    if (totalFornecedores == 0) {
        printf("Nenhum fornecedor cadastrado.\n");
        return;
    }

    printf("Lista de Fornecedores:\n");
    for (int i = 0; i < totalFornecedores; i++) {
        printf("ID: %d | Nome: %s | CNPJ: %s | Produtos Fornecidos: %s\n",
               fornecedores[i].id, fornecedores[i].nome, fornecedores[i].cnpj, fornecedores[i].produtosFornecidos);
    }
    printf("\n");
}

// Módulo de Caixa
void realizarVenda() {
    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado para realizar vendas.\n");
        return;
    }

    int id ;
    float quantidade;
    printf("Digite o ID do produto vendido: ");
    scanf("%d", &id);

    int indice = idProdutoExiste(id);
    if (indice == -1) {
        printf("Produto não encontrado.\n");
        return;
    }

    printf("Produto selecionado: %s | Estoque atual: %2.f | Preço unitário: R$%.2f\n",
           produtos[indice].nome, produtos[indice].quantidade, produtos[indice].preco);
    printf("Quantidade vendida: ");
    scanf("%f", &quantidade);

    if (quantidade > produtos[indice].quantidade) {
        printf("Erro: Estoque insuficiente.\n");
    } else {
        produtos[indice].quantidade -= quantidade;
        float totalVenda = quantidade * produtos[indice].preco;
        caixa += totalVenda;
        printf("Venda realizada! Total arrecadado: R$%.2f\n", totalVenda);
    }
}

void exibirCaixa() {
    printf("Saldo atual do caixa: R$%.2f\n", caixa);
}

// Menu principal
int main() {
    setlocale(LC_ALL,"");
    const char *filename = "dados.bin";
    carregarDados(filename);
    int opcao;

    do {
        printf("=== Sistema de Gerenciamento ===\n");
        printf("1. Adicionar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Ajustar Estoque\n");
        printf("4. Adicionar Fornecedor\n");
        printf("5. Listar Fornecedores\n");
        printf("6. Realizar Venda\n");
        printf("7. Exibir Caixa\n");
        printf("0. Para Salvar e Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarProduto();
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                ajustarEstoque();
                break;
            case 4:
                adicionarFornecedor();
                break;
            case 5:
                listarFornecedores();
                break;
            case 6:
                realizarVenda();
                break;
            case 7:
                exibirCaixa();
                break;
            case 0:
                salvarDados(filename);
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
