/*
 * Ficheiro:  carreiras.h
 * Autor:  Nuno Correia de Matos
 * Descrição: Protótipos das funções referentes a carreiras.
*/

#ifndef IAED_CARREIRAS_H
#define IAED_CARREIRAS_H

#include "definicoes.h"
#include "gerais.h"

ListaCarreiras* carreiras(ListaCarreiras *_carreiras);

void listaCarreiras(Carreira *_carreiras);

void listaCarreira(Carreira *_carreira, int inverso);

ListaCarreiras* criaCarreira(ListaCarreiras *_carreiras, char *nome_carreira);

Carreira* procuraCarreira(Carreira *_carreiras_ini, char *nome_carreira);

ListaCarreiras* removeCarreiraIni(ListaCarreiras *_carreiras);

ListaCarreiras* removeCarreiraFim(ListaCarreiras *_carreiras);

void removeCarreiraMeio(Carreira *carreira);

void removeCarreiraParagem(Carreira *carreira, Paragem *paragem);

void removeCarreiraParagens(Carreira *carreira);

void anulaCarreira(Carreira *carreira);

ListaCarreiras* removeCarreira(ListaCarreiras *_carreiras, char *nome_carreira);

ListaCarreiras* acaoRemoveCarreira(ListaCarreiras *_carreiras);

#endif /* IAED_CARREIRAS_H */
