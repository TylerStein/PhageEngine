#include "pInputSupport.h"

pInputSupport* pInputSupport::_instance = 0;
pInputSupport* pInputSupport::instance()
{
	//Create a new instance if none exists
	if (_instance == 0)
	{
		_instance = new pInputSupport;
	}

	return _instance;
}

pInputSupport::pInputSupport()
{
	standardCursor();
}


pInputSupport::~pInputSupport()
{
	glfwDestroyCursor(cursor);
}
