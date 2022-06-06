#include <pico_hlcr/pico_hlcr.h>

declare_type(Punto);

struct Punto
{
	int32 x;
	int32 y;
};

String Punto_to_string (var this_ptr)
{
	THIS(Punto);
	String casted = new(String);
	format_to(casted, 0, "Punto(%d, %d)", this->x, this->y);
	return casted;
}

Type TYPE(Punto) = new_type
(
	Punto,
	implement(Cast, NULL, Punto_to_string)
);

int main (void)
{
	register_signals();

	Punto x = new(Punto, v(Punto, .x= 1, .y= 2));
	println("#v", x);
	delete(x);

	getchar();
	return 0;
}