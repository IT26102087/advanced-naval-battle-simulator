#include<stdio.h>
#include<common.h>

int main()
{
	Battleship b;
	b.notation = 'U';
	b.x = 500;
	b.y = 500;
	b.vMax = 150;
	b.alive = 1;

	Escortship e;
	e.id = 0;
	e.type = TYPE_A;
	e.x = 300;
	e.y = 400;
	e.alive = 1;

	printf("Battleship: %c at (%.1f, %.1f), alive = %d\n", b.notation, b.x, b.y, b.alive);
	printf("Escort ship %d: type =%d at (%.1f, %.1f), alive = %d\n",e.id, e.type, e.x, e.y, e.alive);
		
	return 0;
}
