/*
 * Ficheiro:  paragens.c
 * Autor:  Nuno Correia de Matos
 * Descrição: Funções referentes a paragens.
*/

#include "paragens.h"


/*
 * Função que lê uma linha do terminal começada pelo comando de paragens e
 * decide a função a ser usada.
*/
ListaParagens* paragens(ListaParagens *_paragens) {

	int fimLinha = leEspacos();

	if (fimLinha)
		listaParagens(_paragens->ini);

	else {
		char *nome_paragem = leNome();
		Paragem *paragem = procuraParagem(_paragens->ini, nome_paragem);
		fimLinha = leEspacos();

		if (fimLinha) {
			listaParagem(paragem, nome_paragem);
			free(nome_paragem);

		} else {
			double latitude, longitude;
			scanf("%lf%lf", &latitude, &longitude);

			if (paragem == NAO_ENCONTRADO)
				if ((latitude >= MIN_LAT && latitude <= MAX_LAT) &&
						(longitude >= MIN_LON && longitude <= MAX_LON))
					_paragens = criaParagem(_paragens, nome_paragem, latitude, longitude);
				else
					printf("invalid location.\n");

			else {
				printf("%s: stop already exists.\n", nome_paragem);
				free(nome_paragem);
			}

			leAteFinalLinha();
		}
	}

	return _paragens;
}


/* Lista todas as paragens */
void listaParagens(Paragem *_paragens_ini) {
	Paragem *aux = _paragens_ini;

	for ( ; aux != NULL; aux = aux->next) {

		printf("%s: %16.12f %16.12f %d\n", aux->nome, aux->latitude,
			   aux->longitude, aux->num_cars);
	}
}

/* Lista as coordenadas de uma paragem */
void listaParagem(Paragem *paragem, char *nome_paragem) {
	if (paragem == NAO_ENCONTRADO)
		printf("%s: no such stop.\n", nome_paragem);
	else
		printf("%16.12f %16.12f\n", paragem->latitude, paragem->longitude);
}


/* Cria uma paragem e inicializa todas as suas entradas. */
ListaParagens* criaParagem(ListaParagens *_paragens, char *nome_paragem,
						   double latitude, double longitude) {

	Paragem *nova_paragem = alocaMemoria(sizeof(Paragem));

	nova_paragem->prev = _paragens->fim;
	nova_paragem->next = NULL;
	if (nova_paragem->prev != NULL)
		nova_paragem->prev->next = nova_paragem;

	nova_paragem->nome = nome_paragem;
	nova_paragem->latitude = latitude;
	nova_paragem->longitude = longitude;

	nova_paragem->carreiras = NULL;
	nova_paragem->num_cars = 0;

	if (_paragens->ini == NULL)
		_paragens->ini = nova_paragem;
	_paragens->fim = nova_paragem;

	return _paragens;
}


/* Procura e devolve uma paragem pelo seu nome, devolve NULL caso não exista. */
Paragem* procuraParagem(Paragem *_paragens_ini, char *nome_paragem) {

	Paragem *aux = _paragens_ini;

	for (; aux != NULL; aux = aux->next)
		if (!strcmp(aux->nome, nome_paragem))
			break;

	return aux;
}


/* Retorna 1 caso uma carreira pare na paragem recebida, 0 caso contrário. */
int encontraParagemCarreira(Paragem *paragem, char *nome_carreira) {

	int i;

	for (i = 0; i < paragem->num_cars; i++) {
		if (!strcmp(paragem->carreiras[i]->nome, nome_carreira))
			return 1;
	}

	return 0;
}


/* Remove uma paragem do meio da lista das paragens. */
ListaParagens* removeParagemIni(ListaParagens *_paragens) {

	Paragem *paragem = _paragens->ini;

	_paragens->ini = paragem->next;
	if (_paragens->ini == NULL) {
		_paragens->fim = NULL;
	} else
		_paragens->ini->prev = NULL;

	return _paragens;
}


/* Remove uma paragem do fim da lista das paragens. */
ListaParagens* removeParagemFim(ListaParagens *_paragens) {

	Paragem *paragem = _paragens->fim;

	_paragens->fim = paragem->prev;
	if (_paragens->fim == NULL) {
		_paragens->ini = NULL;
	} else
		_paragens->fim->next = NULL;

	return _paragens;
}


/* Remove uma paragem do meio da lista das paragens. */
void removeParagemMeio(Paragem *paragem) {

	Paragem *prev = paragem->prev, *next = paragem->next;

	prev->next = next;
	next->prev = prev;
}


/* Remove uma paragem de todas as carreiras que por ela passam. */
void removeParagemCarreira(Paragem *paragem) {

	int i, num_cars = paragem->num_cars;

	for (i = 0; i < num_cars; i++) {

		Carreira *carreira_aux = paragem->carreiras[i];
		Ligacao *ligacao_aux = carreira_aux->ligacoes->ini;

		do {
			if (!strcmp(ligacao_aux->origem->nome, paragem->nome)) {
				carreira_aux->ligacoes->ini = removeLigacaoIni(carreira_aux);
				if ((ligacao_aux = carreira_aux->ligacoes->ini) == VAZIO)
					break;
			} else if (ligacao_aux->next != NULL) {
				if (!strcmp(ligacao_aux->destino->nome, paragem->nome)) {
					carreira_aux->ligacoes->fim = removeLigacao(carreira_aux, ligacao_aux);
					carreira_aux->num_ligacoes--;
				} else
					ligacao_aux = ligacao_aux->next;
			}
		} while (ligacao_aux->next != NULL);

		/*
		 * Caso tenha saído do loop e só restar uma ligação na carreira,
		 * verifica se a paragem também está presente nessa ligação.
		*/
		if (ligacao_aux != NULL) {
			if (!strcmp(ligacao_aux->destino->nome, paragem->nome))
				carreira_aux->ligacoes->fim = removeLigacaoFim(carreira_aux);
			else if (!strcmp(ligacao_aux->origem->nome, paragem->nome))
				carreira_aux->ligacoes->ini = removeLigacaoIni(carreira_aux);
		}

		/*
		 * Caso não existam mais ligações na carreira,
		 * anula as entradas da carreira.
		*/
		if (carreira_aux->ligacoes->ini == NULL
			|| carreira_aux->ligacoes->fim == NULL)
			anulaCarreira(carreira_aux);
	}
}


/*
 * Remove uma paragem da lista de paragens, remove a paragem de todas as
 * carreiras que por ela passam e apaga a paragem.
*/
ListaParagens* removeParagem(ListaParagens *_paragens, char *nome_paragem) {

	Paragem *paragem = procuraParagem(_paragens->ini, nome_paragem);

	if (paragem != NAO_ENCONTRADO) {
		if (paragem->prev == NULL)
			_paragens = removeParagemIni(_paragens);
		else if (paragem->next == NULL)
			_paragens = removeParagemFim(_paragens);
		else
			removeParagemMeio(paragem);

		removeParagemCarreira(paragem);
		libertaMemoria(paragem->nome, paragem->carreiras, paragem);
	} else
		printf("%s: no such stop.\n", nome_paragem);

	return _paragens;
}


/* Função introdutora da ação de remover uma paragem. */
ListaParagens* acaoRemoveParagem(ListaParagens *_paragens) {
	char *nome_paragem = leNome();
	_paragens = removeParagem(_paragens, nome_paragem);

	free(nome_paragem);

	return _paragens;
}


void listaCarreirasParagem(ListaParagens *_paragens, ListaCarreiras *_carreiras) {

	char *nome_paragem = leNome();
	Paragem *paragem = procuraParagem(_paragens->ini, nome_paragem);

	if (paragem != NAO_ENCONTRADO) {

		Carreira *carreira_aux = _carreiras->ini;
		int termina = 0;

		if (carreira_aux->ligacoes->fim != VAZIO) {
			if (!strcmp(carreira_aux->ligacoes->fim->destino->nome, nome_paragem)) {
				printf("%s", carreira_aux->nome);
				termina = 1;
			}

			if (carreira_aux->next != NULL)
				for (carreira_aux = carreira_aux->next ;
					carreira_aux->ligacoes->fim != VAZIO && carreira_aux->next != NULL;
					carreira_aux = carreira_aux->next) {

					if (termina == 0 &&
					    !strcmp(carreira_aux->ligacoes->fim->destino->nome,
					            nome_paragem)) {
						printf("%s", carreira_aux->nome);
						termina = 1;

					} else if (termina == 1 &&
					           !strcmp(carreira_aux->ligacoes->fim->destino->nome,
					                   nome_paragem))
						printf(" %s", carreira_aux->nome);
				}
			if (termina)
				printf("\n");
		}
	} else
		printf("%s: no such stop.\n", nome_paragem);

	free(nome_paragem);
}
