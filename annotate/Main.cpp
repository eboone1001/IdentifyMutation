#include <iostream>

#include <string>
#include <fstream>
#include "annotation.h"
#include "plotGenes.h"
/*
Created by Jordan Burkhardt May 24th 2022
Copyright 2022

this tool has the following usage:
./annotate.exe -a <annotations> -r <reference genome> -o <output file name>

optional flag -b will change the output for better compatibility with blast when working with proteins from the same genome

optional flag -p will produce an html file that plots the proteins on the genome

It inputs gene annotations in .gff file format and a reference genome (of one read) in .fasta file format, and outputs reads for each gene in fasta file format.
*/

int main(int argc, char* argv[]) 
{
	/*
	proper usage: ./annotate.exe -a <annotations> -r <reference genome> -o <output file name>
	*/
	char* annotationfile = new char[0];
	char* refGenome = new char[0];
	char* outPut = new char[0];
	bool a = false;
	bool r = false;
	bool o = false;
	bool b = false;
	bool p = false;
	bool success = true;
	bool skipArgs = false;
	if (argc == 1) {
		annotationfile = new char[6];

		char query[] = { 'a','.','g','f','f','\0'};
		for (int j = 0; j < sizeof(query) / sizeof(char); j++) {
			annotationfile[j] = query[j];
		}

		refGenome = new char[7];
		char ref[] = { 'r','e','f','.','f','a','\0'};
		for (int j = 0; j < sizeof(ref) / sizeof(char); j++)
		{
			refGenome[j] = ref[j];
		}

		outPut = new char[7];
		char out[] = { 'o','u','t','.','f','a','\0' };
		for (int j = 0; j < sizeof(out) / sizeof(char); j++)
		{
			outPut[j] = out[j];
		}
		skipArgs = true;
	}
	for (int i = 1; i < argc; i++)
	{
		int s = 0;
		while (argv[i][s++] != '\0') { }
		if (a) {
			
			annotationfile = new char[s];
			for (int j = 0; j < s; j++)
			{
				annotationfile[j] = argv[i][j];
			}
			a = false;
		}
		else if(r)
		{
			refGenome = new char[s];
			for (int j = 0; j < s; j++)
			{
				refGenome[j] = argv[i][j];
			}
			r = false;
		}
		else if (o)
		{
			outPut = new char[s];
			for (int j = 0; j < s; j++)
			{
				outPut[j] = argv[i][j];
			}
			o = false;
		}
		else if (s >= 3)
		{
			if (argv[i][0] == '-' && (argv[i][1] == 'a' || argv[i][1] == 'A'))
			{
				a = true;
			}
			else if (argv[i][0] == '-' && (argv[i][1] == 'r' || argv[i][1] == 'R'))
			{
				r = true;
			}
			else if (argv[i][0] == '-' && (argv[i][1] == 'o' || argv[i][1] == 'O'))
			{
				o = true;
			}
			else if (argv[i][0] == '-' && (argv[i][1] == 'b' || argv[i][1] == 'B'))
			{
				b = true;
			}
			else if (argv[i][0] == '-' && (argv[i][1] == 'p' || argv[i][1] == 'P'))
			{
				p = true;
			}
			else {
				std::cout << "proper usage: ./annotate.exe -a <annotations> -r <reference genome> -o <output file name>" << '\n' << " you used unknown arguement " << std::string(argv[i]) << std::endl;
				success = false;
				i = argc;
			}
		}
		else{
			std::cout << "make sure to set flags" << std::endl << "./annotate.exe -a <annotations> -r <reference genome> -o <output file name>" << std::endl;
			success = false;
			i = argc;
			//std::cout << "incorrect size for parameter: " << s << std::endl;
		}
		
	}
	if (success || skipArgs) {
		//std::cout << "query: " << std::string(annotationfile) << " ref: " << std::string(refGenome) << std::endl;

		annotations genes(annotationfile);
		std::list<annotation> CDS = genes.getAnnotations(GBKey::CDS);
		/*
		the remainder of this code will create a character array of the reference genome, then output a fasta sequence
		for each CDS in the cds list based on its indeces, with some extra information taken from the annotation
		objects
		*/
		std::string line;
		std::string filename = std::string(refGenome);
		std::ifstream ref(filename);

		std::list<std::string> lines;
		int totallen = 0;
		if (ref.is_open())
		{
			while (std::getline(ref, line))
			{
				if (line[0] == '>')
				{

				}
				else {

					int l = line.length();
					totallen += l;
					lines.push_back(line);
				}



			}
			ref.close();

			int str = 0;
			char* DNAseq = new char[totallen + 1];
			for (std::list<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
			{
				std::string line = *it;
				int l = line.length();

				for (int i = 0; i < l; i++)
				{
					DNAseq[str + i] = line[i];
				}

				str += l;
			}
			DNAseq[totallen] = '\0';

			/*
			Now the DNA is stored in an array of characters, and all proteins are stored in a list of annotations.
			Next the program will iterate through the list of annotations, obtain their starting and ending index, and
			then produce fasta reads cut from those indeces in the reference genome.
			*/
			std::ofstream outfile;
			outfile.open(std::string(outPut));
			int h = 0;
			for (std::list<annotation>::iterator it = CDS.begin(); it != CDS.end(); it++)
			{
				std::string line1;
				if(!b)line1 = ">" + std::string(it->getGenome()) + " " + it->getTag("product") + '\n';
				else {
					std::string pname = it->getTag("product");
					int plen = pname.length();
					for (int i = 0; i < plen; i++)
					{
						if (pname[i] == ' ') pname[i] = '_';
					}
					line1 = ">" + std::string(it->getGenome()) + "|" + pname + '\n';
				}
				outfile << line1;
				std::string line2 = "";
				for (int i = it->startIndex() - 1; i < it->endIndex(); i++)
				{
					line2 += DNAseq[i];
				}
				line2 += '\n';
				outfile << line2;
				/*
				collect relevent information and name it as a protein name, output on first fasta line
				*/
				/*
				output clipped sequence in second fasta line and then a newline character
				*/
			}
			outfile.close();

			plotGenes(CDS, "genome", totallen, 1920, 1080);
			delete[] DNAseq;
		}
		else std::cout << "Unable to open file: " << std::string(refGenome) << std::endl;
	}

	delete [] annotationfile;
	delete [] refGenome;
	delete[] outPut;
}