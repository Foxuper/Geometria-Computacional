#include <Delaunay.h>

// Proyecta un vertice de tres dimensiones sobre el plano formando un vector de dos dimensiones
Vector2 proyectar (Vertice vertice)
{ return (Vector2) {vertice->punto[X], vertice->punto[Z]}; }

/*
Crea un nuevo punto de delaunay para poder ser utilizado en el algoritmo principal.
Se usa el mismo algoritmo de cierre convexo 3D, pero tomando como z = x*x + y*y
*/
void crear_punto_delaunay (Datos datos, int32 x, int32 y)
{ c3d_agregar_vertice(datos, x, x*x + y*y, y); }

// Regresa dentro de un arreglo (Array) las aristas de la triangulación de Delaunay
Array triangulacion_delaunay (Datos datos)
{
	// Se construye el cierre convexo
	construir_cierre(datos);

	// La triangulacion es un arreglo de aristas en 2D
	Array triangulacion = new(Array, TYPE(Segment2));

	// Se marcan todas las caras como visibles y las aristas como eliminadas
	foreach (Cara, cara in datos.caras)
		cara->visible = true;
	foreach (Arista, arista in datos.aristas)
		arista->eliminar = true;

	// Se recorren todas las caras del cierre convexo
	foreach (Cara, cara in datos.caras)
	{
		Vector3 a = a_vector(cara->vertices[0]);
		Vector3 b = a_vector(cara->vertices[1]);
		Vector3 c = a_vector(cara->vertices[2]);
		Vector3 v0 = Vector3Subtract(b, a);
		Vector3 v1 = Vector3Subtract(c, a);

		// Se obtiene el vector normal a la cara
		Vector3 normal = Vector3CrossProduct(v0, v1);

		// Si la normal es negativa, entonces es una cara inferior,
		// por lo tanto, forma parte de la triangulación de Delaunay
		if (normal.y < 0)
		{
			cara->visible = false;
			for (uint8 i = 0; i < 3; i++)
			{
				cara->vertices[i]->punto[Y] = 0;
				cara->aristas[i]->eliminar = false;
			}
		}
	}

	// Se eliminan los componentes que no formen parte de la triangulación de Delaunay
	limpiar(datos, NULL);

	// Se recorren las aristas que formen la triangulación de Delaunay
	foreach (Arista, arista in datos.aristas)
	{
		// Se obtiene la proyeccion de los vertices en el plano
		Vector2 proyectado_a = proyectar(arista->extremos[0]);
		Vector2 proyectado_b = proyectar(arista->extremos[1]);

		// Se obtiene la diagonal y se inserta en la triangulación
		Segment2 diagonal = v(Segment2, proyectado_a, proyectado_b);
		insert(triangulacion, diagonal);
	}

	return triangulacion;
}

Array triangulacion_voronoi (Datos datos)
{
	// La triangulacion es un arreglo de aristas en 2D
	Array triangulacion = new(Array, TYPE(Segment2));

	// Los vertices de la triangulación de voronoi son los
	// centroides de las caras de la triangulación de Delaunay
	Array centroides = new(Array, TYPE(Vector2));

	// Se recorren las caras de la triangulación de Delaunay para obtener los centroides
	foreach (Cara, cara in datos.caras)
	{
		// Se obtienen los puntos proyectados en el plano
		Vector2 a = {cara->vertices[0]->punto[X], cara->vertices[0]->punto[Z]};
		Vector2 b = {cara->vertices[1]->punto[X], cara->vertices[1]->punto[Z]};
		Vector2 c = {cara->vertices[2]->punto[X], cara->vertices[2]->punto[Z]};

		// Formula del libro de O'Rourke para obtener el centroide
		float A = b.x - a.x;
		float B = b.y - a.y;
		float C = c.x - a.x;
		float D = c.y - a.y;
		float E = A*(a.x + b.x) + B*(a.y + b.y);
		float F = C*(a.x + c.x) + D*(a.y + c.y);
		float G = 2*(A*(c.y - b.y) - B*(c.x - b.x));

		// Se inserta el centroide en el arreglo
		float x = (D*E - B*F) / G;
		float y = (A*F - C*E) / G;
		insert(centroides, v(Vector2, x, y));
	}

	// Se recorren las aristas de la triangulación de Delaunay para asegurar que
	// el sentido de sus vertices extremos sea el mismo que el de su cara adyacente
	foreach (Arista, arista in datos.aristas)
	{
		// Se elige cualquier cara no nula adyacente a la arista
		Cara cara = arista->adyacentes[0] ? arista->adyacentes[0] : arista->adyacentes[1];

		// Indice del primer vertice de la arista en la cara adyacente
		uint8 indice;

		// Se busca el indice del primer vertice de la arista en la cara adyacente
		for (indice = 0; indice < 3; indice++)
		{
			if (cara->vertices[indice] is arista->extremos[0])
				break;
		}

		// Se intercambian los vertices extremos de la arista si el sentido es incorrecto
		if (cara->vertices[(indice + 1) % 3] isnt arista->extremos[1])
		{
			Vertice temporal = arista->extremos[0];
			arista->extremos[0] = arista->extremos[1];
			arista->extremos[1] = temporal;
		}
	}

	// Se recorren todas las aristas de la triangulación de Delaunay
	// para obtener las diagonales de la triangulación de voronoi
	foreach (Arista, arista in datos.aristas)
	{
		// Se obtienen los centroides de las caras adyacentes a la arista
		Int indice_a = index_of(datos.caras, arista->adyacentes[0]);
		Int indice_b = index_of(datos.caras, arista->adyacentes[1]);
		Vector2 *centroide_a = indice_a ? get(centroides, indice_a) : NULL;
		Vector2 *centroide_b = indice_b ? get(centroides, indice_b) : NULL;

		// Si la arista tiene dos centroides entonces se agrega la diagonal
		// formada por los dos centroides
		if (centroide_a and centroide_b)
		{
			Segment2 diagonal = v(Segment2, *centroide_a, *centroide_b);
			insert(triangulacion, diagonal);
		}
		else
		{
			// Se obtiene el punto medio de la arista
			Vector3 extremo_a = a_vector(arista->extremos[0]);
			Vector3 extremo_b = a_vector(arista->extremos[1]);

			// Se obtiene el centroide de la cara adyacente a la arista
			Vector2 *centroide = centroide_a ? centroide_a : centroide_b;

			// Se obtiene el vector asociado a la arista
			Vector3 vector_arista = Vector3Subtract(extremo_b, extremo_a);

			// Se obtiene la direccion de la diagonal externa de Voronoi
			Vector3 direccion = Vector3CrossProduct(vector_arista, (Vector3) {0, 1, 0});

			// Se escala la direccion de la diagonal externa de Voronoi
			direccion = Vector3Scale(Vector3Normalize(direccion), -100);

			// Se obtiene el punto de destino de la diagonal externa de Voronoi
			Vector2 destino = Vector2Add(*centroide, (Vector2) {direccion.x, direccion.z});

			// Se agrega la diagonal externa de Voronoi
			Segment2 diagonal = v(Segment2, *centroide, destino);
			insert(triangulacion, diagonal);
		}

		// Se eliminan los indices usados
		delete(indice_a);
		delete(indice_b);
	}

	// Se elimina el arreglo de centroides
	delete(centroides);

	return triangulacion;
}

extern void menu_triangulacion_delaunay (void)
{
	List lista_puntos = new(List);

	while (true)
	{
		BeginDrawing();
			if (dibujar_encabezado("Triangulación de Delaunay"))
				break;

			// Se obtienen los bordes de dibujado
			Corners borde_absoluto = CornAbs(10, 21, 90, 95);
			Rectangle rect_absoluto = ToRectangle(borde_absoluto);
			float longitud_y = (G_XMAX - G_XMIN) * (rect_absoluto.height / rect_absoluto.width);
			Corners borde_relativo = {G_XMIN, longitud_y / 2, G_XMAX, -longitud_y / 2};

			// Color de las diagonales de Delaunay
			Color color = GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_PRESSED));

			// Grosor de las diagonales
			int grosor_linea = (XAbs(1) + YAbs(1)) / 4;

			// Boton para editar los puntos
			if (GuiButton(RectAbs(83, 2, 15, 6), "Editar Puntos"))
			{
				DatosEdicion datos_edicion = {25, 23, 95, 90, G_XMIN, G_XMAX};
				lista_puntos = editar_puntos(lista_puntos, datos_edicion, true);
			}

			// Se dibuja el titulo del grafico
			GuiSetScale(4);
				GuiLabel(RectAbs(5, 16, 0, 0), "Gráfico");
			GuiSetScale(2);

			// Se dibuja la rejilla y el marco del gráfico de Delaunay
			GuiGridCustom(rect_absoluto, ToRectangle(borde_relativo));
			GuiFrame(rect_absoluto);

			// Estructura de datos para el cierre convexo
			Datos datos =
			{
				.vertices = new(List),
				.aristas = new(List),
				.caras = new(List)
			};

			// Se ingresan los puntos a la estructura de 'datos'
			foreach (PTR(Vector2), punto in lista_puntos)
				crear_punto_delaunay(datos, punto->x, punto->y);

			// Se obtiene la triangulacion de Delaunay y la de Voronoi
			Array delaunay = triangulacion_delaunay(datos);
			Array voronoi = triangulacion_voronoi(datos);

			// Se limita el dibujado a la zona del rectangulo absoluto
			BeginRectScissorMode(rect_absoluto);
				// Se dibujan las diagonales
				foreach (Segment2, diagonal in delaunay)
				{
					Vector2 p1 = interpolar_vector(borde_relativo, borde_absoluto, diagonal->a);
					Vector2 p2 = interpolar_vector(borde_relativo, borde_absoluto, diagonal->b);
					DrawLineEx(p1, p2, grosor_linea, ColorAlpha(color, 0.3));
				}

				// Se dibujan las diagonales
				foreach (Segment2, diagonal in voronoi)
				{
					Vector2 p1 = interpolar_vector(borde_relativo, borde_absoluto, diagonal->a);
					Vector2 p2 = interpolar_vector(borde_relativo, borde_absoluto, diagonal->b);
					DrawLineEx(p1, p2, grosor_linea, ORANGE);
				}

				// Se dibujan los puntos
				dibujar_vertices(lista_puntos, borde_relativo, borde_absoluto);
			EndScissorMode();

			// Se eliminan las triangulaciones
			delete(delaunay);
			delete(voronoi);

			// Se eliminan los datos del cierre convexo
			delete(datos.vertices);
			delete(datos.aristas);
			delete(datos.caras);
		EndDrawing();
	}

	delete(lista_puntos);
}