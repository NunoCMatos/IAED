/*
 * Ficheiro:  carreiras.c
 * Autor:  Nuno Correia de Matos
 * Descrição: Funções referentes a carreiras.
*/

#include "carreiras.h"


/*
 * Função que lê uma linha do terminal começada pelo comando de carreiras e
 * decide a função a ser usada.
*/
ListaCarreiras* carreiras(ListaCarreiras *_carreiras) {

	int fimLinha = leEspacos();

	if (fimLinha)
		listaCarreiras(_carreiras->ini);

	else {
		char *nome_carreira = leNome();
		Carreira *carreira = procuraCarreira(_carreiras->ini, nome_carreira);
		fimLinha = leEspacos();

		if (carreira == NAO_ENCONTRADO)
			_carreiras = criaCarreira(_carreiras, nome_carreira);

		else {
			free(nome_carreira);

			if (fimLinha)
				listaCarreira(carreira, FALSO);

			else {
				char *_inverso = leNome();
				int chr_inv = strlen(_inverso);

				if (chr_inv >= 3 && !strncmp(INVERSO, _inverso, chr_inv))
					listaCarreira(carreira, VERDADE);
				else
					printf("incorrect sort option.\n");

				free(_inverso);
				leAteFinalLinha();
			}
		}
	}

	return _carreiras;
}


/* Cria uma carreira e inicializa todas as suas entradas. */
ListaCarreiras* criaCarreira(ListaCarreiras *_carreiras, char *nome_carreira) {

	Carreira *nova_carreira = alocaMemoria(sizeof(Carreira));

	nova_carreira->prev = _carreiras->fim;
	nova_carreira->next = NULL;
	if (nova_carreira->prev != NULL)
		nova_carreira->prev->next = nova_carreira;

	nova_carreira->nome = nome_carreira;
	nova_carreira->ligacoes = alocaMemoria(sizeof(ListaLigacoes));
	nova_carreira->ligacoes->ini = NULL;
	nova_carreira->ligacoes->fim = NULL;

	nova_carreira->num_ligacoes = 0;
	nova_carreira->custo_t = 0;
	nova_carreira->duracao_t = 0;

	if (_carreiras->ini == NULL)
		_carreiras->ini = nova_carreira;
	_carreiras->fim = nova_carreira;

	return _carreiras;
}


/* Procura e devolve uma carreira pelo seu nome, devolve NULL caso não exista. */
Carreira* procuraCarreira(Carreira *_carreiras_ini, char *nome_carreira) {

	Carreira *aux = _carreiras_ini;

	for (; aux != NULL; aux = aux->next)
		if (!strcmp(aux->nome, nome_carreira))
			break;

	return aux;
}


/* Lista todas as carreiras. */
void listaCarreiras(Carreira *_carreiras_ini) {

	Carreira *aux = _carreiras_ini;

	for ( ; aux != NULL; aux = aux->next) {

		if (strlen(aux->nome) < LISTA_CARREIRAS) {
			printf("%s ", aux->nome);
			if (aux->ligacoes->ini != NULL) {
				printf("%s %s ", aux->ligacoes->ini->origem->nome,
				       aux->ligacoes->fim->destino->nome);
				printf("%d ", aux->num_ligacoes);
				printf("%.2f %.2f\n", aux->custo_t, aux->duracao_t);

			} else
				printf("%d %.2f %.2f\n", 0, 0.0, 0.0);
		}
	}
}


/* Lista todas as paragens porque uma carreira passa. */
void listaCarreira(Carreira *_carreira, int inverso) {

	Ligacao *aux;

	if (_carreira->ligacoes->ini == NULL)
		return;

	if (inverso == FALSO) {
		aux = _carreira->ligacoes->ini;
		while (aux->next != NULL) {
			printf("%s, ", aux->origem->nome);
			aux = aux->next;
		}
		printf("%s, %s\n", aux->origem->nome, aux->destino->nome);

	} else {
		aux = _carreira->ligacoes->fim;
		while (aux->prev != NULL) {
			printf("%s, ", aux->destino->nome);
			aux = aux->prev;
		}
		printf("%s, %s\n", aux->destino->nome, aux->origem->nome);

	}
}


/* Remove uma carreira do início da lista de carreiras. */
ListaCarreiras* removeCarreiraIni(ListaCarreiras *_carreiras) {

	Carreira *carreira = _carreiras->ini;

	_carreiras->ini = carreira->next;
	if (_carreiras->ini == NULL) {
		_carreiras->fim = NULL;
	} else
		_carreiras->ini->prev = NULL;

	return _carreiras;
}


/* Remove uma carreira do fim da lista de carreiras. */
ListaCarreiras* removeCarreiraFim(ListaCarreiras *_carreiras) {

	Carreira *carreira = _carreiras->fim;

	_carreiras->fim = carreira->prev;
	if (_carreiras->fim == NULL) {
		_carreiras->ini = NULL;
	} else
		_carreiras->fim->next = NULL;

	return _carreiras;
}


/* Remove uma carreira do meio da lista de carreiras. */
void removeCarreiraMeio(Carreira *carreira) {

	Carreira *prev = carreira->prev, *next = carreira->next;

	prev->next = next;
	next->prev = prev;
}


/* Remove uma carreira de uma paragem. */
void removeCarreiraParagem(Carreira *carreira, Paragem *paragem) {

	int i, j;

	for (i = 0; i < paragem->num_cars; i++)
		if (!strcmp(paragem->carreiras[i]->nome, carreira->nome)) {
			paragem->num_cars--;
			for (j = i; j < paragem->num_cars; j++)
				paragem->carreiras[j] = paragem->carreiras[j + 1];
			paragem->carreiras = realloc(paragem->carreiras, 
										sizeof(Carreira*)*(paragem->num_cars));
			break;
		}
}


/* Remove uma carreira de todas as paragens porque a mesma passa. */
void removeCarreiraParagens(Carreira *carreira) {

	if (carreira->ligacoes->ini != VAZIO) {
		Ligacao *ligacao_aux = carreira->ligacoes->ini;
		removeCarreiraParagem(carreira, ligacao_aux->origem);

		for (; ligacao_aux->next != NULL; ligacao_aux = ligacao_aux->next) {
			removeCarreiraParagem(carreira, ligacao_aux->destino);
			if (ligacao_aux->prev != NULL)
				free(ligacao_aux->prev);
		}

		removeCarreiraParagem(carreira, ligacao_aux->destino);
		free(ligacao_aux);
	}
}


/*
 * Remove uma carreira da lista de carreiras, remove a carreira de todas as
 * paragens porque passa e apaga a carreira.
*/
ListaCarreiras* removeCarreira(ListaCarreiras *_carreiras, char *nome_carreira) {

	Carreira *carreira = procuraCarreira(_carreiras->ini, nome_carreira);

	if (carreira != NAO_ENCONTRADO) {
		if (carreira->prev == NULL)
			_carreiras = removeCarreiraIni(_carreiras);
		else if (carreira->next == NULL)
			_carreiras = removeCarreiraFim(_carreiras);
		else
			removeCarreiraMeio(carreira);

		removeCarreiraParagens(carreira);
		libertaMemoria(carreira->nome, carreira->ligacoes, carreira);

	} else
		printf("%s: no such line.\n", nome_carreira);

	return _carreiras;
}


/* Função introdutora da ação de remover uma carreira. */
ListaCarreiras* acaoRemoveCarreira(ListaCarreiras *_carreiras) {
	char *nome_carreira = leNome();

	_carreiras = removeCarreira(_carreiras, nome_carreira);
	free(nome_carreira);

	return _carreiras;
}


/* Anula todas as variáveis referentes a uma carreira. */
void anulaCarreira(Carreira *carreira) {

	carreira->ligacoes->ini = NULL;
	carreira->ligacoes->fim = NULL;
	carreira->duracao_t = 0;
	carreira->num_ligacoes = 0;
	carreira->custo_t = 0;
}
