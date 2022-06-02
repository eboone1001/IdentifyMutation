#include "mutations.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <list>
#include "annotation.h"

Mutations::Mutations(std::string fileName,annotations genes)
{
	int s = 0;
	while (fileName[s++] != '\0') {}
	if (s >= 5)
	{
		if (fileName[s - 2] == 't' && fileName[s - 3] == 'x' && fileName[s - 4] == 't' && fileName[s - 5] == '.')
		{
			std::string line;
			std::string file = std::string(fileName);
			std::ifstream txt(file);
			if (txt.is_open())
			{
				while (std::getline(txt, line))
				{
					std::string strain = "";
					bool mutations = false;
					std::string word = "";
					std::string pname = "";
					bool loc = false;

					int l = line.length();
					for (int i = 0; i < l; i++)
					{
						if (i == l - 1)
						{
							word += line[i];
							if(!genes.getAnnotations(pname).empty())
							{ 
								annotation ant = genes.getAnnotations(pname).front();

								int loci = 0;
								int AA = 0;
								for (int g = 1; g < word.length() - 1; g++)
								{
									AA = AA * 10 + (int)word[g];
								}
								loci = ant.startIndex() + AA * 3;

								mutants[strain].push_back(loci);
							}
							else {
								std::cout << "protein not found: " << pname << std::endl;
							}
						}
						else if (line[i] == ' ' && !mutations){
							mutations = true;
						}
						else
						{
							if (mutations)
							{
								if (line[i] == ':')
								{
									loc = true;
									pname = word;
									word = "";
								}
								else if (line[i] == ',')
								{
									loc = false;

									if(!genes.getAnnotations(pname).empty())
									{ 
										annotation ant = genes.getAnnotations(pname).front();
										//std::list<annotation>::iterator it = genes.getAnnotations(pname).begin();
										int loci = 0;
										int AA = 0;
										for (int g = 1; g < word.length() - 1; g++)
										{
											int cint = 0;
											bool add = true;
											switch (word[g])
											{
											case '0':
												cint = 0;
												break;
											case '1':
												cint = 1;
												break;
											case '2':
												cint = 2;
												break;
											case '3':
												cint = 3;
												break;
											case '4':
												cint = 4;
												break;
											case '5':
												cint = 5;
												break;
											case '6':
												cint = 6;
												break;
											case '7':
												cint = 7;
												break;
											case '8':
												cint = 8;
												break;
											case '9':
												cint = 9;
												break;
											default:
												add = false;
												break;
											}
											if(add)
											{
												AA = AA * 10 + cint;
											}
										}
										loci = ant.startIndex() + AA * 3;
										if(loci < ant.startIndex() || loci > ant.endIndex())std::cout << "Graphing mutation out of bounds at " << loci << " in protein with start index: " << ant.startIndex() << " end index: " << ant.endIndex() << std::endl;
										mutants[strain].push_back(loci);
									}
									else {
										std::cout << "protein not found: " << pname << std::endl;
									}
									word = "";
									pname = "";
								}
								else {
									if (line[i] == '_') word += ' ';
									else  word += line[i];
								}
							}
							else {
								strain += line[i];
							}
						}
						

					}
				}
				txt.close();
			}

			else std::cout << "Unable to open file: " << std::string(fileName) << std::endl;
		}
		else {
			std::cout << "please use a file ending in .txt" << std::endl;
			//std::cout << " , your file: " << std::string(fileName) << " ending letters: " << fileName[s - 1] << ":f " << fileName[s - 2] << ":f " << fileName[s - 3] << ":g " << fileName[s - 4] << ":. " << std::endl;
		}
	}
	else {
		std::cout << "file name too short, please use a .txt file" << std::endl;
	}
	//std::map<int, std::list<annotation>>::iterator it;

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
std::map<std::string,std::list<int>>* Mutations::getMutations()
{
	return &mutants;
}