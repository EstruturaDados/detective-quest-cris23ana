// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

//--CODIGO
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SALAS 20 //--numero maximo de salas visitadas

//--definindo a estrutura e criação dos nós (sala) e (pista)

/*Começamos com a definição da sala. Ela conterá uma string nome (com até 49 caracteres + \0)
e dois ponteiros: um para a sala à esquerda e outro para o que está à direita. Além disso,
conterá uma string pista associada a sala.*/
typedef struct Sala {
    char nomeSala[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

//Em seguida, criamos o nó para as pistas.
typedef struct Pista{
    char pista[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

//---limpar buffer de entrada para evitar problema de leitura consecutiva de scanf e getchar
void limparBuffer(){
    int buffer;
    while ((buffer = getchar()) != '\n' && buffer != EOF);
}

/*Vamos definir, de modo manual, nossa árvore (mansão). Então, não faremos a função inserir.
Desse modo, basta criamos uma função para alocar um novo nó (sala) na memória, criando dinamicamente uma
sala com nome e pista (opcional)*/

Sala *criarSala(char* nomeSala, char *pista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));

    //--confere se a alocacao funcionou
    if (novaSala == NULL){
        printf("Erro: Falha ao alocar memoria.\n");
    }

    strcpy(novaSala->nomeSala, nomeSala);
    strcpy(novaSala->pista, pista);

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/*Criando um nó para as pistas
Sempre que quisermos adicionar um novo valor à árvore, precisaremos criar um nó com esse valor.
Essa função aloca memória e inicializa os ponteiros como NULL, já que novos nós começam sem filhos.*/
Pista *criarPista(char* pista) {
    Pista *novaPista = (Pista*)malloc(sizeof(Pista));

    //--confere se a alocacao funcionou
    if (novaPista == NULL){
        printf("Erro: Falha ao alocar memoria.\n");
    }
    strcpy(novaPista->pista, pista);
    novaPista->esquerda = NULL;
    novaPista->direita = NULL;
    return novaPista;
}

/*Inserindo elementos na árvore pista
O passo agora é inserir os nós seguindo a regra da BST: valores menores vão à esquerda, maiores à direita:*/
Pista *inserirPista(Pista *raiz, char *pista) {
    if (raiz == NULL){ 
        return criarPista(pista);
    }
    
    if (strcmp(pista, raiz->pista) < 0){
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0){
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    
    return raiz;
}

//Exibindo pistas em ordem alfabetica (in-order)
/*A principal vantagem da BST é poder ser percorrida em ordem crescente com um algoritmo simples.
Esse percurso visita primeiro a esquerda, depois o nó atual, e, por fim, a direita.*/
void exibirPistas(Pista *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("\"%s\"\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

//Liberando a arvore de pistas
void liberarPistas(Pista *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}
//-----Atenção
//Esse passo certifica que cada valor inserido é um novo bloco na árvore, isolado e pronto para ser conectado a outros nós.

/* Criação da função para explorar a mansão a partir de uma sala.*/
void explorarSala(Sala *atual, char *salasVisitadas[], int *contador, Pista **raizPistas){
    
    //--aloca memoria para armazenar o nome da sala visitada atualmente
    //(caso o limite maximo nao tenha sido atingido)
    if (*contador < MAX_SALAS) {
        salasVisitadas[*contador] = malloc(strlen(atual->nomeSala) + 1);
        strcpy(salasVisitadas[*contador], atual->nomeSala); //--copia o nome da sala para a memoria
        (*contador)++;
        //--viabilizar a exploração continua até o jogador alcançar um cômodo que não possua
        //caminhos à esquerda nem à direita (isto é, um nó-folha na árvore).
    }

    printf("\nVoce esta em \"%s\".\n", atual->nomeSala); //--sala atual onde o jogador esta

    //--adicionando pistas na sala
    if (strlen(atual->pista) > 0) {
        printf("Pista: \"%s\"\n", atual->pista);
        *raizPistas = inserirPista(*raizPistas, atual->pista);
    } else {
        printf("Nenhuma pista encontrada aqui.\n");
    }

    if (atual->esquerda == NULL && atual->direita == NULL) {
        printf("\nVoce chegou ao fim da exploracao.\n");
        return;
    }

    char escolha;
    printf("\nEscolha o caminho que deseja seguir:\n");
    if (atual->esquerda != NULL){
        printf("(e) Ir para a esquerda, \"%s\"\n", atual->esquerda->nomeSala);
    }
    if (atual->direita != NULL){
        printf("(d) Ir para a direita, \"%s\"\n", atual->direita->nomeSala);
    }
    printf("(s) Sair\n");
    printf("Opcao: ");
    scanf("%c", &escolha);
    limparBuffer(); //--limpa o buffer

    //--direcoes onde o jogador ira a partir da escolha feita
    if (escolha == 'e' && atual->esquerda != NULL) {
        explorarSala(atual->esquerda, salasVisitadas, contador, raizPistas);
    } else if (escolha == 'd' && atual->direita != NULL) {
        explorarSala(atual->direita, salasVisitadas, contador, raizPistas);
    } else if (escolha == 's') {
        printf("Saindo...\n");
        return;
    } else {
        printf("Opcao invalida. Tente novamente.\n");
        explorarSala(atual, salasVisitadas, contador, raizPistas);
    }
}

//--é preciso liberar a memória ocupada pela árvore após o uso, evitando vazamentos:
void liberarMansao(struct Sala* raiz){
    if (raiz != NULL){
        liberarMansao(raiz->esquerda);
        liberarMansao(raiz->direita);
        free(raiz);
    }
}

int main(){
    //--criando as salas manualmente
    Sala *hall = criarSala("Hall de Entrada", "Pegadas que levam a Cozinha");
    Sala *salaEstar = criarSala("Sala de Estar", "Vaso quebrado");
    Sala *cozinha = criarSala("Cozinha", "Faca faltando no faqueiro");
    Sala *biblioteca = criarSala("Biblioteca", "Livro de Quimica faltando da estante");
    Sala *lavanderia = criarSala("Lavanderia", "Sem pistas");
    Sala *quartoC = criarSala("Quarto Casal", "Sem pistas");
    Sala *quartoS = criarSala("Quarto Solteiro", "Manchas vermelhas no carpete");
    Sala *banheiro = criarSala("Banheiro", "Parece que foi limpo recentemente");

    //--ramificacao da mansao
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = quartoC;

    biblioteca->esquerda = lavanderia;

    cozinha->esquerda = quartoS;
    cozinha->direita = banheiro;

    //--vetor para armazenar o caminho percorrido
    char *salasVisitadas[MAX_SALAS];
    int contador = 0;

    //--pistas
    Pista *arvorePistas = NULL;

    //--exploracao usando recursividade
    printf("=================================================\n");
    printf("             Inicio da exploracao!\n");
    printf("=================================================\n");
    explorarSala(hall, salasVisitadas, &contador, &arvorePistas);

    //--exibir caminho percorrido
    printf("=================================================\n");
    printf("              Caminho percorrido\n");
    printf("=================================================\n");
    for (int i = 0; i < contador; i++) {
        printf("=> %s\n", salasVisitadas[i]);
        free(salasVisitadas[i]);  //--libera memoria de cada nome
    }

    //--exibir pistas em ordem alfabetica
    printf("=================================================\n");
    printf("     Pistas encontradas (em ordem alfabetica)\n");
    printf("=================================================\n");

    exibirPistas(arvorePistas);

    //-liberacao da memoria das salas
    liberarMansao(hall);
    liberarPistas(arvorePistas);

    return 0;
}

