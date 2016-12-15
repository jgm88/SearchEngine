/*
 * indexadorInformacion.cpp
 *
 *  Created on: 13/04/2014
 *      Author: javi
 */

#include "indexadorInformacion.h"

////////////////////INFTERDOC///////////////////////////

	InfTermDoc::InfTermDoc(){
		ft=0;
	}
	InfTermDoc::~InfTermDoc(){
		ft = 0;
		posTerm.clear();
	}
	InfTermDoc::InfTermDoc(const InfTermDoc &i){
		ft = i.ft;
		posTerm = i.posTerm;
	}
	InfTermDoc & InfTermDoc::operator=(const InfTermDoc &i){
		ft = i.ft;
		posTerm = i.posTerm;
		return *this;
	}

	ostream& operator<<(ostream& s, const InfTermDoc& p) {
		s << "ft: " << p.ft;
		set<int>::iterator iter = p.posTerm.begin();
		set<int>::iterator sEnd = p.posTerm.end();

		for (;iter != sEnd; ++iter)
			s << "\t" << *iter;

		return s;
	}


////////////////////END-INFTERDOC///////////////////////////




////////////////////INFORMACIONTERMINO///////////////////////////


	InformacionTermino::InformacionTermino(){
		ftc = 0;
	}

	InformacionTermino::~InformacionTermino(){
		ftc = 0;
		l_docs.clear();
	}
	InformacionTermino::InformacionTermino(const InformacionTermino &i){
		ftc = i.ftc;
		l_docs = i.l_docs;
	}
	InformacionTermino & InformacionTermino::operator =(const InformacionTermino &i){
		ftc = i.ftc;
		l_docs = i.l_docs;
		return *this;
	}

	bool InformacionTermino::borrarDocumento(const long int &idDocumento){

		auto iter = l_docs.find(idDocumento);
		auto iEnd = l_docs.end();

		if(iter!=iEnd){
			ftc -= iter->second.getFt();
			l_docs.erase(iter);
			return true;
		}
		return false;
	}

	ostream& operator<<(ostream& s, const InformacionTermino& p) {
		s << "Frecuencia total: " << p.ftc << "\tfd: " << p.l_docs.size();

		auto iter = p.l_docs.begin();
		auto sEnd = p.l_docs.end();

		for (;iter != sEnd; ++iter)
			s << "\tId.Doc: " << iter->first << "\t" << iter->second;

		return s;
	}


////////////////////END-INFORMACIONTERMINO///////////////////////////




/////////////////////////////INFDOC//////////////////////////////////

	/*
	 * Odiosa variable Global ya que los statics de C++ no son comprensibles
	 */
	time_t timerInfDoc;

	InfDoc::InfDoc(){
		idDoc = 1;
		numPal = numPalSinParada = numPalDiferentes = tamBytes = 0;

		setFechaActual();
	}
	InfDoc::~InfDoc(){
		idDoc = 1;
		numPal = numPalSinParada = numPalDiferentes = tamBytes = 0;
	}
	InfDoc::InfDoc (const InfDoc &i){
		idDoc = i.idDoc;
		numPal = i.numPal;
		numPalSinParada = i.numPalSinParada;
		numPalDiferentes = i.numPalDiferentes;
		tamBytes = i.tamBytes;
		fechaModificacion = i.fechaModificacion;
	}

	InfDoc & InfDoc::operator= (const InfDoc &i){
		idDoc = i.idDoc;
		numPal = i.numPal;
		numPalSinParada = i.numPalSinParada;
		numPalDiferentes = i.numPalDiferentes;
		tamBytes = i.tamBytes;
		fechaModificacion = i.fechaModificacion;

		return *this;
	}

	ostream& operator<<(ostream& s, const InfDoc& p) {
		s<<"idDoc: "<<p.idDoc<<"\tnumPal: " <<p.numPal<< "\tnumPalSinParada: "
		<< p.numPalSinParada << "\tnumPalDiferentes: " << p.numPalDiferentes
		<< "\ttamBytes: " << p.tamBytes;
//		<<"\tfechaModificacion: "
//		<< p.preparafecha(p.fechaModificacion.tm_mday) << "/"
//		<< p.preparafecha(p.fechaModificacion.tm_mon)<< "/"
//		<< p.fechaModificacion.tm_year;
		return s;
	}
	string InfDoc::preparafecha(const int &num)const{
		stringstream ss;
		if(num<10){
			ss<<0<<num;
		}else{
			ss<<num;
		}
		return ss.str();
	}
	void InfDoc::setFechaActual(){
		timerInfDoc = time(0);
		fechaModificacion = *localtime(&timerInfDoc);

		fechaModificacion.tm_year+=1900;
		fechaModificacion.tm_mon+=1;
	}



///////////////////////////END-INFDOC//////////////////////////////////




/////////////////////////INFCOLECCIONDOCS//////////////////////////////

	InfColeccionDocs::InfColeccionDocs (){
		numDocs = numTotalPal = numTotalPalSinParada = numTotalPalDiferentes = tamBytes = 0;
	}
	InfColeccionDocs::~InfColeccionDocs (){
		numDocs = numTotalPal = numTotalPalSinParada = numTotalPalDiferentes = tamBytes = 0;
	}
	InfColeccionDocs::InfColeccionDocs (const InfColeccionDocs &i){
		numDocs = i.numDocs;
		numTotalPal = i.numTotalPal;
		numTotalPalSinParada = i.numTotalPalSinParada;
		numTotalPalDiferentes = i.numTotalPalDiferentes;
		tamBytes = i.tamBytes;
	}

	InfColeccionDocs & InfColeccionDocs::operator= (const InfColeccionDocs &i){
		numDocs = i.numDocs;
		numTotalPal = i.numTotalPal;
		numTotalPalSinParada = i.numTotalPalSinParada;
		numTotalPalDiferentes = i.numTotalPalDiferentes;
		tamBytes = i.tamBytes;

		return *this;
	}

	ostream& operator<<(ostream& s, const InfColeccionDocs& p){
		s << "numDocs: " << p.numDocs << "\tnumTotalPal: " << p.numTotalPal <<
		"\tnumTotalPalSinParada: "<< p.numTotalPalSinParada <<
		"\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes <<
		"\ttamBytes: " << p.tamBytes;
		return s;
	}

///////////////////////END-INFCOLECCIONDOCS////////////////////////////////


/////////////////////INFORMACIONTERMINOPREGUNTA/////////////////////////////


	InformacionTerminoPregunta::InformacionTerminoPregunta (){
		ft = 0;
	}
	InformacionTerminoPregunta::~InformacionTerminoPregunta (){
		ft = 0;
		posTerm.clear();
	}
	InformacionTerminoPregunta::InformacionTerminoPregunta (const InformacionTerminoPregunta &i){
		ft = i.ft;
		posTerm = i.posTerm;
	}
	InformacionTerminoPregunta & InformacionTerminoPregunta::operator= (const InformacionTerminoPregunta &i){
		ft = i.ft;
		posTerm = i.posTerm;
		return *this;
	}

	ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p) {
		s << "ft: " << p.ft;
		set<int>::iterator iter = p.posTerm.begin();
		set<int>::iterator sEnd = p.posTerm.end();

		for (;iter != sEnd; ++iter)
			s << "\t" << *iter;

		return s;
	}

///////////////////////END-INFORMACIONTERMINOPREGUNTA///////////////////////////


///////////////////////////INFORMACIONPREGUNTA//////////////////////////////////


	InformacionPregunta::InformacionPregunta (){
		numTotalPal = numTotalPalSinParada = numTotalPalDiferentes = 0;
	}
	InformacionPregunta::~InformacionPregunta (){
		numTotalPal = numTotalPalSinParada = numTotalPalDiferentes = 0;
	}
	InformacionPregunta::InformacionPregunta (const InformacionPregunta &i){
		numTotalPal = i.numTotalPal;
		numTotalPalSinParada = i.numTotalPalSinParada;
		numTotalPalDiferentes = i.numTotalPalDiferentes;
	}
	InformacionPregunta & InformacionPregunta::operator= (const InformacionPregunta &i){
		numTotalPal = i.numTotalPal;
		numTotalPalSinParada = i.numTotalPalSinParada;
		numTotalPalDiferentes = i.numTotalPalDiferentes;
		return *this;
	}
	void InformacionPregunta::clear(){
		numTotalPal = numTotalPalSinParada = numTotalPalDiferentes = 0;
	}
	ostream& operator<<(ostream& s, const InformacionPregunta& p){
		s << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: "
		<< p.numTotalPalSinParada <<"\tnumTotalPalDiferentes: "
		<< p.numTotalPalDiferentes;
		return s;
	}

//////////////////////////END-INFORMACIONPREGUNTA///////////////////////////////



//////////////////////////TRIE y NODOTRIE///////////////////////////////


	Nodo::Nodo() {  finPalabra = false; }
	Nodo::~Nodo() {
		unordered_map<char,Nodo*>::iterator iter = mapHijos.begin();
		unordered_map<char,Nodo*>::iterator iterEnd = mapHijos.end();
		for(; iter!= iterEnd; ++iter){
			if((*iter).second!=NULL)
				delete (*iter).second;
		}
		mapHijos.clear();
	}

	Nodo* Nodo::getHijo(char c)const{
		//CAMBIAR POR ITERATORS
	    try{
	        return mapHijos.at(c);

	    }catch(const std::out_of_range& oor){
	        return NULL;
	    }
	}

	Trie::Trie(){
	    raiz = new Nodo();
	}

	Trie::~Trie(){
	    if(raiz!= NULL){
	        delete raiz;
	        raiz=NULL;
	    }
	}

	void Trie::addPalabra(const string &palabra){
	    Nodo* nActual = raiz;

	    if ( palabra.length() != 0 ){
	        for ( int i = 0; i < palabra.length(); i++ ){
	            Nodo* nHijo = nActual->getHijo(palabra[i]);
	            if ( nHijo != NULL ){
	                nActual = nHijo;
	            }
	            else{
	                Nodo* nTemp = new Nodo();
	                nActual->addHijo(palabra[i], nTemp);
	                nActual = nTemp;
	            }
	            if ( i == palabra.length() - 1 )
	                nActual->setFinPalabra(true);
	        }
	    }
	}

	bool Trie::delPalabra(const string &palabra){
	    Nodo* nActual = raiz;

	    if ( nActual != NULL ){
	        for ( int i = 0; i < palabra.length(); i++ ){
	            Nodo* nTemp = nActual->getHijo(palabra[i]);
	            if ( nTemp == NULL )
	                return false;
	            nActual = nTemp;
	        }
	    }

	    if(nActual->isFinPalabra()){
	        nActual->setFinPalabra(false);
	        return true;
	    }
	    return false;

	}


	bool Trie::findPalabra(const string &palabra){
	    Nodo* nActual = raiz;

	    if ( nActual != NULL ){
	        for ( int i = 0; i < palabra.length(); i++ ){
	            Nodo* nTemp = nActual->getHijo(palabra[i]);
	            if ( nTemp == NULL )
	                return false;
	            nActual = nTemp;
	        }
	    }
	    return nActual->isFinPalabra();
	}

	void Trie::mostrarPalabras(Nodo * nodo,ostream &s,  string prefix = "") const
	{
	    string currentWord= prefix;
	    if (nodo->isFinPalabra())
	        s << prefix << endl;
	    if(nodo->mapHijos.size()>0)
	    {
	        unordered_map<char,Nodo*>::iterator it = nodo->mapHijos.begin();
	        unordered_map<char,Nodo*>::iterator itEnd= nodo->mapHijos.end();
	        for(; it!=itEnd; ++it)
	        {
	            prefix+= it->first;
	            Nodo *n= it->second;
	            mostrarPalabras(n, s,prefix);
	            prefix= currentWord;
	        }
	    }
	}


//	ostream &operator<<(ostream &s, const Trie &t){
//	    t.mostrarPalabras(t.raiz,s);
//	    return s;
//	}
////////////////////////////END-TRIE y NODOTRIE///////////////////////////////

