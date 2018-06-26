#include "stdafx.h"
#include "Grammar.h"

namespace
{
const std::string LINE_SPLIT_PATTERN = "->|( +)";
const std::string GUIDE_SET_SYMBOL = "/";
} // namespace

CGrammar::CGrammar()
	: m_lineSplitRegex(boost::regex(LINE_SPLIT_PATTERN))
{
}

void CGrammar::ReadAndParseGrammar(std::istream& input)
{
	std::string line;
	std::string leftGrammarPart;

	std::vector<std::string> parsedLine;
	while (getline(input, line))
	{
		ParseLine(line, parsedLine);
		leftGrammarPart = parsedLine.at(0);
		parsedLine.erase(parsedLine.begin());

		Rule::RightPart::Items items;
		Rule::RightPart::Guides guides;

		bool isItems = true;
		for (const auto& parsedElement : parsedLine)
		{
			if (parsedElement == GUIDE_SET_SYMBOL)
			{
				isItems = false;
				continue;
			}

			if (isItems)
			{
				items.emplace_back(parsedElement);
			}
			else
			{
				guides.emplace(parsedElement);
			}
		}

		Rule::RightPart rightPart;
		rightPart.items = items;
		rightPart.guides = guides;

		size_t pos = GetRuleIndex(leftGrammarPart);
		if (pos == -1)
		{
			Rule newRule;
			newRule.leftPart = leftGrammarPart;
			newRule.rightParts = Rule::RightParts(1, rightPart);

			m_grammar.push_back(newRule);
		}
		else
		{
			m_grammar.at(pos).rightParts.emplace_back(rightPart);
		}
	}
}

void CGrammar::ConvertToLL()
{
	/*for (Rule& rule : m_grammar)
	{
		Rule::RightParts tmpRightParts = rule.rightParts;
		unsigned counter = 1;
		while (!tmpRightParts.empty())
		{
			Rule::RightPart curr = tmpRightParts.front();
			tmpRightParts.erase(tmpRightParts.begin());

			Rule::RightParts equals;
			equals.push_back(curr);

			for (size_t j = 0; j < tmpRightParts.size();)
			{
				if (curr.at(0) == tmpRightParts.at(j).at(0))
				{
					equals.push_back(tmpRightParts.at(j));
					tmpRightParts.erase(tmpRightParts.begin() + j);
				}
				else
				{
					++j;
				}
			}

			if (equals.size() > 1)
			{
				ConvertRule(rule, equals, counter);
			}

			++counter;
		}
	}*/
}

CGrammar::Grammar CGrammar::GetGrammar() const
{
	return m_grammar;
}

void CGrammar::ParseLine(const std::string& line, std::vector<std::string>& splitResult) const
{
	boost::algorithm::split_regex(splitResult, line, m_lineSplitRegex);
}

/* return index of first rule with leftGrammarPart or -1 if rule not exist */
long CGrammar::GetRuleIndex(const std::string& leftGrammarPart) const
{
	auto comparator = [&leftGrammarPart](const Rule& rule) {
		return rule.leftPart == leftGrammarPart;
	};
	auto result = std::find_if(m_grammar.begin(), m_grammar.end(), comparator);

	return (result != m_grammar.end()) ? std::distance(m_grammar.begin(), result) : -1;
}

bool CGrammar::AreEquals(Rule::RightParts rightParts, size_t currPos) const
{
	/*if (currPos >= rightParts.at(0).size())
	{
		return false;
	}

	std::string item = rightParts.at(0).at(currPos);
	*/
	bool result = true;

	/*for (size_t i = 1; i < rightParts.size(); ++i)
	{
		if (currPos >= rightParts.at(i).size())
		{
			result = false;
			break;
		}
		if (rightParts.at(i).at(currPos) != item)
		{
			result = false;
			break;
		}
	}*/

	return result;
}

void CGrammar::ConvertRule(Rule& rule, const Rule::RightParts& equals, unsigned counter)
{
	/*Rule::RightPart equalRightPart = GetEqualRightPart(equals);
	Rule::RightParts differentRightParts = GetDifferentRingtParts(equals, equalRightPart.size());

	Rule newRule;
	newRule.leftPart = rule.leftPart;
	newRule.leftPart.insert(newRule.leftPart.size() - 2, std::to_string(counter));
	for (auto rightPart : differentRightParts)
	{
		newRule.rightParts.emplace_back(rightPart);
	}

	Rule::RightPart tmp = equalRightPart;
	tmp.push_back(newRule.leftPart);
	rule.rightParts = Rule::RightParts(1, tmp);

	m_grammar.emplace_back(newRule);*/
}

Rule::RightPart CGrammar::GetEqualRightPart(const Rule::RightParts& equals) const
{
	Rule::RightPart result;
	/*size_t currPos = 0;

	while (currPos < GetMinSize(equals))
	{
		if (AreEquals(equals, currPos))
		{
			result.push_back(equals.at(0).at(currPos));
			++currPos;
		}
		else
		{
			break;
		}
	}*/

	return result;
}

Rule::RightParts CGrammar::GetDifferentRingtParts(const Rule::RightParts& equals, size_t equalsLength)
{
	Rule::RightParts differentRightParts;

	/*for (auto equal : equals)
	{
		if (equal.size() == equalsLength)
		{
			differentRightParts.emplace_back(Rule::RightPart(1, ""));
		}
		if (equal.size() > equalsLength)
		{
			Rule::RightPart tmpRightPart;
			for (size_t i = equalsLength; i < equal.size(); ++i)
			{
				tmpRightPart.emplace_back(equal.at(i));
			}
			differentRightParts.push_back(tmpRightPart);
		}
	}*/

	return differentRightParts;
}

size_t CGrammar::GetMinSize(const Rule::RightParts& equals) const
{
	/*size_t minSize = equals.at(0).size();
	size_t itemSize = 0;

	for (const auto& item : equals)
	{
		itemSize = item.size();
		if (itemSize < minSize)
		{
			minSize = itemSize;
		}
	}
	return minSize;*/
	return 0;
}
