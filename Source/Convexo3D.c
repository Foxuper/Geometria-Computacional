#include <Convexo3D.h>

// Funciones de interfaz para los tipos de dato 'Vertice', 'Arista' y 'Cara'
static void Vertice_build_with (var this_ptr, Tuple args);
static void Arista_build_with  (var this_ptr, Tuple args);
static void Cara_build_with    (var this_ptr, Tuple args);

// Funciones de entrada de datos
static Arista  agregar_arista  (Datos datos);
static Cara    agregar_cara    (Datos datos);
static Cara    construir_cara  (Datos datos, Vertice v0, Vertice v1, Vertice v2, Cara espejo);

// Funciones primitivas
static bool   colineal_3d    (Vertice a, Vertice b, Vertice c);
static int32  signo_volumen  (Cara cara, Vertice vertice);
static uint32 area2_abs      (Vertice a, Vertice b, Vertice c);
static bool   en_arista      (Arista arista, Vertice vertice);

// Funciones de caras y visibilidad
static int32 triangulo_inicial  (Datos datos);
static void  normalizar_sentido (Cara nueva_cara, Cara visible, Arista arista, Vertice vertice);
static Cara  agregar_triangulo  (Datos datos, Cara visible, Arista arista, Vertice vertice, bool normal);
static void  agregar_externo    (Datos datos, Vertice vertice);
static void  procesar_arista    (Datos datos, Arista arista, Vertice vertice, bool cierre_3d);
static bool  agregar_visible    (Datos datos, Vertice vertice, bool *cierre_3d);

// Funciones de limpieza de datos
static void limpiar_aristas  (Datos datos);
static void limpiar_caras    (Datos datos);
static void limpiar_vertices (Datos datos, Vertice *vertice_siguiente);

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

static void Vertice_build_with (var this_ptr, Tuple args)
{
	THIS(Vertice);
	UNUSED(args);
	this->punto[X] = 0;
	this->punto[Y] = 0;
	this->punto[Z] = 0;
	this->incidente = NULL;
	this->en_cierre = false;
	this->procesado = false;
}

static void Arista_build_with (var this_ptr, Tuple args)
{
	THIS(Arista);
	UNUSED(args);
	for (uint8 i = 0; i < 2; i++)
		this->extremos[i] = NULL;
	for (uint8 i = 0; i < 4; i++)
		this->adyacentes[i] = NULL;
	this->eliminar = false;
	this->procesada = false;
}

static void Cara_build_with (var this_ptr, Tuple args)
{
	THIS(Cara);
	UNUSED(args);
	for (uint8 i = 0; i < 3; i++)
	{
		this->vertices[i] = NULL;
		this->aristas[i] = NULL;
	}
	this->coplanar = false;
	this->visible = false;
}

Type TYPE(Vertice) = new_type
(
	Vertice,
	implement(New, Vertice_build_with, NULL)
);

Type TYPE(Arista) = new_type
(
	Arista,
	implement(New, Arista_build_with, NULL)
);

Type TYPE(Cara) = new_type
(
	Cara,
	implement(New, Cara_build_with, NULL)
);

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Convrierte un vertice en una estructura estandar 'Vector3'
Vector3 a_vector (Vertice a)
{
	return (Vector3)
	{
		a->punto[X],
		a->punto[Y],
		a->punto[Z]
	};
}

// Convierte los datos de un cierre convexo a datos de renderizado 'Data3'
Data3 a_data3 (Datos datos, List vertices)
{
	// Se crean dos listas para triangulos y vertices
	List triangles = new(List);
	List edges = new(List);

	// Se rellena la lista de triangulos
	foreach (Cara, cara in datos.caras)
	{
		Vector3 v0 = a_vector(cara->vertices[0]);
		Vector3 v1 = a_vector(cara->vertices[1]);
		Vector3 v2 = a_vector(cara->vertices[2]);
		insert(triangles, v(Triangle3, v0, v1, v2));
	}

	// Se rellena la lista de aristas
	foreach (Arista, arista in datos.aristas)
	{
		Vector3 v0 = a_vector(arista->extremos[0]);
		Vector3 v1 = a_vector(arista->extremos[1]);
		insert(edges, v(Segment3, v0, v1));
	}

	// Se crea la estructura de datos para el renderizado
	return (Data3)
	{
		.vertices = vertices,
		.triangles = triangles,
		.edges = edges
	};
}

// Agrega un nuevo vertice nuevo al conjunto de puntos con coordenadas 'x,y,z'
Vertice c3d_agregar_vertice (Datos datos, int32 x, int32 y, int32 z)
{
	with (Vertice, vertice as new(Vertice))
	{
		// Se le asignan coordenadas al nuevo vertice
		vertice->punto[X] = x;
		vertice->punto[Y] = y;
		vertice->punto[Z] = z;

		// Se inserta el nuevo vertice en el arreglo de vertices
		insert(datos.vertices, vertice);
	}

	// Se retorna el vertice creado
	return last(datos.vertices);
}

// Agrega una nueva arista al conjunto de aristas
static Arista agregar_arista (Datos datos)
{
	with (Arista, arista as new(Arista))
	{
		// Se inserta la nueva arista en el arreglo de aristas
		insert(datos.aristas, arista);
	}

	// Se retorna la arista creada
	return last(datos.aristas);
}

// Agrega una nueva cara al conjunto de caras
static Cara agregar_cara (Datos datos)
{
	with (Cara, cara as new(Cara))
	{
		// Se inserta la nueva cara en el arreglo de caras
		insert(datos.caras, cara);
	}

	// Se retorna la cara creada
	return last(datos.caras);
}

/*
Crea una nueva cara, la agrega al conjunto de caras y asigna sus vertices y aristas.
La cara creada puede ser el "espejo" de otra cara, dicho de otra manera,
se crea una cara exactamente igual a la cara 'espejo', pero con un orden de
vertices contrario a la cara 'espejo'
*/
static Cara construir_cara (Datos datos, Vertice v0, Vertice v1, Vertice v2, Cara espejo)
{
	Cara cara = agregar_cara(datos);

	if (espejo)
	{
		// Se copian los vertices de la cara espejo en orden inverso
		cara->aristas[0] = espejo->aristas[2];
		cara->aristas[1] = espejo->aristas[1];
		cara->aristas[2] = espejo->aristas[0];
	}
	else
	{
		// Se crean las aristas del triangulo inicial
		cara->aristas[0] = agregar_arista(datos);
		cara->aristas[1] = agregar_arista(datos);
		cara->aristas[2] = agregar_arista(datos);
	}

	Arista a0 = cara->aristas[0];
	Arista a1 = cara->aristas[1];
	Arista a2 = cara->aristas[2];

	// Se asignan los vertices de la cara
	cara->vertices[0] = v0;
	cara->vertices[1] = v1;
	cara->vertices[2] = v2;

	// Se enlaza la nueva cara con las aristas
	a0->adyacentes[0] = cara;
	a1->adyacentes[0] = cara;
	a2->adyacentes[0] = cara;

	if (espejo)
	{
		// Se enlaza la cara espejo con las aristas
		a0->adyacentes[1] = espejo;
		a1->adyacentes[1] = espejo;
		a2->adyacentes[1] = espejo;
	}

	// Se asignan los vertices extremos de las aristas
	a0->extremos[0] = v0; a0->extremos[1] = v1;
	a1->extremos[0] = v1; a1->extremos[1] = v2;
	a2->extremos[0] = v2; a2->extremos[1] = v0;

	return cara;
}

/*
Comprueba si tres puntos tridimensionales residen sobre una misma recta.
La comprobacion se realiza verificando que el producto cruz sea nulo
*/
static bool colineal_3d (Vertice a, Vertice b, Vertice c)
{
	Vector3 v1 = Vector3Subtract(a_vector(b), a_vector(a));
	Vector3 v2 = Vector3Subtract(a_vector(c), a_vector(a));
	Vector3 cruz = Vector3CrossProduct(v1, v2);
	return cruz.x is 0 and cruz.y is 0 and cruz.z is 0;
}

/*
Regresa el signo del volumen del tetraedro formado por la 'cara' y el 'vertice' dado.
	- El signo del volumen es negativo si los vertices de la 'cara' tienen sentido
	  contra-reloj visto desde la perspectiva del 'vertice'
	- El signo del volumen es positivo si los vertices de la 'cara' tienen sentido
	  de reloj visto desde la perspectiva del 'vertice'
	- El signo del volumen es cero si el 'vertice' reside sobre la 'cara'
*/
static int32 signo_volumen (Cara cara, Vertice vertice)
{
	Vector3 a = Vector3Subtract(a_vector(cara->vertices[0]), a_vector(vertice));
	Vector3 b = Vector3Subtract(a_vector(cara->vertices[1]), a_vector(vertice));
	Vector3 c = Vector3Subtract(a_vector(cara->vertices[2]), a_vector(vertice));
	float volumen2 = Vector3DotProduct(a, Vector3CrossProduct(b, c));
	return signo(volumen2);
}

// Regresa el doble del area absoluta del triangulo determinado por los vertices 'a,b,c'
static uint32 area2_abs (Vertice a, Vertice b, Vertice c)
{
	Vector3 v1 = Vector3Subtract(a_vector(b), a_vector(a));
	Vector3 v2 = Vector3Subtract(a_vector(c), a_vector(a));
	return Vector3Length(Vector3CrossProduct(v1, v2));
}

/*
Determina si un vertice se encuentra dentro de una cara o sobre su frontera.
Para que la prueba funcione, el vertice debe ser coplanar con la cara
*/
bool dentro_de_cara (Cara cara, Vertice vertice)
{
	// Se obtiene el doble del area real
	uint32 area2_real = area2_abs(cara->vertices[0], cara->vertices[1], cara->vertices[2]);

	// Se asigna valor inicial del doble del area de los triangulos
	uint32 area2_triangulos = 0;

	// Se suma el area de los triangulos formados por las aristas de la cara y el 'vertice' dado
	for (uint8 i = 0; i < 3; i++)
		area2_triangulos += area2_abs(vertice, cara->vertices[i], cara->vertices[(i + 1) % 3]);

	// Si el area de los triangulos es igual al area real, el vertice esta sobre la cara
	if (area2_real is area2_triangulos)
		return true;

	return false;
}

/*
Determina si un vertice se encuentra sobre una arista.
Para que la prueba funcione, la arista debe formar parte de una
cara y el vertice debe estar dentro de dicha cara o sobre su frontera
*/
static bool en_arista (Arista arista, Vertice vertice)
{ return colineal_3d(arista->extremos[0], arista->extremos[1], vertice); }

/*
Construye el triangulo inicial del cierre convexo. En realidad, el primer triangulo consiste de dos
caras, ya que un triangulo en 3D puede ser visto desde cualquier punto en el espacio que no sea
coplanar al triangulo.
1) Se buscan tres vertices que no sean colineares, de tal manera que se forme un triangulo.
   Los vertices de ese triangulo (y todos los demás) se guardan en orden contra reloj visto
   desde la parte exterior del cierre convexo. Ya que el triangulo tiene dos caras, cada cara
   del triangulo tiene sus vertices ordenados en orden contra reloj.
2) Se busca un cuarto vertice que no sea coplanar al triangulo inicial. Dicho vertice será el
   vertice inicial del algoritmo. Este cuarto vertice formará el tetraedro inicial del cierre convexo.
3) Aquellos vertices que se descarten de las dos busquedas anteriores, se guardan en una tupla de
   vertices 'omitidos' para ser procesados en la parte final del algoritmo
*/
static int32 triangulo_inicial (Datos datos)
{
	// Si se tienen menos de 3 vertices, no se puede construir un triangulo inicial
	if (len(datos.vertices) < 3)
		return -1;

	// Se obtienen los primeros tres vertices
	Vertice v0 = first(datos.vertices);
	Vertice v1 = siguiente(datos.vertices, v0);
	Vertice v2 = siguiente(datos.vertices, v1);

	// Indice del cuarto vertice
	int32 indice = 3;

	// Se comprueba que los puntos no sean colineales
	while (colineal_3d(v0, v1, v2))
	{
		// Si son colineales, se obtienen los siguientes vertices
		v0 = siguiente(datos.vertices, v0);
		v1 = siguiente(datos.vertices, v1);
		v2 = siguiente(datos.vertices, v2);

		// Se avanza el indice del cuarto vertice
		indice++;

		// Si todos los vertices son colineales, no se puede construir un triangulo inicial
		if (v0 is first(datos.vertices))
			return -1;
	}

	// Se marcan los vertices como procesados
	v0->procesado = true;
	v1->procesado = true;
	v2->procesado = true;

	// Se crea la cara principal del triangulo y su espejo
	Cara cara = construir_cara(datos, v0, v1, v2, NULL);
	construir_cara(datos, v2, v1, v0, cara);

	// Se regresa el indice del cuarto vertice
	return indice;
}

/*
Una vez que se crea una nueva cara para agregar un 'vertice' al cierre convexo, se debe
asegurar que dicha cara tenga la misma orientacion que la cara visibles desde el 'vertice'.
Esto asegura que el cierre convexo unicamente se pueda 'ver' desde el exterior del mismo
*/
static void normalizar_sentido (Cara nueva_cara, Cara visible, Arista arista, Vertice vertice)
{
	// Indice del primer vertice de la 'arista' en la cara 'visible'
	uint8 indice;

	// Se busca el indice del primer vertice de la 'arista' en la cara 'visible'
	for (indice = 0; indice < 3; indice++)
	{
		if (visible->vertices[indice] is arista->extremos[0])
			break;
	}

	// Se orienta la 'nueva_cara' en el mismo sentido que la cara 'visible'
	if (visible->vertices[(indice + 1) % 3] is arista->extremos[1])
	{
		nueva_cara->vertices[0] = arista->extremos[0];
		nueva_cara->vertices[1] = arista->extremos[1];
	}
	else
	{
		nueva_cara->vertices[0] = arista->extremos[1];
		nueva_cara->vertices[1] = arista->extremos[0];
	}

	// Se agrega el ultimo 'vertice' de la 'nueva_cara'
	nueva_cara->vertices[2] = vertice;
}

/*
Genera nuevo triangulo formado por una 'arista' y un 'vertice'. Dicho triangulo
esta formado por una cara y dos nuevas aristas. El nuevo triangulo forma parte
del cierre convexo
*/
static Cara agregar_triangulo (Datos datos, Cara visible, Arista arista, Vertice vertice, bool normal)
{
	Arista nuevas_aristas[2];

	// Se crean dos nuevas aristas si es que no existen aún
	for (uint8 i = 0; i < 2; i++)
	{
		// La nueva arista será una de las que incidan sobre un extremo de la 'arista' dada
		nuevas_aristas[i] = arista->extremos[i]->incidente;

		// Si no hay arista incidente sobre el extremo, entonces se crea una nueva arista
		if (not nuevas_aristas[i])
		{
			nuevas_aristas[i] = agregar_arista(datos);
			nuevas_aristas[i]->extremos[0] = arista->extremos[i];
			nuevas_aristas[i]->extremos[1] = vertice;
			arista->extremos[i]->incidente = nuevas_aristas[i];
		}
	}

	// Se crea la nueva cara
	Cara nueva_cara = agregar_cara(datos);

	// Se asignan las aristas de la nueva cara
	nueva_cara->aristas[0] = arista;
	nueva_cara->aristas[1] = nuevas_aristas[0];
	nueva_cara->aristas[2] = nuevas_aristas[1];

	// Se normaliza el orden de los vertices de la nueva cara
	normalizar_sentido(nueva_cara, visible, arista, vertice);

	// Se asigna la 'nueva_cara' como adyacente a las 'nuevas_aristas'
	for (uint8 i = 0; i < 2; i++)
	{
		for (uint8 j = (normal ? 0 : 2); j < (normal ? 2 : 4); j++)
		{
			// Unicamente se asigna la cara adyacente que sea nula
			if (not nuevas_aristas[i]->adyacentes[j])
			{
				nuevas_aristas[i]->adyacentes[j] = nueva_cara;
				break;
			}
		}
	}

	// Se cambia la cara adyacente visible de la 'arista' por la nueva cara
	for (uint8 i = 0; i < (normal ? 2 : 4); i++)
	{
		if (arista->adyacentes[i] is visible)
		{
			arista->adyacentes[i] = nueva_cara;
			break;
		}
	}

	// Se regresa la nueva cara creada
	return nueva_cara;
}

/*
Teniendo un cierre convexo plano (todos los puntos son coplanares), esta funcion
agrega un vertice coplanar que se encuentre en el exterior del cierre convexo
*/
static void agregar_externo (Datos datos, Vertice vertice)
{
	// Se recorren todas las aristas del cierre convexo
	foreach (Arista, arista in datos.aristas)
	{
		// Se saltan las aristas ya procesadas
		if (arista->procesada)
			continue;

		// Se saltan las aristas que no formen parte del exterior del cierre convexo
		if (arista->adyacentes[2] or arista->adyacentes[3])
			continue;

		// Se marca la arista como procesada
		arista->procesada = true;

		// Se obtienen las dos caras adyacentes a la arista
		Cara c0 = arista->adyacentes[0];
		Cara c1 = arista->adyacentes[1];

		// Se obtiene el producto cruz de los vectores de la cara 'c0'
		Vector3 a = Vector3Subtract(a_vector(c0->vertices[1]), a_vector(c0->vertices[0]));
		Vector3 b = Vector3Subtract(a_vector(c0->vertices[2]), a_vector(c0->vertices[0]));
		Vector3 cruz = Vector3CrossProduct(a, b);

		// Con el producto cruz anterior se genera un vertice normal a la cara 'c0'
		Vertice normal = v(Vertice, .punto[X] = cruz.x, .punto[Y] = cruz.y, .punto[Z] = cruz.z);

		// Se crea una cara temporal cuya funcion es poder identificar si la arista
		// actual es visible desde el vertice. Hay que recordar que este proceso solo
		// se realiza cuando el cierre convexo es plano
		with (Cara, cara_temporal as new(Cara))
		{
			// Se normaliza el sentido de la cara temporal, de tal manera
			// que se igual al sentido de la cara 'c0'
			normalizar_sentido(cara_temporal, c0, arista, normal);

			// Si el vertice puede ver a la arista, entonces se generan dos
			// triangulos espejo formados por la arista y el vertice
			if (signo_volumen(cara_temporal, vertice) < 0)
			{
				// Debido a que la arista actual pasara a ser "especial", se modifican
				// los valores de sus caras adyacentes
				arista->adyacentes[0] = NULL;
				arista->adyacentes[1] = c1;
				arista->adyacentes[2] = NULL;
				arista->adyacentes[3] = c0;

				// Se toma el sentido de la cara 'c1' en vez de la 'c0' ya que se agrega
				// un vertice externo, de tal manera que los sentidos se deben invertir
				agregar_triangulo(datos, c1, arista, vertice, true);
				agregar_triangulo(datos, c0, arista, vertice, false);

				// Se asignan las caras adyacentes que eran nulas
				arista->adyacentes[0] = c0;
				arista->adyacentes[2] = c1;
			}
		}
	}
}

/*
Procesa una de las aristas de la cara visible de tal manera que se forme un nuevo
triangulo con el vertice dado. El nuevo triangulo forma parte del cierre convexo
*/
static void procesar_arista (Datos datos, Arista arista, Vertice vertice, bool cierre_3d)
{
	// Se saltan las aristas ya procesadas
	if (arista->procesada)
		return;

	// Se marca la arista como procesada
	arista->procesada = true;

	// Se obtienen las caras adyacentes a la 'arista'
	Cara a = arista->adyacentes[0];
	Cara b = arista->adyacentes[1];
	Cara c = arista->adyacentes[2];
	Cara d = arista->adyacentes[3];

	// La arista es "especial" si posee cuatro caras adyacentes
	bool arista_especial = c and d;

	// Se identifica si el vertice es coplanar o no con cualquier cara
	bool vertice_coplanar = a->coplanar or b->coplanar;

	if (not arista_especial or cierre_3d)
	{
		if (vertice_coplanar and not cierre_3d)
		{
			// Si el vertice NO se encuentra sobre la arista, entonces se encuentra dentro
			// de la cara, por lo que se generan dos triangulos espejo, uno para cada cara
			if (not en_arista(arista, vertice))
			{
				agregar_triangulo(datos, a, arista, vertice, true);
				agregar_triangulo(datos, b, arista, vertice, false);
			}
			else
				arista->eliminar = true;
		}
		else
		{
			// Si solo una de las caras adyacentes a la arista es visible desde el vertice,
			// entonces la arista forma parte del borde de la sombra formada por el vertice,
			// asi que se construye un nuevo triangulo formado por la arista y el vertice
			if (xor(a->visible, b->visible))
				agregar_triangulo(datos, a->visible ? a : b, arista, vertice, true);

			// Si ambas caras adyacentes a la arista son visibles desde el vertice, entonces
			// la arista se encuentra en el interior del cierre convexo, por lo que se marca
			// para su eliminación
			else if (a->visible and b->visible)
				arista->eliminar = true;
		}
	}
	else
	{
		if (vertice_coplanar)
		{
			// Si el vertice NO se encuentra sobre la arista, entonces se encuentra dentro
			// de la cara. Debido a que la arista es especial, se deben identificar las caras
			// que contienen al vertice, las cuales se identifican como 'superior' e 'inferior'.
			// Se generan dos triangulos espejo, uno para la cara 'superior' y otro para la 'inferior'
			if (not en_arista(arista, vertice))
			{
				Cara superior = dentro_de_cara(a, vertice) ? a : b;
				Cara inferior = dentro_de_cara(c, vertice) ? c : d;
				agregar_triangulo(datos, superior, arista, vertice, true);
				agregar_triangulo(datos, inferior, arista, vertice, false);
			}
			else
				arista->eliminar = true;
		}
		// La arista ES especial y el vertice NO es coplanar, por lo que el vertice
		// solo puede ver dos de las cuatro caras adyacentes a la arista, asi que las
		// caras visibles se marcan como nulas y las primeras dos caras se reemplazan
		// por las caras que no eran visibles, de manera que la arista deja de ser especial
		else
		{
			if (a->visible and b->visible)
			{
				arista->adyacentes[0] = c;
				arista->adyacentes[1] = d;
			}
			arista->adyacentes[2] = NULL;
			arista->adyacentes[3] = NULL;
		}
	}
}

/*
Agrega un nuevo vertice al cierre convexo si puede ver la parte exterior del mismo, de lo
contrario, se ignora y se considera que ya forma parte del "interior" del cierre convexo
*/
static bool agregar_visible (Datos datos, Vertice vertice, bool *cierre_3d)
{
	bool visible = false;
	bool tiene_volumen = false;
	bool coplanar_externo = false;

	// Se marcan las caras externas que son visibles desde el 'vertice'
	foreach (Cara, cara in datos.caras)
	{
		// Se calcula el signo del volumen del tetraedro
		// formado por la 'cara' y el 'vertice' dado
		int32 signo = signo_volumen(cara, vertice);

		// Si el signo del volumen es cero, entonces el vertice es coplanar con la cara
		if (signo is 0)
		{
			// Si el vertice se encuentra dentro de la cara entonces se marca como visible
			if (dentro_de_cara(cara, vertice))
			{
				cara->coplanar = true;
				cara->visible = true;
				visible = true;
			}
			// Si el vertice esta fuera de la cara, entonces se
			// trata de un vertice coplanar externo a dicha cara.
			// Unicamenente será verdadero si el cierre no es 3D
			else
				coplanar_externo = not *cierre_3d;
		}
		// Si el signo del volumen es estrictamente negativo, entonces se
		// puede asegurar que la cara es visible desde el vertice y que ademas
		// el cierre convexo es tridimensional
		else if (signo < 0)
		{
			cara->visible = true;
			tiene_volumen = true;
			visible = true;
		}
	}

	// Si no hay caras visibles desde el vertice, entonces
	// el vertice ya esta en el interior del cierre convexo
	if (not visible)
	{
		if (coplanar_externo)
		{
			// Se agrega el vertice coplanar externo al cierre convexo
			agregar_externo(datos, vertice);
			return true;
		}
		else
		{
			// Se marca como falso ya que aunque el vertice se encuentra dentro del
			// cierre convexo, no se encuentra en la frontera del cierre como tal
			vertice->en_cierre = false;
			return false;
		}
	}

	// Se recorren todas las caras visibles desde el vertice
	foreach (Cara, cara in datos.caras)
	{
		if (not cara->visible)
			continue;

		// Se procesan todas las aristas de la cara visible
		for (uint8 i = 0; i < 3; i++)
		{
			Arista arista = cara->aristas[i];
			procesar_arista(datos, arista, vertice, *cierre_3d);
		}
	}

	// Si el nuevo vertice forma un tetraedro con volumen, entonces
	// el cierre convexo se convierte en tridimensional
	*cierre_3d |= tiene_volumen;

	return true;
}

/*
Limpia la lista de las aristas, reemplazando las antiguas caras
adyacentes y eliminando las aristas marcadas para su eliminación
*/
static void limpiar_aristas (Datos datos)
{
	// Se recorren todas las aristas del cierre convexo
	foreach (Arista, arista in datos.aristas)
	{
		// Se marca la arista como no procesada
		arista->procesada = false;

		// Se obtienen las cuatro caras adyacentes a la arista
		Cara a = arista->adyacentes[0];
		Cara b = arista->adyacentes[1];
		Cara c = arista->adyacentes[2];
		Cara d = arista->adyacentes[3];

		// Si la cara adyacente es visible, quiere decir que dicha
		// cara se eliminará, por lo que se marca como nula
		for (uint8 i = 0; i < 4; i++)
		{
			Cara adyacente = arista->adyacentes[i];
			if (adyacente and adyacente->visible)
				arista->adyacentes[i] = NULL;
		}

		// Si la arista solo tiene dos caras adyacentes, entonces
		// se reemplazan las primeras dos caras por las adyacentes
		// y las ultimas dos caras se marcan como nulas
		if (xor(a, b) and xor(c, d))
		{
			arista->adyacentes[0] = a ? a : b;
			arista->adyacentes[1] = c ? c : d;
			arista->adyacentes[2] = NULL;
			arista->adyacentes[3] = NULL;
		}
	}

	// Se eliminan las aristas marcadas para su eliminacion
	for (uint32 i = len(datos.aristas); i > 0; i--)
	{
		Arista arista = get(datos.aristas, I(i - 1));

		if (arista->eliminar)
			erase_at(datos.aristas, I(i - 1));
	}
}

// Elimina las caras que se encuentran en el interior del cierre convexo
static void limpiar_caras (Datos datos)
{
	// Se eliminan las caras marcadas como visibles
	for (uint32 i = len(datos.caras); i > 0; i--)
	{
		Cara cara = get(datos.caras, I(i - 1));

		if (cara->visible)
			erase_at(datos.caras, I(i - 1));
	}
}

/*
Limpia la lista de los vertices, marcando los extremos de las aristas
como dentro del cierre y eliminando los vertices que ya han sido procesados
y que ya no forman parte del cierre convexo
*/
static void limpiar_vertices (Datos datos, Vertice *vertice_siguiente)
{
	// Se reinician los marcadores de los vertices
	foreach (Vertice, vertice in datos.vertices)
	{
		vertice->incidente = NULL;
		vertice->en_cierre = false;
	}

	// Se marcan los vertices extremos de las aristas como dentro del cierre
	foreach (Arista, arista in datos.aristas)
	{
		arista->extremos[0]->en_cierre = true;
		arista->extremos[1]->en_cierre = true;
	}

	// Se eliminan los vertices procesados que no esten en el cierre
	for (uint32 i = len(datos.vertices); i > 0; i--)
	{
		Vertice vertice = get(datos.vertices, I(i - 1));

		if (vertice->procesado and not vertice->en_cierre)
		{
			// Si se va a borrar el siguiente vertice, se actualiza el apuntador
			if (vertice_siguiente and *vertice_siguiente is vertice)
				*vertice_siguiente = siguiente(datos.vertices, vertice);
			erase_at(datos.vertices, I(i - 1));
		}
	}
}

/*
Recorre todas las aristas, caras y vertices del cierre convexo, eliminando
los elementos que ya no son necesarios y reemplazando los elementos que
se obtienen de agregar un nuevo vertice al cierre
*/
void limpiar (Datos datos, Vertice *v_siguiente)
{
	limpiar_aristas(datos);
	limpiar_caras(datos);
	limpiar_vertices(datos, v_siguiente);
}

/*
Construye el cierre convexo de una lista de puntos en 3D.
	- Los puntos que forman parte del cierre convexo se marcan como 'en_cierre'.
	- Se otiene tambien una lista de las caras y los vertices que forman el cierre convexo.
*/
void construir_cierre (Datos datos)
{
	// Se genera el triangulo inicial del cierre convexo
	int32 inicio = triangulo_inicial(datos);

	// Un valor de inicio de '-1' indica que los puntos son colineales
	if (inicio is -1)
	{
		return;
	}

	// El cierre convexo no es tridimensional inicialmente
	bool cierre_3d = false;

	// Se obtiene el primer vertice a procesar
	Vertice vertice = get(datos.vertices, I(inicio % len(datos.vertices)));

	// Se guarda el vertice inicial
	Vertice vertice_inicial = vertice;

	// Se recorren todos los vertices
	for (uint32 i = len(datos.vertices); i > 0; i--)
	{
		// Se obtiene el vertice siguiente
		Vertice vertice_siguiente = siguiente(datos.vertices, vertice);

		// Se marca el vertice como procesado
		if (not vertice->procesado)
		{
			vertice->procesado = true;

			// Si el vertice puede ver el exterior del cierre convexo,
			// entonces se agrega al cierre convexo
			agregar_visible(datos, vertice, &cierre_3d);

			// Se limpia el interior del cierre convexo
			limpiar(datos, &vertice_siguiente);
		}

		// Se cambia al siguiente vertice
		vertice = vertice_siguiente;

		// Si se vuelve al vertice inicial, se termina el ciclo
		if (vertice is vertice_inicial)
			break;
	}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void menu_cierre_convexo_3d (void)
{
	List lista_puntos = NULL;

	// Define la camara para ver el gráfico 3D
	Camera3D camera =
	{
		.position =   (Vector3) {-11, 11, 0},
		.target =     (Vector3) {0, 0, 0},
		.up =         (Vector3) {0, 1, 0},
		.fovy =       50,
		.projection = CAMERA_PERSPECTIVE
	};

	// Se inicializa la camara
	InitCameraCustom(camera);

	// Se obtienen las dimensiones del monitor
	int monitor = GetCurrentMonitor();
	int ancho_monitor = GetMonitorWidth(monitor);
	int altura_monitor = GetMonitorHeight(monitor);

	// Se crea una textura para el renderizado
	RenderTexture2D target = LoadRenderTexture(ancho_monitor * 2, altura_monitor * 2);
	SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

	while (true)
	{
		BeginDrawing();
			if (dibujar_encabezado("Cierre Convexo 3D"))
				break;

			// Se obtienen los bordes de dibujado
			Corners borde_absoluto = CornAbs(10, 21, 90, 95);
			Rectangle rect_absoluto = ToRectangle(borde_absoluto);

			// Boton para editar los puntos
			if (GuiButton(RectAbs(83, 2, 15, 6), "Editar Puntos"))
			{
				lista_puntos = editar_puntos_3d(camera, lista_puntos);
				UpdateCameraCustom(&camera);
			}

			// Se dibuja el titulo del grafico
			GuiSetScale(4);
				GuiLabel(RectAbs(10, 16, 0, 0), "Gráfico");
			GuiSetScale(2);

			// Estructura de datos para el cierre convexo
			Datos datos =
			{
				.vertices = new(List),
				.aristas = new(List),
				.caras = new(List)
			};

			// Se ingresan los puntos a la estructura de 'datos'
			foreach (PTR(Vector3), punto in lista_puntos)
				c3d_agregar_vertice(datos, punto->x, punto->y, punto->z);

			// Se construye el cierre convexo
			construir_cierre(datos);

			// Se crea la estructura de datos para el renderizado
			Data3 render_data = a_data3(datos, lista_puntos);

			// Se dibuja el grafico 3D
			dibujar_vertices_3d(target, camera, rect_absoluto, render_data);

			// Se eliminan las listas de triangulos y aristas
			delete(render_data.triangles);
			delete(render_data.edges);

			// Se eliminan los datos del cierre convexo
			delete(datos.vertices);
			delete(datos.aristas);
			delete(datos.caras);

			// Se dibuja el marco del gráfico
			GuiFrame(rect_absoluto);

			// Se actualiza la camara
			if (CheckCollisionPointRec(GetMousePosition(), rect_absoluto))
				UpdateCameraCustom(&camera);
		EndDrawing();
	}

	// Se elimina la textura de renderizado
	UnloadRenderTexture(target);

	delete(lista_puntos);
}