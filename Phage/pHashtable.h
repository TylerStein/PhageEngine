#pragma once
#include <GL\glew.h>
#include <vector>
#include <string>
#include <iostream>
#include "LogManager.h"

template <class KeyType, class T>
class Hashtable;

//HashItems are a combination of a key and a value that will be stored in this table
template <class KeyType, class T>
class HashItem
{
private:
	int key;
	KeyType keyValue;
	T value;

public:
	//constructor
	HashItem(int ke, KeyType keyVal, T val) : key(ke), keyValue(keyVal), value(val) {}
	//this destructor does nothing
	~HashItem(void) {}

	//Returns the hash of the key
	int getKey()
	{
		return key;
	}

	//Returns the value of the key
	KeyType getKeyValue()
	{
		return keyValue;
	}

	//Returns the value of the hashItem
	T getValue()
	{
		return value;
	}

	//Sets the value of HashItem
	void setValue(T val)
	{
		value = val;
	}

	//Equality operator
	bool operator==(HashItem &other)
	{
		return key == other.key;
	}

	//NonEquality operator
	bool operator!=(HashItem &other)
	{
		return ! (*this == other);
	}

	//Assignment operator
	HashItem operator=(HashItem<KeyType, T> other)
	{
		if (this != &other)
		{
			key = other.key;
			keyValue = other.keyValue;
			value = other.value;
		}
		return *this;
	}
};

template<class KeyType, class TT>
class HashtableIterator
{
private:
	//pointer being iterated
	Hashtable<KeyType, TT> *htable;

	//the bucket containing the current vector being iterated
	int currBucket;

	//the iterator for the current vector
	typename std::vector<HashItem<KeyType, TT> >::iterator currIter;

	/**
	* This searches through the hash table of vectors to find the next vector
	* which contains data and sets the currIter to be the iterator for this vector.
	* If it reaches the end of the hash table without finding any data its sets
	* currBucket to -1 to indicate that this iterator has reached the end.  To
	* use this, set currBucket to the first bucket you want it to start searching
	* to find valid data.
	*/
	void nextValidIterator()
	{
		while (currBucket < htable->tableSize)
		{
			currIter = htable->table[currBucket].begin();
			if (currIter != htable->table[currBucket].end())
				break;
			currBucket++;
		}
		if ((currBucket >= htable->tableSize) || (currIter == htable->table[currBucket].end()))
		{
			currBucket = -1;
		}
	}

public:
	//parameterless constructor pointing at no table
	HashtableIterator() : htable(NULL), currBucket(-1) {}

	//constructor pointing at a table
	HashtableIterator(Hashtable<KeyType, TT> &table, int bucket)
	{
		htable = &table;
		if (bucket == -1)
		{
			// this is the indicator for a past-the-end iterator so we set the
			// current bucket to this value
			currBucket = bucket;
		}
		else
		{
			// any bucket which is not pass-the-end is assumed to mean that we should
			// position the iterator at the first valid member of the Hashtable
			// This starts searching at the first bucket and retrieves an iterator for
			// each vector until it finds one which is not the past-the-end iterator.
			// If it searches all buckets and does not find any data, it sets currBucket
			// to -1 to indicate that this is the past-the-end iterator.
			currBucket = 0;
			nextValidIterator();
		}
	}

	//Constructor to create an iterator referencing an existing element.
	HashtableIterator(Hashtable<KeyType, TT> &table, int bucket, typename std::vector<HashItem<KeyType, TT>>::iterator& iter) :	
	currBucket(bucket), currIter(iter)
	{
		htable = &table;
	}

	//assignment operation
	HashtableIterator& operator=(HashtableIterator &other)
	{
		htable = other.htable;
		currBucket = other.currBucket;
		currIter = other.currIter;

		return *this;
	}

	//equivalance operation
	bool operator==(HashtableIterator &other)
	{
		bool result = false;

		if (htable == other.htable)
		{
			if (currBucket == other.currBucket)
			{
				if (currBucket == -1) result = true;
				else result = currIter == other.currIter;
			}
		}

		return result;
	}

	//antiquivalance operation
	bool operator!=(HashtableIterator &other)
	{
		return !this->operator==(other);
	}

	/**
	* Postfix increment operator to move iterator on to next element.
	*/

	//returns next position
	HashtableIterator& operator++(int)
	{
		if (currBucket >= 0)
		{
			if (currIter != htable->table[currBucket].end())
			{
				currIter++;
			}
			if (currIter == htable->table[currBucket].end())
			{
				currBucket++;
				nextValidIterator();
			}
		}
		return *this;
	}

	//returns current position
	HashItem<KeyType, TT>& operator*()
	{
		return *currIter;
	}
};

template <class KeyType, class T>
class Hashtable
{
private:
	std::vector<HashItem<KeyType, T>> *duplicateTable;
	int	tableSize;
	std::vector<HashItem<KeyType, T>> *table;

	friend HashtableIterator<KeyType, T>;

	//Function that transformes a keys value into a new value from 0 to -1
	int hash(int n)
	{
		int result = n % tableSize;
		if (result < 0) result *= -1;
		return result;
	}

	//Function that transformes a string into a value from 0 to -1
	int hash(std::string &s)
	{
		int result = 0;
		int i;

		for (i = 0; i < s.size(); i++)
		{
			int v = (int)s[i];
			result = (result * 256 + v) % tableSize;
		}
		return result;
	}

public:
	typedef HashtableIterator<KeyType, T> iterator;
	
	//constructor to create an empty hashtable, sz should be a prime number 3 times the size of the data you figure you will be using
	Hashtable(int sz)
	{
		tableSize = 0;
		if (sz > 0)
		{
			tableSize = sz;
			table = new std::vector<HashItem<KeyType, T>>[tableSize];
			duplicateTable = table;
		}
	}
		//test to see if the hashtable is working as intended
		void errorCheck(std::string& msg)
		{
			if (tableSize < 0)
			{
				LogManager::instance()->log(LogManager::LOG_ERROR, std::string("tableSize < 0, ") + msg);
			}

			if (table != duplicateTable)
			{
				LogManager::instance()->log(LogManager::LOG_ERROR, std::string("corrupt table, ") + msg);
			}
		}

		//deletes the Hashtable but not the values in the table
		~Hashtable(void)
		{
			if (table != NULL)
			{
				delete [] table;
				table = NULL;
			}
		}

		//returns the tableSize for the Hashtable
		int getTableSize()
		{
			return tableSize;
		}

		//returns a searched HashItem associated with they key(ke)
		iterator get(int ke)
		{
			int k = hash(ke);
			std::vector<HashItem<KeyType, T>>::size_type i = 0;
			std::vector<HashItem<KeyType, T>> *v = &table[k];
			std::vector<HashItem<KeyType, T>>::iterator iter;

			for (iter = v->begin(); iter != v->end(); iter++)
			{
				if ((*iter).getKey() == ke)
				{
					return iterator(*this, k, iter);
				}
			}
			return end();
		}

		//insert a value into the hashtable
		void put(HashItem<KeyType, T> item)
		{
			int k = hash(item.getKey());
			std::vector<HashItem<KeyType, T>> *v = &table[k];

			v->push_back(item);
		}

		//remove a HashItem at value key(ke)
		void remove(int ke)
		{
			int k = hash(ke);
			std::vector<HashItem<KeyType, T>> *v = &table[k];
			std::vector<HashItem<KeyType, T>>::iterator iter;

			for (iter = v->begin(); iter != v->end(); iter++)
			{
				if ((*iter).getKey() == ke)
				{
					v->erase(iter);
					return;
				}
			}
		}

		//clean up the hashtable
		void clear()
		{
			for (int i = 0; i < tableSize; i++)
			{
				std::vector<HashItem<KeyType, T>> *v = &table[i];
				v->clear();
			}
		}

		//Returns a past-the-end iterator for this HashTable
		iterator end()
		{
			return iterator(*this, -1);
		}

		//Returns an iterator to the begining of the HashTable
		iterator begin()
		{
			return iterator(*this, 0);
		}

		//function for hasing a string
		static int stringhash(std::string &s)
		{
			int result = 0;
			std::string::size_type i;

			for (i = 0; i < s.size(); i++)
			{
				int v = (int)s[i];
				result = v + (result << 3) + (result << 8) - result;
			}
			return result;
		}
	};