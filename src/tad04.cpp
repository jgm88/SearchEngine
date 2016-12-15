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
	
	a.ListarTerminos("corpus_corto/fichero2.txt");
	if(a.BorraDoc("corpus_corto/fichero2.txt"))
		cout << "BORRANDO fichero"<< endl;

	a.ListarTerminos("corpus_corto/fichero2.txt");
}
