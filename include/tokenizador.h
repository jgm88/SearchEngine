/*
 * tokenizadorClase.h
 *
 *  Created on: 30/01/2014
 *      Author: javi
 */

#ifndef TOKENIZADOR_H_
#define TOKENIZADOR_H_

#include <iostream>
#include <string.h>
#include <map>
#include <vector>

#include <list>
#include <set>
#include <fstream>
#include <sys/stat.h>
#include <stdlib.h>
#include "indexadorHash.h"

using namespace std;

class IndexadorHash;

class Tokenizador{

	friend ostream& operator<<(ostream&, const Tokenizador&);
	

	public:

		IndexadorHash *indexador;

		string delimiters;
		set<wchar_t> delimitersSet;
		// Inicializa delimiters=",;:.-/+*\\ '’\"{}[]()<>¡!¿?&#=·•\t\n\r@";
		// casosEspeciales a true; pasarAminuscSinAcentos a false
		Tokenizador();

        // Inicializa variable privada delimiters a delimitadoresPalabra
		Tokenizador (const string& delimitadores);
		~Tokenizador ();

		// Inicializa delimiters a delimitadoresPalabra; detectarCompuestos a
		// detectComp; pasarAminuscSinAcentos a minuscSinAcentos
		Tokenizador (const string& delimitadoresPalabra, const bool& detectComp, const bool& minuscSinAcentos);

		Tokenizador& operator= (const Tokenizador&);

		// Cambia la variable privada “casosEspeciales”
		void CasosEspeciales (const bool& nuevoCasosEspeciales);

		// Devuelve el contenido de la variable privada “casosEspeciales”
		bool CasosEspeciales ()const;

		// Cambia la variable privada “pasarAminuscSinAcentos”
		void PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos);

		// Devuelve el contenido de la variable privada “pasarAminuscSinAcentos”
		bool PasarAminuscSinAcentos ()const;

		void DelimitadoresPalabra(const string& nuevoDelimiters);

		void Tokenizar (const string& str, list<string>& tokens) const;

		void Tokenizar (const string& input, ofstream &f,long int & posTerm) const;
		bool Tokenizar(const string& NomFichEntr, const string& NomFichSal) const;
		bool Tokenizar (const string & NomFichEntr) const;
		bool TokenizarListaFicheros (const string& lista) const;
		bool TokenizarDirectorio(const string &dirAIndexar)const;

		string DelimitadoresPalabra() const;

		static bool isEmail(const string& str, unsigned short &pos, const set<wchar_t> &delimitersSet, unsigned short &estado);
		static bool isCompuesta(const string& str, unsigned short &pos, const set<wchar_t> &delimitersSet, unsigned short &estado);
		static bool isPunto(const string& str, unsigned short &pos, const set<wchar_t> &delimitersSet, unsigned short &estado);
		static bool isComa(const string& str, unsigned short &pos, const set<wchar_t> &delimitersSet, unsigned short &estado);
		static bool isURL(const string& str, unsigned short &pos, const set<wchar_t> &delimitersSet, unsigned short &estado);

		void quitaAcentos(string &token)const;

		void borrarDelimiter(char a);
		void anyadirSet(char a);

		void AnyadirDelimitadoresPalabra(const string& nuevoDelimiters);


	private:

		//Modelo funcion para HASH
		typedef bool (*funcion) (const string &input, unsigned short &pos, const set<wchar_t> &delimitersSet, unsigned short &estado);

		//controlar si el espacio estuvo al principio o no
		bool inicioConEspeciales;
		// Si true detectará palabras compuestas y casos especiales
		bool casosEspeciales;

		// Si true pasará el token a minúsculas y quitará acentos
		bool pasarAminuscSinAcentos;

		map<string, string> mapAcentos;
		void initMapAcentos();

		map<char,funcion> mapEspeciales;
		void initmapEspeciales();

		void TokenizarEsp(const string& input, list<string>& tokens)const;
		void TokenizarMayus(const string& input, list<string>& tokens)const;
		void TokenizarEspMayus (const string& input, list<string>& tokens)const;
		void TokenizarSin (const string& input, list<string>& tokens)const;

		string sAcentos;



};

#endif /* TOKENIZADOR_H_ */
