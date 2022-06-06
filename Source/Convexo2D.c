#include <Convexo2D.h>

// Funciones genericas del tipo de dato 'Punto'
static void   Punto_build_with (var this_ptr, Tuple args);
static String Punto_to_string  (var this_ptr);

// Funciones de ordenamiento
static void  intercambiar (Puntos puntos, uint32 indice_a, uint32 indice_b);
static int32 comparar     (Puntos puntos, uint32 indice_a, uint32 indice_b);
static void  ordenar      (Puntos puntos);

// Funciones del algoritmo de Graham
static void  encontrar_menor (Puntos puntos);
static void  limpiar         (Puntos puntos);
static Array graham          (Puntos puntos);

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

static void Punto_build_with (var this_ptr, Tuple args)
{
	THIS(Punto);
	UNUSED(args);
	this->p[X] = 0;
	this->p[Y] = 0;
	this->indice = 0;
	this->eliminar = false;
}

static String Punto_to_string (var this_ptr)
{
	THIS(Punto);
	String casted = new(String);
	format_to(casted, 0, "P(%d, %d)", this->p[X], this->p[Y]);
	return casted;
}

static Type TYPE(Punto) = new_type
(
	Punto,
	implement(New, Punto_build_with, NULL),
	implement(Cast, NULL, Punto_to_string)
);

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Agrega un nuevo vertice nuevo al conjunto de puntos con coordenadas 'x,y'
void agregar_punto (Puntos puntos, int32 x, int32 y)
{
	with (Punto, punto as new(Punto))
	{
		// Se le asignan coordenadas al nuevo punto
		punto->p[X] = x;
		punto->p[Y] = y;
		punto->indice = len(puntos);

		// Se inserta el nuevo punto en el arreglo de puntos
		insert(puntos, punto);
	}
}

// Intercambia dos puntos del arreglo de puntos
static void intercambiar (Puntos puntos, uint32 indice_a, uint32 indice_b)
{
	// Se omite el intercambio si los indices son iguales
	if (indice_a is indice_b) return;

	// Se crean copias de los puntos a intercambiar
	Punto copia_a = copy(get(puntos, I(indice_a)));
	Punto copia_b = copy(get(puntos, I(indice_b)));

	// Se asignan los indices de los puntos a intercambiar
	copia_a->indice = indice_b;
	copia_b->indice = indice_a;

	// Se intercambian los puntos
	set(puntos, I(indice_a), copia_b);
	set(puntos, I(indice_b), copia_a);

	// Se eliminan las copias
	delete(copia_a);
	delete(copia_b);
}

/*
Compara dos puntos de acuerdo a su angulo respecto al primer punto 'v0' de la lista de puntos
	- Regresa '1' si el angulo del vector 'v0 -> a' es mayor que el angulo de 'v0 -> b'
	- Regresa '-1' si el angulo del vector 'v0 -> a' es menor que el angulo de 'v0 -> b'
	- Si el angulo de ambos vectores es igual, entonces se realizan las siguiente comparaciones:
		- Regresa '1' si el punto 'b' se encuentra mas cerca al punto 'v0' que 'a'
		- Regresa '-1' si el punto 'a' se encuentra mas cerca al punto 'v0' que 'b'
		- Regresa '0' si 'a' y 'b' son iguales
*/
static int32 comparar (Puntos puntos, uint32 indice_a, uint32 indice_b)
{
	Punto v0 = first(puntos);
	Punto a = get(puntos, I(indice_a));
	Punto b = get(puntos, I(indice_b));
	int32 signo = signo_area(v0->p, a->p, b->p);

	// Si los puntos son colineales, se eliminará uno de los dos puntos
	if (signo is 0)
	{
		// Se comparan las diferencias de coordenadas respecto al primer punto
		int32 dx = abs(a->p[X] - v0->p[X]) - abs(b->p[X] - v0->p[X]);
		int32 dy = abs(a->p[Y] - v0->p[Y]) - abs(b->p[Y] - v0->p[Y]);

		// Si alguna de las diferencias es menor a cero, entonces
		// se marca el punto 'a' para ser eliminado y se regresa -1
		if (dx < 0 or dy < 0)
		{
			a->eliminar = true;
			return -1;
		}

		// Si alguna de las diferencias es mayor a cero, entonces
		// se marca el punto 'b' para ser eliminado y se regresa 1
		else if (dx > 0 or dy > 0)
		{
			b->eliminar = true;
			return 1;
		}

		// Si ambas diferencias son iguales, entonces se marca
		// el punto con menor indice para ser eliminado y se regresa 0
		else
		{
			if (a->indice < b->indice)
				a->eliminar = true;
			else
				b->eliminar = true;
			return 0;
		}
	}

	return -signo;
}

// Ordena los puntos de acuerdo a su angulo respecto al primer punto de la lista de puntos
static void ordenar (Puntos puntos)
{
	uint32 n = len(puntos);

	for (uint32 i = 0; i < n; i++)
	{
		bool intercambio = false;

		for (uint32 j = 1; j < n - i - 1; j++)
		{
			if (comparar(puntos, j, j + 1) > 0)
			{
				intercambiar(puntos, j, j + 1);
				intercambio = true;
			}
		}

		if (not intercambio)
			break;
	}
}

/*
Encuentra el punto con la menor coordenada 'Y' y la mayor coordenada 'X', en otras palabras,
el punto mas bajo que se encuentra mas a la derecha. Dicho punto se intercambia con el primero
*/
static void encontrar_menor (Puntos puntos)
{
	Int indice_menor = I(0);

	foreach (Punto, punto in puntos)
	{
		Punto menor = get(puntos, indice_menor);

		// Si la coordenada 'Y' del punto actual es menor que la del punto 'menor'
		if (punto->p[Y] < menor->p[Y])
			assign(indice_menor, I(punto->indice));

		// Si la coordenada 'Y' es igual y la coordenada 'X' es mayor que la del punto menor
		else if (punto->p[Y] is menor->p[Y] and punto->p[X] > menor->p[X])
			assign(indice_menor, I(punto->indice));
	}

	// Se intercambia el punto menor con el primero de la lista
	intercambiar(puntos, 0, indice_menor->value);
}

// Remueve todos los puntos que se encuentran marcados para ser eliminados
static void limpiar (Puntos puntos)
{
	// Se eliminan todos los puntos marcados con 'eliminar'
	for (uint32 i = len(puntos); i > 0; i--)
	{
		Punto punto = get(puntos, I(i - 1));

		if (punto->eliminar)
			erase_at(puntos, I(i - 1));
	}

	uint32 indice = 0;

	// Se vuelven a asignar los indices de los puntos
	foreach (Punto, punto in puntos)
		punto->indice = indice++;
}

// Reliza un escaneo de Graham sobre una lista de puntos ordenados angularmente
static Array graham (Puntos puntos)
{
	Array pila = new(Array, TYPE(Punto));

	// Se insertan los dos primeros puntos en la pila
	if (len(puntos) >= 1)
		insert(pila, get(puntos, I(0)));

	if (len(puntos) >= 2)
		insert(pila, get(puntos, I(1)));

	// Los primeros dos puntos de la pila forman parte del cierre convexo
	uint32 indice = 2;

	// Se recorren los demas puntos
	while (indice < len(puntos))
	{
		if (len(pila) is 1)
		{
			printf("Error");
			break;
		}

		// Se obtiene el punto actual a revisar
		Punto v = get(puntos, I(indice));

		// Se obtienen los dos puntos superiores de la pila
		Punto p1 = get(pila, I(len(pila) - 2));
		Punto p2 = get(pila, I(len(pila) - 1));

		// Si el punto 'v' se encuentra a la izquierda del segmento 'p1 -> p2'
		if (izquierda(p1->p, p2->p, v->p))
		{
			// Se inserta el punto 'v' en la pila
			insert(pila, v);
			indice++;
		}
		else
			// Se elimina el ultimo punto de la pila
			erase(pila, last(pila));
	}

	return pila;
}

// Regresa dentro de un arreglo (Array) los puntos que forman el cierre convexo
Array cierre_convexo_2d (Puntos puntos)
{
	Puntos copia_puntos = copy(puntos);

	// Se busca el punto con menor coordenada 'Y' y la mayor coordenada 'X'
	encontrar_menor(puntos);

	// Se ordenan los puntos segun su angulo respecto al punto menor
	ordenar(puntos);

	// Se remueven los puntos que se encuentran marcados para ser eliminados
	limpiar(puntos);

	// Se obtiene el cierre convexo
	Array cierre_convexo = graham(puntos);

	// Se restauran los puntos originales
	assign(puntos, copia_puntos);
	delete(copia_puntos);

	// Se regresa el cierre convexo
	return cierre_convexo;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void menu_cierre_convexo_2d (void)
{
	List lista_vertices = NULL;

	while (true)
	{
		BeginDrawing();
			if (dibujar_encabezado("Cierre Convexo 2D"))
				break;

			// Se obtienen los bordes de dibujado
			Corners borde_absoluto = CornAbs(10, 21, 90, 95);
			Rectangle rect_absoluto = ToRectangle(borde_absoluto);
			float longitud_y = (G_XMAX - G_XMIN) * (rect_absoluto.height / rect_absoluto.width);
			Corners borde_relativo = {G_XMIN, longitud_y / 2, G_XMAX, -longitud_y / 2};

			// Boton para editar el polígono
			if (GuiButton(RectAbs(83, 2, 15, 6), "Editar Puntos"))
			{
				DatosEdicion datos_edicion = {25, 23, 95, 90, G_XMIN, G_XMAX};
				lista_vertices = editar_puntos(lista_vertices, datos_edicion, true);
			}

			// Se dibuja el titulo del grafico
			GuiSetScale(4);
				GuiLabel(RectAbs(10, 16, 0, 0), "Gráfico");
			GuiSetScale(2);

			// Se dibuja la rejilla y el marco del gráfico
			GuiGridCustom(rect_absoluto, ToRectangle(borde_relativo));
			GuiFrame(rect_absoluto);

			// Se crea un nuevo arreglo de puntos para el algoritmo
			Puntos puntos = new(Array, TYPE(Punto));

			// Se rellena el arreglo de puntos con los vertices de la lista
			foreach (PTR(Vector2), vertice in lista_vertices)
				agregar_punto(puntos, vertice->x, vertice->y);

			// Se obtiene el cierre convexo y se elimina el arreglo de puntos
			Array cierre_convexo = cierre_convexo_2d(puntos);
			delete(puntos);

			// Se crea una lista para los puntos del cierre convexo
			List vertices = new(List);

			// Se rellena la lista con los puntos del cierre convexo
			foreach (Punto, punto in cierre_convexo)
				insert(vertices, v(Vector2, punto->p[X], punto->p[Y]));

			// Se elimina el cierre convexo, ya que no se necesita mas
			delete(cierre_convexo);

			// Se limita el dibujado a la zona del rectangulo absoluto
			BeginRectScissorMode(rect_absoluto);
				// Se dibuja el cierre convexo en color naranja
				GuiBackupStyle();
					GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, ColorToInt(ORANGE));
					dibujar_aristas(vertices, borde_relativo, borde_absoluto);
				GuiRestoreStyle();

				// Se dibujan los puntos
				dibujar_vertices(lista_vertices, borde_relativo, borde_absoluto);
			EndScissorMode();

			// Se elimina el arreglo de puntos del cierre convexo
			delete(vertices);
		EndDrawing();
	}

	delete(lista_vertices);
}