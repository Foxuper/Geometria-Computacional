#ifndef CONVEXO3D_H
#define CONVEXO3D_H

// Funciones generales
#include <General.h>

// Funciones de operaciones vectoriales
#undef to
#include <raylib/src/raymath.h>
#define to ,

// Se declaran los tipos a usar
extern declare_type(Vertice);
extern declare_type(Arista);
extern declare_type(Cara);

// - punto       Coordenadas del vertice
// - incidente   Nueva arista que incide en el vertice
// - en_cierre   Indica si el vertice esta sobre la frontera del cierre
// - procesado   Indica si el vertice ya fue procesado
struct Vertice
{
	Punto3D punto;
	Arista incidente;
	bool en_cierre;
	bool procesado;
};

// - extremos     Los dos vertices extremos de la arista
// - adyacentes   Las dos caras adyaentes a la arista
// - eliminar     Indica si la arista debe ser eliminada
// - procesada    Indica si la arista ya fue procesada
struct Arista
{
	Vertice extremos[2];
	Cara adyacentes[4];
	bool eliminar;
	bool procesada;
};

// - vertices   Los tres vertices que forman la cara
// - aristas    Las tres aristas que forman la cara
// - coplanar   Indica si la cara es coplanar con cierto vertice
// - visible    Indica si la cara es visible desde cierto vertice
struct Cara
{
	Vertice vertices[3];
	Arista aristas[3];
	bool coplanar;
	bool visible;
};

// - vertices   Lista de vertices que se usan como entrada
// - aristas    Aristas generadas que forman el cierre convexo
// - caras      Caras generadas que forman el cierre convexo
// - es_3d      Indica si el cierre convexo es tridimensional
typedef struct Datos
{
	List vertices;
	List aristas;
	List caras;
}
Datos;

// Funciones de entrada de datos
Vertice c3d_agregar_vertice (Datos datos, int32 x, int32 y, int32 z);

// Funciones de conversion
Vector3 a_vector (Vertice a);
Data3   a_data3  (Datos datos, List vertices);

// Funciones primitivas
bool dentro_de_cara (Cara cara, Vertice vertice);

// Funciones de limpieza de datos
void limpiar (Datos datos, Vertice *v_siguiente);

// Funcion principal
void construir_cierre (Datos datos);

#endif // CONVEXO3D_H