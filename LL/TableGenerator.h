#pragma once
#include "Grammar.h"
#include "Table.h"

class CTableGenerator
{
public:
	void Generate(const CGrammar::Grammar& grammar);
	CTable Get() const;

private:
	void Initialize(const CGrammar::Grammar& grammar);
	void Fill();

	void AddReferences(const std::string& leftPart, size_t currentRowNumber);
	void AddGuides(const std::string& leftPart, const Rule::RightPart::Items& items, const Rule::RightPart::Guides& guides);

	bool IsTerminal(const std::string& symbol);
	void CTableGenerator::ProcessTerminal(TableRow& row, const std::string& item, const Rule::RightPart::Items& items, size_t currentRowNumber);
	void CTableGenerator::ProcessNonTerminal(TableRow& row, const std::string& item, const Rule::RightPart::Items& items, size_t currentRowNumber);

	CTable m_table;

	std::vector<std::pair<std::string, Rule::RightPart::Items>> m_unresolvedNextIds;
	std::map<std::string, std::vector<size_t>> m_tableReferences;
	std::map<std::string, Rule::RightPart::Guides> m_guidesSet;
};
