#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include "annotation.h"
/*
Created by Jordan Burkhardt June 1st 2022
Copyright 2022

The following file contains the function implementations for the GBTags, annotation, and annotations classes.
*/
std::string hex_color()
{
	char hex_characters[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	std::string s = "#";
	for (int i = 0; i < 6; i++)
	{
		s += hex_characters[rand() % 16];
	}
	return s;
}

std::string anti_hex_color(std::string color)
{
	char hex_characters[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	std::string s = "#";
	for (int i = 0; i < color.length(); i++)
	{
		int index = 0;
		for (int j = 0; j < 16; j++)
		{
			if (color[i] == hex_characters[j])
			{
				index = (j + 8) % 16;
			}
		}
		s += hex_characters[index];
	}
	return s;
}

void plotGenes(std::list<annotation> genes, std::string outf, int genomeLength, int width = 1920, int height = 1080)
{
	double scale = double(width) / double(genomeLength);
	std::ofstream outfile;
	outfile.open(std::string(outf) + ".html");
	outfile << "<!DOCTYPE html>\n";
	outfile << "<html>\n";
	outfile << "<body>\n";
	outfile << "<canvas id = \"genome\" width = \"" << width << "\" height = \"" << height << "\">\n";
	outfile << "Your browser does not support the HTML5 canvas tag.< / canvas>\n";
	outfile << "<script>\n";

	outfile << "\tscale = " << scale << ";\n";
	outfile << "\tvar c = document.getElementById(\"genome\");\n";
	outfile << "\tvar ctx = c.getContext(\"2d\");\n";
	outfile << "\tctx.beginPath();\n";
	outfile << "\tctx.fillRect(0, 25, " << width << ", 5); \n";
	
	//std::string colors[] = { "#FF0000", "#fcfc03" , "#6bfc03", "#03fc84", "#03fc84","#4b9196","#187a1c","#869e87","#736de3","#e36de1","#e3d56d"};
	//int colLen = 11;
	srand(time(NULL));
	for (std::list<annotation>::iterator it = genes.begin(); it != genes.end(); it++)
	{
		std::string line;
		int sLoc = it->startIndex();
		int eLoc = it->endIndex();
		std::cout << "sLoc: " << sLoc << " scale: " << scale << " X: " << sLoc * scale << " rounded: " << int(sLoc * scale) << std::endl;
		
		int X = int(sLoc * scale);
		int Y = int((eLoc - sLoc) * scale);
		//int cC = rand() % colLen;
		//outfile << "\tctx.fillStyle = \"" << colors[cC] << "\";\n";
		outfile << "\tctx.fillStyle = \"" << hex_color() << "\";\n";
		outfile << "\tctx.fillRect(" << X << ", 0, " << Y << ", 50); \n";
		outfile << "\tctx.fillStyle = \"#000000\";\n";
		

		std::string geneName = it->getTag("gene");
		double chars = geneName.length();
		int fontSize = 20;
		bool disp = true;
		if (chars * 12.5 + 10 < Y) fontSize = 20;
		else if (chars * 10 + 10 < Y) fontSize = 16;
		else if (chars * 7.5 + 10 < Y) fontSize = 12;
		else disp = false;
		if (disp) {
			outfile << "\tctx.font = \"" << fontSize << "px Verdana\";\n";
			outfile << "\tctx.fillText(\"" << geneName << "\", " << (X + 5) << ", 30); \n";
		}
		/*
		collect relevent information and name it as a protein name, output on first fasta line
		*/
		/*
		output clipped sequence in second fasta line and then a newline character
		*/
	}
	outfile << "\tctx.stroke();\n";
	outfile << "</script>\n";
	outfile << "</body>\n";
	outfile << "</html>\n";
	outfile.close();
}