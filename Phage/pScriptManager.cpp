#include "pScriptManager.h"
#include <exception>


pScriptManager::pScriptManager()
{
	scriptDictionary = std::map<std::string, pScript*>();
}


pScriptManager::~pScriptManager()
{
}

void pScriptManager::StartScripts()
{
	for (std::map<std::string, pScript*>::iterator iter = scriptDictionary.begin(); iter != scriptDictionary.end(); iter++) {
		iter->second->OnStart();
	}
}

void pScriptManager::UpdateScripts(double deltaTime)
{
	for (std::map<std::string, pScript*>::iterator iter = scriptDictionary.begin(); iter != scriptDictionary.end(); iter++) {
		iter->second->OnUpdate(deltaTime);
	}
}

void pScriptManager::StopScripts()
{
	for (std::map<std::string, pScript*>::iterator iter = scriptDictionary.begin(); iter != scriptDictionary.end(); iter++) {
		iter->second->OnEnd();
	}
}

pScript* pScriptManager::addScript(std::string name, pScript * script)
{

	scriptDictionary.emplace(name, script);
	return script;
}

pScript * pScriptManager::getScript_unsafe(std::string name)
{
	return scriptDictionary[name];
}


pScript * pScriptManager::getScript(std::string name)
{
	pScript* res = nullptr;

	try {
		res = scriptDictionary[name];
	}
	catch (std::exception e) {
		return nullptr;
	}

	return res;
}

void pScriptManager::removeScript(std::string name)
{
	try {
		pScript* tmp = scriptDictionary[name];
		if (tmp == nullptr) {
			return;
		}
		else {
			//Iterate through the dictionary to remove the target script
			std::map<std::string, pScript*> tmpMap = std::map<std::string, pScript*>();
			for (std::map<std::string, pScript*>::iterator iter = scriptDictionary.begin(); iter != scriptDictionary.end(); iter++){
				if (iter->first != name) {
					tmpMap.emplace(iter->first, iter->second);
				}
			}
			scriptDictionary = tmpMap;
		}
	}
	catch(std::exception e){
		LogManager::instance()->warning("Could not find nor remove script named " + name);
		return;
	}
}

void pScriptManager::removeScript(pScript * script)
{
	try {

			//Iterate through the dictionary to remove the target script
			std::map<std::string, pScript*> tmpMap = std::map<std::string, pScript*>();
			for (std::map<std::string, pScript*>::iterator iter = scriptDictionary.begin(); iter != scriptDictionary.end(); iter++) {
				if (iter->second != script) {
					tmpMap.emplace(iter->first, iter->second);
				}
			}
			scriptDictionary = tmpMap;
	}
	catch (std::exception e) {
		LogManager::instance()->warning("Could not find nor remove script given by reference!");
		return;
	}
}
