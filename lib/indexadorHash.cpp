/*
 * indexadorHash.cpp
 *
 *  Created on: 13/04/2014
 *      Author: javi
 */

#include "indexadorHash.h"

	IndexadorHash::IndexadorHash(const string& fichStopWords, const string& delimitadores,
	const bool& detectComp, const bool& minuscSinAcentos, const string&
	dirIndice, const int& tStemmer, const bool& almEnDisco, const bool&
	almPosTerm){

		tok = new Tokenizador(delimitadores,detectComp,minuscSinAcentos);
		tok->indexador = this;

		this->directorioIndice = dirIndice;
		this->tipoStemmer = tStemmer;
		this->almacenarEnDisco = almEnDisco;
		this->almacenarPosTerm = almPosTerm;
		pregunta = "";

		this->ficheroStopWords = fichStopWords;
		rellenaStops();

	}
	IndexadorHash::IndexadorHash(const IndexadorHash &i){
		tok = new Tokenizador(*i.tok);
		tok->indexador = this;

		directorioIndice = i.directorioIndice;
		tipoStemmer = i.tipoStemmer;
		almacenarEnDisco = i.almacenarEnDisco;
		almacenarPosTerm = i.almacenarPosTerm;
		pregunta = i.pregunta;
		ficheroStopWords = i.ficheroStopWords;
		stopWords = i.stopWords;
		infPregunta = i.infPregunta;
		indicePregunta = i.indicePregunta;
		informacionColeccionDocs = i.informacionColeccionDocs;
		indiceDocs = i.indiceDocs;
		indice = i.indice;
	}
	IndexadorHash & IndexadorHash::operator =(const IndexadorHash &i){
		tok = new Tokenizador(*i.tok);
		tok->indexador = this;

		directorioIndice = i.directorioIndice;
		tipoStemmer = i.tipoStemmer;
		almacenarEnDisco = i.almacenarEnDisco;
		almacenarPosTerm = i.almacenarPosTerm;
		pregunta = i.pregunta;
		ficheroStopWords = i.ficheroStopWords;
		stopWords = i.stopWords;
		infPregunta = i.infPregunta;
		indicePregunta = i.indicePregunta;
		informacionColeccionDocs = i.informacionColeccionDocs;
		indiceDocs = i.indiceDocs;
		indice = i.indice;
		return *this;
	}
	void IndexadorHash::rellenaStops(){
		ifstream input;
		string word="";
		struct stat dir;
		try{

			input.open(ficheroStopWords.c_str());

			while(!input.eof() && getline(input, word)){
				if(word.length()!=0){
					stopWords.insert(word);
				}
				word="";
			}
			input.close();
		}catch(ios_base::failure & er){
			std::cerr << "ERROR: No existe el archivo: "<< ficheroStopWords << endl;
		}
	}

	IndexadorHash::~IndexadorHash(){
		vaciarIndexador();
	}
	void IndexadorHash::vaciarIndexador(){
		indice.clear();
		indiceDocs.clear();
		indicePregunta.clear();
		pregunta.clear();
		stopWords.clear();
		ficheroStopWords.clear();
		directorioIndice.clear();
		tipoStemmer = 0;

		if(tok!=NULL){
			delete tok;
			tok=NULL;
		}
	}

	IndexadorHash::IndexadorHash(const string & directorioindexacion){
		indexadorDeFichero(directorioindexacion);
	}

	void IndexadorHash::indexadorDeFichero(const string & directorioindexacion){
		ifstream input;
		string word="";
		struct stat dir;
		try{
			input.open(directorioindexacion.c_str());

			getline(input, word);
			string delimitadores = word;
			getline(input, word);
			bool detectComp = atoi( word.c_str() );
			getline(input, word);
			bool minuscSinAcentos = atoi( word.c_str() );
			tok = new Tokenizador(delimitadores,detectComp,minuscSinAcentos);
			tok->indexador = this;

			getline(input, word);
			this->directorioIndice = word;
			getline(input, word);
			this->tipoStemmer = atoi( word.c_str() );
			getline(input, word);
			this->almacenarEnDisco = atoi( word.c_str() );
			getline(input, word);
			this->almacenarPosTerm = atoi( word.c_str() );
			getline(input, word);
			pregunta = word;

			getline(input, word);
			this->ficheroStopWords = word;
			rellenaStops();

			Indexar("./indiceDocs.txt");

			input.close();


		}catch(ios_base::failure & er){
			std::cerr << "ERROR: No existe el archivo: "<< directorioindexacion << endl;
		}
	}

	bool IndexadorHash::GuardarIndexacion() const{

		ofstream outputFile(directorioIndice);
		outputFile<<tok->delimiters<<endl;
		outputFile<<tok->CasosEspeciales()<<endl;
		outputFile<<tok->PasarAminuscSinAcentos()<<endl;
		outputFile<<directorioIndice<<endl;
		outputFile<<tipoStemmer<<endl;
		outputFile<<almacenarEnDisco<<endl;
		outputFile<<almacenarPosTerm<<endl;
		outputFile<<pregunta<<endl;
		outputFile<<ficheroStopWords<<endl;

		outputFile.close();

		ofstream outputIndiceDocs("./indiceDocs.txt");


		auto iterEnd = indiceDocs.end();


		for(long int i=1;i<=indiceDocs.size();++i){
			auto iter = indiceDocs.begin();
			for(;iter!=iterEnd;++iter){
				if(iter->second.idDoc==i){
					outputIndiceDocs<<iter->first<<endl;
				}
			}
		}

		return true;
	}

	bool IndexadorHash::RecuperarIndexacion (const string& directorioIndexacion){
		try{
			ifstream input;
			input.open(directorioIndexacion.c_str());
			input.close();
			vaciarIndexador();
			indexadorDeFichero(directorioIndexacion);
			return true;

		}catch(ios_base::failure & er){
			std::cerr << "ERROR: No existe el archivo: "<< directorioIndexacion << endl;
			return false;
		}
	}

	bool IndexadorHash::IndexarPregunta(const string &preg){
		pregunta = preg;
		indicePregunta.clear();
		infPregunta.clear();

		list<string> tokens;
		tok->Tokenizar(preg, tokens);

		int indice=0;
		for(string palabra : tokens){
			auto iterStops = stopWords.find(palabra);
			auto iterStopsEnd = stopWords.end();
			if(iterStops==iterStopsEnd){
				stemmer.stemmer(palabra,tipoStemmer);

				InformacionTerminoPregunta infTermPreg;
				bool insertar = !DevuelvePregunta(palabra, infTermPreg);

				++infTermPreg.ft;
				infTermPreg.posTerm.insert(indice);

				if(insertar){
					++infPregunta.numTotalPalDiferentes;
					indicePregunta.insert({palabra,infTermPreg});
				}else{
					indicePregunta.find(palabra)->second=infTermPreg;
				}
				++infPregunta.numTotalPalSinParada;
			}
			++infPregunta.numTotalPal;
			++indice;
		}

		return true;
	}
	bool IndexadorHash::DevuelvePregunta(string &preg)const{
		if(!pregunta.empty()){
			preg = pregunta;
			return true;
		}
		return false;
	}
	bool IndexadorHash::DevuelvePregunta(const string& word, InformacionTerminoPregunta & inf)const{
		InformacionTerminoPregunta infTemp;
		inf = infTemp;

		auto iter = indicePregunta.find(word);
		auto iterEnd = indicePregunta.end();
		if(iter!=iterEnd){
			inf = iter->second;
			return true;
		}

		return false;
	}

	bool IndexadorHash::DevuelvePregunta(InformacionPregunta& inf)const{
		InformacionPregunta infTemp;
		inf = infTemp;

		if(!pregunta.empty()){
			inf = infPregunta;
			return true;
		}
		return false;

	}

	void IndexadorHash::ImprimirIndexacion() const {

		cout << "Terminos indexados: " << endl;

		for(auto iterIndex : indice)
				cout << iterIndex.first << '\t' << iterIndex.second << endl;
		cout << "Documentos indexados: " << endl;

		for(auto iterDocs : indiceDocs)
	        cout << iterDocs.first << '\t' << iterDocs.second << endl;
	}
	void IndexadorHash::ImprimirIndexacionPregunta(){
		cout << "Pregunta indexada: " << pregunta << endl;
		cout << "Terminos indexados en la pregunta: " << endl;

		auto iter = indicePregunta.begin();
		auto iterEnd = indicePregunta.end();

		for(;iter!=iterEnd;++iter)
			cout << iter->first << '\t' << iter->second << endl;

		cout << "Informacion de la pregunta: " << infPregunta << endl;

	}

	void IndexadorHash::ImprimirPregunta(){
		cout << "Pregunta indexada: " << pregunta << endl;
		cout << "Informacion de la pregunta: " << infPregunta << endl;
	}


    bool IndexadorHash::Indexar(const string &ficheroDocumentos ){

		ifstream fichOpen;
		fichOpen.open(ficheroDocumentos.c_str());
		string nomFichero="";
		struct stat dir;
		if(!fichOpen){
			cerr << "ERROR: No existe la lista de ficheros: " << ficheroDocumentos << endl;
			return false;
		}
		else{
			while(getline(fichOpen,nomFichero)){
				if(nomFichero.length()!=0){
					stat(nomFichero.c_str(),&dir);
					if(S_ISREG(dir.st_mode)){

						nombreDocAux = nomFichero;

				    	InfDoc infDoc;

				    	infDoc.idDoc = indiceDocs.size()+1;
				    	infDoc.fechaModificacion = *gmtime(&(dir.st_mtime));
				    	infDoc.fechaModificacion.tm_mon+=1;
				    	infDoc.fechaModificacion.tm_year+=1900;
				    	infDoc.tamBytes = dir.st_size;

				    	// Si el nombre de fichero se repite y la fecha es mas moderna, sustituimos
						auto iterDoc = indiceDocs.find(nomFichero);
						if(iterDoc!=indiceDocs.end()){
							struct tm * fechaHay = &iterDoc->second.fechaModificacion;
							struct tm * fechaEntra = &infDoc.fechaModificacion;
							//seconds = difftime(now,mktime(&newyear));
							if(0<difftime(mktime(fechaEntra),mktime(fechaHay))){
								infDoc.idDoc=iterDoc->second.idDoc;
								BorraDoc(nomFichero);
							}
							else{
								continue;
							}
						}

						infDocAux = infDoc;
						indiceDocs.insert({nomFichero,infDoc});

						tok->Tokenizar(nomFichero);

						indiceDocs.find(nomFichero)->second = infDocAux;


						informacionColeccionDocs.tamBytes+=infDoc.tamBytes;
						++informacionColeccionDocs.numDocs;
					}
				}
				nomFichero="";
			}
		}
		fichOpen.close();
		return true;

    }

	void IndexadorHash::indexarPalabraTok(const string &token,const long int &posTerm){

		string palabra = token;

		auto iterStops = stopWords.find(palabra);
		auto iterStopsEnd = stopWords.end();
		if(iterStops!=iterStopsEnd){
			++infDocAux.numPal;
			++informacionColeccionDocs.numTotalPal;
			return;
		}

		stemmer.stemmer(palabra,tipoStemmer);

		InformacionTermino infTerm;
		Devuelve(palabra, infTerm);
		++infTerm.ftc;

		InfTermDoc infTermDoc;
		Devuelve(palabra, nombreDocAux, infTermDoc);

		++infTermDoc.ft;
		if(almacenarPosTerm)
			infTermDoc.addPosTerm(posTerm);

		auto iterLdocs = infTerm.l_docs.find(infDocAux.idDoc);
		auto iterLdocsEnd = infTerm.l_docs.end();

		if(iterLdocs!=iterLdocsEnd){
			iterLdocs->second=infTermDoc;
		}
		else{
			infTerm.l_docs.insert({infDocAux.idDoc, infTermDoc});
			++infDocAux.numPalDiferentes;
		}

		if(!Inserta(palabra,infTerm)){
			Actualiza(palabra, infTerm);
		}
		else{
			++informacionColeccionDocs.numTotalPalDiferentes;
		}


		++infDocAux.numPal;
		++infDocAux.numPalSinParada;

		++informacionColeccionDocs.numTotalPal;
		++informacionColeccionDocs.numTotalPalSinParada;
	}
	bool IndexadorHash::IndexarDirectorio(const string &dirAIndexar){
		struct stat dir;
			// Compruebo la existencia del directorio
			int err=stat(dirAIndexar.c_str(), &dir);
			if(err==-1 || !S_ISDIR(dir.st_mode))
				return false;
			else {
				string cmdRemoveTk = "rm -R "+dirAIndexar+"/*.tk > /dev/null";
				system(cmdRemoveTk.c_str());
				// Hago una lista en un fichero con find>fich
				string cmd="find "+dirAIndexar+" -follow -type f |sort > listaIndexarMia.txt";
				system(cmd.c_str());
				return Indexar("listaIndexarMia.txt");
			}
	}

	bool IndexadorHash::Devuelve(const string& word, InformacionTermino& inf) const{
		InformacionTermino infTermTemp;
		inf = infTermTemp;

		auto iterIndice = indice.find(word);
		auto iterIndiceEnd = indice.end();

		if(iterIndice!=iterIndiceEnd){
			inf = iterIndice->second;
			return true;
		}
		return false;
	}

	bool IndexadorHash::Devuelve(const string& word, const string& nomDoc, InfTermDoc& infTermDoc) const{
		InfTermDoc infTermDocTemp;
		infTermDoc = infTermDocTemp;

		auto iterWord = indice.find(word);
		auto iterWordEnd = indice.end();

		if(iterWord !=iterWordEnd){
			auto iterDocs = indiceDocs.find(nomDoc);
			auto iterDocsEnd = indiceDocs.end();

			if(iterDocs !=iterDocsEnd){
				long int idDoc = iterDocs->second.idDoc;

				auto iterLdocs = iterWord->second.l_docs.find(idDoc);
				auto iterInfEnd = iterWord->second.l_docs.end();

				if(iterLdocs!=iterInfEnd){
					infTermDoc = iterLdocs->second;
					return true;
				}
			}
		}
		return false;
	}

	bool IndexadorHash::Existe(const string & word)const{
		auto iterIndice = indice.find(word);
		auto iterIndiceEnd = indice.end();
		return iterIndice!=iterIndiceEnd;
	}

	bool IndexadorHash::Borra(const string & word){
		InformacionTermino infTerm;

		if(Devuelve(word, infTerm)){
			InfTermDoc infTermDoc;

			auto iterDocs =  indiceDocs.begin();
			auto iterDocsEnd = indiceDocs.end();

			//Actualizamos la informacion de los documentos
			for(;iterDocs!=iterDocsEnd;++iterDocs){

				//Recogemos la informacion del termino en el documento
				if(Devuelve(word, iterDocs->first, infTermDoc)){
					iterDocs->second.numPal -= infTermDoc.ft;
					--iterDocs->second.numPalDiferentes;
					--iterDocs->second.numPalSinParada;
				}

				//Actializamos la coleccion
				informacionColeccionDocs.numTotalPal -= infTerm.ftc;
				informacionColeccionDocs.numTotalPalSinParada -= infTerm.ftc;
			}

			indice.erase(indice.find(word));
			--informacionColeccionDocs.numTotalPalDiferentes;
			return true;
		}
		return false;
	}
	/*
	 * Iteramos por el indice de palabras, si esta en el documento
	 * borramos su asociacion con el mismo ,restamos el número de
	 * palabras ya que su ocurrencia en el documento desaparece y
	 * por ultimo si la frecuencia del termino en la coleccion indica
	 * que este ya no se encuentra en ningun documento, lo borramos del indice
	 */
	bool IndexadorHash::BorraDoc(const string &nomDoc){

		auto iterDoc = indiceDocs.find(nomDoc);
		auto iEndDoc = indiceDocs.end();

		if(iterDoc!=iEndDoc){
			long int idDoc = iterDoc->second.idDoc;
			auto iter = indice.begin();
			auto iEnd = indice.end();

			vector<unordered_map<string, InformacionTermino>::iterator> vIteradores;

			for(;iter!=iEnd;++iter){

				if(iter->second.borrarDocumento(idDoc)){

					if(iter->second.ftc<=0){
						vIteradores.push_back(iter);
					}

				}
			}
			for(auto iterBorrar : vIteradores){
				indice.erase(iterBorrar);
				--informacionColeccionDocs.numTotalPalDiferentes;
			}

			informacionColeccionDocs.tamBytes -= iterDoc->second.tamBytes;
			informacionColeccionDocs.numTotalPal -= iterDoc->second.numPal;
			informacionColeccionDocs.numTotalPalSinParada -= iterDoc->second.numPalSinParada;
			--informacionColeccionDocs.numDocs;
			indiceDocs.erase(nomDoc);
			return true;
		}
		return false;
	}

	void IndexadorHash::VaciarIndice(){
		indice.clear();
	}

	bool IndexadorHash::Actualiza(const string & word, const InformacionTermino & inf){
		auto iter = indice.find(word);
		if(iter!=indice.end()){
			iter->second=inf;
			return true;
		}
		return false;
	}

	bool IndexadorHash::Inserta(const string & word, const InformacionTermino & inf){
		return indice.insert({word,inf}).second;
	}



/////////////////////Listar/////////////////

	void IndexadorHash::ListarPalParada() const{
		auto iter = stopWords.begin();
		auto  sEnd = stopWords.end();

		for (;iter != sEnd; ++iter)
			cout<< *iter<<endl;
	}
	void IndexadorHash::ListarInfColeccDocs() const{
		cout << informacionColeccionDocs <<endl;
	}
	void IndexadorHash::ListarTerminos() const{
		auto iter = indice.begin();
		auto sEnd = indice.end();

		for (;iter != sEnd; ++iter)
			cout << iter->first << '\t' << iter->second <<endl;
	}

	bool IndexadorHash::ListarTerminos(const string& nomDoc) const{

		auto iter1 = indiceDocs.find(nomDoc);

		if(iter1 != indiceDocs.end()){
			//recogemos el id del indice
			long int id = iter1->second.idDoc;

			auto iter = indice.begin();
			auto sEnd = indice.end();

			for (;iter != sEnd; ++iter){
				if(iter->second.l_docs.find(id)!=iter->second.l_docs.end()){
					//si encuentra el id en l_docs
					cout << iter->first << '\t' << iter->second <<endl;
				}
			}
			return true;
		}
		else{
			return false;
		}

	}

	void IndexadorHash::ListarDocs() const{

		auto iter = indiceDocs.begin();
		auto iEnd = indiceDocs.end();

		for(;iter!=iEnd;++iter)
			cout<< iter->first << '\t' << iter->second << endl;
	}

	bool IndexadorHash::ListarDocs(const string& nomDoc) const{
		auto iter = indiceDocs.find(nomDoc);

		if(iter != indiceDocs.end()){
			cout<< nomDoc << '\t' << iter->second <<endl;
			return true;
		}
		else {
			return false;
		}
	}


	string  IndexadorHash::DevolverDelimitadores () const{ return tok->delimiters;}
	bool IndexadorHash::DevolverCasosEspeciales () const{ return tok->CasosEspeciales();}
	bool IndexadorHash::DevolverPasarAminuscSinAcentos () const{ return tok->PasarAminuscSinAcentos();}



    ostream& operator<<(ostream& s, const IndexadorHash& p){
		s << "Fichero con el listado de palabras de parada: " <<
		p.ficheroStopWords << endl;
		s << "Tokenizador: " << *p.tok << endl;
		s << "Directorio donde se almacenara el indice generado: " <<
		p.directorioIndice << endl;
		s << "Stemmer utilizado: " << p.tipoStemmer << endl;
		s << "Informacion de la coleccion indexada: "<< p.informacionColeccionDocs<< endl;
		s << "Se almacenara parte del indice en disco duro: "<<p.almacenarEnDisco << endl;
		s << "Se almacenaran las posiciones de los terminos: "<<p.almacenarPosTerm << endl;

		return s;
    }


	int IndexadorHash::NumPalIndexadas() const{ return indice.size();}

	string IndexadorHash::DevolverFichPalParada () const{ return ficheroStopWords;}

	int IndexadorHash::NumPalParada() const{ return stopWords.size();}






