#ifndef STRUCTURESESSIONS_H
#define STRUCTURESESSIONS_H

#include"DataTable.h"

struct sessions {
	DataTable **sessionDataTablesArray;
	int amountOfSessions;
};

#endif // !STRUCTURESESSIONS_H