/*
 * Ficheiro:  gerais.h
 * Autor:  Nuno Correia de Matos
 * Descrição: Protótipos das funções gerais.
*/

#ifndef IAED_GERAIS_H
#define IAED_GERAIS_H

#include "definicoes.h"
#include "paragens.h"
#include "carreiras.h"

ListaCarreiras* criaListaCarreiras(void);

ListaParagens* criaListaParagens(void);

int leEspacos(void);

char* leNome(void);

void leAteFinalLinha(void);

void* alocaMemoria(size_t size);

void libertaMemoria(void *primeiro, void *segundo, void *terceiro);

void apaga(ListaCarreiras *_carreiras, ListaParagens *_paragens);

#endif /* IAED_GERAIS_H */
