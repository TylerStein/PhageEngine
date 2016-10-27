#include "pResourceManager.h"

std::ostream& operator<<(std::ostream& os, pResourceHandle<std::string> &hndl)
{
	os << hndl.index;
	return os;
}
