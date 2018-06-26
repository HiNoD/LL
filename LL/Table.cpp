#include "stdafx.h"
#include "Table.h"

void CTable::Add(const TableRow& row)
{
	m_rows.emplace_back(row);
}

size_t CTable::Size() const
{
	return m_rows.size();
}

TableRow& CTable::Get(size_t index)
{
	return m_rows.at(index);
}

TableRow CTable::Get(size_t index) const
{
	return m_rows.at(index);
}