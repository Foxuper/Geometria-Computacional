#ifndef GENERAL_H
#define GENERAL_H

#include <Principal.h>

// - X   Coordenada x del punto
// - Y   Coordenada y del punto
// - Z   Coordenada z del punto
#define X 0
#define Y 1
#define Z 2

// Tipo de dato 'Punto2D' que sirve para representar puntos 2D
typedef int32 Punto2D[2];
typedef int32 Punto3D[3];

// Funciones primitivas generales
bool  xor   (bool a, bool b);
int32 signo (float x);

// Funciones de area y sus derivados
float area2           (Punto2D a, Punto2D b, Punto2D c);
int32 signo_area      (Punto2D a, Punto2D b, Punto2D c);
bool  izquierda       (Punto2D a, Punto2D b, Punto2D c);
bool  derecha         (Punto2D a, Punto2D b, Punto2D c);
bool  colineal        (Punto2D a, Punto2D b, Punto2D c);
bool  izquierda_sobre (Punto2D a, Punto2D b, Punto2D c);
bool  derecha_sobre   (Punto2D a, Punto2D b, Punto2D c);

// Funciones generales para arreglos
var anterior  (var arreglo, var valor);
var siguiente (var arreglo, var valor);

#endif // GENERAL_H