/*
 * Ficheiro:  paragens.h
 * Autor:  Nuno Correia de Matos
 * Descrição: Protótipos das funções referentes a paragens.
*/

#ifndef IAED_PARAGENS_H
#define IAED_PARAGENS_H

#include "definicoes.h"
#include "ligacoes.h"
#include "gerais.h"

ListaParagens* paragens(ListaParagens *_paragens);

void listaParagens(Paragem *_paragens_ini);

void listaParagem(Paragem *paragem, char *nome_paragem);

ListaParagens* criaParagem(ListaParagens *_paragens, char *nome_paragem,
                           double latitude, double longitude);

Paragem* procuraParagem(Paragem *_paragens_ini, char *nome_paragem);

int encontraParagemCarreira(Paragem *paragem, char *nome_carreira);

ListaParagens* removeParagemIni(ListaParagens *_paragens);

ListaParagens* removeParagemFim(ListaParagens *_paragens);

void removeParagemMeio(Paragem *paragem);

void removeParagemCarreira(Paragem *paragem);

void listaCarreirasParagem(ListaParagens *_paragens, ListaCarreiras *_carreiras);

ListaParagens* removeParagem(ListaParagens *_paragens, char *nome_paragem);

ListaParagens* acaoRemoveParagem(ListaParagens *_paragens);

#endif /* IAED_PARAGENS_H */
