/*
 * Ficheiro:  project2.c
 * Autor:  Nuno Correia de Matos
 * Descrição: Programa de controlo de carreiras e paragens de autocarros.
*/

/* Imports */
#include "definicoes.h"
#include "gerais.h"
#include "carreiras.h"
#include "paragens.h"
#include "ligacoes.h"
/* ------- */

/* Função decisora de qual das funções principais das carreiras a ser usada */
ListaCarreiras* acoesCarreiras(int comando, ListaCarreiras *_carreiras) {
	switch (comando) {
		case 'c':
			_carreiras = carreiras(_carreiras);
			break;

		case 'r':
			_carreiras = acaoRemoveCarreira(_carreiras);
			break;
	}

	return _carreiras;
}


/* Função decisora de qual das funções principais das paragens a ser usada */
ListaParagens* acoesParagens(int comando, ListaParagens *_paragens) {
	switch (comando) {
		case 'p':
			_paragens = paragens(_paragens);
			break;

		case 'e':
			_paragens = acaoRemoveParagem(_paragens);
			break;
	}

	return _paragens;
}


/* Função decisora de qual das funções principais que não retornam a ser usada */
void voidFunctions(int comando, ListaCarreiras *_carreiras, ListaParagens *_paragens) {
	switch (comando) {
		case 'l':
			ligacoes(_carreiras, _paragens);
			break;

		case 'i':
			nos(_paragens);
			break;

		case 'f':
			listaCarreirasParagem(_paragens, _carreiras);
			break;

		case 'q':
		case 'a':
			apaga(_carreiras, _paragens);
			break;
	}
}


/* Função principal */
int main(void) {

	int comando;

	ListaCarreiras *_carreiras = criaListaCarreiras();
	ListaParagens *_paragens = criaListaParagens();

	do {
		switch (comando = getchar()) {
			case 'c':
			case 'r':
				_carreiras = acoesCarreiras(comando, _carreiras);
				break;

			case 'p':
			case 'e':
				_paragens = acoesParagens(comando, _paragens);
				break;

			case 'f':
			case 'l':
			case 'i':
			case 'a':
			case 'q':
				voidFunctions(comando, _carreiras, _paragens);
				break;

			default:
				/* Ignora */
				break;
		}
	} while (comando != 'q');

	libertaMemoria(_carreiras, _paragens, NULL);
	
	return 0;
}
