#include <stdio.h>

int sub_me(int a, int b)
{
	int c;
	c= a-b;
	return c;
}

int add_me(int a, int b)
{
	int c;
	c = a+b;
	int  d = sub_me(a, b);
	return c;
}


int main()
{
	int i;
	i = add_me(10, 20);
	//i= sub_me(10, 20);
	return 0;
}
