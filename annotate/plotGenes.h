#ifndef plot
#define plot
#include <iostream>
#include <string>
#include <fstream>
#include "annotation.h"
void plotGenes(std::list<annotation> genes, std::string outf, int genomeLength, int width = 1920, int height = 1080);
#endif
