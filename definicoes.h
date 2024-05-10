/*
 * Ficheiro:  definicoes.h
 * Autor:  Nuno Correia de Matos
 * Descrição: Definições de constantes, enumerados e estruturas de dados.
*/

#ifndef IAED_DEFINICOES_H
#define IAED_DEFINICOES_H

/* Imports gerais */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/* -------------- */

/* Constantes */
#define MAX_INSTRUCAO 65535
#define MAX_DOUBLES 64
#define INVERSO "inverso"

#define MIN_LAT (-90.0)
#define MAX_LAT 90.0
#define MIN_LON (-180.0)
#define MAX_LON 180.0
#define LISTA_CARREIRAS 6
#define NO_MEMORY 1
#define VERDADE 1
#define FALSO 0
#define NAO_ENCONTRADO NULL
#define EXTREMO NULL
#define VAZIO NULL
/* ---------- */

/* Estruturas */
typedef struct ligacao Ligacao;
typedef struct carreira Carreira;
typedef struct paragem Paragem;

typedef struct listaCarreiras {
	Carreira *ini, *fim;
} ListaCarreiras;

typedef struct listaParagens {
	Paragem *ini, *fim;
} ListaParagens;

typedef struct listaLigacoes {
	Ligacao *ini, *fim;
} ListaLigacoes;

struct ligacao {
	Ligacao *prev, *next;
	Paragem *origem, *destino;
	double custo, duracao;
};

struct carreira {
	char *nome;
	ListaLigacoes *ligacoes;
	int num_ligacoes;
	double custo_t, duracao_t;
	Carreira *prev, *next;
};

struct paragem {
	char *nome;
	double latitude, longitude;
	Carreira* *carreiras;
	int num_cars;
	Paragem *prev, *next;
};
/* ---------- */

#endif /* IAED_DEFINICOES_H */
