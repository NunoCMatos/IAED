/*
 * Ficheiro:  paragens.c
 * Autor:  Nuno Correia de Matos
 * Descrição: Funções gerais.
*/

#include "gerais.h"


/* Cria a lista das carreiras. */
ListaCarreiras* criaListaCarreiras(void) {

	ListaCarreiras *_carreiras = alocaMemoria(sizeof(ListaCarreiras));
	_carreiras->ini = NULL;
	_carreiras->fim = NULL;

	return _carreiras;
}


/* Cria a lista das paragens. */
ListaParagens* criaListaParagens(void) {

	ListaParagens *_paragens = alocaMemoria(sizeof(ListaParagens));
	_paragens->ini = NULL;
	_paragens->fim = NULL;

	return _paragens;
}


/* Lê espaços. Devolve 1 se chegou ao final de linha ou 0 caso contrário. */
int leEspacos(void) {
	int c;
	while ((c = getchar()) == ' ' || c == '\t');
	if (c == '\n')
		return 1;
	ungetc(c, stdin);
	return 0;
}


/* Lê um nome para uma string e um ponteiro para a string. */
char* leNome(void) {

	char* output = alocaMemoria(sizeof(char)*MAX_INSTRUCAO);
	int i = 0, c;

	leEspacos();
	output[0] = getchar();

	if (output[0] != '"') {
		i = 1;
		while ((c = getchar()) != ' ' && c != '\t' && c != '\n')
			output[i++] = c;
		ungetc(c, stdin);
	}
	else {
		while ((c = getchar()) != '"')
			output[i++] = c;
	}
	output[i++] = '\0';

	return realloc(output, sizeof(char)*i);
}


/* Le todo o texto até ao final de linha e apaga. */
void leAteFinalLinha(void) {
	while (getchar() != '\n');
}


/*
 * Aloca memória e verificar se o fez com sucesso ou não. Caso sem sucesso,
 * imprime que não tem memória e finaliza o programa.
*/
void* alocaMemoria(size_t size) {

	void* aux = malloc(size);

	if (aux == NULL) {
		printf("No memory.");
		exit(NO_MEMORY);
	}

	return aux;
}


/* Apaga todas as carrreiras, paragens e ligações até então criadas. */
void apaga(ListaCarreiras *_carreiras, ListaParagens *_paragens) {
	if (_paragens->ini != VAZIO) {
		Paragem *paragem_aux, *next_par = _paragens->ini;
		do {
			paragem_aux = next_par;
			next_par = paragem_aux->next;

			_paragens = removeParagem(_paragens, paragem_aux->nome);
		} while (next_par != NULL);
	}

	if (_carreiras->ini != VAZIO) {
		Carreira *carreira_aux, *next_car = _carreiras->ini;

		do {
			carreira_aux = next_car;
			next_car = carreira_aux->next;

			_carreiras = removeCarreira(_carreiras, carreira_aux->nome);
		} while (next_car != NULL);
	}

}


/* Liberta memória de qualquer objeto, desde que não seja NULL. */
void libertaMemoria(void *primeiro, void *segundo, void *terceiro) {

	if (primeiro != NULL)
		free(primeiro);

	if (segundo != NULL)
		free(segundo);

	if (terceiro != NULL)
		free(terceiro);
}
