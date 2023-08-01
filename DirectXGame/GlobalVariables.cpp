#include "GlobalVariables.h"

GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables* globalVariables;
	return globalVariables;
}

void GlobalVariables::CreateGroup(const std::string& groupName) { datas_[groupName]; }
