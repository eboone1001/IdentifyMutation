#include "annotation.h"
#include <map>
#include <list>
#include <string>
#include <fstream>
#include <iostream>


/*
Created by Jordan Burkhardt May 24th 2022
Copywrite 2022

The following file contains the function implementations for the GBTags, annotation, and annotations classes.
*/


std::string makeString(char* str)
{
	std::string ret = "";
	while (*str != '\0')
	{
		ret += *str;
		str++;
	}
	return ret;
}

std::string makeString(char* str, int length)
{
	std::string ret = "";
	for(int i = 0; i < length; i++)
	{
		ret += str[i];
	}
	return ret;
}

GBTags::GBTags(char* line)
{
	//get length of line and store in variable s
	int s = 0;
	while (line[s++] != '\0') {}

	//create variables to parse the GBTag section
	bool k = true;
	bool v = false;
	char* key = nullptr;
	int keyL = 0;
	char* value = nullptr;
	int valL = 0;
	//parse the GBtags section, creating maps with keys from each element
	for (int i = 0; i < s; i++)
	{
		if (k)
		{
			if (line[i] == '=')
			{
				k = false;
				v = true;
			}
			else {
				char* tempKey = key;
				key = new char[keyL + 1];
				for (int j = 0; j < keyL; j++)
				{
					key[j] = tempKey[j];
				}
				delete[] tempKey;
				key[keyL++] = line[i];
				
			}
		}
		else if (v)
		{
			if (line[i] == ';')
			{
				v = false;

				// add string terminator to value
				char* tempVal = value;
				value = new char[valL + 1];
				for (int j = 0; j < valL; j++)
				{
					value[j] = tempVal[j];
				}
				delete[] tempVal;
				value[valL++] = '\0';

				// add string terminator to key
				char* tempKey = key;
				key = new char[keyL + 1];
				for (int j = 0; j < keyL; j++)
				{
					key[j] = tempKey[j];
				}
				delete[] tempKey;
				key[keyL++] = '\0';
				tags.insert(std::pair<std::string, std::string>(std::string(key), std::string(value)));
				std::cout << "inserting key: " << std::string(key) << " value: " << std::string(value) << std::endl;
				//if there is a memory access error involving strings, comment out the following 2 lines
				delete[] key;
				delete[] value;
				key = nullptr;
				value = nullptr;
				keyL = 0;
				valL = 0;
				k = true;
			}
			else {
				char* tempVal = value;
				value = new char[valL + 1];
				for (int j = 0; j < valL; j++)
				{
					value[j] = tempVal[j];
				}
				delete[] tempVal;
				value[valL++] = line[i];
			}
		}
	}
}
GBTags::GBTags(GBTags& other){
	for (std::map<std::string, std::string>::iterator it = other.tags.begin(); it != other.tags.end(); it++)
	{
		this->tags.insert(std::pair<std::string, std::string>(it->first, it->second));
	}
}
GBTags& GBTags::operator=(GBTags& other)
{
	for (std::map<std::string, std::string>::iterator it = other.tags.begin(); it != other.tags.end(); it++)
	{
		this->tags.insert(std::pair<std::string, std::string>(it->first, it->second));
	}
	return *this;
}
std::string GBTags::getTag(char* key)
{
	std::string skey = key;
	std::string val = tags[skey];
	
	return val;
}
std::string GBTags::getTag(std::string key)
{
	return tags[key];

}
void GBTags::print()
{
	std::cout << "all gbtags: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = tags.begin(); it != tags.end(); it++)
	{
		std::cout << "key: " << it->first << " value: " << it->second << std::endl;
	}
}
GBTags::~GBTags()
{
}

annotation::annotation(char* line)
{
	//determine length of line and store in variable s
	int s = 0;
	while (line[s++] != '\0') {}
	//integer tabs will determine which column of the annotation we are in
	int tabs = 0;
	int typeLength = 0; //length of characters in type variable
	int genLength = 0; //length of chars in genome variable
	for(int i = 0; i < s; i++)
	{
		if (line[i] == '\t')
		{
			tabs++;
		}
		else if (line[i] == '\0')
		{
			i = s;
		}
		else {
			/*
			declaring variables required inside switch statements because for some reason you cannot declare variables inside of them
			*/
			char* tempGenome = nullptr;
			char* tempType = nullptr;
			switch (tabs)
			{
				case 0:
					tempGenome = genome;
					genome = new char[genLength + 1];
					for (int j = 0; j < genLength; j++)
					{
						genome[j] = tempGenome[j];
					}
					delete[] tempGenome;
					genome[genLength++] = line[i];
					break;
				case 1:
					/*
					we do not want to collect data from the 2nd column, however we must append \0 to genome, as it is finised
					*/
					tempGenome = genome;
					genome = new char[genLength + 1];
					for (int j = 0; j < genLength; j++)
					{
						genome[j] = tempGenome[j];
					}
					delete[] tempGenome;
					genome[genLength++] = '\0';
					break;
				case 2:
					tempType = type;
					type = new char[typeLength + 1];
					for (int j = 0; j < typeLength; j++)
					{
						type[j] = tempType[j];
					}
					delete[] tempType;
					type[typeLength++] = line[i];
					break;
				case 3:
					/*
					before we collect the start index, lets append \0 to type as it is finished
					*/
					tempType = type;
					type = new char[typeLength + 1];
					for (int j = 0; j < typeLength; j++)
					{
						type[j] = tempType[j];
					}
					delete[] tempType;
					type[typeLength++] = '\0';
					//start index
					start = start * 10;
					start += line[i] - 48;
					break;
				case 4:
					//end index
					end = end * 10;
					end += line[i] - 48;
					break;
				case 8:
					tags = new GBTags(line + i);
					i = s;
					break;
				default:
					break;
			}
		}
	}

	std::string GBKeyStr = tags->getTag("gbkey");
	if (GBKeyStr == "Prot") key = GBKey::PROT;
	else if (GBKeyStr == "Gene") key = GBKey::Gene;
	else if (GBKeyStr == "Src") key = GBKey::src;
	else if (GBKeyStr == "5'UTR") key = GBKey::UTR5;
	else if (GBKeyStr == "3'UTR") key = GBKey::UTR3;
	else if (GBKeyStr == "stem_loop") key = GBKey::stem_loop;
	else if (GBKeyStr == "CDS") key = GBKey::CDS;
	else key = GBKey::unknown;


}
annotation::annotation(const annotation& other):key(other.key),start(other.start),end(other.end)
{
	int s = 0;
	while (other.type[s++] != '\0') {}
	this->type = new char[s];
	for (int i = 0; i < s; i++)
	{
		this->type[i] = other.type[i];
	}

	s = 0;
	while (other.genome[s++] != '\0') {}
	this->genome = new char[s];
	for (int i = 0; i < s; i++)
	{
		this->genome[i] = other.genome[i];
	}

	tags = new GBTags(*other.tags);
}
annotation& annotation::operator=(const annotation& other)
{
	this->key = other.key;
	this->start = other.start;
	this->end = other.end;
	this->tags = new GBTags(*other.tags);

	int s = 0;
	while (other.type[s++] != '\0') {}
	this->type = new char[s];
	for (int i = 0; i < s; i++)
	{
		this->type[i] = other.type[i];
	}

	s = 0;
	while (other.genome[s++] != '\0') {}
	this->genome = new char[s];
	for (int i = 0; i < s; i++)
	{
		this->genome[i] = other.genome[i];
	}

	return *this;
}
int annotation::startIndex()
{
	return start;
}
int annotation::endIndex()
{
	return end;
}
GBKey annotation::getKey()
{
	return key;
}
char* annotation::getGenome()
{
	return genome;
}

std::string annotation::getTag(std::string key)
{
	return tags->getTag(key);
}
void annotation::print()
{
	tags->print();
}

annotation::~annotation()
{
	delete tags;
	delete type;
	delete genome;
}


annotations::annotations(char* fileName)
{
	int s = 0;
	while (fileName[s++] != '\0') {}

	if(s >= 5)
	{
		if (fileName[s - 2] == 'f' && fileName[s - 3] == 'f' && fileName[s - 4] == 'g' && fileName[s - 5] == '.')
		{
			std::string line;
			std::string file = std::string(fileName);
			std::ifstream gfffile(file);
			if (gfffile.is_open())
			{
				while (std::getline(gfffile, line))
				{
					if (line[0] != '#')
					{
						int l = line.length();
						char* char_array = new char[l + 1];
						for (int i = 0; i < l; i++)
						{
							char_array[i] = line[i];
						}
						annotation* current = new annotation(char_array);
						delete[] char_array;
						/*
						std::list<annotation> list = set[current->startIndex()];
						list.push_back(*current);
						set[current->startIndex()] = list;
						*/
						set[current->startIndex()].push_back(*current);
					}
				}
				gfffile.close();
			}

			else std::cout << "Unable to open file: " << std::string(fileName) << std::endl;
		}
		else {
			std::cout << "please use a file ending in .gff" << std::endl;
			//std::cout << " , your file: " << std::string(fileName) << " ending letters: " << fileName[s - 1] << ":f " << fileName[s - 2] << ":f " << fileName[s - 3] << ":g " << fileName[s - 4] << ":. " << std::endl;
		}
	}
	else {
		std::cout << "file name too short, please use a .gff file" << std::endl;
	}
	std::map<int, std::list<annotation>>::iterator it;

	/*//prints out all annotations
	for (it = set.begin(); it != set.end(); it++)
	{
		std::list<annotation>::iterator itt;
		for (itt = it->second.begin(); itt != it->second.end(); itt++)
		{
			itt->print();
		}
	}
	*/
	

}


std::list<annotation> annotations::getAnnotations(int startIndex)
{
	std::list<annotation> ans;

	std::map<int, std::list<annotation>>::iterator it;

	for (it = set.begin(); it != set.end(); it++)
	{
		std::list<annotation>::iterator itt;
		for (itt = it->second.begin(); itt != it->second.end(); itt++)
		{
			if (itt->startIndex() == startIndex)
			{
				ans.push_back(*itt);
			}
		}
	}
	return ans;
}

std::list<annotation> annotations::getAnnotations(GBKey key)
{
	std::list<annotation> ans;

	std::map<int, std::list<annotation>>::iterator it;

	for (it = set.begin(); it != set.end(); it++)
	{
		std::list<annotation>::iterator itt;
		for(itt = it->second.begin();itt != it->second.end();itt++)
		{
			if (itt->getKey() == key)
			{
				ans.push_back(*itt);
			}
		}
	}
	return ans;
}

annotations::~annotations()
{
}
