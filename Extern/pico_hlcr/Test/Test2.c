#include <pico_hlcr/pico_hlcr.h>

declare_type(Punto);
declare_type(Vertice);

struct Punto
{
	int x;
	int y;
};

struct Vertice
{
	Punto punto;
	uint32 indice;
};

void Punto_build_with (var this_ptr, Tuple args)
{
	THIS(Punto);
	UNUSED(args);

	this->x = 0;
	this->y = 0;
}

void Vertice_build_with (var this_ptr, Tuple args)
{
	THIS(Vertice);
	UNUSED(args);

	this->punto = new(Punto);
	this->indice = 0;
}

void Vertice_assign (var this_ptr, var other_ptr)
{
	THIS(Vertice);
	Vertice other = other_ptr;

	if (type_of(this) is type_of(other))
	{
		assign(this->punto, other->punto);
		this->indice = other->indice;
	}
}

void Vertice_destroy (var this_ptr)
{
	THIS(Vertice);
	delete(this->punto);
}

String Punto_to_string (var this_ptr)
{
	THIS(Punto);
	String string = new(String);
	format_to(string, 0, "P(%d, %d)", this->x, this->y);
	return string;
}

String Vertice_to_string (var this_ptr)
{
	THIS(Vertice);
	String string = new(String);
	format_to(string, 0, "V%d(%d, %d)", this->indice, this->punto->x, this->punto->y);
	return string;
}

Type TYPE(Punto) = new_type
(
	Punto,
	implement(New, Punto_build_with, NULL),
	implement(Cast, NULL, Punto_to_string)
);

Type TYPE(Vertice) = new_type
(
	Vertice,
	implement(New, Vertice_build_with, Vertice_destroy),
	implement(Assign, Vertice_assign, NULL, NULL),
	implement(Cast, NULL, Vertice_to_string)
);

int main (void)
{
	Array array = new(Array, TYPE(Ptr));

	insert(array, new(Vertice));
	Vertice vertice = deref(first(array));

	vertice->indice = 1;
	vertice->punto->x = 1;
	vertice->punto->y = 2;

	println("Array: #v", array);

	delete(deref(first(array)));
	delete(array);

	getchar();
	return 0;
}