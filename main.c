/*
 * main.c
 *
 *  Created on: 16 de jun de 2020
 *      Author: lucas.rodrigues
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Caixinha {
	struct Caixinha *ant;
	char nome_dependente[70];
	char grau_parentesco[30];
	struct Caixinha *prox;
} Caixinha;

typedef struct ListaDuplaEncadeada {
	Caixinha *inicio;
} ListaDuplaEncadeada;

typedef struct nodo {
	struct nodo *esq;
	char nome[70];
	char empresa[40];
	float valor_plano;
	ListaDuplaEncadeada *dependentes;
	struct nodo *dir;
} tipo_abb;

void criarArvore(tipo_abb **arvore);
void criarLista(ListaDuplaEncadeada *lista);
void inserir(tipo_abb **t, char tNome[70], char tEmp[40], float tValPla);
tipo_abb* pesquisar(tipo_abb **t, char tNome[70]);
void printArvore(tipo_abb **arv_encontrada);
void emOrdem(tipo_abb **No);
void excluirNo(tipo_abb **arvore, char tNome[70]);
tipo_abb* menor_valor(tipo_abb *t);
void inserirDependentes(ListaDuplaEncadeada *dependentes, char tNome_dep[70],
		char tGrau_Pare[30]);
void exibirNaOrdemDependentes(ListaDuplaEncadeada *dependentes);
ListaDuplaEncadeada* inserirDadosLista();
int main() {
	tipo_abb *arv_encontrada;
	tipo_abb *arvore;
	char tNome[70], tEmp[40];
	float tValPla;
	int i;
	criarArvore(&arvore);

	for (i = 0; i < 4; i++) {
		strcpy(tNome, "");
		fflush(stdin);
		printf("\nNome Funcionario: ");
		gets(tNome);
		if (strcmp(tNome, "") == 0)
			gets(tNome);

		printf("\nEmpresa: ");
		gets(tEmp);
		printf("\nValor do plano: ");
		scanf("%f", &tValPla);

		inserir(&arvore, tNome, tEmp, tValPla);

	}

	strcpy(tNome, "");
	fflush(stdin);
	printf("\nDigite um nome para ser procurado: ");
	gets(tNome);
	if (strcmp(tNome, "") == 0)
		gets(tNome);

	arv_encontrada = pesquisar(&arvore, tNome);
	printArvore(&arv_encontrada);

	printf("\n\n ------ Em Ordem --------- \n\n");
	emOrdem(&arvore);

	strcpy(tNome, "");
	fflush(stdin);
	printf("\nDigite um nome para ser removido: ");
	gets(tNome);
	if (strcmp(tNome, "") == 0)
		gets(tNome);

	excluirNo(&arvore, tNome);
	printf("\n\n -------- Depois da remoção -------- \n");
	emOrdem(&arvore);

	return 0;
}
void criarArvore(tipo_abb **arvore) {
	*arvore = NULL;
}
void criarLista(ListaDuplaEncadeada *lista) {
	lista->inicio = NULL;
}
void inserir(tipo_abb **t, char tNome[70], char tEmp[40], float tValPla) {
	if (*t == NULL) {
		*t = (tipo_abb*) malloc(sizeof(tipo_abb));
		(*t)->dependentes = inserirDadosLista();
		strcpy((*t)->nome, tNome);
		strcpy((*t)->empresa, tEmp);
		(*t)->valor_plano = tValPla;
		(*t)->esq = NULL;
		(*t)->dir = NULL;
	} else if (strcmp(tNome, (*t)->nome) > 0)
		inserir(&((*t)->dir), tNome, tEmp, tValPla);
	else
		inserir(&((*t)->esq), tNome, tEmp, tValPla);
}
tipo_abb* pesquisar(tipo_abb **t, char tNome[70]) {
	if (t != NULL) {
		if (strcmp((*t)->nome, tNome) == 0)
			return *t;
		else if (strcmp(tNome, (*t)->nome) > 0)
			pesquisar(&((*t)->dir), tNome);
		else if (strcmp(tNome, (*t)->nome) < 0)
			pesquisar(&((*t)->esq), tNome);
		else
			return NULL;
	}else
		return NULL;
}
void printArvore(tipo_abb **arv_encontrada) {
	if (*arv_encontrada != NULL) {
		printf("\nNome: %s", (*arv_encontrada)->nome);
		printf("\nEmpresa: %s", (*arv_encontrada)->empresa);
		printf("\nValor do plano: %.2f", (*arv_encontrada)->valor_plano);
		printf("\n");
		exibirNaOrdemDependentes(&((*arv_encontrada)->dependentes));
	}
}
void emOrdem(tipo_abb **No) {
	if (*No != NULL) {
		emOrdem(&((*No)->esq));
		puts((*No)->nome);
		puts((*No)->empresa);
		printf("%.2f \n", (*No)->valor_plano);
		exibirNaOrdemDependentes(&((*No)->dependentes));
		emOrdem(&((*No)->dir));
	}
}
void excluirNo(tipo_abb **arvore, char tNome[70]) {
	tipo_abb *aux = (tipo_abb*) malloc(sizeof(tipo_abb)); // variável para auxiliar a busca pelo menor valor da subárvore direita
	if (strcmp((*arvore)->nome, "") == 0)
		return;

	if (*arvore != NULL) {
		if (strcmp(tNome, (*arvore)->nome) > 0)
			excluirNo(&((*arvore)->dir), tNome);
		else if (strcmp(tNome, (*arvore)->nome) < 0)
			excluirNo(&((*arvore)->esq), tNome);
		else if (((*arvore)->esq != NULL) && ((*arvore)->dir != NULL)) {
			aux = menor_valor((*arvore)->dir); // variavel aux recebe o endereço do menor valor na subárvore direita
			strcpy((*arvore)->nome, (aux->nome)); // o nó a ser retirado recebe o valor do nó que contem o menor valor da subárvore direita
			strcpy((*arvore)->empresa, (aux->empresa));
			(*arvore)->valor_plano = (aux->valor_plano);
			(*arvore)->dependentes = (aux->dependentes);
			excluirNo(&(*arvore)->dir, (*arvore)->nome);
		} else { // se o nó só tem um filho ou nenhum
			aux = *arvore; // variável aux aponta para o nó a ser retirado
			if ((*arvore)->esq == NULL) // se o nó não tem filho esquerdo
				*arvore = (*arvore)->dir; // o filho direito assume o lugar do pai
			else
				// se o nó nó tem filho direito
				*arvore = (*arvore)->esq; // o filho esquerdo assume o lugar do pai

			free(aux->dependentes);
			free(aux); // libera o nó desejado
			printf("Valor retirado da arvore.");
		}
	} else
		printf("Não existe esse valor na arvore");

}

tipo_abb* menor_valor(tipo_abb *t) { // t aponta para o nó raiz da árvore ou raiz da subárvore
	if (t == NULL) // se a árvore estiver vazia
		return NULL;
	else if (t->esq == NULL) // se o nó não tem filho esquerdo, este é o menor valor da subárvore direita
		return t; // retorna o endereço do nó que contem o menor valor da subárvore direita
	else
		// se o nó tem filho esquerdo
		return menor_valor(t->esq); // chamada recursiva para continuar a procura do menor valor
}

void inserirDependentes(ListaDuplaEncadeada *dependentes, char tNome_dep[70],
		char tGrau_Pare[30]) {
	Caixinha *nova;
	nova = (Caixinha*) malloc(sizeof(Caixinha));
	strcpy(nova->nome_dependente, tNome_dep);
	strcpy(nova->grau_parentesco, tGrau_Pare);

	if (dependentes->inicio == NULL) {
		nova->ant = nova;
		nova->prox = nova;
		dependentes->inicio = nova;
	} else {
		Caixinha *aux;
		aux = dependentes->inicio; //aponta para o inicio da lista
		while (aux->prox != dependentes->inicio) //pecorre ate achar o ultimo elemento
			aux = aux->prox;

		nova->ant = aux; //aux agora sera o anterior
		nova->prox = dependentes->inicio; //o proximo do ultimo elemento sera inicio
		aux->prox = nova; //o ultimo elemento agora sera nova
		dependentes->inicio->ant = nova; //caso não esteja vazia o anterior do primeiro elemento, aponta para o ultimo
	}
}
void exibirNaOrdemDependentes(ListaDuplaEncadeada *dependentes) {
	Caixinha *aux;
	aux = dependentes->inicio; //aponta para o inicio da lista
	if (aux != NULL) {
		do {
			if (aux->nome_dependente != NULL)
				printf("Nome do dependente:  %s , ", aux->nome_dependente);
			if (aux->grau_parentesco != NULL)
				printf("Grau de parentesco: %s \n", aux->grau_parentesco);
			aux = aux->prox;
		} while (aux != dependentes->inicio);
	}
}
ListaDuplaEncadeada* inserirDadosLista() {
	int j, rsp;
	char tNome_dep[70], tGrau_par[30];
	ListaDuplaEncadeada *lista_dep;
	criarLista(&lista_dep);
	printf("\nTem dependentes [1- SIM | 0 - NÃO]: ");
	scanf("%d", &rsp);
	if (rsp == 1) {
		int size, j;
		printf("\nQuantos? ");
		scanf("%d", &size);
		for (j = 0; j < size; j++) {
			strcpy(tNome_dep, "");
			strcpy(tGrau_par, "");
			fflush(stdin);
			printf("\nNome do dependente: ");
			gets(tNome_dep);
			if (strcmp(tNome_dep, "") == 0)
				gets(tNome_dep);

			printf("\nGrau de parentesco: ");
			gets(tGrau_par);
			if (strcmp(tGrau_par, "") == 0)
				gets(tGrau_par);

			inserirDependentes(&lista_dep, tNome_dep, tGrau_par);
		}
	}
	return lista_dep;
}
