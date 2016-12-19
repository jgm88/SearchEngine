# SearchEngine
A little search engine made in C++

##  Qué se pide
Se pide construir la clase Buscador a partir de la indexación generada en la práctica anterior
(IndexadorHash), implementando los modelos DFR y BM25.
Se valorará la eficiencia de la implementación, especialmente cualquier modificación en la
representación interna (parte privada de las clases) o el algoritmo utilizado.

##  Modelo de similitud Deviation From Ramdomness (DFR)

![DFR](https://github.com/jgm88/SearchEngine/blob/master/imgs/1.PNG)

Siendo:

*  q: la query o pregunta que realiza el usuario
* d: el documento del que se calcula su valor de similitud simq, d respecto a la
pregunta q que realiza el usuario
* k: número de términos de la query q
* wi,q: peso en la query del término i de la query q
* wi,d: peso en el documento del término i de la query q
* ft: número total de veces que el término t aparece en toda la colección
* ft,d: número de veces que el término t aparece en el documento d
* ft,q: número de veces que el término t aparece en la query q
* nt: número de documentos en los que aparece el término t
* ld: longitud en bytes del documento
* avr_ld: media en bytes del tamaño de los documentos
* N: cantidad de documentos en la colección
* Valor recomendado de c = 2
* t es la razón entre la frecuencia del término en la colección y la cantidad de
documentos en la colección

##  Modelo de similitud Okapi BM25

![](https://github.com/jgm88/SearchEngine/blob/master/imgs/2.PNG)

Siendo:
* score(D, Q) : valor de similitud para el documento D respecto a la pregunta Q
que realiza el usuario
* n : número de términos de la query Q
* f(qi, D) : la frecuencia del término qi en el documento D
* |D| : el número de palabras (no de parada) del documento D
* avgdl: la media de todas las |D| en la colección
* N: cantidad de documentos en la colección
* n(qi): número de documentos en los que aparece el término qi
* Constantes para configuración: k1 = 1,2 b = 0,75
