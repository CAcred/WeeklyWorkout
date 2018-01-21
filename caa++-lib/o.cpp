/************************************************************************-
 *	Output caa++ library
 *	This library contains methods to easily output information into files.
 *
 * Complete caa++ o Library:
 *
 - 	insertAt(string file, vector<string> input, vector<int> pos)
 -	removeString(string file, string target)	
 *
 *	Started: 06/21/15
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

/** Updated C. A. Acred on 6/29/15
 * Inserts the items of inputs into file at lines pos.
 * The order of inputs directly correlates to the line numbers in pos.
 *	For example, input[0] will be places at line pos[0].
 *
 * Line numbers start at 1, not 0. Infinite loop will be cause by a 0 input.
 * New line is included with every input automatically.
 * 
 * Returns -1 if there was an error.
 */
 int insertAt(string file, vector<string> input, vector<int> pos)
 {
	 if (pos.size() == 0)
		 return -1;
	 if (input.size() == 0)
		 return -1;
	 if (input.size() != pos.size())
		 return -1;
	fstream temp;
	fstream fileS;
	
	temp.open("tempFileInsert.txt", fstream::out | fstream::trunc);
	fileS.open(file, fstream::in);
	
	// Create temp:
	int fileIndex = 1; // The current in in the new file, not the old
	string line;
	vector<string> ovrlplog;
	
	for (int i = 0; i < pos.size(); i++)
	{
		while (pos[i] != fileIndex) // While we aren't on the first insert:
		{
			if (ovrlplog.size() == 0) // If there's not an overlap:
			{
				getline(fileS, line); // Get the next line
				fileIndex++; // Increment our line index
				temp << line + "\n"; // insert the line into temp
			}
			else // Otherwise:
			{
				temp << ovrlplog[ovrlplog.size() - 1] + "\n"; // Insert the first overlap
				ovrlplog.pop_back(); // Remove this overlap
				fileIndex++; // Increment to the next line
			}
		}
		getline(fileS, line);
		if (line.compare("") != 0) // If there's something here
		{
			ovrlplog.insert(ovrlplog.begin(), line); // Save it in overlap
		}
		temp << input[i] + "\n";
		fileIndex++;
	}
	
	while (ovrlplog.size() > 0)
	{
		temp << ovrlplog[ovrlplog.size() - 1] + "\n"; // Insert the first overlap
		ovrlplog.pop_back(); // Remove this overlap
	}
	
	while (getline(fileS, line))
	{
		temp << line + "\n"; 
	}
	
	temp.close();
	fileS.close();
	
	temp.open("tempFileInsert.txt", fstream::in);
	fileS.open(file, fstream::out | fstream::trunc);
	
	// Write temp to file:
	while (getline(temp, line))
	{
		fileS << line + "\n";
	}
	temp.close();
	
	// Delete temp:
	if (remove("tempFileInsert.txt") != 0)
	{
		cout << "Debug:: Error Deleting insert temp";
	}
	
	// Close file:
	fileS.close();
	return 0;
 }
 
 /** Updated C. A. Acred on 6/29/15
 * Removes line pos in file.
 * File lines start at 1. A 0 pos will result in an infinite loop.
 *
 * 
 * Returns -1 if there was an error.
 */
 int removeLine(string file, int pos)
 {
	 if (pos < 1)
		 return -1;
	 
	 fstream fileS;
	 fstream temp;
	 
	 fileS.open(file, fstream::in);
	 temp.open("tempFileDelete.txt", fstream::out);
	 
	 string line;
	 int fileIndex = 1;
	 while (getline(fileS, line))
	 {
		if (fileIndex != pos)
		temp << line + "\n";
		fileIndex++;
	 }
	 
	 temp.close();
	 fileS.close();
	 
	 fileS.open(file, fstream::out | fstream::trunc);
	 temp.open("tempFileDelete.txt", fstream::in);
	 
	 while (getline(temp, line))
	 {
		fileS << line + "\n";
	 }

	temp.close();
	
	// Delete temp:
	if (remove("tempFileDelete.txt") != 0)
	{
		cout << "Debug:: Error Deleting delete temp";
	}
	
	fileS.close();
	return 0;
 }
 
 
 
 
 
 
 