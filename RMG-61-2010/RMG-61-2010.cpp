#include "pch.h"

#include <windows.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <clocale>
#include <cstdlib>
#include <cmath>
#include <stdarg.h>
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
using std::ofstream;


#include "List.h"
#include "Service.h"
#include "Matrix.h"

int main() {
	setlocale(LC_CTYPE, "rus");

	Service::callMenu();

	//system("pause");
	return 0;
}