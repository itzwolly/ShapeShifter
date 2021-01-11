#include "mge\util\StatsManager.h"
#include "mge\config.hpp"
#include <string>
#include <sstream>

std::map<std::string, std::string>* StatsManager::_playerData = new std::map<std::string, std::string>();

StatsManager::StatsManager() { }

StatsManager::~StatsManager() { }

void StatsManager::WriteFile(std::string pFileName) {
	std::ofstream stats;
	stats.open(config::MGE_CONFIG_PATH + pFileName);

	if (stats.is_open()) {
		for (std::map<std::string, std::string>::iterator it = _playerData->begin(); it != _playerData->end(); ++it) {
			stats << it->first << "," << it->second << "\n";
		}
		stats.close();
	}
}

void StatsManager::UpdateMap(std::string pKey, std::string pValue) {
	if (_playerData->count(pKey)) {
		std::map<std::string, std::string>::iterator it = _playerData->find(pKey);

		if (it != _playerData->end()) {
			it->second = pValue;
		}
	} else {
		_playerData->insert(std::pair<std::string, std::string>(pKey, pValue));
	}
}

std::string StatsManager::GetMapValue(std::string pKey) {
	if (_playerData->count(pKey)) {
		return _playerData->find(pKey)->second;
	}
	return "";
}

std::string StatsManager::GetFileValue(std::string pFileName, std::string pKey) {
	std::ifstream file;
	file.open(config::MGE_CONFIG_PATH + pFileName);

	std::string line;
	while (std::getline(file, line)) {
		std::string key, val;
		std::istringstream iss(line);

		while (std::getline(std::getline(iss, key, ',') >> std::ws, val)) {
			if (key == pKey) {
				file.close();
				return val;
			}
		}
	}
	file.close();
	return "";
}
