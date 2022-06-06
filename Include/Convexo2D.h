#ifndef CONVEXO3D_H
#define CONVEXO3D_H

// Funciones generales
#include <General.h>

// Se declaran los tipos a usar
static declare_type(Punto);

// - p          Coordenadas del punto
// - indice     Indice identificador del punto
// - eliminar   Indica si el punto está marcado para ser eliminado
struct Punto
{
	Punto2D p;
	uint32 indice;
	bool eliminar;
};

// Tipo de dato 'Puntos' que consiste en un arreglo de 'Punto'
// - Puntos = Array(Punto)
typedef Array Puntos;

// Funciones de entrada de datos
void agregar_punto (Puntos puntos, int32 x, int32 y);

// Funcion principal
Array cierre_convexo_2d (Puntos puntos);

#endif // CONVEXO3D_H