#ifndef __GFX_BUDGET_LINE_H__
#define __GFX_BUDGET_LINE_H__
#include <string>

class BudgetLine {
private:
protected:
public:
	std::string resource_name;
	unsigned int amount;
	BudgetLine(std::string resource_name, unsigned int amount);
	~BudgetLine();

};
#endif // __GFX_BUDGET_LINE_H__