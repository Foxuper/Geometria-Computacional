#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <raygui.h>
#include <dark_theme.h>
#include <light_theme.h>
#include <pico_hlcr/pico_hlcr.h>

// Triangulacion.c
extern void menu_area_poligono (void);
extern void menu_triangulacion_poligono (void);

// Convexo2D.c
extern void menu_cierre_convexo_2d (void);

// Convexo3D.c
extern void menu_cierre_convexo_3d (void);

// Delaunay.c
extern void menu_triangulacion_delaunay (void);

// Dimensiones de la pantalla inicial
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

// Dimensiones maximas relativas
#define MAX_XREL 100
#define MAX_YREL 100

// Dimensiones relativas de los gráficos 2D
#define G_XMIN -11
#define G_XMAX  11

// Estos tipos externos encapsulan las estructuras 'Vector2' y 'Vector3'
extern declare_existent(Vector2);
extern declare_existent(Vector3);

// Se declaran los tipos externos
extern declare_type(Segment2);
extern declare_type(Segment3);
extern declare_type(Triangle3);

// Estructura de uso general que almacena segmentos en dos dimensiones
struct Segment2
{
	Vector2 a;
	Vector2 b;
};

// Estructura de uso general que almacena segmentos en tres dimensiones
struct Segment3
{
	Vector3 a;
	Vector3 b;
};

// Estructura de uso general que almacena triangulos en tres dimensiones
struct Triangle3
{
	Vector3 a;
	Vector3 b;
	Vector3 c;	
};

// Estructura de uso general que almacena informacion 3D
typedef struct Data3
{
	List edges;
	List vertices;
	List triangles;
}
Data3;

// Estructura de uso general que almacena las esquinas de un rectangulo
typedef struct Corners
{
	float x1, y1;
	float x2, y2;
}
Corners;

// Estructura de uso general que guarda los datos de rango de
// dibujado para el menu de edicion de puntos y polígonos
typedef struct DatosEdicion
{
	float x1, y1;
	float x2, y2;
	float xmin, xmax;
}
DatosEdicion;

// Tipo de dato que enumera los posibles sentidos de un polígono
typedef enum Sentido
{
	SENTIDO_NULO,
	SENTIDO_POSITIVO,
	SENTIDO_NEGATIVO
}
Sentido;

// Funciones de extension de la librería RayGUI
void     GuiBackupStyle       (void);
void     GuiRestoreStyle      (void);
void     GuiScaleStyle        (int control, int property, float scale);
void     GuiSetScale          (float scale);
void     BeginRectScissorMode (Rectangle bound);
void     GuiFrame             (Rectangle bound);
void     GuiLabelCenter       (Rectangle bound, const char *text);
void     GuiGridCustom        (Rectangle absolute_bound, Rectangle relative_bound);
void     InitCameraCustom     (Camera3D camera);
void     UpdateCameraCustom   (Camera3D *camera);
void     DrawRenderTexture    (RenderTexture2D target, Rectangle source, Rectangle dest);
void     DrawHorizontalGrid   (Vector3 min, Vector3 max, float step);
void     DrawHorizontalPlane  (Vector3 min, Vector3 max, Color color, Color frame_color, float y);
Vector3 *Draw3DSelector       (RenderTexture2D target, Camera camera, Rectangle dest, List points, float *y);

// Funciones de transformacion de coordenadas. Se utilizan para poder
// utilizar una escala de coordenadas relativa y asi poder redimensionar
// la ventana escalando todo el contenido automaticamente
int       XAbs    (float relative_x);
int       YAbs    (float relative_y);
float     XRel    (int absolute_x);
float     YRel    (int absolute_y);
Vector2   VecAbs  (float x, float y);
Rectangle RectAbs (float x, float y, float width, float height);
Corners   CornAbs (float x1, float y1, float x2, float y2);

// Funciones de transformacion entre rectangulos y esquinas
Corners   ToCorners   (Rectangle rectangle);
Rectangle ToRectangle (Corners corners);

// Funciones de interpolacion de valores entre diferentes coordenadas
float   interpolar        (float x1, float x2, float y1, float y2, float x);
Vector2 interpolar_vector (Corners origen, Corners destino, Vector2 vector);

// Funciones lista para facilitar la creación de los menus
List   generar_lista_capitulos  (void);
List   generar_lista_algoritmos (void);
String generar_texto_lista      (List lista);

// Funciones de dibujado
bool     dibujar_encabezado (char *titulo);
Sentido sentido_poligono    (List poligono);
void    dibujar_vertices    (List vertices, Corners borde_poligono, Corners borde_absoluto);
void    dibujar_aristas     (List vertices, Corners borde_poligono, Corners borde_absoluto);
void    dibujar_poligono    (List poligono, Corners borde_poligono, Corners borde_absoluto);
void    dibujar_vertices_3d (RenderTexture2D target, Camera camera, Rectangle dest, Data3 data);

// Funciones de lectura de objetos
List editar_puntos    (List poligono, DatosEdicion datos, bool solo_puntos);
List editar_puntos_3d (Camera camera, List puntos);

bool probar_salida  (void);
void menu_excepcion (void);
void menu_principal (void);

#endif // PRINCIPAL_H