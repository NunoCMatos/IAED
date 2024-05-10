/*
 * Ficheiro:  ligacoes.h
 * Autor:  Nuno Correia de Matos
 * Descrição: Protótipos das funções referentes a ligações.
*/

#ifndef IAED_LIGACOES_H
#define IAED_LIGACOES_H

#include "definicoes.h"
#include "gerais.h"
#include "carreiras.h"
#include "paragens.h"

void ligacoes(ListaCarreiras *_carreiras, ListaParagens *_paragens);

void leNomesComando(char *nome_carreira, char *nome_origem, char *nome_destino);

void adicionaLigacao(Carreira *carreira, Paragem *origem, Paragem *destino,
					 double custo, double duracao);

Ligacao* criaLigacao(Carreira *carreira, Paragem *origem, Paragem *destino,
					 double custo, double duracao);

ListaLigacoes* adicionaPrimeiraLig(ListaLigacoes *ligacoes, Ligacao *ligacao);

Ligacao* adicionaLigacaoIni(ListaLigacoes *ligacoes, Ligacao *ligacao);

Ligacao* adicionaLigacaoFim(ListaLigacoes *ligacoes, Ligacao *ligacao);

void adicionaCarreiraParagem(Paragem *paragem, Carreira *carreira);

Ligacao* removeLigacaoIni(Carreira *carreira);

Ligacao* removeLigacaoFim(Carreira *carreira);

Ligacao* removeLigacao(Carreira *carreira, Ligacao *ligacao);

void nos(ListaParagens *_paragens);

#endif /* IAED_LIGACOES_H */
