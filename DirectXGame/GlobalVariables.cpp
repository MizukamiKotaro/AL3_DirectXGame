#include "GlobalVariables.h"
#include "ImGuiManager.h"
#include <json.hpp>
#include <fstream>

GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables globalVariables;
	return &globalVariables;
}

void GlobalVariables::Update() {

	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {

		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) {
		return;
	}

	for (std::map<std::string, Group>::iterator itGroup = datas_.begin();
	     itGroup != datas_.end(); ++itGroup) {
		
		const std::string& groupName = itGroup->first;

		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str())) {
			continue;
		}
		
		for (std::map<std::string, Item>::iterator itItem = group.items.begin();
		     itItem != group.items.end(); ++itItem) {
			
			const std::string& itemName = itItem->first;

			Item& item = itItem->second;


			if (std::holds_alternative<int32_t>(item.value)) {
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			} else if (std::holds_alternative<float>(item.value)) {
				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, -10.0f, 10.0f);
			} else if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}
		}


		ImGui::Text("\n");

		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();
	}



	ImGui::EndMenuBar();
	ImGui::End();

}

void GlobalVariables::CreateGroup(const std::string& groupName) { datas_[groupName]; }

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) {
	
	Group& group = datas_[groupName];

	Item newItem{};
	newItem.value = value;

	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	Group& group = datas_[groupName];

	Item newItem{};
	newItem.value = value;

	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, Vector3 value) {
	Group& group = datas_[groupName];

	Item newItem{};
	newItem.value = value;

	group.items[key] = newItem;
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {

	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	Group& group = itGroup->second;

	if (group.items.find(key) == group.items.end()) {
		SetValue(groupName, key, value);
	}

	/*for (std::map<std::string, Group>::iterator itGroup = datas_.begin();
	     itGroup != datas_.end(); ++itGroup) {
		
		Group& group = itGroup->second;

		if (groupName == itGroup->first) {
			if (group.items.find(key) == group.items.end()) {
				SetValue(groupName, key, value);
				break;
			}
		}
	}*/
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	Group& group = itGroup->second;

	if (group.items.find(key) == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3& value) {
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	Group& group = itGroup->second;

	if (group.items.find(key) == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const {

	assert(datas_.find(groupName) != datas_.end());

	const Group& group = datas_.at(groupName);

	assert(group.items.find(key) != group.items.end());

	return std::get<int32_t>(group.items.find(key)->second.value);
}

float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const {

	assert(datas_.find(groupName) != datas_.end());

	const Group& group = datas_.at(groupName);

	assert(group.items.find(key) != group.items.end());

	return std::get<float>(group.items.find(key)->second.value);
}

Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) const {

	assert(datas_.find(groupName) != datas_.end());

	const Group& group = datas_.at(groupName);

	assert(group.items.find(key) != group.items.end());

	return std::get<Vector3>(group.items.find(key)->second.value);
}

void GlobalVariables::SaveFile(const std::string& groupName) {


	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	assert(itGroup != datas_.end());

	nlohmann::json root;

	root = nlohmann::json::object();
	
	
	root[groupName] = nlohmann::json::object();


	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
	     itItem != itGroup->second.items.end(); ++itItem) {
		
		const std::string& itemName = itItem->first;

		Item& item = itItem->second;


		if (std::holds_alternative<int32_t>(item.value)) {
			
			
			root[groupName][itemName] = std::get<int32_t>(item.value);
		} else if (std::holds_alternative<float>(item.value)) {

			root[groupName][itemName] = std::get<float>(item.value);
		} else if (std::holds_alternative<Vector3>(item.value)) {

			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = nlohmann::json::array({value.x, value.y, value.z});
		}

	}

	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(dir)) {
		std::filesystem::create_directories(dir);
	}


	std::string filePath = kDirectoryPath + groupName + ".json";

	std::ofstream ofs;

	ofs.open(filePath);

	if (ofs.fail()) {
		std::string message = "Failed open data file file for write";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	ofs << std::setw(4) << root << std::endl;

	ofs.close();
}

void GlobalVariables::LoadFiles() {

	std::filesystem::path dir(kDirectoryPath);

	if (!std::filesystem::exists(dir)) {
		return;
	}

	std::filesystem::directory_iterator dir_it(dir);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		
		const std::filesystem::path& filePath = entry.path();


		std::string extension = filePath.extension().string();

		if (extension.compare(".json") != 0) {
			continue;
		}

		LoadFile(filePath.stem().string());
	}

}

void GlobalVariables::LoadFile(const std::string& groupName) {

	std::string filePath = kDirectoryPath + groupName + ".json";

	std::ifstream ifs;

	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "Failed open data file file for read";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}


	nlohmann::json root;

	ifs >> root;

	ifs.close();


	nlohmann::json::iterator itGroup = root.find(groupName);

	assert(itGroup != root.end());


	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		
		const std::string& itemName = itItem.key();


		if (itItem->is_number_integer()) {
			
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		} else if (itItem->is_number_float()) {

			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		} else if (itItem->is_array() && itItem->size() == 3) {

			Vector3 value = {itItem->at(0), itItem->at(1), itItem->at(2)};
			SetValue(groupName, itemName, value);
		}
	}
}
