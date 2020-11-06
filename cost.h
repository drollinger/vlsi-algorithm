/*************************************************************
 * Name: Dallin Drollinger
 * A#: A01984170
 * Description: cost.h lists the functions used in cost.cpp
 *              and also include dimensions definition
 *************************************************************/
#ifndef COST_H_
#define COST_H_

#include <vector>
#include <stack>
#include <map>

//Dimensions of blocks are stored using objects of this struct
struct dimensions 
{
	dimensions(){};
	dimensions(double w, double h)
	{
		width = w;
		height = h;
	}
	dimensions flip() { return dimensions(height, width); };
	bool is_square() { return width == height; };
	double width;
	double height;
};

//Helper Functions
double cost(std::string npe, std::map<char, dimensions> modules);
void insertDimensions(double w, double h, std::vector<dimensions> &dim);
std::map<char, dimensions> getBlocksMap(std::string inputFile);
void pushOptions(std::stack<std::vector<dimensions>> &stack,
	double widthFunc(double, double),
	double heightFunc(double, double));

#endif
