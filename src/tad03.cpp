#include <iostream> 
#include <string>
#include <list> 
#include "indexadorHash.h"

using namespace std;

int
main(void)
{
	IndexadorHash a("./StopWordsEspanyol.txt", ". ,:", true, true, "./indicePrueba", 1, false, true);
	a.Indexar("listaFicheros_corto.txt");
	InformacionTermino infTermino;
	a.VaciarIndice();

	if(a.Devuelve("pal1",infTermino))
	{
		cout << "FALLO EN VACIAR INDICE DEL INDEXADOR"<< infTermino<<endl;
	}
	a.ListarTerminos();

	if(a.Indexar("listaFicheros_corto.txt"))
	{
		cout << "TERMINOS INDEXADOS DE NUEVO"<< endl;
		a.ListarTerminos();
	}

	if(a.Actualiza("pal2",infTermino))
	{
		cout << "ACTUALIZA EL TERMINO pal2 "<< "NUEVO VALOR "<<endl;
		cout << infTermino<< endl;
	}

}
