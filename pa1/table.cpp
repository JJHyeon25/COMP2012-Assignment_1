/*
 * table.cpp
 *
 *  Created on: 2020. 3. 22.
 *      Author: jujonghyeon25
 */
#include "table.h"
#include <iostream>
using namespace std;

Table::Table(){
	attrs = nullptr;
	entries = nullptr;
	numAttrs = 0;
	numEntries = 0;
}

Table::Table(const Table &another){
	numAttrs = another.numAttrs;

	numEntries = another.numEntries;

	attrs = new string[numAttrs];
	for(int i = 0; i < numAttrs; ++i){
		attrs[i] = another.attrs[i];
	}

	entries = new string*[numEntries];
	for(int i = 0; i < numEntries; ++i){
		entries[i] = new string[numAttrs];
	}
	for(int i = 0; i < numEntries; ++i){
		for(int j = 0; j < numAttrs; ++j){
			entries[i][j] = another.entries[i][j];
		}
	}
}

Table::~Table(){
	if(numAttrs > 0){
		delete [] attrs;
		attrs = nullptr;
	}

	if(numEntries > 0){
		for (int i = 0; i < numEntries; ++i){
			delete[] entries[i];
		}
		delete[] entries;
		entries = nullptr;
	}
}



bool Table::addAttribute(const string &attr, int index, const string &default_value){
	if (index > numAttrs || index < -1)
		return false;

	if (numAttrs == 0){
		++numAttrs;
		attrs = new string[numAttrs];
		attrs[0] = attr;

		entries = new string*[numEntries];
		for (int i = 0; i < numEntries; ++i){
			entries[i] = new string[numAttrs];
			entries[i][0] = default_value;
		}
	}
	else{
		if (index == -1)
			index = numAttrs;

		++numAttrs;

		string* tempAttrs = new string[numAttrs];
		for (int i = 0; i < numAttrs; ++i){
			if (i < index)
				tempAttrs[i] = attrs[i];
			else if (i > index)
				tempAttrs[i] = attrs[i - 1];
			else tempAttrs[i] = attr;
		}

		string** tempEntries = new string*[numEntries];
		for(int i = 0; i < numEntries; ++i){
			tempEntries[i] = new string[numAttrs];
		}
		for(int i = 0; i < numEntries; ++i){
			for(int j = 0; j < numAttrs; ++j){
				if (j < index)
					tempEntries[i][j] = entries[i][j];
				else if (j > index)
					tempEntries[i][j] = entries[i][j - 1];
				else
					tempEntries[i][j] = default_value;
			}
		}

		delete [] attrs;
		attrs = tempAttrs;
		tempAttrs = nullptr;

		for (int i = 0; i < numEntries; ++i){
			delete[] entries[i];
		}
		delete[] entries;
		entries = tempEntries;
		tempEntries = nullptr;
	}
	return true;
}

bool Table::addEntry(const string *entry, int index){
	if (index > numEntries || index < -1)
		return false;

	if (numEntries == 0){
		++numEntries;
		entries = new string*[numEntries];
		entries[0] = new string[numAttrs];
		for (int i = 0; i < numEntries; ++i){
			for (int j = 0; j < numAttrs; ++j){
				entries[i][j] = entry[j];
			}
		}
	}
	else{

		if (index == -1)
			index = numEntries;

		++numEntries;

		string** tempEntries = new string*[numEntries];
		for(int i = 0; i < numEntries; ++i){
			tempEntries[i] = new string[numAttrs];
		}
		for(int i = 0; i < numEntries; ++i){
			for(int j = 0; j < numAttrs; ++j){
				if (i < index)
					tempEntries[i][j] = entries[i][j];
				else if (i > index)
					tempEntries[i][j] = entries[i - 1][j];
				else
					tempEntries[i][j] = entry[j];
			}
		}

		for (int i = 0; i < numEntries - 1; ++i){
			delete[] entries[i];
		}
		delete[] entries;
		entries = tempEntries;
		tempEntries = nullptr;
	}
	return true;
}

bool Table::deleteAttribute(int index){
	if (index < 0 || index > numAttrs - 1)
		return false;

	if (numAttrs == 1){
		delete [] attrs;
		attrs = nullptr;

		--numAttrs;

		if(numEntries == 0)
			return true;

		for (int i = 0; i < numEntries; ++i){
			delete entries[i];
		}
		delete[] entries;
		numEntries = 0;
		return true;
	}

	--numAttrs;

	string* tempAttrs = new string[numAttrs];
	for (int i = 0; i < numAttrs + 1; ++i){
		if (i < index)
			tempAttrs[i] = attrs[i];
		else if (i == index)
			continue;
		else
			tempAttrs[i - 1] = attrs[i];
	}

	string** tempEntries = new string*[numEntries];
	for(int i = 0; i < numEntries; ++i){
		tempEntries[i] = new string[numAttrs];
	}
	for(int i = 0; i < numEntries; ++i){
		for(int j = 0; j < numAttrs + 1; ++j){
			if (j < index)
				tempEntries[i][j] = entries[i][j];
			else if (j == index)
				continue;
			else
				tempEntries[i][j - 1] = entries[i][j];
		}
	}

	delete [] attrs;
	attrs = tempAttrs;
	tempAttrs = nullptr;

	for (int i = 0; i < numEntries; ++i){
		delete[] entries[i];
	}
	delete[] entries;
	entries = tempEntries;
	tempEntries = nullptr;

	return true;
}

bool Table::deleteEntry(int index){
	if (index < 0 || index > numEntries - 1)
		return false;

	if (numEntries == 1){
		for (int i = 0; i < numEntries; ++i){
			delete[] entries[i];
		}
		delete entries;
		numEntries--;
		return true;
	}

	--numEntries;

	string** tempEntries = new string*[numEntries];
	for(int i = 0; i < numEntries; ++i){
		tempEntries[i] = new string[numAttrs];
	}
	for(int i = 0; i < numEntries + 1; ++i){
		for(int j = 0; j < numAttrs; ++j){
			if (i < index)
				tempEntries[i][j] = entries[i][j];
			else if (i == index)
				continue;
			else
				tempEntries[i - 1][j] = entries[i][j];
		}
	}
	for (int i = 0; i < numEntries + 1; ++i){
		delete[] entries[i];
	}
	delete[] entries;
	entries = tempEntries;
	tempEntries = nullptr;
	return true;
}

bool Table::append(const Table &another){
	if(this->numAttrs != another.numAttrs)
		return false;
	for (int i = 0; i < numAttrs; ++i){
		string s1 = attrs[i];
		string s2 = another.attrs[i];
		if (s1 != s2)
			return false;
	}

	for (int i = 0; i < another.numEntries; i++){
		this->addEntry(another.entries[i]);
	}

	return true;
}
