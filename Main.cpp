#include <iostream>

#include <string>
/*
main.cpp file takes 4 command line arguements, flags for query and reference, and file names for query and reference.
If no command line arguements are used it will provide defaults query.fa and ref.fa
*/

int main(int argc, char* argv[]) 
{
	/*
	proper usage: ./identify.exe -q <query/ies> -r <reference genome>
	*/
	char* queries = new char[0];
	char* refGenome = new char[0];
	bool q = false;
	bool r = false;
	bool success = true;
	if (argc == 1) {
		queries = new char[9];

		char query[] = { 'q','u','e','r','y','.','f','a','\0'};
		for (int j = 0; j < sizeof(query) / sizeof(char); j++) {
			queries[j] = query[j];
		}

		refGenome = new char[7];
		char ref[] = { 'r','e','f','.','f','a','\0'};
		for (int j = 0; j < sizeof(ref) / sizeof(char); j++)
		{
			refGenome[j] = ref[j];
		}
	}
	for (int i = 0; i < argc; i++)
	{
		int s = 0;
		while (argv[i][s++] != '\0') { }
		if (q) {
			
			queries = new char[s];
			for (int j = 0; j < s; j++)
			{
				queries[j] = argv[i][j];
			}
			q = false;
		}
		else if(r)
		{
			refGenome = new char[s];
			for (int j = 0; j < s; j++)
			{
				refGenome[j] = argv[i][j];
			}
			r = false;
		}else if (s >= 3)
		{
			if (argv[i][0] == '-' && (argv[i][1] == 'q' || argv[i][1] == 'Q'))
			{
				q = true;
			}
			else if (argv[i][0] == '-' && (argv[i][1] == 'r' || argv[i][1] == 'R'))
			{
				r = true;
			}
			else {
				std::cout << "proper usage: ./identify.exe -q <query/ies> -r <reference genome>" << std::endl;
				success = false;
				i = argc;
			}
		}
		else{
			std::cout << "make sure to set flags" << std::endl << "proper usage : . / identify.exe - q <query / ies> -r <reference genome>" << std::endl;
			success = false;
			i = argc;
			//std::cout << "incorrect size for parameter: " << s << std::endl;
		}
		
	}
	if (success) {
		std::cout << "query: " << std::string(queries) << " ref: " << std::string(refGenome) << std::endl;
	}
	


	delete [] queries;
	delete [] refGenome;
}