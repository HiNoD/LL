#pragma once
#include "TableRow.h"

class CTable
{
public:
	void Add(const TableRow&);
	size_t Size() const;
	TableRow& Get(size_t index);
	TableRow Get(size_t index) const;

private:
	std::vector<TableRow> m_rows;
};
