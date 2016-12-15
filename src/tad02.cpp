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

	a.ImprimirIndexacion();
	if(a.GuardarIndexacion())
	{
		cout << "ALMACENADO EN DISCO "<< endl;
		a.ImprimirIndexacion();
	}else
		cout << "ERROR DE ALMACENAJE"<< endl;

	IndexadorHash b("./indicePrueba");

	if(b.DevolverAlmEnDisco()== a.DevolverAlmEnDisco() && b.DevolverAlmacenarPosTerm() == a.DevolverAlmacenarPosTerm())
		cout << "HA CARGADO PARTE PRIVADA"<< endl;
	else
		cout << "NO CARGADA PARTE PRIVADA"<< endl;
	cout << "DOCS CARGADOS EN a"<< endl;
	a.ListarDocs();
	cout << "DOCS CARGADOS EN b"<< endl;
	b.ListarDocs();



}
