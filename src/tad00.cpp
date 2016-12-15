#include "indexadorHash.h"

using namespace std;

int main() {

	/*
	* TADS TRIE
	*/

	Trie* trie = new Trie();
		trie->addPalabra("Cosa");
		trie->addPalabra("Proveer");
		trie->addPalabra("Proveeres");
		trie->addPalabra("Pro");

		if ( trie->findPalabra("Cos") )
			cout << "Existe Cos" << endl;

		if ( trie->findPalabra("Cosa") )
			cout << "Existe Cosa" << endl;

		if ( trie->findPalabra("Cosaa") )
			cout << "Existe Cosaa" << endl;

		if ( trie->findPalabra("Pro") )
			cout << "Existe Pro" << endl;

		if(trie->delPalabra("Proveer"))
			cout << "Borrado Proveer"<< endl;

		if ( trie->delPalabra("Proveer") )
			cout << "Existe Proveer" << endl;

		if ( trie->delPalabra("Pro") )
			cout << "Borrado Pro" << endl;

		if ( trie->delPalabra("proveer") )
			cout << "Existe proveer" << endl;

	   cout << (*trie);
	delete trie;

}