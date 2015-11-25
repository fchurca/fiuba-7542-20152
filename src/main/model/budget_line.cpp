#include "budget_line.h"

BudgetLine::BudgetLine(std::string resource_name, unsigned int amount) :
	resource_name(resource_name),
	amount(amount)
{
}

BudgetLine::~BudgetLine() {
}