//
//  main.cpp
//  QuijotePalabrasDiferentes
//
//  Created by VANGELI ONTIVEROS on 28/07/14.
//  Copyderecha (c) 2014 VANGELI ONTIVEROS. All derechas reserved.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>

using namespace std;

/* PROPUESTA DE SOLUCION DEL PROBLEMA DE PROGRAMACION LUDICA
http://www.unocero.com/2014/07/19/programacion-ludica-cuantas-palabras-diferentes-tiene-el-quijote/
*/

/* IMPLEMENTACION ARBOL BINARIO:
	Cualquier implementacion de arbol binario sirve. Modificado para considerar el contador de palabras
	Ejem: http://www.thegeekstuff.com/2013/02/c-binary-tree/
 */
struct nodo    //estructura nodo del arbol
{
  string palabra;
  int contador;
  nodo *izquierda;
  nodo *derecha;
};

class binaryTree  //clase que implemeta los métodos del arbol
{
    public:
        binaryTree();

        void agregarPalabra(string palabra);
        void eliminar_arbol();
        void print_inorder(nodo *nodoRaiz,ofstream *outputStream);
        
        nodo *root;

    private: //metodos "genericos del arbol"
        void destroy_tree(nodo *hoja);
        void insert(string palabra, nodo *hoja);
        
};

binaryTree::binaryTree()
{
  root=NULL;     //inicializar en Null el nodo raiz en el constructor
}

void binaryTree::destroy_tree(nodo *hoja)   //destruye un nodo y sus decendientes de manera recursiva
{
  if(hoja!=NULL)
  {
    destroy_tree(hoja->izquierda);
    destroy_tree(hoja->derecha);
    delete hoja;
  }
}

void binaryTree::insert(string palabra, nodo *hoja)  //inserta un nodo en el arbol
{
  if(palabra.compare(hoja->palabra)<0) //si la palabra es "menor" se inserta del lado izquierdo
  {
    if(hoja->izquierda!=NULL)         //busqueda recursiva hasta alcanzar el nodo hoja
     insert(palabra, hoja->izquierda);
    else
    {
      hoja->izquierda=new nodo;     //alcanzado el nodo hoja, se crea un nuevo hijo
      hoja->izquierda->palabra=palabra;
      hoja->izquierda->contador=1;    //se inicializa su contador en 1
      hoja->izquierda->izquierda=NULL;   
      hoja->izquierda->derecha=NULL;   
    }  
  }
  else if(palabra.compare(hoja->palabra)==0){    //si la palabra es "igual", sólo aumentamos en contador del nodo
  	hoja->contador++;
  }
  else if(palabra.compare(hoja->palabra)>0)  //si la palabra es "mayor" se inserta el nodo del lado derecho
  {
    if(hoja->derecha!=NULL)
      insert(palabra, hoja->derecha);
    else
    {
      hoja->derecha=new nodo;
      hoja->derecha->palabra=palabra;
      hoja->derecha->contador=1;
      hoja->derecha->izquierda=NULL;  
      hoja->derecha->derecha=NULL;
    }
  }
}

void binaryTree::agregarPalabra(string palabra)
{
  if(root!=NULL)  //si el nodo raiz es diferente de null, comienza la insercion recursiva 
    insert(palabra, root);
  else                //se trata del nodo raíz, por tanto se crea un nuevo nodo
  {
    root=new nodo;
    root->palabra=palabra;
    root->contador=1;
    root->izquierda=NULL;
    root->derecha=NULL;
  }
}

void binaryTree::eliminar_arbol() //libera la memoria eliminando los nodos desde la raíz
{
  destroy_tree(root);
}

void binaryTree::print_inorder(nodo *nodoRaiz,ofstream *outputStream)  //nos permite imprimir el  nodo de manera ordenada
{
    if (nodoRaiz)
    {
        print_inorder(nodoRaiz->izquierda,outputStream);
        //cout << nodoRaiz->palabra <<" " <<nodoRaiz->contador<<"\n";
        *outputStream << nodoRaiz->palabra <<" " <<nodoRaiz->contador<<"\n";
        print_inorder(nodoRaiz->derecha,outputStream);
    }
}

/* FIN ARBOL BINARIO */


/* COMIENZA PROGRAMA*/

// función que pasamos como filtro para remover cualquier caracter que no sea letra
//(ver uso mas abajo)
bool soloLetras(char c)
{
    return isalpha( c ) == 0;
}


int main()
{
	
	binaryTree *arbolBinario = new binaryTree;  //nuevo arbol
	
    const string archivo = "pg2000.txt";  //el archivo a leer
    
    ifstream input;
    input.open( archivo.c_str() );
    
    if ( !input )
    {
        cout << "ERROR AL ABRIR EL ARCHIVO\n";
        return 0;
    }
    
    cout << "*** CONTADOR DE PALABRAS DEL QUIJOTE ***\n\n";
    string tok;  //variable que almacena cada token ó palabra del stream
    
    /*estructuras  para medir el tiempo de ejecucion
	extraido de: http://es.cppreference.com/w/cpp/chrono
	*/
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    /////////////////////////
    
    
    while ( input )
    {
    	
    	/*el operador >> nos permite extraer caracteres del stream hasta que se encuentre un espacio o EOF
			   http://www.cplusplus.com/reference/istream/istream/operator%3E%3E/
		*/
        input >> tok;
    
        /*necesitamos eliminar cualquier caracter especial de la palabra tal como ?,!, etc 
		  para ello usamos los metodos erase para eliminar caracteres de un string junto con
		  la funcion remove_if que permite pasar una funcion "filtro"
		  
		  http://www.cplusplus.com/reference/string/string/erase/
		  http://www.cplusplus.com/reference/algorithm/remove_if/?kw=remove_if
		
		*/
        tok.erase(remove_if(tok.begin(),tok.end(),soloLetras),tok.end());
        
        
        /*convertimos el string a minusculas
		http://notfaq.wordpress.com/2007/08/04/cc-convert-string-to-upperlower-case/
		*/
        transform(tok.begin(), tok.end(), tok.begin(), ::tolower);
        
        
        /*
        mientras que la palabra no sea vacia, la insertamos en el arbol
		*/
        if ( tok.empty()==false )
        {
        	arbolBinario->agregarPalabra(tok);
        }
        
    }


	/* finalizo la incersión y conteo, imprimir el tiempo de ejecucion*/
	end = std::chrono::system_clock::now();
 
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds << "ms\n";


	/*
	Imprimir el arbol en un archivo de texto
	
	http://www.cplusplus.com/doc/tutorial/files/
	*/
	ofstream archivoDeSalida;
 	archivoDeSalida.open ("resultadoContadorPalabras.txt",ios::out|ios::trunc);
	arbolBinario->print_inorder(arbolBinario->root,&archivoDeSalida);
    archivoDeSalida.close();
    
    //liberar memoria
   	arbolBinario->eliminar_arbol();
   	
   	cout << "*** PROGRAMA TERMINADO CON EXITO ***\n\n";
    cin.get();
    return 0;
}


