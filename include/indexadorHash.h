#ifndef INDEXADORHASH_H_
#define INDEXADORHASH_H_

#include "indexadorInformacion.h"
#include "tokenizador.h"
#include "stemmer.h"
#include <unordered_set>
#include <unistd.h>
#include <time.h>
#include <vector>

class Tokenizador;
class IndexadorHash {

	friend ostream& operator<<(ostream& s, const IndexadorHash& p);

	public:

		IndexadorHash(const string& fichStopWords, const string& delimitadores,
		const bool& detectComp, const bool& minuscSinAcentos, const string&
		dirIndice, const int& tStemmer, const bool& almEnDisco, const bool&
		almPosTerm);

		IndexadorHash(const string& directorioIndexacion);

		IndexadorHash(const IndexadorHash&);

		~IndexadorHash();

		IndexadorHash& operator= (const IndexadorHash&);

		bool Indexar(const string& ficheroDocumentos);

		bool IndexarDirectorio(const string& dirAIndexar);

		bool GuardarIndexacion() const;

		bool RecuperarIndexacion (const string& directorioIndexacion);

		void ImprimirIndexacion() const;

		bool IndexarPregunta(const string& preg);

		bool DevuelvePregunta(string& preg) const;

		bool DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const;

		bool DevuelvePregunta(InformacionPregunta& inf) const;

		void ImprimirIndexacionPregunta() ;

		void ImprimirPregunta();

		bool Devuelve(const string& word, InformacionTermino& inf) const;

		bool Devuelve(const string& word, const string& nomDoc, InfTermDoc& InfDoc) const;

		bool Existe(const string& word) const;

		bool Borra(const string& word);

		bool BorraDoc(const string& nomDoc);

		void VaciarIndice();

		bool Actualiza(const string& word, const InformacionTermino& inf);

		bool Inserta(const string& word, const InformacionTermino& inf);

		int NumPalIndexadas() const;

		string DevolverFichPalParada () const;
		int NumPalParada() const;

		string DevolverDelimitadores () const;

		bool DevolverCasosEspeciales () const;

		bool DevolverPasarAminuscSinAcentos () const;

		bool DevolverAlmacenarPosTerm () const{ return almacenarPosTerm;}

		string DevolverDirIndice () const{ return directorioIndice;}

		int DevolverTipoStemming () const{ return tipoStemmer;}

		bool DevolverAlmEnDisco () const{ return almacenarEnDisco;}

		void ListarPalParada() const;

		void ListarInfColeccDocs() const;

		void ListarTerminos() const;

		bool ListarTerminos(const string& nomDoc) const;

		void ListarDocs() const;

		bool ListarDocs(const string& nomDoc) const;


		void indexarPalabraTok(const string &,const long int &);

		Tokenizador *tok;

		int stringToInt(const string &s)const;

		void indexadorDeFichero(const string & directorioindexacion);

		void vaciarIndexador();

	protected:

		/**
		 * Tratamientos de indexarPalabra
		 */

		InfDoc infDocAux;
		string nombreDocAux;
		stemmerPorter stemmer;

		/*
		 * Fin Indexarpalabra
		 */
		void rellenaStops();

		unordered_map<string, InformacionTermino> indice;

		unordered_map<string, InfDoc> indiceDocs;

		InfColeccionDocs informacionColeccionDocs;

		string pregunta;

		unordered_map<string, InformacionTerminoPregunta> indicePregunta;

		InformacionPregunta infPregunta;

		string ficheroStopWords;



		string directorioIndice;

		int tipoStemmer;

		bool almacenarEnDisco;

		bool almacenarPosTerm;

		Trie indiceTrie;

		/*
		 * total heap usage: 1,226 allocs, 1,226 frees, 50,952 bytes allocated
		 * Palabra existe = real	0m19.603s
		 * Palabra NO existe = real	0m21.036s
		 */
		unordered_set<string> stopWords;
		/*
		 * total heap usage: 5,529 allocs, 5,529 frees, 509,580 bytes allocated
		 * 	Palabra existe = real	1m47.586s
		 * 	Palabra NO existe = real	9m24.020s
		 */
		Trie stopWordsTrie;

};

#endif /*INDEXADORHASH_H_*/
