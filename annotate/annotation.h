#ifndef annotate
#define annotate

#include <string>
#include <map>
#include <list>


/*
Created by Jordan Burkhardt May 24th 2022
Copyright 2022

The following file is a header file containing several classes used for containing data from gff files.
GBKey: an enum for the different possible values of GBKey
GBTags: a class that stores all of the varying keys and values including the GBKey in the last column of a .gff row
annotation: contains the annotation type, genome name, start index, stop index, and GBTags for a row
annotations: contains all rows for a .gff file

*/
enum GBKey {
	stem_loop,
	PROT,
	Gene,
	src,
	UTR5,
	UTR3,
	CDS,
	unknown
};

class GBTags {
public:
	GBTags(char* line);
	GBTags(GBTags& other);
	GBTags& operator=(GBTags& other);
	std::string getTag(char* key);
	std::string getTag(std::string key);
	void print();
	~GBTags();
private:
	std::map<std::string, std::string> tags;
};


class annotation {
public:
	annotation(char* line);
	annotation(const annotation& other);
	annotation& operator=(const annotation& other);
	int startIndex();
	int endIndex();
	GBKey getKey();
	char* getGenome();
	void print();
	std::string getTag(std::string key);


	~annotation();
private:
	GBTags* tags;
	GBKey key;
	char* type;
	char* genome;
	int start;
	int end;
};

class annotations {
public:
	annotations(char* fileName);
	std::list<annotation> getAnnotations(int startIndex);
	std::list<annotation> getAnnotations(GBKey key);

	~annotations();
private:
	std::map<int, std::list<annotation>> set;

};

class gene : public annotation {
	public:
		gene();
		bool withinBounds();
		void addCDS(char* line);
	private:

};

class CDS : public annotation {

};





#endif annotate
