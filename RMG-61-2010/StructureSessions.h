#ifndef STRUCTURESESSIONS_H
#define STRUCTURESESSIONS_H

#include"DataTable.h"

struct sessions {
	DataTable **sessionDataTablesArray;
	DataTable *crmConcentraions;
	DataTable *crmUncertainties;
	int amountOfSessions;
};

#endif // !STRUCTURESESSIONS_H