#pragma once
#include "Vector3.h"
#include <variant>
#include <map>
#include <string>

class GlobalVariables {
public:
	
	static GlobalVariables* GetInstance();

	void CreateGroup(const std::string& groupName);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables& obj) = delete;
	GlobalVariables& operator=(const GlobalVariables& obj) = delete;

private:
	struct Item {
		std::variant<int32_t, float, Vector3> value;
	};

	struct Group {
		std::map<std::string, Item> items;
	};

	std::map<std::string, Group> datas_;
};

