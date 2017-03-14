#pragma once
#include "pSceneObject.h"
#include "pScript.h"
#include <map>

class pScriptManager
{
	friend class pScript;
public:
	pScriptManager();
	~pScriptManager();

	void StartScripts();
	void UpdateScripts(double deltaTime);
	void StopScripts();

	pScript* addScript(std::string name, pScript* script);

	//No error catch
	pScript* getScript_unsafe(std::string name);
	//Error catching
	pScript* getScript(std::string name);

	void removeScript(std::string name);
	void removeScript(pScript* script);
	
private:
	std::map<std::string, pScript*> scriptDictionary;
};

