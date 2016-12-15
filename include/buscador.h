/*
 * buscador.h
 *
 *  Created on: 03/06/2014
 *      Author: javi
 */

#ifndef BUSCADOR_H_
#define BUSCADOR_H_

#include "indexadorHash.h"
#include <math.h>
#include <queue>
#include <stdexcept>


class ResultadoRI {

	friend ostream& operator<<(ostream&, const ResultadoRI&);
	friend class Buscador;

public:

	ResultadoRI(const double& kvSimilitud, const long int& kidDoc, const int& np,const string& nombreDoc);
	double VSimilitud() const;
	long int IdDoc() const;
	bool operator< (const ResultadoRI& lhs) const;


private:

	double vSimilitud;
	long int idDoc;
	int numPregunta;
	string nombreDoc;
};

class Buscador: public IndexadorHash {

	friend ostream& operator<<(ostream& s, const Buscador& p);

	//Creamos un prototipo de funcion para que se mas extensible
	typedef void (Buscador::*ptrFunc)(unordered_map<string, InfDoc>::iterator &, double &);

private:

	//DONE
	Buscador();
	int formSimilitud;
	double c;
	double k1;
	double b;
	priority_queue<ResultadoRI> docsOrdenados;
	void splitNomDoc(string &s);
	void tokensByChar(vector<string> &v,const string &str,const string& caracter);

public:

	void DFR(unordered_map<string, InfDoc>::iterator &, double &);
	void BM25(unordered_map<string, InfDoc>::iterator &, double &);
	//DONE
	Buscador(const string& directorioIndexacion, const int& f);
	//DONE
	Buscador(const Buscador&);
	//DONE
	~Buscador();
	//DONE
	Buscador& operator= (const Buscador&);

	bool Buscar(const int& numDocumentos);
	bool Buscar(const int& numDocumentos, const int &numPregunta);
	bool Buscar(const string& dirPreguntas, const int& numDocumentos, const int& numPregInicio, const int& numPregFin);
	void ImprimirResultadoBusqueda(const int& ) const;
	bool ImprimirResultadoBusqueda(const int& , const string& ) const;
	void ImprimirResultadoBusqueda(const int& numDocumentos, stringstream &ss)const;
	//DONE
	int DevolverFormulaSimilitud() const;
	//DONE
	bool CambiarFormulaSimilitud(const int&);
	//DONE
	void CambiarParametrosDFR(const double&);
	//DONE
	double DevolverParametrosDFR() const;
	//DONE
	void CambiarParametrosBM25(const double&, const double&);
	//DONE
	void DevolverParametrosBM25(double& , double& ) const;
};


#endif /* BUSCADOR_H_ */
