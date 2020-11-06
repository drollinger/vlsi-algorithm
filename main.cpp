/*************************************************************
 * Name: Dallin Drollinger
 * A#: A01984170
 * Description:
 *
 *
 *************************************************************/
#include <map>
#include "cost.h"

using namespace std;

//Hard Coded NPE's
#define topology_3a "12V3V4V5V6V7V8V9VaVbVcVdVeVfVgViVjVkVlV"
#define topology_3b "12H3H4H5H6H7H8H9HaHbHcHdHeHfHgHiHjHkHlH"
#define topology_3c "213546H7VHVa8V9HcVHgHibdHkVHfeHVlHVjHVH"

//Main Function
int main()
{
	//Read input from hard coded input_file
	map<char, dimensions> blocks = getBlocksMap("input_file.txt");



}
