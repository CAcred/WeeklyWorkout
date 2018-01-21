/************************************************************************-
 *	Util caa++ library
 *	This library contains methods to do several types of utility, general purpose
 *  tasks. 
 *
 * Complete caa++ util Library:
 *
 - 		vector<string> splitString(string input, string splitter)
 - 		string combineVecToStr(vector<string> input, string separator)
 - 		string toLowerCase(string inRaw)
 - 		string getSubstringTill(string target, int strt, char endChar)
 - 		string removeFromStr(string input, char target)
 - 		string removeCharFromVec(vector<string> input, char target)
 -		bool vecContains(vector<string>input, string target)
 -		bool strIsNum(string target)
 -		string intToStr(int input)
 -		string getCurDate(int selection)
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
#include <sstream>
#include <ctime>

using namespace std;




/** Updated C. A. Acred on 5/22/2015
 * This method takes a string and splits it into a vector with the second input.
 */
vector<string> splitString(string input, string splitter)
{
	vector<string> output; // initialize output vector
	int currentElement = 0; // set current vector element to 0
	string currentChar; // declare a container for the current char
	if (input[0] != ' ')
		output.push_back(""); // Push an empty string onto output
	for(int i = 0; i < input.length(); i++) // for each char in input
	{
		currentChar = input.substr(i, 1); // currentChar = the char in input @ i
		if (currentChar.compare(splitter) == 0) // if this char and the splitter are the same
		{
			while (currentChar.compare(splitter) == 0) // while they are the same
			{
				i++; // advance i by 1
				currentChar = input.substr(i, 1); // get the next char
			}
			if (i == input.length())
				return output;
			output.push_back(currentChar); // put the current char as the first in a new element
			if (output.size() > 1)
				currentElement++; // point to the new element
		}
		else
		{
			output[currentElement] = output[currentElement] + currentChar; // output @ currentElement += currentChar
		}
	}
	return output;
}

/** Updated C. A. Acred on 6/30/2015
 * This method takes input and returns it as one string.
 * Each element is separated by what ever is in separator.
 */
string combineVecToStr(vector<string> input, string separator)
{
	string result;
	for (int i = 0;i < input.size(); i++)
	{
		if (i == 0)
		{
			result = input[i];
		}
		else
		{
			result = result + separator + input[i];
		}
	}
	result = result.substr(0, -1);
	return result;
}

/** Updated C. A. Acred on 06/18/2015
 * This method returns a lower case version of inRaw.
 *
 */
string toLowerCase(string inRaw)
{
	string result = "";
	for (int i = 0; i < inRaw.length(); i++)
	{
		result += tolower(inRaw[i]);
	}
	return result;
}

/** Updated C. A. Acred on 6/20/15
 * Returns the substring of target, starting at position strt, and ending once endChar is found.
 * Does not include endChar in returned string.
 */
string getSubstringTill(string target, int strt, char endChar)
{
	char curChar = ' ';
	string result;
	for (int i = strt; i < target.length(); i++)
	{
		curChar = target[i];
		if (curChar == endChar)
			return result;
		result += curChar;
	}
	return result;
}

/** Updated C. A. Acred on 06/18/2015
 * This method removes target from char and returns it in result.
 * 	Note: Original string not modified. 
 *
 */
string removeCharFromStr(string input, char target)
{
	string result;
	for (int i = 0; i < input.length(); i++)
	{
		if (input[i] != target)
		{
			result += input[i];
		}
	}
	return result;
}

/** Updated C. A. Acred on 07/2/2015
 * This method removes white space from each element in input. Returns this result.
 *
 */
vector<string> removeCharFromVec(vector<string> input, char target) 
{
	vector<string> result (input.size());
	for (int i = 0; i < input.size(); i++)
	{
		result[i] = removeCharFromStr(input[i], target);
	}
	return result;
}

/** Updated C. A. Acred on 07/6/2015
 * This method returns true if input contains target.
 *
 */
bool vecContains(vector<string>input, string target)
{
	return (std::find(input.begin(), input.end(), target) != input.end());
}

/** Updated C. A. Acred on 07/6/2015
 * This method returns true if target is a number.
 *
 */
bool strIsNum(string target)
{
	for (int i = 0; i < target.length(); i++)
	{
		if (!(isdigit(target[i])))
			return false;
	}
	return true;
}

/** Updated C. A. Acred on 07/15/2015
 * This method returns the string of input.
 *
 */
 string intToStr(int input)
 {
	ostringstream temp;
    temp << input;
    return temp.str();
 }
 
/** Updated C. A. Acred on 07/17/2015
 * This method returns a tm structure with current date.
 *
 */
 tm* getCurDate()
 {
	time_t t = time(0); 
	struct tm* now = localtime(&t);
	return now;
 }