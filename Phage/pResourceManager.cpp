#include "pResourceManager.h"

std::ostream& operator<<(std::ostream& os, ResourceHandle<std::string> &hndl)
{
	os << hndl.index;
	return os;
}
