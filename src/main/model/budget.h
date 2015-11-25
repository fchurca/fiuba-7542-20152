#ifndef __GFX_BUDGET_H__
#define __GFX_BUDGET_H__
#include <vector>
#include "budget_line.h"

class Budget {
private:
protected:
public:
	std::vector<BudgetLine> lines;
	std::string name;
	Budget(std::string name);
	~Budget();

};
#endif // __GFX_BUDGET_H__