#include "LN.h"
#include "return_codes.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	LN a = 4;
	LN b = 3;
	LN c = a / b;
	c.print();
	return SUCCESS;
}
