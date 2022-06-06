#include <Principal.h>

#define FLOAT(x) F((double) x)

#define PICO_GLM_IMPLEMENTATION
#include <pico_glm.h>

declare_existent_u(vec2);
declare_existent_u(vec3);

String vec2_to_string (var this_ptr)
{
	THISP(vec2);
	String casted = new(String);
	print_to(casted, 0, "(#v, #v)", FLOAT(this->x), FLOAT(this->y));
	return casted;
}

String vec3_to_string (var this_ptr)
{
	THISP(vec3);
	String casted = new(String);
	print_to(casted, 0, "(#v, #v, #v)", FLOAT(this->x), FLOAT(this->y), FLOAT(this->z));
	return casted;
}

Type TYPE(vec2) = new_type(vec2, implement(Cast, NULL, vec2_to_string));
Type TYPE(vec3) = new_type(vec3, implement(Cast, NULL, vec3_to_string));

int main (void)
{
	vec2* a = v(vec2, .x = 1.1, .y = 2.02);
	vec2* b = v(vec2, .x = 1/sqrt(2), .y = 1/sqrt(2));

	*a = vec2_reflect(*a, *b);

	//println("Vec: #v", a);
	//getchar();

	// Se registran las señales de error de C
	register_signals();

	// Se agrega el manejador de excepciones
	exception_handler(menu_excepcion);

	// Se muestra el menú principal
	menu_principal();
	return 0;
}