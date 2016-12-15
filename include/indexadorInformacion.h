/*
 * indexadorInformacion.h
 *
 *  Created on: 13/04/2014
 *      Author: javi
 */

#ifndef INDEXADORINFORMACION_H_
#define INDEXADORINFORMACION_H_

#include <iostream>
#include <unordered_map>
#include <list>
#include <set>
#include <stdio.h>
#include <time.h>
#include <stdexcept>
#include <sstream>

using namespace std;



class InfTermDoc {

	friend ostream& operator<<(ostream& s, const InfTermDoc& p);
	friend class IndexadorHash;
	friend class Buscador;

	public:


		InfTermDoc ();
		~InfTermDoc ();
		InfTermDoc (const InfTermDoc &);
		InfTermDoc & operator= (const InfTermDoc &);

		void setFt(const int &ft){ this->ft=ft; }
		int getFt()const{ return ft; }
		void addPosTerm(const int &t){ posTerm.insert(t); }
		void clearPosTerm(){ posTerm.clear(); }

	private:

		int ft;
		set<int> posTerm;

};

class InformacionTermino {

	friend class IndexadorHash;
	friend class Buscador;
	friend ostream& operator<<(ostream& , const InformacionTermino& );

	public:
		InformacionTermino (const InformacionTermino &);
		InformacionTermino ();
		~InformacionTermino ();
		InformacionTermino & operator= (const InformacionTermino &);

		void setFtc(const int &ftc){ this->ftc = ftc; }
		int getFtc()const{ return ftc; }

		void addL_docs(const long int &i,const InfTermDoc &itd){
			l_docs.insert(make_pair(i , itd));
		}
		void clearL_docs(){ l_docs.clear(); }

		bool borrarDocumento(const long int &idDocumento);

	private:

		int ftc;
		unordered_map<long int, InfTermDoc> l_docs;

};



class InfDoc {

	friend class IndexadorHash;
	friend class Buscador;
	friend ostream& operator<<(ostream& s, const InfDoc& p);

	public:
		InfDoc (const InfDoc &);
		InfDoc ();
		~InfDoc ();
		InfDoc & operator= (const InfDoc &);

		void setFechaActual();
		struct tm getFecha() const{ return fechaModificacion; }

	private:

		string preparafecha(const int &)const;
		long int idDoc;
		int numPal;
		int numPalSinParada;
		int numPalDiferentes;
		int tamBytes;
		struct tm fechaModificacion;

};

class InfColeccionDocs {

	friend class IndexadorHash;
	friend class Buscador;
	friend ostream& operator<<(ostream& s, const InfColeccionDocs& p);

	public:

		InfColeccionDocs (const InfColeccionDocs &);
		InfColeccionDocs ();
		~InfColeccionDocs ();
		InfColeccionDocs & operator= (const InfColeccionDocs &);

	private:

		long int numDocs;
		long int numTotalPal;
		long int numTotalPalSinParada;
		long int numTotalPalDiferentes;
		long int tamBytes;

};

class InformacionTerminoPregunta {

	friend class IndexadorHash;
	friend class Buscador;
	friend ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p);

	public:
		InformacionTerminoPregunta (const InformacionTerminoPregunta &);
		InformacionTerminoPregunta ();
		~InformacionTerminoPregunta ();
		InformacionTerminoPregunta & operator= (const InformacionTerminoPregunta &);

	private:

		int ft;
		set<int> posTerm;

};

class InformacionPregunta {

	friend class IndexadorHash;
	friend class Buscador;
	friend ostream& operator<<(ostream& s, const InformacionPregunta& p);

	public:
		InformacionPregunta (const InformacionPregunta &);
		InformacionPregunta ();
		~InformacionPregunta ();
		InformacionPregunta & operator= (const InformacionPregunta &);
		void clear();

	private:

		long int numTotalPal;
		long int numTotalPalSinParada;
		long int numTotalPalDiferentes;

};


class Nodo {

	friend class Trie ;

	public:
		Nodo();
		~Nodo();
		bool isFinPalabra()const { return finPalabra; }
		void setFinPalabra(bool mark) { finPalabra =mark; }
		void addHijo(const char& caracter, Nodo* nHijo){
			mapHijos.insert(make_pair(caracter,nHijo));
		}
		Nodo* getHijo(char c)const;

	private:
		bool finPalabra;
		unordered_map<char,Nodo*> mapHijos;
};

class Trie {

	friend ostream& operator<<(ostream& s, const Trie& t);

	public:
		Trie();
		~Trie();
		void addPalabra(const string &palabra);
		bool findPalabra(const string &palabra);
		bool delPalabra(const string &palabra);
		void mostrarPalabras(Nodo * nodo,  ostream &s, string prefix) const;

	private:
		Nodo* raiz;
};


#endif /* INDEXADORINFORMACION_H_ */
