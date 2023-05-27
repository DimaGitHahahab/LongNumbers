#include "LN.h"
#include "return_codes.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	LN b = LN("952");
	LN a = LN("952");
	b *= b;
	LN c = a + b;
	c.print();
	return SUCCESS;
}
