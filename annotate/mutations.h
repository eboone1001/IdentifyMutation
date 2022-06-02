#ifndef mutation
#define mutation
#include <string>
#include <map>
#include <list>
#include "annotation.h"
class Mutations {
public:
	Mutations(std::string fileName,annotations genes);
	//Mutations(Mutations& other);
	//Mutations& operator=(Mutations& other);
	std::map<std::string,std::list<int>>* getMutations();
	//~Mutations();
private:
	std::map<std::string,std::list<int>> mutants;
};
#endif
