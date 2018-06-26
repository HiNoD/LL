#include "stdafx.h"
#include "TableGenerator.h"

namespace
{
const std::string EMPTY_SYMBOL = "<>";
const std::string EOF_SYMBOL = "#";
} // namespace

void CTableGenerator::Generate(const CGrammar::Grammar& grammar)
{
	Initialize(grammar);
	Fill();
}

CTable CTableGenerator::Get() const
{
	return m_table;
}

void CTableGenerator::Initialize(const CGrammar::Grammar& grammar)
{
	size_t currentRowNumber = 0;
	for (const auto& rule : grammar)
	{
		const Rule::RightParts& rightParts = rule.rightParts;

		for (const auto& part : rightParts)
		{
			TableRow row;

			row.referencingSet = part.guides;
			if (&part != &rightParts.back())
			{
				row.isError = false;
			}

			m_unresolvedNextIds.emplace_back(rule.leftPart, part.items);
			AddReferences(rule.leftPart, currentRowNumber);
			AddGuides(rule.leftPart, part.items, part.guides);

			m_table.Add(row);
			++currentRowNumber;
		}
	}
}

void CTableGenerator::Fill()
{
	size_t nextUnresolvedIndex = 0;
	size_t currentRowNumber = m_table.Size();

	for (const auto& unresolvedNextId : m_unresolvedNextIds)
	{
		m_table.Get(nextUnresolvedIndex).next = currentRowNumber;

		if (unresolvedNextId.second.empty())
		{
			++currentRowNumber;
			TableRow row;
			row.referencingSet = m_guidesSet.at(EMPTY_SYMBOL + unresolvedNextId.first);
			m_table.Add(row);
		}

		for (const auto& item : unresolvedNextId.second)
		{
			++currentRowNumber;
			TableRow row;

			if (IsTerminal(item))
			{
				ProcessTerminal(row, item, unresolvedNextId.second, currentRowNumber);
			}
			else
			{
				ProcessNonTerminal(row, item, unresolvedNextId.second, currentRowNumber);
			}

			m_table.Add(row);
		}

		++nextUnresolvedIndex;
	}
}

bool CTableGenerator::IsTerminal(const std::string& symbol)
{
	if (symbol.empty())
	{
		return false;
	}

	return symbol.front() != '<' && symbol.back() != '>';
}

void CTableGenerator::ProcessTerminal(TableRow& row, const std::string& item, const Rule::RightPart::Items& items, size_t currentRowNumber)
{
	row.referencingSet = Rule::RightPart::Guides({ item });

	if (&item != &items.back())
	{
		row.next = currentRowNumber;
	}

	if (item == EOF_SYMBOL)
	{
		row.isEnd = true;
	}
	else
	{
		row.isShift = true;
	}
}

void CTableGenerator::ProcessNonTerminal(TableRow& row, const std::string& item, const Rule::RightPart::Items& items, size_t currentRowNumber)
{
	row.referencingSet = m_guidesSet.at(item);

	row.next = m_tableReferences.at(item).front();

	if (&item != &items.back())
	{
		row.idAtStack = currentRowNumber;
	}
}

void CTableGenerator::AddReferences(const std::string& leftPart, size_t currentRowNumber)
{
	auto reference = m_tableReferences.find(leftPart);
	if (reference == m_tableReferences.end())
	{
		m_tableReferences.emplace(leftPart, std::vector<size_t>(1, currentRowNumber));
	}
	else
	{
		reference->second.emplace_back(currentRowNumber);
	}
}

void CTableGenerator::AddGuides(const std::string& leftPart, const Rule::RightPart::Items& items, const Rule::RightPart::Guides& guides)
{
	auto guidesPos = m_guidesSet.find(leftPart);
	if (guidesPos == m_guidesSet.end())
	{
		m_guidesSet.emplace(leftPart, guides);
	}
	else
	{
		guidesPos->second.insert(guides.cbegin(), guides.cend());
	}

	if (items.empty())
	{
		auto guidesPos1 = m_guidesSet.find(EMPTY_SYMBOL + leftPart);
		if (guidesPos1 == m_guidesSet.end())
		{
			m_guidesSet.emplace(EMPTY_SYMBOL + leftPart, guides);
		}
		else
		{
			guidesPos1->second.insert(guides.cbegin(), guides.cend());
		}
	}
}
