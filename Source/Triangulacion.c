#include <Triangulacion.h>

// Funciones de interfaz para el tipo de dato 'Vertice'
static void   Vertice_build_with (var this_ptr, Tuple args);
static String Vertice_to_string  (var this_ptr);

// Funciones primitivas
static bool en_cono             (Poligono poligono, Vertice a, Vertice b);
static bool entre_segmento      (Punto2D a, Punto2D b, Punto2D c);
static bool interseccion_propia (Punto2D a, Punto2D b, Punto2D c, Punto2D d);
static bool interseccion        (Punto2D a, Punto2D b, Punto2D c, Punto2D d);

// Funciones basicas de triangulacion
static bool  diagonal_inoex     (Poligono poligono, Vertice a, Vertice b);
static bool  diagonal_interna   (Poligono poligono, Vertice a, Vertice b);
static void  identificar_orejas (Poligono poligono);

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

static void Vertice_build_with (var this_ptr, Tuple args)
{
	THIS(Vertice);
	UNUSED(args);
	this->punto[X] = 0;
	this->punto[Y] = 0;
	this->es_oreja = false;
}

static String Vertice_to_string (var this_ptr)
{
	THIS(Vertice);
	String casted = new(String);
	format_to(casted, 0, "V(%d, %d)", this->punto[X], this->punto[Y]);
	return casted;
}

static Type TYPE(Vertice) = new_type
(
	Vertice,
	implement(New, Vertice_build_with, NULL),
	implement(Cast, NULL, Vertice_to_string)
);

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Agrega un nuevo vertice nuevo al poligono con coordenadas 'x,y'
void tri_agregar_vertice (Poligono poligono, int32 x, int32 y)
{
	with (Vertice, vertice as new(Vertice))
	{
		// Se le asignan coordenadas al nuevo vertice
		vertice->punto[X] = x;
		vertice->punto[Y] = y;

		// Se inserta el nuevo vertice en el arreglo de vertices
		insert(poligono, vertice);
	}
}

/*
Regresa el doble del area con signo del polígono dado
	- El area es positiva si los vertices tienen un orden contrario al sentido de las manecillas del reloj
	- El area es negativa si los vertices tienen un orden igual al sentido de las manecillas del reloj
*/
float area2_poli (Poligono poligono)
{
	float area = 0;
	Vertice primer_vertice = first(poligono);

	// Si el primer vertice es nulo, el poligono no tiene vertices
	if (primer_vertice is NULL)
		return area;

	// Se asigna un punto fijo igual al primer vertice
	Punto2D punto_fijo = {primer_vertice->punto[X], primer_vertice->punto[Y]};

	// Se recorren los vertices del poligono
	foreach (Vertice, v in poligono)
	{
		// 'v1' y 'v2' son los dos vertices siguientes a 'v'
		Vertice v1 = siguiente(poligono, v);
		Vertice v2 = siguiente(poligono, v1);
		area += area2(punto_fijo, v1->punto, v2->punto);

		// Se rompe el ciclo cuando 'v2' sea el ultimo vertice
		if (v2 is last(poligono))
			break;
	}

	return area;
}

/*
Regresa verdadero si y solo si el segmento dirigido 'ab' es estrictamente
interior al poligono en el cono formado por el vertice 'a' y sus adyacentes
*/
static bool en_cono (Poligono poligono, Vertice a, Vertice b)
{
	// 'a0' y 'a1' son los vertices adyancentes a 'a'
	Vertice a0 = anterior(poligono, a);
	Vertice a1 = siguiente(poligono, a);

	// El vertice 'a' es convexo
	if (izquierda_sobre(a0->punto, a->punto, a1->punto))
	{
		// Para que la diagonal se encuentre dentro del cono formado por el
		// vertice 'a' y sus adyacentes, deben cumplirse ambas condiciones:
		// 	- El vertice 'a0' debe estar a la izquierda de la diagonal 'ab'
		//	- El vertice 'a1' debe estar a la derecha de la diagonal 'ab'
		bool a0_izquierda = izquierda(a->punto, b->punto, a0->punto);
		bool a1_derecha = derecha(a->punto, b->punto, a1->punto);
		return a0_izquierda && a1_derecha;
	}

	// El vertice 'a' no es convexo
	else
	{
		// Para que la diagonal se encuentre dentro del cono no convexo formado
		// por el vertice 'a' y sus adyacentes, NO debe cumplirse ninguna de las
		// siguientes condiciones:
		//	- El vertice 'a1' debe estar a la izquierda o sobre la diagonal 'ab'
		//	- El vertice 'a0' debe estar a la derecha o sobre la diagonal 'ab'
		bool a1_izquierda = izquierda_sobre(a->punto, b->punto, a1->punto);
		bool a0_derecha = derecha_sobre(a->punto, b->punto, a0->punto);
		return !(a1_izquierda && a0_derecha);
	}
}

/*
Regresa verdadero si y solo si el punto 'c' se encuentra entre el segmento
cerrado 'ab'. Primero se comprueba que el punto 'c' se colineal al segmento 'ab'
*/
static bool entre_segmento (Punto2D a, Punto2D b, Punto2D c)
{
	// Si el punto 'c' no es colineal con el segmento 'ab', se regresa falso
	if (!colineal(a, b, c))
		return false;

	// Si el segmento 'ab' es vertical, se comparan las coordenadas 'y' de los puntos
	if (a[X] == b[X])
		return (a[Y] <= c[Y] && c[Y] <= b[Y]) || (a[Y] >= c[Y] && c[Y] >= b[Y]);

	// En caso contrario, se comparan las coordenadas 'x' de los puntos
	return (a[X] <= c[X] && c[X] <= b[X]) || (a[X] >= c[X] && c[X] >= b[X]);
}

/*
Regresa verdadero si y solo si el segmento dirigido `ab` intersecta con el segmento
dirigido `cd` de manera propia, lo cual ocurre cuando ambos segmentos comparten un mismo
punto estrictamente interior. La propiedad de intersección se asegura comprobando que los
extremos de un segmento se encuentren cada uno en un lado distinto del segmento contrario
*/
static bool interseccion_propia (Punto2D a, Punto2D b, Punto2D c, Punto2D d)
{
	// Si un extremo del segmento 'cd' está sobre el segmento 'ab', se regresa falso
	if (colineal(a, b, c) || colineal(a, b, d))
		return false;

	// Si un extremo del segmento 'ab' está sobre el segmento 'cd', se regresa falso
	if (colineal(c, d, a) || colineal(c, d, b))
		return false;

	// Se comprueba que los extremos de los segmentos se encuentren a los lados del segmento contrario
	bool extremo_ab = xor(izquierda(a, b, c), izquierda(a, b, d));
	bool extremo_cd = xor(izquierda(c, d, a), izquierda(c, d, b));

	// Si los dos extremos de ambos segmentos se encuentran en un lado distinto del segmento
	// contrario, entonces los segmentos se intersectan
	return extremo_ab && extremo_cd;
}

/*
Regresa verdadero si y solo si el segmento dirigido 'ab' intersecta
con el segmento dirigido 'cd' de manera propia o impropia
*/
static bool interseccion (Punto2D a, Punto2D b, Punto2D c, Punto2D d)
{
	// Si los segmentos intersectan de manera propia, se regresa verdadero
	if (interseccion_propia(a, b, c, d))
		return true;

	// Se prueba si los puntos extremos de los segmentos se encuentran sobre el segmento contrario
	bool cd_entre_ab = entre_segmento(a, b, c) || entre_segmento(a, b, d);
	bool ab_entre_cd = entre_segmento(c, d, a) || entre_segmento(c, d, b);

	// Si sucede alguno de los casos anteriores, se regresa verdadero
	if (cd_entre_ab || ab_entre_cd)
		return true;

	// En caso contrario, se regresa falso
	return false;
}

/*
Regresa verdadadero si y solo si el segmento dirigido 'ab' es una diagonal
propia interna o externa del poligono. Se ignoran las aristas incidentes
sobre el vertice 'a' o 'b'
*/
static bool diagonal_inoex (Poligono poligono, Vertice a, Vertice b)
{
	// Se recorren todos los vertices del poligono, regresando falso si se llega
	// a encontrar una interseccion entre el segmento 'ab' y la frontera del poligono
	foreach (Vertice, v1 in poligono)
	{
		// Los vertices 'v1,v2' son adyacentes y forman una arista del poligono
		Vertice v2 = siguiente(poligono, v1);

		// Si los vertices 'a' y 'b' no coinciden con ninguno de los vertices 'v1' y 'v2'
		if ((a != v1 && a != v2) && (b != v1 && b != v2))
		{
			// Si el segmento dirigido 'ab' intersecta con el segmento dirigido 'v1v2'
			if (interseccion(a->punto, b->punto, v1->punto, v2->punto))
				return false;
		}
	}

	// Si no se encontró ninguna interseccion, se regresa verdadero
	return true;
}

/*
Regresa verdadero si y solo si el segmento dirigido 'ab' es una diagonal
propia interna del poligono
*/
static bool diagonal_interna (Poligono poligono, Vertice a, Vertice b)
{
	return en_cono(poligono, a, b) && en_cono(poligono, b, a) && diagonal_inoex(poligono, a, b);
}

/*
Etiqueta todos los vertices de un poligono como oreja o no. Si los vertices
adyacentes 'v0' y 'v1' forman una diagonal interna del poligono, entonces 'v'
es una oreja, en caso contrario, 'v' no es oreja
*/
static void identificar_orejas (Poligono poligono)
{
	foreach (Vertice, v in poligono)
	{
		Vertice v0 = anterior(poligono, v);
		Vertice v1 = siguiente(poligono, v);
		v->es_oreja = diagonal_interna(poligono, v0, v1);
	}
}

/*
Regresa dentro de un arreglo (Array) las diagonales internas que
triangulan un poligono en un tiempo cuadratico O(n^2)
*/
Array triangular (Poligono poligono)
{
	Array diagonales = new(Array, TYPE(Segment2));
	bool oreja_encontrada = true;

	// Se crea una copia de los vertices del poligono
	Poligono copia_poligono = copy(poligono);

	// Se marcan todos los vertices que son orejas
	identificar_orejas(poligono);

	// Se cortaran orejas del poligono hasta que solo quede un triangulo (3 vertices)
	while (len(poligono) > 3)
	{
		// Se marca la oreja como no encontrada
		oreja_encontrada = false;

		// Se recorren todos los vertices del poligono hasta encontrar una oreja
		foreach (Vertice, v in poligono)
		{
			// Si el vertice actual es una oreja se cortara del poligono
			// y se genera una nueva diagonal para la triangulación
			if (v->es_oreja)
			{
				oreja_encontrada = true;

				// 'v0' y 'v1' son los dos vertices anteriores a 'v'
				Vertice v1 = anterior(poligono, v);
				Vertice v0 = anterior(poligono, v1);

				// 'v2' y 'v3' son los dos vertices posteriores a 'v'
				Vertice v2 = siguiente(poligono, v);
				Vertice v3 = siguiente(poligono, v2);

				// Los vertices 'v1' y 'v2' forman un triangulo junto con el vertice 'v'.
				// El algoritmo unicamente regresa la diagonal formada por 'v1' y 'v2'
				Segment2 diagonal = v(Segment2, .a = {0, 0}, .b = {0, 0});
				diagonal->a = (Vector2) {v1->punto[X], v1->punto[Y]};
				diagonal->b = (Vector2) {v2->punto[X], v2->punto[Y]};
				insert(diagonales, diagonal);

				// Al cortar a 'v' del poligono, se conectarán sus adyacentes 'v1' y 'v2'
				// directamente, asi que se debe de identificar nuevamente si dichos vertices
				// forman una oreja o no
				v1->es_oreja = diagonal_interna(poligono, v0, v2);
				v2->es_oreja = diagonal_interna(poligono, v1, v3);

				// Se corta el vertice oreja 'v' del poligono
				erase(poligono, v);

				// Se rompe el ciclo, ya que se ha cortado una oreja
				break;
			}
		}

		// Si el poligono no tiene ninguna oreja, se tiene un error
		if (!oreja_encontrada)
		{
			// Se restaura el poligono original y se elimina la copia
			assign(poligono, copia_poligono);
			delete(copia_poligono);
			delete(diagonales);
			return NULL;
		}
	}

	// Se restaura el poligono original y se elimina la copia
	assign(poligono, copia_poligono);
	delete(copia_poligono);
	return diagonales;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void menu_area_poligono (void)
{
	List lista_vertices = NULL;

	while (true)
	{
		BeginDrawing();
			if (dibujar_encabezado("Área de un polígono"))
				break;

			// Se obtienen los bordes de dibujado
			Corners borde_absoluto = CornAbs(16, 21, 84, 85);
			Rectangle rect_absoluto = ToRectangle(borde_absoluto);
			float longitud_y = (G_XMAX - G_XMIN) * (rect_absoluto.height / rect_absoluto.width);
			Corners borde_relativo = {G_XMIN, longitud_y / 2, G_XMAX, -longitud_y / 2};

			// Boton para editar el polígono
			if (GuiButton(RectAbs(83, 2, 15, 6), "Editar Polígono"))
			{
				DatosEdicion datos_edicion = {25, 23, 95, 90, G_XMIN, G_XMAX};
				lista_vertices = editar_puntos(lista_vertices, datos_edicion, false);
			}

			// Se dibuja el titulo del grafico
			GuiSetScale(4);
				GuiLabel(RectAbs(16, 16, 0, 0), "Gráfico");
			GuiSetScale(2);

			// Se dibuja la rejilla y el marco del gráfico
			GuiGridCustom(rect_absoluto, ToRectangle(borde_relativo));
			GuiFrame(rect_absoluto);

			// Se crea un nuevo polígono para el algoritmo
			Poligono poligono = new(Array, TYPE(Vertice));

			// Se rellena el poligono con los vertices de la lista
			foreach (PTR(Vector2), vertice in lista_vertices)
				tri_agregar_vertice(poligono, vertice->x, vertice->y);

			// Se calcula el area del poligono
			float area = area2_poli(poligono) / 2;

			// Se dibuja el área del polígono
			with (String, texto as new(String))
			{
				GuiSetScale(6);
					GuiBackupStyle();
					format_to(texto, 0, "Área = %g u²", (double) fabsf(area));
					GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(ORANGE));
					GuiLabelCenter(RectAbs(50, 93, 0, 0), c_string(texto));
					GuiRestoreStyle();
				GuiSetScale(2);
			}

			// Se limita el dibujado a la zona del rectangulo absoluto
			BeginRectScissorMode(rect_absoluto);
				// Se dibuja el polígono como tal
				dibujar_poligono(lista_vertices, borde_relativo, borde_absoluto);
			EndScissorMode();

			// Se elimina el polígono del algoritmo
			delete(poligono);
		EndDrawing();
	}

	delete(lista_vertices);
}

void menu_triangulacion_poligono (void)
{
	List lista_vertices = NULL;

	while (true)
	{
		BeginDrawing();
			if (dibujar_encabezado("Triangulación de un polígono"))
				break;

			// Se obtienen los bordes de dibujado
			Corners borde_absoluto = CornAbs(10, 21, 90, 95);
			Rectangle rect_absoluto = ToRectangle(borde_absoluto);
			float longitud_y = (G_XMAX - G_XMIN) * (rect_absoluto.height / rect_absoluto.width);
			Corners borde_relativo = {G_XMIN, longitud_y / 2, G_XMAX, -longitud_y / 2};

			// Boton para editar el polígono
			if (GuiButton(RectAbs(83, 2, 15, 6), "Editar Polígono"))
			{
				DatosEdicion datos_edicion = {25, 23, 95, 90, G_XMIN, G_XMAX};
				lista_vertices = editar_puntos(lista_vertices, datos_edicion, false);
			}

			// Se dibuja el titulo del grafico
			GuiSetScale(4);
				GuiLabel(RectAbs(10, 16, 0, 0), "Gráfico");
			GuiSetScale(2);

			// Se dibuja la rejilla y el marco del gráfico
			GuiGridCustom(rect_absoluto, ToRectangle(borde_relativo));
			GuiFrame(rect_absoluto);

			// Se crea un nuevo polígono para el algoritmo
			Poligono poligono = new(Array, TYPE(Vertice));

			// Se rellena el poligono con los vertices de la lista

			if (sentido_poligono(lista_vertices) is SENTIDO_NEGATIVO)
			{
				foreach (PTR(Vector2), vertice in lista_vertices)
					tri_agregar_vertice(poligono, vertice->x, vertice->y);
			}
			else if (sentido_poligono(lista_vertices) is SENTIDO_POSITIVO)
			{
				foreach (PTR(Vector2), vertice in reverse(lista_vertices))
					tri_agregar_vertice(poligono, vertice->x, vertice->y);
			}

			// Se realiza la triangulación del polígono
			Array diagonales = triangular(poligono);

			// Se limita el dibujado a la zona del rectangulo absoluto
			BeginRectScissorMode(rect_absoluto);
				// Se dibujan las diagonales
				foreach (Segment2, diagonal in diagonales)
				{
					Vector2 p1 = interpolar_vector(borde_relativo, borde_absoluto, diagonal->a);
					Vector2 p2 = interpolar_vector(borde_relativo, borde_absoluto, diagonal->b);
					int grosor_linea = (XAbs(1) + YAbs(1)) / 4;
					DrawLineEx(p1, p2, grosor_linea, ORANGE);
				}

				// Se dibuja el polígono como tal
				dibujar_poligono(lista_vertices, borde_relativo, borde_absoluto);
			EndScissorMode();

			// Se elimina el polígono del algoritmo
			delete(poligono);

			// Se elimina el arrero de diagonales
			delete(diagonales);
		EndDrawing();
	}

	delete(lista_vertices);
}