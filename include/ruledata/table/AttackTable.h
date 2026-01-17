#pragma once
#include <table/BoundIntRowLookupTable.h>
#include <table/TableColumnArmourTypeMatcher.h>

class AttackTable : public BoundIntRowLookupTable<TableColumnArmourTypeMatcher, ArmourType::Type, std::string> {
public:
	AttackTable(std::string id) : BoundIntRowLookupTable("AttackTable_" + id, 0, 150) {}
};
