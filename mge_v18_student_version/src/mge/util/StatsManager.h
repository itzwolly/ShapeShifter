#ifndef STATSMANAGER_H
#define STATSMANAGER_H

#include <iostream>
#include <string>
#include <fstream>
#include <map>

class StatsManager {
	public:
		StatsManager();
		~StatsManager();
		static void WriteFile(std::string pFileName);
		static void UpdateMap(std::string pKey, std::string pValue);
		static std::string GetMapValue(std::string pKey);
		static std::string GetFileValue(std::string pFileName, std::string pKey);

	private:
		static std::map<std::string, std::string>* _playerData;
};

#endif // DEBUGHUD_HPP