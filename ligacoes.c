/*
 * Ficheiro:  ligacoes.c
 * Autor:  Nuno Correia de Matos
 * Descrição: Funções referentes a ligações.
*/

#include "ligacoes.h"


/*
 * Função que lê uma linha do terminal começada pelo comando de ligações e
 * decide a função a ser usada.
*/
void ligacoes(ListaCarreiras *_carreiras, ListaParagens *_paragens) {

	Carreira *carreira = NULL;
	Paragem *origem = NULL, *destino = NULL;

	char *nome_carreira = leNome(), *nome_origem = leNome(), *nome_destino = leNome();
	double custo, duracao;

	scanf("%lf%lf", &custo, &duracao);
	leAteFinalLinha();

	carreira = procuraCarreira(_carreiras->ini, nome_carreira);
	if (carreira == NAO_ENCONTRADO)
		printf("%s: no such line.\n", nome_carreira);
	else {
		origem = procuraParagem(_paragens->ini, nome_origem);
		if (origem == NAO_ENCONTRADO)
			printf("%s: no such stop.\n", nome_origem);
		else {
			destino = procuraParagem(_paragens->ini, nome_destino);
			if (destino == NAO_ENCONTRADO)
				printf("%s: no such stop.\n", nome_destino);
			else if (custo < 0.0 || duracao < 0.0)
				printf("negative cost or duration.\n");
			else
				adicionaLigacao(carreira, origem, destino, custo, duracao);
		}
	}

	libertaMemoria(nome_carreira, nome_origem, nome_destino);
}


/*
 * Adiciona a ligação a uma carreira e, caso esta já não passe pela paragem,
 * origem ou destino, regista que a carreira passa na paragem, origem ou destino.
*/
void adicionaLigacao(Carreira *carreira, Paragem *origem, Paragem *destino,
                     double custo, double duracao) {

	Ligacao *ligacao;

	if (carreira->num_ligacoes == 0) {
		adicionaCarreiraParagem(origem, carreira);
		if (strcmp(origem->nome, destino->nome))
			adicionaCarreiraParagem(destino, carreira);
		ligacao = criaLigacao(carreira, origem, destino, custo, duracao);
		carreira->num_ligacoes++;
		carreira->ligacoes = adicionaPrimeiraLig(carreira->ligacoes, ligacao);
	} else {
		if (!strcmp(origem->nome, carreira->ligacoes->fim->destino->nome)) {
			if (!encontraParagemCarreira(destino, carreira->nome))
				adicionaCarreiraParagem(destino, carreira);

			ligacao = criaLigacao(carreira, origem, destino, custo, duracao);
			carreira->ligacoes->fim = adicionaLigacaoFim(carreira->ligacoes, ligacao);
		} else if (!strcmp(destino->nome, carreira->ligacoes->ini->origem->nome)) {
			if (!encontraParagemCarreira(origem, carreira->nome))
				adicionaCarreiraParagem(origem, carreira);

			ligacao = criaLigacao(carreira, origem, destino, custo, duracao);
			carreira->ligacoes->ini = adicionaLigacaoIni(carreira->ligacoes, ligacao);
		} else
			printf("link cannot be associated with bus line.\n");
	}
}


/* Cria uma ligação e inicializa todas as suas entradas. */
Ligacao* criaLigacao(Carreira *carreira, Paragem *origem, Paragem *destino,
					 double custo, double duracao) {

	Ligacao *ligacao = alocaMemoria(sizeof(Ligacao));

	ligacao->next = NULL;
	ligacao->prev = NULL;

	ligacao->origem = origem;
	ligacao->destino = destino;

	ligacao->custo = custo;
	ligacao->duracao = duracao;

	carreira->num_ligacoes += 1;
	carreira->custo_t += custo;
	carreira->duracao_t += duracao;

	return ligacao;
}


/* Adiciona a primeira ligação de uma carreira. */
ListaLigacoes* adicionaPrimeiraLig(ListaLigacoes *ligacoes, Ligacao *ligacao) {

	ligacoes->ini = ligacao;
	ligacoes->fim = ligacao;

	return ligacoes;
}


/* Adiciona uma ligação ao início da lista de ligações de uma carreira. */
Ligacao* adicionaLigacaoIni(ListaLigacoes *ligacoes, Ligacao *ligacao) {

	ligacao->next = ligacoes->ini;
	ligacoes->ini->prev = ligacao;

	return ligacoes->ini = ligacao;
}


/* Adiciona uma ligação ao fim da lista de ligações de uma carreira. */
Ligacao* adicionaLigacaoFim(ListaLigacoes *ligacoes, Ligacao *ligacao) {

	ligacao->prev = ligacoes->fim;
	ligacao->prev->next = ligacao;

	return ligacoes->fim = ligacao;
}


/* Regista que a carreira recebida passa pela paragem recebida. */
void adicionaCarreiraParagem(Paragem *paragem, Carreira *carreira) {

	int i, j, compare;
	paragem->carreiras = realloc(paragem->carreiras, 
								sizeof(Carreira*)*(paragem->num_cars + 1));

	for (i = 0; i < paragem->num_cars; i++) {
		compare = strcmp(carreira->nome, paragem->carreiras[i]->nome);
		if (compare < 0) {
			for (j = paragem->num_cars; j > i; j--)
				paragem->carreiras[j] = paragem->carreiras[j - 1];
			break;
		}
	}

	paragem->carreiras[i] = carreira;
	paragem->num_cars++;
}


/* Remove uma ligação do início da lista de ligações de uma carreira. */
Ligacao* removeLigacaoIni(Carreira *carreira) {

	Ligacao *ligacao = carreira->ligacoes->ini;

	carreira->ligacoes->ini = ligacao->next;
	if (carreira->ligacoes->ini != NULL) {
		carreira->custo_t -= ligacao->custo;
		carreira->duracao_t -= ligacao->duracao;
		carreira->num_ligacoes--;
		carreira->ligacoes->ini->prev = NULL;
	} else
		if (strcmp(ligacao->destino->nome, ligacao->origem->nome))
			removeCarreiraParagem(carreira, ligacao->destino);

	free(ligacao);

	return carreira->ligacoes->ini;
}


/* Remove uma ligação do fim da lista de ligações de uma carreira. */
Ligacao* removeLigacaoFim(Carreira *carreira) {

	Ligacao *ligacao = carreira->ligacoes->fim;

	carreira->ligacoes->fim = ligacao->prev;
	if (carreira->ligacoes->fim != NULL) {
		carreira->custo_t -= ligacao->custo;
		carreira->duracao_t -= ligacao->duracao;
		carreira->num_ligacoes--;
		carreira->ligacoes->fim->next = NULL;
	} else
		if (strcmp(ligacao->destino->nome, ligacao->origem->nome))
			removeCarreiraParagem(carreira, ligacao->origem);

	free(ligacao);

	return carreira->ligacoes->fim;
}


/* Remove uma ligação do meio da lista de ligações de uma carreira. */
Ligacao* removeLigacao(Carreira *carreira, Ligacao *ligacao) {

	Ligacao *next = ligacao->next;

	ligacao->destino = next->destino;
	ligacao->custo += next->custo;
	ligacao->duracao += next->duracao;
	ligacao->next = next->next;
	if (next->next != NULL)
		next->next->prev = ligacao;
	else
		carreira->ligacoes->fim = ligacao;

	free(next);

	return carreira->ligacoes->fim;
}


/* Lista todas as paragens na qual passam pelo menos duas carreiras, e lista estas. */
void nos(ListaParagens *_paragens) {

	Paragem *aux = _paragens->ini;
	int i;

	for ( ; aux != NULL; aux = aux->next)
		if (aux->num_cars >= 2) {
			printf("%s %d:", aux->nome, aux->num_cars);
			for (i = 0; i < aux->num_cars; i++)
				printf(" %s", aux->carreiras[i]->nome);
			printf("\n");
		}
}
