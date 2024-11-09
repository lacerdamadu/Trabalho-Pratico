#include "Minerais.h"
#include <stdio.h>
#include <stdlib.h>
#define InicioArranjo 0
#define MaxTam 3

typedef struct {

 Minerais listaminerais[MaxTam];
 int Primeiro, Ultimo;

} ListaMinerais;

void IniVListaM(ListaMinerais* ListaM); //Inicializa uma lista de minerais vazia

int InsMineral(ListaMinerais* ListaM, Minerais *NovoM); //Insere um mineral ao final da lista

int RetMineral(ListaMinerais* ListaM, int pos, Minerais *MinRet); //Retira o mineiral da lista que está na posição "pos"

void ImprimeListaM(ListaMinerais* ListaM); //Imprime toda a lista de Minerais