#include <stdio.h>
#include <stdlib.h>
#include <locale.h> // Permite a configuração de localidade (locale) para internacionalização, afetando a formatação de números, datas, etc.
#include <stdbool.h> // Define um tipo booleano, `bool`, e os valores `true` e `false`, facilitando a leitura e escrita de condições lógicas.
#include <string.h> // Fornece funções para manipulação de strings, como cópia, concatenação, comparação e busca de strings.


#define MAX_TRANSACOES 100

// Estrutura para armazenar detalhes de uma transação
typedef struct {
    float valor;
    char data[11]; // Formato DD/MM/AAAA
    char tipoProduto[50];
    int quantidade;
    char clienteFornecedor[100];
} Transacao;

// Arrays globais para armazenar transações de compras e vendas
Transacao compras[MAX_TRANSACOES];
Transacao vendas[MAX_TRANSACOES];
int totalCompras = 0, totalVendas = 0;

// Limpa o buffer de entrada para evitar leituras indesejadas
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Lê um valor float positivo do usuário
bool lerFloatPositivo(float *valor) {
    char linha[256];
    if (fgets(linha, sizeof(linha), stdin)) {
        if (sscanf(linha, "%f", valor) == 1 && *valor > 0) {
            return true;
        }
    }
    printf("Entrada inválida, por favor insira um número positivo.\n");
    return false;
}

// Gera uma nota fiscal para uma transação e salva em um arquivo .txt
void gerarNotaFiscal(Transacao t, const char* tipoTransacao) {
    static int numeroNota = 1; // Contador estático para numerar as notas fiscais
    char nomeArquivo[50];
    sprintf(nomeArquivo, "NotaFiscal_%s_%d.txt", tipoTransacao, numeroNota++);
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Não foi possível criar a nota fiscal.\n");
        return;
    }
    // Escrevendo os detalhes da transação no arquivo
    fprintf(arquivo, "---- Nota Fiscal ----\n");
    fprintf(arquivo, "Tipo: %s\n", tipoTransacao);
    fprintf(arquivo, "Data: %s\n", t.data);
    fprintf(arquivo, "Produto: %s\n", t.tipoProduto);
    fprintf(arquivo, "Quantidade: %d\n", t.quantidade);
    fprintf(arquivo, "Valor: R$%.2f\n", t.valor);
    fprintf(arquivo, "Cliente/Fornecedor: %s\n", t.clienteFornecedor);
    fprintf(arquivo, "---------------------\n");
    fclose(arquivo);
    printf("Nota fiscal %s criada.\n", nomeArquivo);
}

// Solicita ao usuário e coleta dados para preencher uma estrutura Transacao
void coletarDadosTransacao(Transacao *t) {
    // Coleta de dados da transação do usuário
    printf("Informe a data da transação (DD/MM/AAAA): ");
    fgets(t->data, 11, stdin);
    limparBufferEntrada(); // Limpeza necessária após a leitura da data
    printf("Informe o tipo de produto: ");
    fgets(t->tipoProduto, 50, stdin);
    t->tipoProduto[strcspn(t->tipoProduto, "\n")] = 0; // Remove a quebra de linha
    printf("Informe a quantidade: ");
    scanf("%d", &t->quantidade);
    limparBufferEntrada(); // Limpeza do buffer após leitura de um número
    printf("Informe o cliente/fornecedor: ");
    fgets(t->clienteFornecedor, 100, stdin);
    t->clienteFornecedor[strcspn(t->clienteFornecedor, "\n")] = 0; // Remove a quebra de linha
}

// Realiza uma compra, coletando dados e armazenando na lista de compras
void realizarCompra() {
    if (totalCompras < MAX_TRANSACOES) {
        Transacao novaCompra;
        printf("Informe o valor da compra (use duas casas decimais): ");
        while (!lerFloatPositivo(&novaCompra.valor)) {
            printf("Tente novamente: ");
        }
        coletarDadosTransacao(&novaCompra);
        compras[totalCompras++] = novaCompra; // Adiciona a nova compra ao array
        printf("Compra de R$%.2f realizada com sucesso!\n", novaCompra.valor);
        gerarNotaFiscal(novaCompra, "Compra");
    } else {
        printf("Limite de transações de compra alcançado.\n");
    }
}

// Realiza uma venda, similar ao processo de compra
void realizarVenda() {
    if (totalVendas < MAX_TRANSACOES) {
        Transacao novaVenda;
        printf("Informe o valor da venda (use duas casas decimais): ");
        while (!lerFloatPositivo(&novaVenda.valor)) {
            printf("Tente novamente: ");
        }
        coletarDadosTransacao(&novaVenda);
        vendas[totalVendas++] = novaVenda; // Adiciona a nova venda ao array
        printf("Venda de R$%.2f realizada com sucesso!\n", novaVenda.valor);
        gerarNotaFiscal(novaVenda, "Venda");
    } else {
        printf("Limite de transações de venda alcançado.\n");
    }
}

// Exibe um resumo do caixa, detalhando cada compra e venda
void verCaixa() {
    float totalComprasValor = 0, totalVendasValor = 0;
    printf("Transações de Compra:\n");
    for (int i = 0; i < totalCompras; i++) {
        // Loop para listar detalhes de cada compra e somar o total
        printf("- Data: %s, Produto: %s, Quantidade: %d, Fornecedor: %s, Valor: R$ %.2f\n",
               compras[i].data, compras[i].tipoProduto, compras[i].quantidade,
               compras[i].clienteFornecedor, compras[i].valor);
        totalComprasValor += compras[i].valor;
    }
    printf("Transações de Venda:\n");
    for (int i = 0; i < totalVendas; i++) {
        // Loop similar para vendas
        printf("- Data: %s, Produto: %s, Quantidade: %d, Cliente: %s, Valor: R$ %.2f\n",
               vendas[i].data, vendas[i].tipoProduto, vendas[i].quantidade,
               vendas[i].clienteFornecedor, vendas[i].valor);
        totalVendasValor += vendas[i].valor;
    }
    // Exibe os totais de compras, vendas e o saldo do caixa
    printf("Total em Compras: R$ %.2f\n", totalComprasValor);
    printf("Total em Vendas: R$ %.2f\n", totalVendasValor);
    printf("Total em Caixa: R$ %.2f\n", totalVendasValor - totalComprasValor);
}

// Mostra o menu principal do programa
void mostrarMenu() {
    system("clear"); // Use "cls" em sistemas Windows
    printf("SISTEMA DE COMPRAS E CONTROLE DE CAIXA\n");
    printf("======================================\n");
    printf("1. Realizar compra\n");
    printf("2. Realizar venda\n");
    printf("3. Ver caixa\n");
    printf("4. Sair\n");
    printf("======================================\n");
    printf("Escolha uma opção: ");
}

int main() {
    setlocale(LC_ALL, "");
    int opcao;
    do {
        mostrarMenu();
        while (scanf("%d", &opcao) != 1) {
            printf("Opção inválida! Por favor, insira um número.\n");
            limparBufferEntrada();
            mostrarMenu();
        }
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                realizarCompra();
                break;
            case 2:
                realizarVenda();
                break;
            case 3:
                verCaixa();
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        printf("Pressione qualquer tecla para continuar...\n");
        getchar(); // Espera o usuário pressionar uma tecla antes de continuar
    } while (opcao != 4);

    return 0;
}
