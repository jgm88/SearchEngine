#include <iostream>
#include <string>
#include <list>
#include "indexadorHash.h"
#include "buscador.h"

using namespace std;

int
main() {
	IndexadorHash b("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, false);

//	b.Indexar("./listaFicheros_corto.txt");
	b.IndexarDirectorio("/home/javi/EI/EIPractica3/CorpusTime/Documentos");
//	b.GuardarIndexacion();

	cout <<"INDEXADO"<<endl;

	Buscador a("./indicePrueba", 0);
	string preg;
	double kk1; double kb;

//	a.IndexarPregunta("pal1 pal4");
//
//	if(a.Buscar(2))
//		a.ImprimirResultadoBusqueda(2);
//	a.ImprimirResultadoBusqueda(2, "ficheroDFR.txt");
//
//	a.CambiarFormulaSimilitud(1);
//
//	if(a.Buscar(2))
//		a.ImprimirResultadoBusqueda(2);
//		a.ImprimirResultadoBusqueda(2, "ficheroBM25.txt");


		a.Buscar("/home/javi/EI/EIPractica3/materiales_buscador/CorpusTime/Preguntas", 423, 1, 83);
		a.ImprimirResultadoBusqueda(423, "fich_salida_buscador_alumno.txt");

		cout<<"BUSCADO"<<endl;
}


