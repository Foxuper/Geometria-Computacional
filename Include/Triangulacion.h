#ifndef TRIANGULACION_H
#define TRIANGULACION_H

// Funciones generales
#include <General.h>

// Funciones matemáticas
#include <math.h>

// Se declaran los tipos a usar
static declare_type(Vertice);

// - punto      Coordenadas del vértice
// - es_oreja   Indica si el vértice es una oreja
struct Vertice
{
	Punto2D punto;
	bool es_oreja;
};

// Tipo de dato 'Poligono' que consiste en un arreglo de 'Vertice'
// - Poligono = Array(Vertice)
typedef Array Poligono;

// Funciones de entrada de datos
void tri_agregar_vertice (Poligono poligono, int32 x, int32 y);

// Funciones principal de area de poligonos
float area2_poli (Poligono poligono);

// Funcion principal de triangulacion de poligonos
Array triangular (Poligono poligono);

#endif // TRIANGULACION_H