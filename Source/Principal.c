#include <Principal.h>

// Libreria de interfaz grafica
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

// Funciones para operaciones con vectores
#undef to
#include <raylib/src/raymath.h>
#define to ,

// Se define un apuntador para las funciones del menu
typedef void (*Funcion) (void);

// Estado global de la aplicación
static bool salir = false;

// Escala de la GUI
static float guiScale = 1;

// Indice del respaldo del estilo de la GUI
static uint8 guiBackupIndex = 0;

// Respaldo del estilo de la GUI con 3 niveles de pila
static uint8 guiBackupStyle[3][sizeof(guiStyle)] = {0};

// Tipo de dato para almacenar el estado de la camara global
typedef struct CameraDataCustom
{
	Vector2 previousMousePosition;
	float targetDistance;
	Vector2 angle;
}
CameraDataCustom;

// Variable de camara global
static CameraDataCustom CAMERA =
{
	.previousMousePosition = {0, 0},
	.targetDistance = 0,
	.angle = {0, 0}
};

// Implementación del metodo "convertir a texto" para Vector2p
String Vector2p_to_string (var this_ptr)
{
	THIS(PTR(Vector2));
	String casted = new(String);
	double x = (double) this->x, y = (double) this->y;
	format_to(casted, 0, "%3g, %3g", x, y);
	return casted;
}

// Implementación del metodo "convertir a texto" para Vector3p
String Vector3p_to_string (var this_ptr)
{
	THIS(PTR(Vector3));
	String casted = new(String);
	double x = (double) this->x, y = (double) this->y, z = (double) this->z;
	format_to(casted, 0, "%3g, %3g, %3g", z, x, y);
	return casted;
}

// Implementación del metodo "convertir a texto" para Segment2
String Segment2_to_string (var this_ptr)
{
	THIS(Segment2);
	String casted = new(String);
	double x1 = (double) this->a.x, y1 = (double) this->a.y;
	double x2 = (double) this->b.x, y2 = (double) this->b.y;
	format_to(casted, 0, "(%g, %g) -> (%g, %g)", x1, y1, x2, y2);
	return casted;
}

// Implementación del metodo "convertir a texto" para Segment3
String Segment3_to_string (var this_ptr)
{
	THIS(Segment3);
	String casted = new(String);
	double x1 = (double) this->a.x, y1 = (double) this->a.y, z1 = (double) this->a.z;
	double x2 = (double) this->b.x, y2 = (double) this->b.y, z2 = (double) this->b.z;
	format_to(casted, 0, "(%g, %g, %g) -> (%g, %g, %g)", x1, z1, y1, x2, z2, y2);
	return casted;
}

// Definición de la clase Vector2p
Type TYPE(Vector2) = new_type
(
	Vector2,
	implement(Cast, NULL, Vector2p_to_string)
);

// Definición de la clase Vector3p
Type TYPE(Vector3) = new_type
(
	Vector3,
	implement(Cast, NULL, Vector3p_to_string)
);

// Definición de la clase Segment2
Type TYPE(Segment2) = new_type
(
	Segment2,
	implement(Cast, NULL, Segment2_to_string)
);

// Definición de la clase Segment3
Type TYPE(Segment3) = new_type
(
	Segment3,
	implement(Cast, NULL, Segment3_to_string)
);

// Definición de la clase Triangle3
Type TYPE(Triangle3) = new_type(Triangle3);

// Guarda un respaldo del estilo de la GUI
void GuiBackupStyle (void)
{ memcpy(guiBackupStyle[guiBackupIndex++], guiStyle, sizeof(guiStyle)); }

// Restaura el respaldo del estilo de la GUI
void GuiRestoreStyle (void)
{ memcpy(guiStyle, guiBackupStyle[--guiBackupIndex], sizeof(guiStyle)); }

// Escala una propiedad de estilo de la GUI
void GuiScaleStyle (int control, int property, float scale)
{ GuiSetStyle(control, property, GuiGetStyle(control, property) * scale); }

// Escala todas las propiedades de dimensión de la GUI
void GuiSetScale (float scale)
{
	float factor = scale / guiScale;
	guiScale = scale;

	// Global style scaling
	GuiScaleStyle(DEFAULT, BORDER_WIDTH, factor);
	GuiScaleStyle(DEFAULT, TEXT_PADDING, factor);
	GuiScaleStyle(DEFAULT, TEXT_SIZE, factor);
	GuiScaleStyle(DEFAULT, TEXT_SPACING, factor);

	// Control style scaling
	GuiScaleStyle(TOGGLE, GROUP_PADDING, factor);
	GuiScaleStyle(SLIDER, SLIDER_WIDTH, factor);
	GuiScaleStyle(SLIDER, SLIDER_PADDING, factor);
	GuiScaleStyle(PROGRESSBAR, PROGRESS_PADDING, factor);
	GuiScaleStyle(CHECKBOX, CHECK_PADDING, factor);
	GuiScaleStyle(COMBOBOX, COMBO_BUTTON_WIDTH, factor);
	GuiScaleStyle(COMBOBOX, COMBO_BUTTON_PADDING, factor);
	GuiScaleStyle(DROPDOWNBOX, ARROW_PADDING, factor);
	GuiScaleStyle(DROPDOWNBOX, DROPDOWN_ITEMS_PADDING, factor);
	GuiScaleStyle(TEXTBOX, TEXT_INNER_PADDING, factor);
	GuiScaleStyle(TEXTBOX, TEXT_LINES_PADDING, factor);
	GuiScaleStyle(VALUEBOX, TEXT_INNER_PADDING, factor);
	GuiScaleStyle(VALUEBOX, TEXT_LINES_PADDING, factor);
	GuiScaleStyle(SPINNER, SPIN_BUTTON_WIDTH, factor);
	GuiScaleStyle(SPINNER, SPIN_BUTTON_PADDING, factor);
	GuiScaleStyle(LISTVIEW, LIST_ITEMS_HEIGHT, factor);
	GuiScaleStyle(LISTVIEW, LIST_ITEMS_PADDING, factor);
	GuiScaleStyle(LISTVIEW, SCROLLBAR_WIDTH, factor);
	GuiScaleStyle(LISTVIEW, SCROLLBAR_SIDE, factor);
	GuiScaleStyle(COLORPICKER, COLOR_SELECTOR_SIZE, factor);
	GuiScaleStyle(COLORPICKER, HUEBAR_WIDTH, factor);
	GuiScaleStyle(COLORPICKER, HUEBAR_PADDING, factor);
	GuiScaleStyle(COLORPICKER, HUEBAR_SELECTOR_HEIGHT, factor);
	GuiScaleStyle(COLORPICKER, HUEBAR_SELECTOR_OVERFLOW, factor);
	GuiScaleStyle(SCROLLBAR, ARROWS_SIZE, factor);
	GuiScaleStyle(SCROLLBAR, SCROLL_SLIDER_PADDING, factor);
	GuiScaleStyle(SCROLLBAR, SCROLL_SLIDER_SIZE, factor);
	GuiScaleStyle(SCROLLBAR, SCROLL_PADDING, factor);

	// Special control style scaling
	GuiSetStyle(STATUSBAR, TEXT_PADDING, 10 * scale);
}

// Comienza el modo de tijera usando un rectangulo
void BeginRectScissorMode (Rectangle bound)
{ BeginScissorMode(bound.x, bound.y, bound.width, bound.height); }

// Dibuja un marco con el estilo actual de la GUI
void GuiFrame (Rectangle bound)
{
	int border_width = GuiGetStyle(DEFAULT, BORDER_WIDTH);
	Color border_color = GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL));
	GuiDrawRectangle(bound, border_width, border_color, BLANK);
}

// Dibuja una linea de texto centrada en un rectangulo
void GuiLabelCenter (Rectangle bound, const char *text)
{
	GuiBackupStyle();
		GuiSetStyle(LABEL, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
		GuiLabel(bound, text);
	GuiRestoreStyle();
}

// Dibuja una rejilla de lineas en cierta posición abosluta de la ventana
void GuiGridCustom (Rectangle absolute_bound, Rectangle relative_bound)
{
	Corners absolute_corners = ToCorners(absolute_bound);
	Color color =  GetColor(GuiGetStyle(DEFAULT, LINE_COLOR));

	// Si el borde relativo se encuentra volteado, se invierte horizontalmente
	if (relative_bound.height < 0)
	{
		relative_bound.y *= -1;
		relative_bound.height *= -1;
	}

	// Se calculan los intervalos entre cada linea
	float x_spacing = (float) absolute_bound.width / relative_bound.width;
	float y_spacing = (float) absolute_bound.height / relative_bound.height;
	float x_delta = x_spacing * (1 - ceilf(relative_bound.width / 2)) + (absolute_bound.width / 2);
	float y_delta = y_spacing * (1 - ceilf(relative_bound.height / 2)) + (absolute_bound.height / 2);

	// Se dibujan las lineas verticales
	for (float i = absolute_corners.x1 + x_delta; i < absolute_corners.x2; i += x_spacing)
		DrawLineEx((Vector2) {i, absolute_corners.y1}, (Vector2) {i, absolute_corners.y2}, 1, color);

	// Se dibujan las lineas horizontales
	for (float i = absolute_corners.y1 + y_delta; i < absolute_corners.y2; i += y_spacing)
		DrawLineEx((Vector2) {absolute_corners.x1, i}, (Vector2) {absolute_corners.x2, i}, 1, color);
}

// Inicializa una camara 3D
void InitCameraCustom (Camera3D camera)
{
	Vector3 v1 = camera.position;
	Vector3 v2 = camera.target;

	// Diferencia entre el punto objetivo y la posicion de la camara
	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	float dz = v2.z - v1.z;

	// Posición inicial de la camara
	CAMERA.previousMousePosition = GetMousePosition();

	// Distancia entre el punto objetivo y la posicion de la camara
	CAMERA.targetDistance = sqrtf(dx*dx + dy*dy + dz*dz);

	// Angulo inicial de la camara
	CAMERA.angle.x = atan2f(dx, dz);
	CAMERA.angle.y = atan2f(dy, sqrtf(dx*dx + dz*dz));
}

// Actualiza la posición de la camara
void UpdateCameraCustom (Camera3D *camera)
{
	// Se obtiene la posicion actual del mouse
	Vector2 mousePositionDelta = {0, 0};
	Vector2 mousePosition = GetMousePosition();

	// Se obtiene el movimiento del mouse
	mousePositionDelta.x = mousePosition.x - CAMERA.previousMousePosition.x;
	mousePositionDelta.y = mousePosition.y - CAMERA.previousMousePosition.y;
	CAMERA.previousMousePosition = mousePosition;

	// Se calcula el movimiento de la camara al presionar el boton izquierdo del mouse
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		// Se calcula el angulo de rotacion de la camara
		CAMERA.angle.x += -0.01f * mousePositionDelta.x;
		CAMERA.angle.y += -0.01f * mousePositionDelta.y;

		// Se limita el angulo de rotacion de la camara
		CAMERA.angle.y = Clamp(CAMERA.angle.y, -85 * DEG2RAD, 85 * DEG2RAD);
	}

	// Se actualiza la posicion de la camara
	camera->position.x = -sinf(CAMERA.angle.x) * CAMERA.targetDistance*cosf(CAMERA.angle.y) + camera->target.x;
	camera->position.y = -sinf(CAMERA.angle.y) * CAMERA.targetDistance + camera->target.y;
	camera->position.z = -cosf(CAMERA.angle.x) * CAMERA.targetDistance*cosf(CAMERA.angle.y) + camera->target.z;
}

// Dibuja un elemento de renderizado 'RenderTexture2D'
void DrawRenderTexture (RenderTexture2D target, Rectangle source, Rectangle dest)
{
	source = (Rectangle) {source.x, source.y, source.width, -source.height};
	DrawTexturePro(target.texture, source, dest, (Vector2) {0, 0}, 0, WHITE);
}

// Dibuja una rejilla horizontal
void DrawHorizontalGrid (Vector3 min, Vector3 max, float step)
{
	Color gridColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
	for (float i = min.x; i <= max.x; i += step)
		DrawLine3D((Vector3) {i, 0, min.z}, (Vector3) {i, 0, max.z}, gridColor);
	for (float i = min.z; i <= max.z; i += step)
		DrawLine3D((Vector3) {min.x, 0, i}, (Vector3) {max.x, 0, i}, gridColor);
}

// Dibuja un plano horizontal
void DrawHorizontalPlane (Vector3 min, Vector3 max, Color color, Color frame_color, float y)
{
	// Dibuja las lineas laterales
	min.y = y;
	DrawLine3D(min, (Vector3) {min.x, y, max.z}, frame_color);
	DrawLine3D(min, (Vector3) {max.y, y, min.z}, frame_color);

	// Dibuja las lineas laterales tangentes
	max.y = y;
	DrawLine3D((Vector3) {max.x, y, min.z}, max, frame_color);
	DrawLine3D((Vector3) {min.x, y, max.z}, max, frame_color);

	// Dibuja el plano como tal
	Vector3 difference = Vector3Subtract(max, min);
	Vector3 half = {difference.x / 2, 0, difference.z / 2};
	DrawCube(Vector3Add(min, half), difference.x, 0, difference.z, color);
}

// Dibuja un selector especial para puntos 3D
Vector3 *Draw3DSelector (RenderTexture2D target, Camera camera, Rectangle dest, List points, float *y)
{
	const Vector3 min = {-5, -5, -5};
	const Vector3 max = {5, 5, 5};

	// El vector resultado guarda el punto donde se hizo click
	static Vector3 result = {0, 0, 0};

	// Se obtiene la posición del mouse
	Vector2 mouse = GetMousePosition();

	// Se elige el color de los elementos del renderizado
	Color color = GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_PRESSED));

	// Es verdadero si se genera un nuevo punto
	bool newPoint = false;

	// Radio del punto de selección
	float radius = 0.15f + (XAbs(0.3) + YAbs(0.3)) / 100.0f;

	// Se actualiza la altura del plano de selección
	if (CheckCollisionPointRec(mouse, dest))
	{
		float mouseWheelMove = GetMouseWheelMove();
		*y = Clamp(*y + 0.2f * mouseWheelMove, min.y, max.y);
	}

	// Se lee la posicion del mouse y se interpola con el rectangulo de destino
	Vector3 planePosition = (Vector3) {0, roundf(*y), 0};
	mouse.x = interpolar(dest.x, dest.x + dest.width, 0, GetScreenWidth(), mouse.x);
	mouse.y = interpolar(dest.y, dest.y + dest.height, 0, GetScreenHeight(), mouse.y);

	// Se traza un rayo desde el mouse y se intersecta en el plano de selección
	Vector3 planeSize = (Vector3) {(max.x - min.x) / 2, 0, (max.z - min.z) / 2};
	Vector3 box_min = Vector3Subtract(planePosition, planeSize);
	Vector3 box_max = Vector3Add(planePosition, planeSize);
	Ray ray = GetMouseRay(mouse, camera);
	RayCollision collision = GetRayCollisionBox(ray, (BoundingBox) {box_min, box_max});

	// Se crea una copia de la textura original y se modifican sus dimensiones
	Texture2D copy = target.texture;
	target.texture.width = dest.width * 2;
	target.texture.height = dest.height * 2;

	// Se comienza el renderizado en la textura
	BeginTextureMode(target);

		// Se limpia el fondo de la textura
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

		// Se comienza el modo de dibujo 3D
		BeginMode3D(camera);

			// Se dibuja una rejilla horizontal en y = 0
			DrawHorizontalGrid(min, max, 1);

			// Se dibujan los puntos
			foreach (PTR(Vector3), point in points)
				DrawSphereEx(*point, radius, 5, 10, color);

			// Si se detecta una colision, se dibuja un punto en la posicion de la colisión
			if (collision.hit)
			{
				newPoint = true;

				// Se redondean las coordenadas para que sean enteros
				result.x = roundf(collision.point.x);
				result.y = roundf(collision.point.y);
				result.z = roundf(collision.point.z);

				// Se remueven los posibles valores de '-0'
				result.x = (fabsf(result.x) < 1) ? 0 : result.x;
				result.y = (fabsf(result.y) < 1) ? 0 : result.y;
				result.z = (fabsf(result.z) < 1) ? 0 : result.z;

				// Se obtiene el resultado en forma de objeto 'Vector3p'
				Vector3 *result_object = v(Vector3, result.x, result.y, result.z);

				// Si el punto ya existe, se dibuja de un color naranja
				if (points and contains(points, result_object))
				{
					DrawSphereEx(result, radius, 5, 10, ORANGE);
					DrawCylinder(result, radius / 4, radius / 4, -roundf(*y), 10, ColorAlpha(ORANGE, 0.8));
				}
				// Sino, se dibuja con un color normal
				else
				{
					DrawSphereEx(result, radius, 5, 10, color);
					DrawCylinder(result, radius / 4, radius / 4, -roundf(*y), 10, ColorAlpha(color, 0.8));
				}
			}

			// Se dibuja el plano de selección con altura 'y'
			DrawHorizontalPlane(min, max, ColorAlpha(color, 0.3), color, roundf(*y));
		EndMode3D();
	EndTextureMode();

	// Se restaura la textura original y se dibuja en la pantalla
	target.texture = copy;
	DrawRenderTexture(target, (Rectangle) {0, 0, dest.width * 2, dest.height * 2}, dest);

	return newPoint ? &result : NULL;
}

// Valores relativos a valores absolutos
int XAbs (float relative_x)
{ return relative_x * GetScreenWidth() / MAX_XREL; }
int YAbs (float relative_y)
{ return relative_y * GetScreenHeight() / MAX_YREL; }

// Valores absolutos a valores relativos
float XRel (int absolute_x)
{ return (float) absolute_x * MAX_XREL / GetScreenWidth(); }
float YRel (int absolute_y)
{ return (float) absolute_y * MAX_YREL / GetScreenHeight(); }

// Vector con valores absolutos dados valores relativos
Vector2 VecAbs (float x, float y)
{ return (Vector2) {XAbs(x), YAbs(y)}; }

// Rectangulo con valores absolutos dados valores relativos
Rectangle RectAbs (float x, float y, float width, float height)
{ return (Rectangle) {XAbs(x), YAbs(y), XAbs(width), YAbs(height)}; }

// Esquinas de un rectangulo con valores absolutos dados valores relativos
Corners CornAbs (float x1, float y1, float x2, float y2)
{ return (Corners) {XAbs(x1), YAbs(y1), XAbs(x2), YAbs(y2)}; }

// Convierte un rectangulo a las coordenadas de sus esquinas
Corners ToCorners (Rectangle rectangle)
{
	return (Corners)
	{
		rectangle.x,
		rectangle.y,
		rectangle.x + rectangle.width,
		rectangle.y + rectangle.height
	};
}

// Convierte las coordenadas de las esquinas a un rectangulo
Rectangle ToRectangle (Corners corners)
{
	return (Rectangle)
	{
		corners.x1,
		corners.y1,
		corners.x2 - corners.x1,
		corners.y2 - corners.y1
	};
}

// Interpola un valor 'x' dados los puntos extremos
float interpolar (float x1, float x2, float y1, float y2, float x)
{
	float m = (y2 - y1) / (x2 - x1);
	float y = m * (x - x1) + y1;
	return y;
}

// Interpola las coordenadas de un punto entre dos rectangulos
Vector2 interpolar_vector (Corners origen, Corners destino, Vector2 vector)
{
	return (Vector2)
	{
		interpolar(origen.x1, origen.x2, destino.x1, destino.x2, vector.x),
		interpolar(origen.y1, origen.y2, destino.y1, destino.y2, vector.y)
	};
}

// Genera una lista con los capítulos del libro
List generar_lista_capitulos (void)
{
	List lista_capitulos = new
	(
		List,
		S("Triangulación de Polígonos"),
		S("Particionado de Polígonos"),
		S("Cierres Convexos en Dos Dimensiones"),
		S("Cierres Convexos en Tres Dimensiones"),
		S("Diagramas de Voronoi"),
		S("Construcción de Arreglos"),
		S("Busqueda e Intersección"),
		S("Planeación de Movimiento")
	);

	return lista_capitulos;
}

// Genera una lista con todos los algoritmos implementados
List generar_lista_algoritmos (void)
{
	List lista_algoritmos = new
	(
		List,

		// Capitulo 1 - Triangulación de Polígonos
		R(new(List,
			S("Área de un Polígono"),
			S("Triangulación de un Polígono"))),

		// Capitulo 2 - Particionado de Polígonos
		R(new(List,
			S("Partición Trapezoidal"))),

		// Capitulo 3 - Cierres Convexos en Dos Dimensiones
		R(new(List,
			S("Algoritmo de Graham"))),

		// Capitulo 4 - Cierres Convexos en Tres Dimensiones
		R(new(List,
			S("Algoritmo Incremental"))),

		// Capitulo 5 - Diagramas de Voronoi
		R(new(List,
			S("Triangulación de Delaunay"))),

		// Capitulo 6 - Construcción de Arreglos
		R(new(List,
			S("Algoritmo Incremental"))),

		// Capitulo 7 - Busqueda e Intersección
		R(new(List,
			S("Cruzado de Rayos (Punto en Polígono)"),
			S("Cruzado de Rayos (Punto en Políedro)"),
			S("Intersección de Polígonos Convexos"))),

		// Capitulo 8 - Planeación de Movimiento
		R(new(List,
			S("Algoritmo de Minkowski"),
			S("Algoritmo de Configuración de Enlace")))
	);

	return lista_algoritmos;
}

// Genera una cadena de texto formateada para ser mostrada en una lista gráfica
String generar_texto_lista (List lista)
{
	String texto_lista = new(String);

	foreach (var, valor in lista)
	{
		with (String, texto as str(valor))
			concat(texto_lista, texto);

		if (valor isnt last(lista))
			concat(texto_lista, S(";"));
	}

	return texto_lista;
}

// Dibuja un encabezado generico en la pantalla
bool dibujar_encabezado (char *titulo)
{
	// Color de fondo del encabezado
	Color color = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL));

	// Se limpia la pantalla
	ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

	// Se dibuja el titulo principal
	GuiSetScale(4);
		GuiDrawRectangle(RectAbs(0, 0, MAX_XREL, 10), 0, BLANK, color);
		GuiLabelCenter(RectAbs(50, 5, 0, 0), titulo);
	GuiSetScale(2);

	// Se muestra el botón para volver al menú anterior
	return GuiButton(RectAbs(2, 2, 10, 6), "Volver") or probar_salida();
}

// Determina el sentido de los vertices de un polígono
Sentido sentido_poligono (List poligono)
{
	int suma = 0;

	// Se hace una suma para cada vertice del poligono
	foreach (PTR(Vector3), vertice in poligono)
	{
		// Se obtiene el siguiente vertice
		Int indice = index_of(poligono, vertice);
		int indice_siguiente = (c_int(indice) + 1) % len(poligono);
		Vector2 *vertice_siguiente = get(poligono, I(indice_siguiente));
		delete(indice);

		// Se hace una suma del area por cada arista del polígono
		suma += ((vertice_siguiente->x - vertice->x) * (vertice_siguiente->y + vertice->y));
	}

	if (suma > 0)
		return SENTIDO_POSITIVO;
	if (suma < 0)
		return SENTIDO_NEGATIVO;
	return SENTIDO_NULO;
}

// Dibuja una lista de vertices en una region absoluta de la pantalla
void dibujar_vertices (List vertices, Corners borde_poligono, Corners borde_absoluto)
{
	// Se obtiene el color del gráfico
	Color color = GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_PRESSED));

	// Se obtiene el grosor de la linea
	int grosor_linea = (XAbs(1) + YAbs(1)) / 4;

	// Se grafican los vertices del poligono
	foreach (PTR(Vector2), vertice in vertices)
	{
		Vector2 posicion = interpolar_vector(borde_poligono, borde_absoluto, *vertice);
		DrawCircle(posicion.x, posicion.y, grosor_linea * 2, color);
	}
}

// Dibuja aristas dadas por una lista de vertices en una region absoluta de la pantalla
void dibujar_aristas (List vertices, Corners borde_poligono, Corners borde_absoluto)
{
	// Se obtiene el color del gráfico
	Color color = GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_PRESSED));

	// Se obtiene el grosor de la linea
	int grosor_linea = (XAbs(1) + YAbs(1)) / 4;

	// Se grafican las aristas del poligono
	foreach (PTR(Vector2), vertice in vertices)
	{
		// Se obtiene el siguiente vertice
		Int indice = index_of(vertices, vertice);
		int indice_siguiente = (c_int(indice) + 1) % len(vertices);
		Vector2 *vertice_siguiente = get(vertices, I(indice_siguiente));
		delete(indice);

		// Se interpolan los puntos extremos en coordenadas absolutas
		Vector2 a = interpolar_vector(borde_poligono, borde_absoluto, *vertice);
		Vector2 b = interpolar_vector(borde_poligono, borde_absoluto, *vertice_siguiente);

		// Se dibuja la arista
		DrawLineEx(a, b, grosor_linea, color);
	}
}

// Dibuja un polígono generico en una region absoluta de la pantalla
void dibujar_poligono (List poligono, Corners borde_poligono, Corners borde_absoluto)
{
	// Se grafican las aristas del poligono
	dibujar_aristas(poligono, borde_poligono, borde_absoluto);

	// Se grafican los vertices del poligono
	dibujar_vertices(poligono, borde_poligono, borde_absoluto);
}

// Dibuja una lista de vertices 3D en una region absoluta de la pantalla
void dibujar_vertices_3d (RenderTexture2D target, Camera camera, Rectangle dest, Data3 data)
{
	const Vector3 min = {-5, -5, -5};
	const Vector3 max = {5, 5, 5};

	// Se elige el color de los elementos del renderizado
	Color color = GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_PRESSED));

	// Radio del punto de selección
	float radius = 0.15f + (XAbs(0.3) + YAbs(0.3)) / 100.0f;

	// Se crea una copia de la textura original y se modifican sus dimensiones
	Texture2D copy = target.texture;
	target.texture.width = dest.width * 2;
	target.texture.height = dest.height * 2;

	// Se comienza el renderizado en la textura
	BeginTextureMode(target);

		// Se limpia el fondo de la textura
		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

		// Se comienza el modo de dibujo 3D
		BeginMode3D(camera);

			// Se dibuja una rejilla horizontal en y = 0
			DrawHorizontalGrid(min, max, 1);

			// Se dibujan los vertices
			foreach (PTR(Vector3), vertice in data.vertices)
				DrawSphereEx(*vertice, radius, 5, 10, color);

			// Se dibujan las aristas
			foreach (Segment3, arista in data.edges)
				DrawCylinderEx(arista->a, arista->b, radius / 4, radius / 4, 4, ORANGE);

			// Se dibujan las caras
			foreach (Triangle3, triangulo in data.triangles)
				DrawTriangle3D(triangulo->a, triangulo->b, triangulo->c, ColorAlpha(ORANGE, 0.5));
		EndMode3D();
	EndTextureMode();

	// Se restaura la textura original y se dibuja en la pantalla
	target.texture = copy;
	DrawRenderTexture(target, (Rectangle) {0, 0, dest.width * 2, dest.height * 2}, dest);
}

/*
Vista gráfica que permite editar puntos y polígonos
	- Si 'solo_puntos' es verdadero, se entrara en modo de edición de puntos
	- Si 'solo_puntos' es falso, se entrara en modo de edición de polígonos
*/
List editar_puntos (List poligono, DatosEdicion datos, bool solo_puntos)
{
	// Variables locales para la GUI
	int scroll_puntos = 0;

	// Se obtiene el color del gráfico
	Color color = GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_PRESSED));

	while (true)
	{
		BeginDrawing();
			if (dibujar_encabezado(solo_puntos ? "Editar Puntos" : "Editar Polígono"))
				break;

			// Se genera el texto que contiene los vertices del polígono
			String texto_puntos = generar_texto_lista(poligono);

			// Se obtiene la posición del mouse
			Vector2 mouse = GetMousePosition();

			// Punto que contiene la posición relativa del mouse
			Vector2 *punto = NULL;

			// Se obtiene el grosor de la linea
			int grosor_linea = (XAbs(1) + YAbs(1)) / 4;

			// Se obtienen los bordes de dibujado
			Corners borde_absoluto = CornAbs(datos.x1, datos.y1, datos.x2, datos.y2);
			Rectangle rect_absoluto = ToRectangle(borde_absoluto);
			float x_rel = XRel(borde_absoluto.x1);
			float longitud_y = (datos.xmax - datos.xmin) * (rect_absoluto.height / rect_absoluto.width);
			Corners borde_relativo = {datos.xmin, longitud_y / 2, datos.xmax, -longitud_y / 2};

			// Se prueba si el mouse está dentro del gráfico
			if (CheckCollisionPointRec(mouse, rect_absoluto))
			{
				// Se transforman las coordenadas del mouse tomando como origen
				// la esquina superior izquierda del grafico
				mouse.x -= rect_absoluto.x;
				mouse.y -= rect_absoluto.y;

				// Se crea un nuevo rectangulo centrado en el origen para poder interpolar
				Corners origen = {0, 0, rect_absoluto.width, rect_absoluto.height};

				// Se convierten las coordenadas del mouse en un punto con coordenadas relativas
				Vector2 vector = interpolar_vector(origen, borde_relativo, (Vector2) {mouse.x, mouse.y});
				punto = v(Vector2, roundf(vector.x), roundf(vector.y));

				// Se remueven los posibles valores de '-0'
				punto->x = fabsf(punto->x) < 1 ? 0 : punto->x;
				punto->y = fabsf(punto->y) < 1 ? 0 : punto->y;

				// Se detecta el clic izquierdo del mouse
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					if (poligono is NULL)
						poligono = new(List);

					if (contains(poligono, punto))
						// Si el punto ya existe, se elimina
						erase(poligono, punto);
					else
						// Sino, se agrega
						insert(poligono, punto);
				}

				// Se detecta el clic derecho del mouse
				if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
				{
					// Se elimina el ultimo punto del polígono
					if (poligono isnt NULL)
						erase(poligono, last(poligono));
				}
			}

			// Se dibuja la rejilla y el marco del gráfico
			GuiGridCustom(rect_absoluto, ToRectangle(borde_relativo));
			GuiFrame(rect_absoluto);

			// Se dibujan los titulos
			GuiSetScale(4);
				GuiLabel(RectAbs(x_rel - 20, 18, 0, 0), solo_puntos ? "Puntos" : "Vertices");
				GuiLabel(RectAbs(x_rel, 18, 0, 0), "Gráfico");
			GuiSetScale(2);

			// Se crea una lista de los vertices del polígono
			GuiListView(RectAbs(x_rel - 20, 23, 15, 67), c_string(texto_puntos), &scroll_puntos, -1);

			// Se limita el dibujado a la zona del rectangulo absoluto
			BeginRectScissorMode(rect_absoluto);
				// Se dibuja el polígono
				if (solo_puntos)
					dibujar_vertices(poligono, borde_relativo, borde_absoluto);
				else
					dibujar_poligono(poligono, borde_relativo, borde_absoluto);
			EndScissorMode();

			// Si el punto seleccionado es valido...
			if (punto)
			{
				// ...se obtiene la posición real del punto
				Vector2 posicion = interpolar_vector(borde_relativo, borde_absoluto, *punto);

				// Si el punto ya existe, se dibuja de un color naranja
				if (poligono and contains(poligono, punto))
					DrawCircle(posicion.x, posicion.y, grosor_linea * 2, ORANGE);
				// Sino, se dibuja con un color semi-transparente
				else
					DrawCircle(posicion.x, posicion.y, grosor_linea * 2, ColorAlpha(color, 0.6));
			}

			// Estado que muestra el punto en el gráfico
			with (String, texto as str(punto))
			{
				char *texto_estado = punto ? c_string(texto) : "";
				GuiStatusBar(RectAbs(0, MAX_YREL - 5, MAX_XREL, 5), texto_estado);
			}

			// Estado que muestra el número de vertices del polígono
			with (String, texto as new(String))
			{
				if (solo_puntos)
					format_to(texto, 0, "Número de puntos: %d", len(poligono));
				else
					format_to(texto, 0, "Número de vertices: %d", len(poligono));
				GuiStatusBar(RectAbs(10, MAX_YREL - 5, MAX_XREL - 10, 5), c_string(texto));
			}

			if (not solo_puntos)
			{
				// Estado que muestra el sentido de los vertices del polígono
				int sentido = sentido_poligono(poligono);
				char *texto_sentido = "Sentido: Desconocido";
				texto_sentido = (sentido is SENTIDO_POSITIVO) ? "Sentido: Reloj" : texto_sentido;
				texto_sentido = (sentido is SENTIDO_NEGATIVO) ? "Sentido: Contra-Reloj" : texto_sentido;
				GuiStatusBar(RectAbs(36, MAX_YREL - 5, MAX_XREL - 36, 5), texto_sentido);
			}

			delete(texto_puntos);
		EndDrawing();
	}

	// Si el polígono no tiene puntos...
	if (empty(poligono))
	{
		// ... entonces se elimina
		delete(poligono);
		poligono = NULL;
	}

	return poligono;
}

// Vista gráfica que permite editar una lista de puntos en 3D
List editar_puntos_3d (Camera camera, List puntos)
{
	// Variables locales para la GUI
	int scroll_puntos = 0;
	float altura = 0;

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
			if (dibujar_encabezado("Editar Puntos 3D"))
				break;

			// Se genera el texto que contiene los puntos
			String texto_puntos = generar_texto_lista(puntos);

			// Se crean los bordes para el area de dibujo del grafico
			Rectangle rect_absoluto = RectAbs(25, 23, 70, 67);

			// Se dibujan los titulos
			GuiSetScale(4);
				GuiLabel(RectAbs(5, 18, 0, 0), "Puntos");
				GuiLabel(RectAbs(25, 18, 0, 0), "Gráfico");
			GuiSetScale(2);

			// Se crea una lista de los vertices del polígono
			GuiListView(RectAbs(5, 23, 15, 67), c_string(texto_puntos), &scroll_puntos, -1);

			// Se dibuja el gráfico en la textura
			Vector3 *punto = Draw3DSelector(target, camera, rect_absoluto, puntos, &altura);

			// Se obtiene el punto en forma de objeto 'Vector3p'
			Vector3 *punto_objeto = punto ? v(Vector3, punto->x, punto->y, punto->z) : NULL;

			// Se detecta el clic izquierdo del mouse
			if (punto and IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				if (puntos is NULL)
					puntos = new(List);

				if (contains(puntos, punto_objeto))
					// Si el punto ya existe, se elimina
					erase(puntos, punto_objeto);
				else
					// Sino, se agrega
					insert(puntos, punto_objeto);
			}

			// Si el mouse se encuentra dentro del area de dibujo del gráfico
			if (CheckCollisionPointRec(GetMousePosition(), rect_absoluto))
			{
				// Se actualiza la posición de la cámara
				UpdateCameraCustom(&camera);

				// Se elimina el ultimo punto si se presiona el boton derecho del mouse
				if (puntos and IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
					erase(puntos, last(puntos));
			}

			// Se dibuja la sección del gráfico
			GuiFrame(rect_absoluto);

			// Estado que muestra el punto en el gráfico
			with (String, texto as str(punto_objeto))
			{
				char *texto_estado = punto ? c_string(texto) : "";
				GuiStatusBar(RectAbs(0, MAX_YREL - 5, MAX_XREL, 5), texto_estado);
			}

			// Estado que muestra el número de vertices del polígono
			with (String, texto as new(String))
			{
				format_to(texto, 0, "Número de puntos: %d", len(puntos));
				GuiStatusBar(RectAbs(18, MAX_YREL - 5, MAX_XREL - 18, 5), c_string(texto));
			}

			delete(texto_puntos);
		EndDrawing();
	}

	// Si el polígono no tiene puntos...
	if (empty(puntos))
	{
		// ... entonces se elimina
		delete(puntos);
		puntos = NULL;
	}

	// Se elimina la textura de renderizado
	UnloadRenderTexture(target);

	return puntos;
}

// Función que determina si se debe finalizar la aplicaicón
bool probar_salida (void)
{
	if (WindowShouldClose() or salir)
		salir = true;
	return salir;
}

// Funcíon que se muestra en caso de que ocurra una excepción no controlada
void menu_excepcion (void)
{
	// Se obtiene la excepción generada
	Exception e = current(TYPE(Exception));

	// Se reinicia el estado de la aplicación
	salir = false;

	while (not salir)
	{
		BeginDrawing();
			// Color de fondo del encabezado
			Color color = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL));

			// Se limpia la pantalla
			ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

			GuiSetScale(4);
				// Se dibuja el titulo principal
				GuiDrawRectangle(RectAbs(0, 0, MAX_XREL, 10), 0, BLANK, color);
				GuiLabelCenter(RectAbs(50, 5, 0, 0), "Error de ejecución");

				// Se muestra el tipo de excepción
				with (String, mensaje as new(String))
				{
					format_to(mensaje, 0, "Uncaught %s", type_name(e->exception));
					GuiLabelCenter(RectAbs(50, 45, 0, 0), c_string(mensaje));
				}

				// Se muestra el mensaje de la excepción
				GuiLabelCenter(RectAbs(50, 55, 0, 0), c_string(e->message));

				// Se muestra el boton de salir
				salir |= GuiButton(RectAbs(40, 80, 20, 10), "Salir");
				salir |= probar_salida();
			GuiSetScale(2);
		EndDrawing();
	}
}

// Función que muestra el menú principal
void menu_principal (void)
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Geometría computacional");
	GuiLoadStyleDarkTheme();
	GuiSetScale(2);

	List lista_capitulos = generar_lista_capitulos();
	List lista_algoritmos = generar_lista_algoritmos();

	int scroll_capitulo = 0;
	int indice_capitulo = -1;

	int scroll_algoritmo = 0;
	int indice_algoritmo = -1;

	int indice_tema = 0;

	while (not salir)
	{
		Color color_normal_base = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL));
		String texto_capitulos = generar_texto_lista(lista_capitulos);
		String texto_algoritmos = generar_texto_lista(deref(get(lista_algoritmos, I(indice_capitulo))));

		char *cadena_capitulos = c_string(texto_capitulos);
		char *cadena_algoritmos = (len(texto_algoritmos) > 0) ? c_string(texto_algoritmos) : NULL;

		BeginDrawing();
			// Se limpia la pantalla
			ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

			// Titulo principal
			GuiSetScale(4);
				GuiDrawRectangle(RectAbs(0, 0, MAX_XREL, 17), 0, BLANK, color_normal_base);
				GuiLabelCenter(RectAbs(50, 5, 0, 0), "Proyecto Final");
				GuiLabelCenter(RectAbs(50, 12, 0, 0), "Geometría Computacional");
			GuiSetScale(2);

			// Etiquetas de capitulos y algoritmos
			GuiSetScale(4);
				GuiLabel(RectAbs(7, 24, 0, 0), "Capítulo");
				GuiLabel(RectAbs(54, 24, 0, 0), "Algoritmo");
			GuiSetScale(2);

			// Selectores de capitulos y algoritmos
			int aux_capitulo = GuiListView(RectAbs(7, 30, 40, 59), cadena_capitulos, &scroll_capitulo, indice_capitulo);
			indice_algoritmo = GuiListView(RectAbs(54, 30, 40, 28), cadena_algoritmos, &scroll_algoritmo, indice_algoritmo);

			// Selector de tema de la GUI
			int aux_tema = GuiComboBox(RectAbs(54, 82, 40, 7), "Tema Oscuro;Tema Claro", indice_tema);

			// Se selecciona un nuevo capitulo
			if (aux_capitulo != indice_capitulo)
			{
				indice_capitulo = aux_capitulo;
				scroll_algoritmo = 0;
				indice_algoritmo = -1;
			}

			// Se selecciona un tema de la GUI distinto
			if (aux_tema != indice_tema)
			{
				indice_tema = aux_tema;

				if (indice_tema == 0)
					GuiLoadStyleDarkTheme();
				else
					GuiLoadStyleLightTheme();
			}

			// Se inicializa la función del algoritmo seleccionado con NULO
			Funcion menu_algoritmo = NULL;

			// Se hace un 'switch' según el capitulo seleccionado
			switch (indice_capitulo)
			{
				case 0:
					if (indice_algoritmo is 0)
						menu_algoritmo = menu_area_poligono;
					else if (indice_algoritmo is 1)
						menu_algoritmo = menu_triangulacion_poligono;
					break;
				case 1:
					break;
				case 2:
					if (indice_algoritmo is 0)
						menu_algoritmo = menu_cierre_convexo_2d;
					break;
				case 3:
					if (indice_algoritmo is 0)
						menu_algoritmo = menu_cierre_convexo_3d;
					break;
				case 4:
					if (indice_algoritmo is 0)
						menu_algoritmo = menu_triangulacion_delaunay;
					break;
				case 5:
					break;
				case 6:
					break;
				case 7:
					break;
			}

			// Se muestra el botón de ejecución según el algoritmo seleccionado
			if (!menu_algoritmo)
			{
				GuiSetState(GUI_STATE_DISABLED);
				GuiButton(RectAbs(54, 62, 40, 16), "Ejecutar Algoritmo");
				GuiSetState(GUI_STATE_NORMAL);
			}
			else if (GuiButton(RectAbs(54, 62, 40, 16), "Ejecutar Algoritmo"))
				menu_algoritmo();

			// Barra de estado con mi nombre
			GuiStatusBar(RectAbs(0, MAX_YREL - 5, MAX_YREL, 5), "Alonso Ballesteros Torres");
		EndDrawing();

		delete(texto_algoritmos);
		delete(texto_capitulos);
		salir = probar_salida();
	}

	delete(lista_algoritmos);
	delete(lista_capitulos);

	finally
		CloseWindow();
}