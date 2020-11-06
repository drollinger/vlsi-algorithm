/*************************************************************
 * Name: Dallin Drollinger
 * A#: A01984170
 * Description: cost.cpp reads block sizes in a given input file
 *     and using a given NPE it calculates the cost.
 *************************************************************/
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <math.h>
#include "cost.h"

using namespace std;

//GETBLOCKSMAP: given an input file, it returns the 
//              map of char to dimensions values
map<char, dimensions> getBlocksMap(string inputFile)
{
	//Read input from hard coded input_file
	ifstream inFile(inputFile);
    if (inFile.fail())
    {
        cout << "Error openning file '" << inputFile << "'\n";
        cout << "Please ensure file is in correct directory\n";
        exit(1);
    }
	char name;
	double area, ratio;
	map<char, dimensions> blocks;
	while (inFile >> name >> area >> ratio)
	{
		blocks[name].height = sqrt(area * ratio);
		blocks[name].width = sqrt(area / ratio);
	}
    return blocks;
}

//overload comparison for dimensions for min_element use
bool operator <(const dimensions& a, const dimensions& b) {
	return a.height * a.width < b.height * b.width;
}
//COST: takes NPE as input with module cost map and uses
//      annealing algorithm to find minimum area. Prints
//      results and returns minimum area. 
double cost(string npe, map<char, dimensions> modules)
{
	stack<vector<dimensions>> stack;
	vector<dimensions> dimOps;
	//Quick functions for calculating widths and heights
	auto add = [](double a, double b) -> double { return a + b; };
	auto dmax = [](double a, double b) -> double { return max(a, b); };
	//iterate over each char in string
	for (char &c : npe)
		switch (c)
		{
			//case vertical split, width is added, height is max 
			case 'V':
				pushOptions(stack, add, dmax);
				break;
			//case horizontal split, width is max, height is added
			case 'H':
				pushOptions(stack, dmax, add);
				break;
			//default push new values
			default:
				dimOps = vector<dimensions>(1, modules[c]);
				//prevent duplicate push
				if (!modules[c].is_square())
					dimOps.insert(dimOps.end(), modules[c].flip());
				stack.push(dimOps);
				break;
		}

	//sort through remaining list of possibilities for minimum option
	auto minDim = min_element(stack.top().begin(), stack.top().end());

	//calcuate minimum area to print and return
	double minArea = minDim->width * minDim->height;
	return minArea;
}

//PUSHOPTIONS: takes two dimension lists off the stack and using
//             dimension calculating functions calculates all optimal 
//             dimensions and pushes them as a list on the stack
void pushOptions(stack<vector<dimensions>> &stack,
	double widthFunc(double, double),
	double heightFunc(double, double))
{
	//declare lists of dimensions for iterations
	vector<dimensions> dimOptions = vector<dimensions>();
	vector<dimensions> firstOptions = stack.top(); stack.pop();
	vector<dimensions> secondOptions = stack.top(); stack.pop();
	//insert all possibilities and push final result
	for (dimensions optionOne : firstOptions)
		for (dimensions optionTwo : secondOptions)
			//function only inserts optimal possibilities
			insertDimensions(
				widthFunc(optionOne.width, optionTwo.width),
				heightFunc(optionOne.height, optionTwo.height),
				dimOptions
			);
	stack.push(dimOptions);
}

//INSERTDIMENSIONS: takes potential width and height and then adds
//                  them as a dimension option if width and height 
//                  cannot be fit into an existing option. 
void insertDimensions(double w, double h, vector<dimensions> &dimList)
{
	if (dimList.empty())
		dimList.insert(dimList.end(), dimensions(w, h));
	else
	{
		bool insertNew = true;
		for (dimensions &existing : dimList)
		{
			//overwrite existing option if new is better
			if (existing.width >= w && existing.height >= h)
			{
				existing.width = w;
				existing.height = h;
				insertNew = false;
			}
			//don't add new option if existing is better
			else if (existing.width <= w && existing.height <= h)
				insertNew = false;
		}
		if (insertNew)
			dimList.insert(dimList.end(), dimensions(w, h));
	}
}

