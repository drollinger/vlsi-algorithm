/*************************************************************
 * Name: Dallin Drollinger
 * A#: A01984170
 * Description:
 *
 *
 *************************************************************/
#include <map>
#include <math.h>
#include <vector>
#include "cost.h"

//Initialize hardcoded values that never change
#define EINIT "12V3V4V5V6V7V8V9VaVbVcVdVeVfVgViVjVkVlV"
#define CHIPRATIO 2.0
#define NMOVES 10
#define ISEED 3
#define P 0.99
#define EPSILON 0.001
#define TEMPINIT -1
#define LAMBDATF 0.005
enum dir{
    LEFT = 0,
    RIGHT = 1
};

using namespace std;
//Define helper functions
string getNewE(string E);
string m1(string E);
string m2(string E);
string m3(string E);
double getInitialTemp(string E, map<char, dimensions> blocks, int numOfUphills);

//Main Function
int main()
{
	//Read input from hard coded input_file
	map<char, dimensions> blocks = getBlocksMap("input_file.txt");

    //Initialize random seed
    srand (ISEED);

    //Initializing remaining hardcoded values that may vary 
    string E = EINIT;
    string bestE = E;
    double currentECost = cost(E, blocks);
    double bestECost = currentECost;
    double chipratio = CHIPRATIO;

    //Get temperature values
    double ratio = 0.85; 
    double temp = TEMPINIT;
    if (temp < 0)
        getInitialTemp(E, blocks, 1000);
    //The program will change ratio to 0.1 when temperature falls
    // below lambdatf * t0. After ratio is set to 0.1, the program in
    // general will terminate very quickly. 
    double tempThreashold = temp * LAMBDATF;

    int moves;
    int rejects;
    int uphill;
    do
    {
        if (temp < tempThreashold) ratio = 0.1;
        moves = rejects = uphill = 0;
        do
        {
            moves++;
            string newE = getNewE(E);
            double newEcost = cost(newE, blocks);
            double deltaCost = newEcost - currentECost;
            if (deltaCost < 0 || rand()/(float)RAND_MAX < exp((-1*deltaCost)/temp))
            {
                E = newE;
                currentECost = newEcost;
                if (deltaCost > 0) uphill++;
                if (currentECost < bestECost) 
                {
                    bestE = E;
                    bestECost = currentECost;
                }
            }
            else rejects++;
        } while (uphill <= NMOVES && moves <= 2*NMOVES);
        temp = ratio*temp;
    //Out_of_Time not used since time limit was not given in parameters
    } while (rejects/moves <= 0.95 && temp > EPSILON);
}

double getInitialTemp(string E, map<char, dimensions> blocks, int numOfUphills)
{
    double sumOfUphills = 0;
    double currentECost = cost(E, blocks);
    string newE;
    double newEcost;
    double deltaCost = 0;
    for (int i = 0; i < numOfUphills; i += deltaCost > 0 ? 1 : 0)
    {
        newE = getNewE(E);
        newEcost = cost(newE, blocks);
        deltaCost = newEcost - currentECost;
        E = newE;
        currentECost = newEcost;
        if (deltaCost > 0)
            sumOfUphills += deltaCost;
    }
    return -1*(sumOfUphills / numOfUphills) / log(P);
}

string getNewE(string E)
{
    //Random selection between 0 - 2
    switch (rand()%3)
    {
        //M1: two adjacent operands
        case 0:
            return m1(E);
            break;
        //M2: nonzero length chain
        case 1:
            return m2(E);
            break;
        //M3: adjacent operand and operator 
        case 2:
            return m3(E);
            break;
    }
    return E;
}

string m1 (string E)
{
    int sw1, sw2;
    int count = 0;
    int length = E.length();
    int operands = int(length/2);
    int s = rand()%operands;
    //False/0 is left, True/1 is right 
    int leftOrRight = (s == 0) || (rand()%2 && s < operands) ;
    for (int i = 0; i < length; i++)
    {
        if (E[i] != 'H' && E[i] != 'V')
        {
            if (count == s-(leftOrRight == LEFT)) sw1 = i;
            if (count == s+(leftOrRight == RIGHT)) 
            {
                sw2 = i;
                i = length;
            }
            count++;
        }
    }
    char temp = E[sw1];
    E[sw1] = E[sw2];
    E[sw2] = temp;
    return E;
}

string m2 (string E)
{
    struct cTrack { 
        int start; int size; 
        cTrack(int st, int si) {start = st; size = si;};
    };
    int length = E.length();
    int c = false;
    vector<cTrack> chains;
    for(int i = 0; i < length; i++)
    {
        if (E[i] == 'H' || E[i] == 'V')
        {
            if (c)
                chains.back().size++;
            else
                chains.insert(chains.end(), cTrack(i, 1));
            c = true;
        }
        else c = false;
    }
    int run = rand()%chains.size();
    int start = chains.at(run).start;
    for (int i = 0; i < chains.at(run).size; i++)
        E[start+i] = E[start+i] == 'H' ? 'V' : 'H';
    return E;
}

string m3 (string E)
{
    return E;
}
