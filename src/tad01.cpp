#include <iostream> 
#include <string>
#include <list> 
#include "indexadorHash.h"

using namespace std;

int
main(void)
{
	IndexadorHash a("./StopWordsEspanyol.txt", ". ,:", true, true, "./indicePrueba", 1, false, true);
	cout << "IndexadorHash a:\n################\n" << a << endl;

	a.Indexar("listaFicheros_corto.txt");

	a.ListarTerminos();

	if(a.Borra("pal2"))
	{
		cout << "SE HA BORRADO LA PALABRA pal2"<<endl;
		a.ListarTerminos();
		cout << endl;
	}else
	{
		cout << "FALLO EN BORRADO LA PALABRA ";
		a.ListarTerminos();
		cout << endl;
	}

	if(a.Indexar("listaFicheros_corto.txt"))
	{
		cout << "SOLO DEBE AÑADIR pal2"<< endl;
		a.ListarTerminos();
		cout << endl;
	}else
	{
		cout << "HA FALLADO EN LA INDEXACION"<< endl;
		a.ListarTerminos();
		cout << endl;	
	}

}