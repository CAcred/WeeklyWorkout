/************************************************************************-
 *	Input caa++ library
 *	This library contains methods to easily input information from files.
 *
 * Complete caa++ i Library:
 *
 - 		findString(string inputPath, string search)
 -		getLine(string path, int pos)
 *
 *	Started: 06/30/15
 *	©2015 C. A. Acred all rights reserved.
 ----------------------------------------------------------*
*/

#include <iostream>
#include <stdlib.h> 
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h> 
#include <stdio.h>
#include <algorithm>

using namespace std;


/** Updated C. A. Acred on 06/18/2015
 * This method returns the index of the first occurrence of search if it exists in input.
 * The index of the first char will be returned.
 *	-1 will be returned if search is not found.
 *
 */
int findString(string inputPath, string search)
{
	fstream input;
	input.open(inputPath, fstream::in);
	int index;
	
	
	string line;
	int curIndex = 0;
	int curSearchInt = 0;
	while (getline(input, line))
	{
		string temp;
		for (int i = 0; i < line.length(); i++)
		{
			if (line.at(i) == search.at(curSearchInt))
			{
				temp += line.at(i);
				if (temp.length() == search.length())
				{
					if (temp.compare(search) == 0)
					{
						input.close();
						return curIndex - (temp.length() - 1);
					}
					temp = "";
				}
				curSearchInt++;
			}
			else
			{
				curSearchInt = 0;
				temp = "";
			}
			curIndex++;
		}
	}
	input.close();
	return -1;
}

/** Updated C. A. Acred on 6/29/15
 * Returns line pos in file.
 * File lines start at 1.
 *
 * 
 * Returns empty string if there was an error.
 */
string getLine(string path, int pos)
{
	fstream input;
	if (pos == 0)
	{
		return "";
	}
	input.open(path, fstream::in);
	
	int i = 1;
	string line;
	while (i <= pos)
	{
		getline(input, line);
		i++;
	}
	input.close();
	return line;	
}

