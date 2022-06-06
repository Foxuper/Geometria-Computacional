#include <General.h>

// Regresa verdadero si y solo si uno solo de los argumentos es verdadero
bool xor (bool a, bool b)
{
	return a ? !b : b;
}

/*
Regresa el signo de un numero real 'x'
	- Si 'x' es positivo, regresa 1
	- Si 'x' es negativo, regresa -1
	- Si 'x' es cero, regresa 0
*/
int32 signo (float x)
{
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

/*
Regresa el doble del area con signo del triangulo determinado por los puntos 'a,b,c'
	- El area es positiva si 'a,b,c' tiene orientación contraria al sentido de las manecillas del reloj
	- El area es negativa si 'a,b,c' tiene orientación igual al sentido de las manecillas del reloj
	- El area es cero si los puntos 'a,b,c' son colineales
*/
float area2 (Punto2D a, Punto2D b, Punto2D c)
{
	// Esta formula se deriva del producto cruz entre dos vectores
	return (b[X] - a[X]) * (c[Y] - a[Y]) - (c[X] - a[X]) * (b[Y] - a[Y]);
}

// Regresa el signo del area del triangulo determinado por los puntos 'a,b,c'
int32 signo_area (Punto2D a, Punto2D b, Punto2D c)
{
	return signo(area2(a, b, c));
}

// Indica si el punto 'c' se encuentra a la izquierda de la linea dirigida 'ab'
bool izquierda (Punto2D a, Punto2D b, Punto2D c)
{
	return signo_area(a, b, c) > 0;
}

// Indica si el punto 'c' se encuentra a la derecha de la linea dirigida 'ab'
bool derecha (Punto2D a, Punto2D b, Punto2D c)
{
	return signo_area(a, b, c) < 0;
}

// Indica si el punto 'c' se encuentra sobre la trayectoria de la linea dirigida 'ab'
bool colineal (Punto2D a, Punto2D b, Punto2D c)
{
	return signo_area(a, b, c) == 0;
}

// Indica si el punto 'c' se encuentra a la izquierda o sobre la linea dirigida 'ab'
bool izquierda_sobre (Punto2D a, Punto2D b, Punto2D c)
{
	return signo_area(a, b, c) >= 0;
}

// Indica si el punto 'c' se encuentra a la derecha o sobre la linea dirigida 'ab'
bool derecha_sobre (Punto2D a, Punto2D b, Punto2D c)
{
	return signo_area(a, b, c) <= 0;
}

// Obtiene el valor anterior a cierto 'valor' en un 'arreglo'
// Si el 'valor' es el primero del arreglo, se retorna el ultimo valor
var anterior (var arreglo, var valor)
{
	if (valor is first(arreglo))
		return last(arreglo);

	var anterior = NULL;
	with (Int, indice as index_of(arreglo, valor))
		anterior = get(arreglo, I(indice->value - 1));
	return anterior;
}

// Obtiene el valor siguiente a cierto 'valor' en un 'arreglo'
// Si el 'valor' es el ultimo del arreglo, se retorna el primero valor
var siguiente (var arreglo, var valor)
{
	if (valor is last(arreglo))
		return first(arreglo);
	
	var siguiente = NULL;
	with (Int, indice as index_of(arreglo, valor))
		siguiente = get(arreglo, I(indice->value + 1));
	return siguiente;
}