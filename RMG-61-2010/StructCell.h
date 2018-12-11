#ifndef STRUCTCELL_H
#define STRUCTCELL_H

#include <string>
using std::string;

struct cell {
	float concentration;
	bool exist;
	int status;
	string messageWithNullStatus;
};

#endif // !STRUCTCELL_H