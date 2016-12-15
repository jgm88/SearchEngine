/*
 * buscador.cpp
 *
 *  Created on: 03/06/2014
 *      Author: javi
 */

#include "buscador.h"

////////////////////ResultadoRI///////////////////////////

ResultadoRI::ResultadoRI(const double &kvSimilitud, const long int &kidDoc, const int &np, const string &nombreDoc){
	vSimilitud = kvSimilitud;
	idDoc = kidDoc;
	numPregunta = np;
	this->nombreDoc = nombreDoc;
}
double ResultadoRI::VSimilitud() const{
	return vSimilitud;
}
long int ResultadoRI::IdDoc() const{
	return idDoc;
}

bool ResultadoRI::operator<(const ResultadoRI &lhs) const{
	if(numPregunta == lhs.numPregunta)
		return (vSimilitud < lhs.vSimilitud);
	else
		return (numPregunta > lhs.numPregunta);
}
ostream& operator<<(ostream &os, const ResultadoRI &res){
	os << res.vSimilitud << "\t\t" << res.idDoc << "\t" << res.numPregunta << endl;
	return os;
}

////////////////////END-ResultadoRI///////////////////////////


////////////////////BUSCADOR///////////////////////////

Buscador::Buscador(const string& directorioIndexacion, const int& f):
IndexadorHash(directorioIndexacion){
	if(f==0 || f==1)
		formSimilitud=f;
	else
		formSimilitud=0;
	c = 2; k1 = 1.2; b = 0.75;
}

Buscador::Buscador(const Buscador& busc): IndexadorHash(busc){
	if(this!=&busc){
		formSimilitud = busc.formSimilitud;
		c = busc.c; k1 = busc.k1; b = busc.b;
	}
}

Buscador::~Buscador(){ }
Buscador& Buscador::operator= (const Buscador& busc){
	if(this!=&busc){
		formSimilitud = busc.formSimilitud;
		c = busc.c; k1 = busc.k1; b = busc.b;
	}
	return *this;
}
//Le pasamos argumentos al padre ya que no tiene constructor por defecto
Buscador::Buscador():IndexadorHash(""){
	formSimilitud = 0;
	c = 2;	k1 = 1.2;	b = 0.75;
}
int Buscador::DevolverFormulaSimilitud() const{
	return formSimilitud;
}
bool Buscador::CambiarFormulaSimilitud(const int &f){
	if(f==0 || f==1){
		formSimilitud=f;
		return true;
	}
	return false;
}

void Buscador::CambiarParametrosDFR(const double &c){
	this->c = c;
}
double Buscador::DevolverParametrosDFR() const{
	return c;
}
void Buscador::CambiarParametrosBM25(const double &k1, const double &b){
	this->k1 = k1; this->b = b;
}
void Buscador::DevolverParametrosBM25(double &k1, double &b) const{
	k1 = this->k1; b = this->b;
}

void Buscador::ImprimirResultadoBusqueda(const int& numDocumentos) const{
	stringstream ss;
	ImprimirResultadoBusqueda(numDocumentos, ss);
	cout << ss.str();
}

void Buscador::ImprimirResultadoBusqueda(const int& numDocumentos, stringstream &ss)const{


	if(docsOrdenados.size()>0){
		string pregunta, metodo;
		priority_queue<ResultadoRI> colaTemp = docsOrdenados;
		ResultadoRI resRI(0.0,0,0, "");

		//Variables para controlar la posicion del documento
		short posDoc = 0;
		int nPregTemp = colaTemp.top().numPregunta;

		DevuelvePregunta(pregunta);
		if(formSimilitud == 0)
			metodo = "DFR";
		else
			metodo =  "BM25";


		for(int i=0;i<numDocumentos && i<docsOrdenados.size(); ++i){
			resRI = colaTemp.top();
			if(nPregTemp!=resRI.numPregunta){
				nPregTemp=resRI.numPregunta;
				posDoc=0;
			}
			ss 	<< resRI.numPregunta<< " "<<metodo<<" " <<resRI.nombreDoc<<" "<<posDoc<<" "<< resRI.vSimilitud<< " ";
			if(resRI.numPregunta == 0)
				ss << pregunta << endl;
			else
				ss << "ConjuntoDePreguntas" << endl;

			colaTemp.pop();
			++posDoc;
		}
	}
}

bool Buscador::ImprimirResultadoBusqueda(const int& numDocumentos, const string& nombreFichero)const{
	try{
		ofstream outputFile(nombreFichero);
		stringstream ss;
		ImprimirResultadoBusqueda(numDocumentos, ss);
		outputFile << ss.str();
		outputFile.close();
		return true;

	}catch (exception &e) {
		cerr << "ERROR: ImprimirResultadoBusqueda a fichero";
		return false;
	}
}
void Buscador::DFR(unordered_map<string, InfDoc>::iterator &iterDoc, double &vSimilitud){

	vSimilitud = 0.0;
	double wiq, wid, lambda, ftd, avrLd, ld, nt;
	InfTermDoc infTermDoc;
	InformacionTermino infTerm;

	avrLd = (double)informacionColeccionDocs.tamBytes/informacionColeccionDocs.numDocs;
	ld = iterDoc->second.tamBytes;

	auto iter = indicePregunta.begin();
	auto iterEnd = indicePregunta.end();

	for(;iter!=iterEnd;++iter){
		if(Devuelve(iter->first, infTerm))
			if(Devuelve(iter->first,iterDoc->first, infTermDoc)){

				lambda =(double) infTerm.ftc/indiceDocs.size();
				nt = infTerm.l_docs.size();
				ftd = (double) (infTermDoc.ft *  log2(1+((double)c*avrLd/ld)));
				wiq = (double) iter->second.ft/infPregunta.numTotalPal;
				wid  = (double)(log2(1 + lambda) + ftd * (double)log2((1 + lambda)/lambda)) * (double)((infTerm.ftc + 1) / (nt * (ftd + 1)));
				vSimilitud+=wiq*wid;
			}
	}
}
void Buscador::BM25(unordered_map<string, InfDoc>::iterator &iterDoc, double &vSimilitud){

	vSimilitud = 0.0;
	auto iter = indicePregunta.begin();
	auto iterEnd = indicePregunta.end();
	double IDF, avgdl, x;
	long int N, nq, D;
	int fqD;
	N = informacionColeccionDocs.numDocs;
	avgdl = informacionColeccionDocs.numTotalPalSinParada/N;
	D = iterDoc->second.numPalSinParada;

	InfTermDoc infTermDoc;
	InformacionTermino infTerm;

	for(;iter!=iterEnd;++iter){
		if(Devuelve(iter->first, infTerm)) {
			if(Devuelve(iter->first,iterDoc->first, infTermDoc)){
				fqD = (double)infTermDoc.ft;
				nq = (double)infTerm.l_docs.size();
				IDF = log((double)(N - nq + 0.5)/(nq + 0.5));
				x = (double)(fqD*(double)(k1 + 1))/(fqD + k1*(1 - b +(double)(b * (double)(D/avgdl))));
				vSimilitud+=IDF*x;
			}
		}
	}

}
bool Buscador::Buscar(const int& numDocumentos){
	docsOrdenados = priority_queue<ResultadoRI>();
	Buscar(numDocumentos, 0);
}
void Buscador::splitNomDoc(string &str){

	    vector<string> tokens;
//
//	    split(tokens, s, is_any_of("/"));
//	    s = tokens[tokens.size()-1];
//	    split(tokens, s, is_any_of("."));
//	    if(tokens.size()>1)
//	    	s = tokens[0];

		tokensByChar(tokens, str, "/");
		str = tokens[tokens.size()-1];

		tokensByChar(tokens, str, ".");
		if(tokens.size()>1)
			str = tokens[0];


}
void Buscador::tokensByChar(vector<string> &tokens, const string &str,const string& caracter){
	tokens.clear();
	string::size_type lastpos = str.find_first_not_of(caracter,0);
	string::size_type pos = str.find_first_of(caracter,lastpos);
	while(string::npos != pos || string::npos != lastpos)
	{
		tokens.push_back(str.substr(lastpos,pos - lastpos));
		lastpos = str.find_first_not_of(caracter, pos);
		pos = str.find_first_of(caracter, lastpos);
	}
}
bool Buscador::Buscar(const int& numDocumentos, const int &numPregunta){

	priority_queue<ResultadoRI> colaTemp;
	int indDocsSize = indiceDocs.size();

	string rutaDoc;
	ptrFunc metodo;
	if(formSimilitud == 0)
		metodo = &Buscador::DFR;
	else
		metodo = &Buscador::BM25;

	try{
		ResultadoRI resRI(0.0,0,numPregunta, "");

		//Recorremos los documentos de la coleccion
		auto iter = indiceDocs.begin();
		auto iterEnd = indiceDocs.end();
		for(;iter!=iterEnd;++iter){

			resRI.idDoc = iter->second.idDoc;
			rutaDoc = iter->first;
			splitNomDoc(rutaDoc);
			resRI.nombreDoc = rutaDoc;
			(this->*metodo)(iter, resRI.vSimilitud);
			colaTemp.push(resRI);
		}

		//seleccionamos solo los mejores hasta el numero dado
		for(short i=0; i<numDocumentos && i<indDocsSize; ++i){
			docsOrdenados.push(colaTemp.top());
			colaTemp.pop();
		}
		return true;
	}
	catch (exception &e){
		return false;
	}
}
bool Buscador::Buscar(const string& dirPreguntas, const int& numDocumentos, const int& numPregInicio, const int& numPregFin){

	try{
		for(short i = numPregInicio;i <= numPregFin;++i){
			stringstream output;
			output << dirPreguntas << "/" << i << ".txt";
			string nombre = output.str();
			ifstream fichero(nombre.c_str());

			if(fichero.is_open()){
				string preg;
				getline(fichero,preg);
				IndexarPregunta(preg);
				fichero.close();
				Buscar(numDocumentos,i);
			}
		}
		return true;
	}
	catch (exception &e){
		return false;
	}
}
ostream& operator<<(ostream & s, const Buscador& b){
	string preg;
	s << "Buscador: " << endl;
	if(b.DevuelvePregunta(preg))
		s << "\tPregunta indexada: " << preg << endl;
	else
		s << "\tNo hay ninguna pregunta indexada" << endl;
	s << "\tDatos del indexador: " << endl << (IndexadorHash) b;

	return s;
}

