/*
 * tokenizadorClase.h
 *
 *  Created on: 30/01/2014
 *      Author: javi
 */

#include "tokenizador.h"


void Tokenizador::initMapAcentos(){

	sAcentos = "àáâäÀÁÂÄèéêëÈÉÊËìíîïÌÍÎÏòóôöÒÓÔÖùúûüÙÚÛÜ";
	string sNoAcentos = "aaaaaaaaeeeeeeeeiiiiiiiioooooooouuuuuuuu";

	unsigned short size = sNoAcentos.length();
	unsigned short sizeAcent = sNoAcentos.length();

	unsigned short i = 0;
	unsigned short j =0;
	for(;i<size;++i, j+=2){
		mapAcentos.insert(std::make_pair(sAcentos.substr(j,2),sNoAcentos.substr(i,1)));
	}
	mapAcentos.insert(std::make_pair("ñ","ñ"));
}

void Tokenizador::initmapEspeciales(){
	mapEspeciales.insert(std::make_pair('-',&isCompuesta));
	mapEspeciales.insert(std::make_pair('@',&isEmail));
	mapEspeciales.insert(std::make_pair('.',&isPunto));
	mapEspeciales.insert(std::make_pair(',',&isComa));
	mapEspeciales.insert(std::make_pair(':',&isURL));
}

Tokenizador::Tokenizador(){
	char s[]=",;:.-/+*\\ '’\"{}[]()<>¡!¿?&#=·•\t\n\r@";
		delimiters = s;
		unsigned short size = strlen(s)+1;
		delimitersSet.insert(s,s+size);

	casosEspeciales=true;
	pasarAminuscSinAcentos=false;
	initMapAcentos();
	initmapEspeciales();

	inicioConEspeciales=true;
	delimitersSet.insert(' ');
}

Tokenizador::Tokenizador (const string& delimitadoresPalabra, const bool& detectComp, const bool& minuscSinAcentos){

	casosEspeciales=detectComp;
	pasarAminuscSinAcentos=minuscSinAcentos;
	if(minuscSinAcentos) initMapAcentos();
	if(detectComp){
		initmapEspeciales();
		inicioConEspeciales=true;
		delimitersSet.insert(' ');
	}
	else{
		inicioConEspeciales=false;
	}
	if(&delimitadoresPalabra!=NULL){
		delimiters=delimitadoresPalabra;
		unsigned short size = delimitadoresPalabra.length()+1;
		char delimAux [size];
		memcpy(delimAux, delimitadoresPalabra.c_str(), size);
		delimitersSet.insert(delimAux,delimAux+size);
	}
}
Tokenizador::Tokenizador (const string& delimCopia){
	casosEspeciales=false;
	pasarAminuscSinAcentos=false;
	initMapAcentos();
	initmapEspeciales();

	if(&delimCopia!=NULL){
		delimiters=delimCopia;
		unsigned short size = delimCopia.length()+1;
		char delimAux [size];
		memcpy(delimAux, delimCopia.c_str(), size);
		delimitersSet.insert(delimAux,delimAux+size);
	}
	inicioConEspeciales=false;

}
Tokenizador::~Tokenizador (){
	delimitersSet.clear();
	delimiters.clear();
}
Tokenizador & Tokenizador::operator=(const Tokenizador &t){
	if(this!=&t && &t!=NULL)
		delimiters = t.delimiters;
		delimitersSet = t.delimitersSet;
	return(*this);
}

void Tokenizador::CasosEspeciales (const bool& nuevoCasosEspeciales){
	casosEspeciales=nuevoCasosEspeciales;
	if(nuevoCasosEspeciales && inicioConEspeciales==false){
		delimitersSet.insert(' ');
		initmapEspeciales();
	}
	else if(nuevoCasosEspeciales==false){
		borrarDelimiter(' ');
	}
}

bool Tokenizador::CasosEspeciales ()const{
	return casosEspeciales;
}

void Tokenizador::PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos){
	pasarAminuscSinAcentos=nuevoPasarAminuscSinAcentos;
	if(nuevoPasarAminuscSinAcentos) initMapAcentos();
}

bool Tokenizador::PasarAminuscSinAcentos ()const{
	return pasarAminuscSinAcentos;
}

void Tokenizador::quitaAcentos(string &token)const{

	unsigned short size = token.length();
	string s="";
	string::size_type iter=token.find_first_of(sAcentos,0);
	while(iter!=string::npos && iter<=size){
		s = token.substr(iter,2);
		if(s!="ñ" && mapAcentos.find(s)!=mapAcentos.end()){
			token.replace(iter,2,mapAcentos.at(s));
			iter=token.find_first_of(sAcentos,iter);
		}
		else iter+=2;
	}
}

////////------TRATAMIENTO CASOS ESPECIALES------//////


bool Tokenizador::isURL(const string& input, unsigned short &pos,
		const set<wchar_t> &delimitersSet, unsigned short &estado){

	set<wchar_t>::iterator notDel = delimitersSet.end();
	unsigned short avance = pos+1;

	while(input[avance]=='_' ||input[avance]==':' ||input[avance]=='/' ||
			input[avance]=='.' ||input[avance]=='?' ||input[avance]=='&' ||
			input[avance]=='-' ||input[avance]=='=' ||input[avance]=='#' ||
			input[avance]=='@' ||delimitersSet.find(input[avance])==notDel){
		++avance;
	}
	pos = avance;
	return true;
}
bool Tokenizador::isEmail(const string& input, unsigned short &pos,
		const set<wchar_t> &delimitersSet, unsigned short &estado){

	set<wchar_t>::iterator notDel = delimitersSet.end();

	//comprobamos si la posicion siguiente es delimitador y avanzamos
	unsigned short avance = pos+1;

	while(delimitersSet.find(input[avance])==notDel){
		++avance;
	}
	if(pos!=avance-1){
		//EmailON
		estado=1;
		pos=avance;
		return true;
	}
	return false;
}
bool Tokenizador::isCompuesta(const string& input, unsigned short &pos,
		const set<wchar_t> &delimitersSet, unsigned short &estado){

	set<wchar_t>::iterator notDel = delimitersSet.end();
	unsigned short avance = pos+1;

	while(input[avance]=='-' || delimitersSet.find(input[avance])==notDel){
		++avance;
	}
	if(pos!=avance-1){
		pos=avance;
		return true;
	}
	return false;

}

bool Tokenizador::isPunto(const string& input, unsigned short &pos,
		const set<wchar_t> &delimitersSet, unsigned short &estado){

	set<wchar_t>::iterator notDel = delimitersSet.end();
	unsigned short avance = pos+1;

	//EmailOn
	if(estado==1){

		while(input[avance]=='.' || input[avance]=='-'
		|| input[avance]=='_' || delimitersSet.find(input[avance])==notDel){
			++avance;
		}
		if(pos!=avance-1){
			pos=avance;
			return true;
		}
	}
	while(delimitersSet.find(input[avance])==notDel){
		++avance;
	}
	if(pos!=avance-1){
		pos=avance;
		return true;
	}
	return false;

}
bool Tokenizador::isComa(const string& input, unsigned short &pos,
		const set<wchar_t> &delimitersSet, unsigned short &estado){

	set<wchar_t>::iterator notDel = delimitersSet.end();

	//comprobamos si la posicion siguiente es delimitador y avanzamos
	unsigned short avance = pos+1;

	while((delimitersSet.find(input[avance])==notDel && !isalpha(input[avance]))
	|| input[avance]=='E' ||input[avance]=='+'){
		++avance;
	}
	if(pos!=avance-1){
		//EmailON
		estado=1;
		pos=avance;
		return true;
	}
	return false;
}

//////////----CASOS TOKENIZAR--------////////

void Tokenizador::Tokenizar (const string& str, ofstream &f, long int &posTerm) const{
	string::size_type lastpos = str.find_first_not_of(delimiters,0);
	string::size_type pos = str.find_first_of(delimiters,lastpos);
	while(string::npos != pos || string::npos != lastpos)
	{
		string token = str.substr(lastpos, pos - lastpos);

		f<< token<<endl;
		indexador->indexarPalabraTok(token, posTerm);

		lastpos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastpos);
		++posTerm;
	}
}
void Tokenizador::TokenizarSin (const string& str, list<string>& tokens) const{

		string::size_type lastpos = str.find_first_not_of(delimiters,0);
		string::size_type pos = str.find_first_of(delimiters,lastpos);
		while(string::npos != pos || string::npos != lastpos)
		{
			tokens.push_back(str.substr(lastpos,pos - lastpos));
			lastpos = str.find_first_not_of(delimiters, pos);
			pos = str.find_first_of(delimiters, lastpos);
		}


		//	unsigned short size = str.size();
		//	unsigned short lastPost = 0;
		//	unsigned short pos = 0;
		//	set<wchar_t>::iterator notDel = delimitersSet.end();
		//
		//	for(;pos<=size;++pos){
		//		if(delimitersSet.find(str[pos])!=notDel){
		//			if(delimitersSet.find(str[lastPost])==notDel)
		//				tokens.push_back(str.substr(lastPost,pos - lastPost));
		//			while(pos<=size && delimitersSet.find(str[pos])!=notDel){
		//				++pos;
		//				lastPost = pos;
		//			}
		//		}
		//	}
}
bool Tokenizador::Tokenizar(const string& NomFichEntr, const string& NomFichSal) const{
	long int posTerm = 0;
	ifstream i;
	ofstream f;
	string cadena;
	list<string> tokens;
	i.open(NomFichEntr.c_str());
	f.open(NomFichSal.c_str());
	//posibles errores
	if(!i || i.bad() || i.fail()){
		cerr << "ERROR: No existe el archivo: " << NomFichEntr << endl;
		return false;
	}
	else{
		while(!i.eof()){
			cadena="";
			getline(i, cadena);
			if(cadena.length()!=0){
				Tokenizar(cadena, f, posTerm);
			}
		}
	}
	i.close();
	f.close();
	return true;
}
bool Tokenizador::Tokenizar (const string & NomFichEntr) const{

	return Tokenizar(NomFichEntr, NomFichEntr+".tk");
}
bool Tokenizador::TokenizarListaFicheros (const string& lista) const{

	ifstream input;
	string url;
	struct stat dir;
	input.open(lista.c_str());
	if(!input){
		cerr << "ERROR: No existe el archivo: " << lista << endl;
		return false;
	}
	else{
		while(getline(input, url)){
			if(url.length()!=0){
				stat(url.c_str(),&dir);
				if(S_ISREG(dir.st_mode))
					Tokenizar(url);
			}
			url="";
		}
	}
	input.close();
	return true;
}
bool Tokenizador::TokenizarDirectorio(const string &dirAIndexar)const{
	struct stat dir;
	// Compruebo la existencia del directorio
	int err=stat(dirAIndexar.c_str(), &dir);
	if(err==-1 || !S_ISDIR(dir.st_mode))
		return false;
	else {
		// Hago una lista en un fichero con find>fich
		string cmd="find "+dirAIndexar+" -follow |sort > .lista_fich";
		system(cmd.c_str());
		return TokenizarListaFicheros(".lista_fich");
	}
}

void Tokenizador::TokenizarEsp(const string& input, list<string>& tokens)const{


	string token;
	//ESTADOS = ConstruyeEmail, EsAcronimo, EsNumero
	unsigned short estado=0;
	unsigned short size = input.size();
	unsigned short lastPost = 0;
	unsigned short pos = 0;
	set<wchar_t>::iterator notDel = delimitersSet.end();
	map<char,funcion>::const_iterator notEsp = mapEspeciales.end();

	//MAPESPECIALES FUNCTION debe ser un BOOL, no permite hacer llamadas
	//iterativas a la HASH desde funciones staticas en su interior

	for(;pos<=size;++pos){
		////PARCHE URL////
		if(input[pos]==':'){

			if((pos-lastPost==3 && input.substr(pos-3,3) == "ftp")
			||(pos-lastPost==4 && input.substr(pos-4,4) == "http")
			||(pos-lastPost==5 && input.substr(pos-5,5) == "https")){

				mapEspeciales.at(':')(input,pos,delimitersSet, estado);
				tokens.push_back(input.substr(lastPost,pos - lastPost));
				lastPost = pos;
			}
		}
		////FIN PARCHE URL////

		//Si LP no es un Delimiter y Pos si que lo es, sacamos la substring
		//y actualizamos LP
		else if(delimitersSet.find(input[pos])!=notDel
		&& delimitersSet.find(input[lastPost])==notDel){

			//BUSCA en HASHESPECIALES[pos];

			while(mapEspeciales.find(input[pos])!=notEsp
			&& mapEspeciales.at(input[pos])(input,pos,delimitersSet, estado));

			//Los ACRONIMOS que te ALEGRAN
			token = input.substr(lastPost,pos - lastPost);
			if((token[0]=='.' || token[token.size()-1]=='.')
				&& delimitersSet.find('.')==notDel){

				bool esAcro = false;
				for(unsigned short i=1;i<token.size()-1;++i){
					if(token[i]=='.') esAcro=true;
				}
				if(esAcro){
					if(token[0]=='.'){
						token=token.substr(1,token.size());
					}
					if(token[token.size()-1]=='.'){
						token=token.substr(0,token.size()-1);
					}
				}
			}
			if((token[0]=='.'||token[0]==',') && !isalpha(token[1])){
				token="0"+token;
			}
			//FIN ACRONIMOS
			tokens.push_back(token);
			lastPost = pos;
			//EmailOFF
			estado=0;
		}
		if(delimitersSet.find(input[pos])==notDel
		&& delimitersSet.find(input[lastPost])!=notDel){
			lastPost = pos;
		}
	}
}
void Tokenizador::TokenizarMayus(const string& input, list<string>& tokens)const{

	string token;
	//ESTADOS = ConstruyeEmail, EsAcronimo, EsNumero
	unsigned short estado=0;
	unsigned short size = input.size();
	unsigned short lastPost = 0;
	unsigned short pos = 0;
	set<wchar_t>::iterator notDel = delimitersSet.end();
	map<char,funcion>::const_iterator notEsp = mapEspeciales.end();

	//MAPESPECIALES FUNCTION debe ser un BOOL, no permite hacer llamadas
	//iterativas a la HASH desde funciones staticas en su interior

	for(;pos<=size;++pos){

		//Si LP no es un Delimiter y Pos si que lo es, sacamos la substring
		//y actualizamos LP
		if(delimitersSet.find(input[pos])!=notDel
		&& delimitersSet.find(input[lastPost])==notDel){

			token = input.substr(lastPost,pos - lastPost);
			for(unsigned short i=0;i<token.size();++i){
				token[i]=tolower(token[i]);
			}
			////FUNCION QUITAR ACENTOS///
			quitaAcentos(token);
			tokens.push_back(token);
			lastPost = pos;
			//EmailOFF
			estado=0;
		}
		if(delimitersSet.find(input[pos])==notDel
		&& delimitersSet.find(input[lastPost])!=notDel){
			lastPost = pos;
		}
	}
}
void Tokenizador::TokenizarEspMayus (const string& input, list<string>& tokens)const{

	string token;
	//ESTADOS = ConstruyeEmail, EsAcronimo, EsNumero
	unsigned short estado=0;
	unsigned short size = input.size();
	unsigned short lastPost = 0;
	unsigned short pos = 0;
	set<wchar_t>::iterator notDel = delimitersSet.end();
	map<char,funcion>::const_iterator notEsp = mapEspeciales.end();

	//MAPESPECIALES FUNCTION debe ser un BOOL, no permite hacer llamadas
	//iterativas a la HASH desde funciones staticas en su interior

	for(;pos<=size;++pos){

		////PARCHE URL////
		if(input[pos]==':'){

			if((pos-lastPost==3 && input.substr(pos-3,3) == "ftp")
			||(pos-lastPost==4 && input.substr(pos-4,4) == "http")
			||(pos-lastPost==5 && input.substr(pos-5,5) == "https")){

				mapEspeciales.at(':')(input,pos,delimitersSet, estado);
				string token = input.substr(lastPost,pos - lastPost);
				for(unsigned short i=0;i<token.size();++i){
					token[i]=tolower(token[i]);
				}
				tokens.push_back(token);
				lastPost = pos;
			}
		}
		////FIN PARCHE URL////

		//Si LP no es un Delimiter y Pos si que lo es, sacamos la substring
		//y actualizamos LP
		else if(delimitersSet.find(input[pos])!=notDel
		&& delimitersSet.find(input[lastPost])==notDel){

			//BUSCA en HASHESPECIALES[pos];

			while(mapEspeciales.find(input[pos])!=notEsp
			&& mapEspeciales.at(input[pos])(input,pos,delimitersSet, estado));

			//Los ACRONIMOS que te ALEGRAN///
			token = input.substr(lastPost,pos - lastPost);

			if((token[0]=='.' || token[token.size()-1]=='.')
				&& delimitersSet.find('.')==notDel){

				bool esAcro = false;
				for(unsigned short i=1;i<token.size()-1;++i){
					if(token[i]=='.') esAcro=true;
				}
				if(esAcro){
					if(token[0]=='.'){
						token=token.substr(1,token.size());
					}
					if(token[token.size()-1]=='.'){
						token=token.substr(0,token.size()-1);
					}
				}
			}
			//FIN ACRONIMOS

			quitaAcentos(token);

			for(unsigned short i=0;i<token.size();++i){
				token[i]=tolower(token[i]);
			}

			tokens.push_back(token);
			lastPost = pos;
			//EmailOFF
			estado=0;
		}
		if(delimitersSet.find(input[pos])==notDel
		&& delimitersSet.find(input[lastPost])!=notDel){
			lastPost = pos;
		}
	}
}

void Tokenizador::Tokenizar (const string& input, list<string>& tokens)const{
	tokens.clear();
	if(casosEspeciales && pasarAminuscSinAcentos==false)
		TokenizarEsp(input,tokens);
	else if(casosEspeciales && pasarAminuscSinAcentos)
		TokenizarEspMayus(input,tokens);
	else if(casosEspeciales==false && pasarAminuscSinAcentos)
		TokenizarMayus(input,tokens);
	else TokenizarSin(input, tokens);
}

void Tokenizador::DelimitadoresPalabra(const string& nuevoDelimiters){
	if(&nuevoDelimiters != NULL){
		delimiters = nuevoDelimiters;
		delimitersSet.clear();
		unsigned short size = nuevoDelimiters.length()+1;
		char delimAux [size];
		memcpy(delimAux, nuevoDelimiters.c_str(), size);
		delimitersSet.insert(delimAux,delimAux+size);
		if(casosEspeciales) delimitersSet.insert(' ');
	}
}
string Tokenizador::DelimitadoresPalabra() const{
	return delimiters;
}
ostream& operator<<(ostream& os, const Tokenizador& t){
	os<<"DELIMITADORES: "<<t.DelimitadoresPalabra();
	os<<" TRATA CASOS ESPECIALES: "<< t.CasosEspeciales();
	os<<" PASAR A MINUSCULAS Y SIN ACENTOS: "<<t.PasarAminuscSinAcentos();
	return os;
}
void Tokenizador::borrarDelimiter(char a){
	delimitersSet.erase(a);
}
void Tokenizador::anyadirSet(char a){
	delimitersSet.insert(a);
}
void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters){
	if(&nuevoDelimiters != NULL){
		unsigned short size = nuevoDelimiters.length()+1;
		unsigned short i = 0;
		for(;i<size-1;++i)
			//string::npos Maximum value for size_t
			if(delimiters.find_first_of(nuevoDelimiters[i])==string::npos)
				delimiters.push_back(nuevoDelimiters[i]);

		char delimAux [size];
		memcpy(delimAux, nuevoDelimiters.c_str(), size);
		delimitersSet.insert(delimAux,delimAux+size);
	}
}
