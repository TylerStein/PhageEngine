#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "pHashTable.h"
//Based off of Robs designe for Humdinger
/*
* A handle is used to represent a resource by storing the index at which the resource
* is stored in the ResourceManager.  This allows efficient access to resources stored in
* the ResourceManager.  The Handle is a template class so that it can only be used with
* the correct type of ResourceManager.
*/
template <class ResouceType>
class ResourceHandle
{
private:
	int index;

public:
	//constructor that creates a NULL handle
	ResourceHandle() :
		index(-1) {}

	//Constructor that creates a handle to refrence at the indicated location of index
	ResourceHandle(int idx) :
		index(idx) {}

	//tests if this handle refrences NULL
	bool isNull() const
	{
		return index < 0;
	}

	//turn the handle NULL
	void nullify()
	{
		index - 1;
	}

	//returns the current index value
	int getIndex() const
	{
		return index;
	}

	//Assigment operator
	ResourceHandle& operator=(ResourceHandle& other)
	{
		index = other.index;
		return *this;
	}

	//Equality operator
	bool operator==(ResourceHandle& other) const
	{
		return index == other.index;
	}

	//Inequality operator
	bool operator!=(ResourceHandle& other) const
	{
		return index != other.index;
	}

	//Less than operator
	bool operator<(ResourceHandle& other) const
	{
		return index < other.index;
	}

	//Greater than operator
	bool operator>(ResourceHandle& other) const
	{
		return index < other.index;
	}

	friend std::ostream& operator<<(std::ostream& os, ResourceHandle &hndl);
};

/**
* The ResourceManager is an in-memory container which stores resources.  The ResourceManager
* is a template class so that each resource type must have its own instance of the
* ResourceManager.  The resources are stored by pointer in a vector which can be accessed by
* the index contained in a ResourceHandle.  There is a secondary index which uses a Hashtable
* to provide rapid access to resources by name when a ResourceHandle is not available.
* still based off of robs code from humdinger
*/
template<class ResourceType>
class ResourceManager
{
private:
	std::vector<ResourceType*> resourceList;
	//implement hashtable *nametable
	Hashtable<std::string, ResourceHandle<ResourceType>> *nameTable;
public:
	//Create an empty ResourceManager with a default pHashtable size
	ResourceManager(void)
	{
		nameTable = new Hashtable<std::string, ResourceHandle<ResourceType>>(111);
	}

	//Create a empty ResourceManager with a hashtable with size sz
	ResourceManager(int sz)
	{
		nameTable = new Hashtable<std::string, ResourceHandle<ResourceType>>(sz);
	}

	//Destructor.This will not delete any of the resources stored in the ResourceManager.
	~ResourceManager(void)
	{
		if (nameTable != NULL)
		{
			delete nameTable;
			nameTable = NULL;
		}
	}

	//Trys to add a new resource to ResourceManager, if its not new then it does nothing
	ResourceHandle<ResourceType> put(std::string &name, ResourceType *res)
	{
		Hashtable<std::string, ResourceHandle<ResourceType>>::iterator	iter;
		int key = Hashtable<std::string, ResourceHandle<ResourceType>>::stringhash(name);

		iter = nameTable->get(key);
		if (iter == nameTable->end())
		{
			int idx = resourceList.size();
			resourceList.push_back(res);
			ResourceHandle<ResourceType> handle(idx);
			HashItem<std::string, ResourceHandle<ResourceType>> item(key, name, handle);
			nameTable->put(item);
			return handle;
		}
		return (*iter).getValue();
	}

	//Returns a pointer to the resource, if invalid a NULL will be returned
	ResourceType* get(ResourceHandle<ResourceType> &handle) const
	{
		int idx = handle.getIndex();
		ResourceType *result = NULL;

		if (idx >= 0 && idx < int(resourceList.size()))
		{
			result = resourceList[idx];
		}
		return result;
	}

	//Searches for a resource name and returns a handle for it. If this fails a NULL will be returned to handle
	ResourceHandle<ResourceType> get(const std::string &name) const
	{
		Hashtable<std::string, ResourceHandle<ResourceType>>::iterator	iter;
		int key = Hashtable<std::string, ResourceHandle<ResourceType>>::stringhash((std::string)name);

		iter = nameTable->get(key);
		if (iter == nameTable->end())
		{
			return ResourceHandle<ResourceType>();
		}
		return (*iter).getValue();
	}

	//Remove a resource by name from the manager
	void remove(std::string &name)
	{
		Hashtable<std::string, ResourceHandle<ResourceType>>::iterator	iter;
		int key = Hashtable<std::string, ResourceHandle<ResourceType>>::stringhash(name);

		iter = nameTable->get(key);
		if (iter != nameTable->end())
		{
			int idx = (*iter).getValue().getIndex();
			resourceList[idx] = NULL;
			nameTable->remove(key);
		}
	}

	//Remove all keys and values from the manager, deleting them.
	void clearKeysAndValues()
	{
		std::vector<int>	deleteKeyList;
		Hashtable<std::string, ResourceHandle<ResourceType> >::iterator iter = nameTable->begin();

		// First, we traverse the hashtable and delete the models.
		
		// We save the string keys to be deleted and delete these on a second pass so we do not mess with the iterator.
		while (iter != nameTable->end())
		{
			HashItem<std::string, ResourceHandle<ResourceType> > item = *iter;
			ResourceHandle<ResourceType> handle = item.getValue();
			if (!handle.isNull())
			{
				ResourceType *res = resourceList[handle.getIndex()];
				if (res != NULL)
				{
					delete res;
					resourceList[handle.getIndex()] = NULL;
				}
				deleteKeyList.push_back(item.getKey());
			}
			iter++;
		}

		std::vector<int>::iterator viter = deleteKeyList.begin();
		while (viter != deleteKeyList.end())
		{
			int ke = *viter;
			nameTable->remove(ke);
			viter++;
		}

		// Now, clear the resource list so it does not grow forever
		resourceList.clear();
	}

	//check for errors by using nametables method but with a fed message
	void errorCheck(std::string& msg)
	{
		nameTable->errorCheck(msg);
	}
};

template<class ResourceType>
std::ostream& operator<<(std::ostream& os, ResourceHandle<ResourceType> &hndl)
{
	os << hndl.index;
	return os;
}