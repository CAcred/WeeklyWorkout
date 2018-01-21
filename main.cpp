/************************************************************************-
 *	Weekly Workout: A program to keep up with your personal weekly work out schedule.
 *	Started: 05/21/15
 *  Finished: 07/24/15
 *  Updates:
 *		- Specify (2.0); Started 10/9/15, Finished 10/13/15; Features Single Exercise Graphing, Optional weight parameter, Can now record one exercise at a time.
 *	©2015 C. A. Acred all rights reserved.
 ----------------------------------------------------------*
*/
// Compile: g++ main.cpp -o weeklyworkout -std=c++0x -static-libgcc -static-libstdc++
// Debug:   g++ -g main.cpp -o weeklyworkout -std=c++0x -static-libgcc -static-libstdc++

// Note: All new lines should be at the end of an element, not beginning. 
/* API: Profiles are saved in rec/profiles.txt, logs in rec/log.txt and are stored like this:
 *	
 * In profiles.txt, a profile is started by '<profile>:'. The name and then "|(date)" is added. The date is that most recent recorded exercise and is used to know whether to increment or decrement exercise reps or not.
 * Each new line is another exercise. The format for an exercise:
 * exerNamHead+<name>+exerSeparator+exerRepHead+<rep/min>+exerSeparator+exerDowHead+[days_of_week]+exerSeparator+exerIncHead+(incrementation_per_instance)+exerSeparator+exerDisHead+(dis)+exerSeparator
 * Optional exerWeightHead+[num in pounds]
 * I.E.: name:Push_up:rep/min:50r:dow:mon/fri:inc:4:dis:0
 * The rep/min parameter had either an 'r' or an 'm' at the end, to signify either being measured in minutes or in repetitions.
 *
 * In log.txt, an account profile is started by '<profile>:'
 * Each account log starts with '<log>:dd/mm/yyyy'
 * Each new line has an exercise and it's information: (name):#(repetition or minute)
 *	
 */

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h> 
#include <stdio.h>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include "caa++-lib/o.cpp"
#include "caa++-lib/i.cpp"
#include "caa++-lib/util.cpp"

using namespace std;

// Global Vars
string headerSpace = "\t\t";
string instructionSpace = "\t";
string prmpt;
vector<string> daysOFWeek {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
string currentAccount = "no account";
string currentDOW;
string dash = "----------";

vector<string> commandHelpGet {"Get Commands::\n", 
							   "\tget (a date here):: Prints out the record for that date, if any.\n\t\t\t\t    You can also type 'today' instead of a date.\n\n", 
							   "\tget required:: Prints out the exercises set to be done today.\n\n", "\tget routine:: Prints out all the exercises you have active in your routine.\n\n",
							   "\tget graph:: Prints out a graph of your current logged exercises in the console.\n\n",
							   "\tget file (path, including name, for the new file):: Exports a graph of your logs to a .csv(Exel-Sheet format) file.\n\t\tIf path is empty, the default is in your desktop with the name::\n\t\t" + currentAccount+ "Record(current date)\n\t\tMake sure you adjust the format settings to be separated with commas and nothing else.\n\n"};

vector<string> commandHelpRem {"Remove Commands::\n",
							   "\tremove (a date here):: Removes the date specified from your logs.\n\t\t\t\t\t       You can also type 'today'.\n\n", 
							   "\tremove logs:: Clears your logs.\n\n",
							   "\tremove exercise (name of the exercise):: Removes the exercise specified.\n\n",
							   "\tremove account (name of account):: Removes specified account.\n\n"};

vector<string> commandHelpRec {"Record Commands::\n\t",
							   "\trecord (a date here):: Records the specified date. You can also use 'today'. You can also specify an exercise after the date.\n\n"};

vector<string> commandHelpAdd {"Add Commands::\n",
							   "\tadd exercise (name of exercise) (rep/min) (daysOfWeek) (inc/dec per instance) (weight)\n\t\tname of exercise = The name you want for this exercise. No spaces!\n\t\trep/min = The # of repetitions or minutes you want this to start with. For reps, add 'r', and for min add 'm' to the end of the #.\n\t\tdaysOfWeek = The days of the week you want this to be on.\n\t\tinc/dec per instance = The number of reps or mins you want to be added or subtracted if you achieve or miss your goal. \n\t\tweight = pounds, if applicable; This is optional.\n\n",
							   "\tadd account (name of account):: Adds an account with the specified name\n\n"};

// Files:
fstream logFile;
fstream profiles;

char exerSeparator = ':';
string exerNamHead = "name:";
string exerRepHead = "rep/min:";
string exerDowHead = "dow:";
string exerIncHead = "inc:";
string exerDisHead = "dis:";
string exerWeiHead = "wei:";

string profHeader = "<profile>:";
string routineItemSpace = "\t";
string mostRecentDate = "";

string logHeader = "<log>:";
string logSpace = "\t";
string logItemSpace = "\t\t";

string logPath = "res/log.txt";
string profilePath = "res/profiles.txt";



// Method prototypes 
void processGetCom(vector<string> input, int offset);
void processRecordCom(vector<string> input, int offset);
void processRemoveCom(vector<string> input, int offset);
void processAddCom(vector<string> input, int offset);
void printError(string reason);
void printRoutine(string account);
void signIn();
void exitSafe();
void logOut();
void cls();
char* getDesktopPath(void);
string prompt();
string getExerItem(string line, int item);
string processDate(string dateStr);
int processInput(vector<string> input, vector<string> permissions);
int processInput(vector<string> input);
int help(string context);
int findProfile(string profile, string file);
int findExercise(string name, string account);
int findLog(string date, string account);
int removeLog(string date, string account);
int removeLogEntry(string name, string account);
int editLogEntry(string name, string newName, string account);
int clearLogs(string account);
int dateIsMoreRecent(string date1, string date2);
int removeAccount(string path, string name);
int getDOWFromDate(string month, string day, string year);
vector<string> getRoutine(string account);
vector<string> getLog(string date, string account);
vector<vector<string>> getLogs(string account);


int main()
{	
	cls();
	
	
	signIn();
	string inRaw = "";
	vector<string> input;
	while (true)
	{
		inRaw = prompt();
		
		input = splitString(inRaw, " ");
		
		if (processInput(input) == -1)
			printError("Invalid command");
	}
}

/** Updated C. A. Acred on 07/6/15
 * Signs the user in.
 */
void signIn()
{
	cout << "\n\n" + headerSpace + dash + " .::Welcome to WeeklyWorkout!::. " + dash + "\n\n\n";
	cout << instructionSpace + "Please enter your account name\n";
	cout << instructionSpace + "To add an account type:: add account (name)\n\n\n";
	vector<string> input;
	string inRaw;
	
	while (true)
	{
		inRaw = prompt();
		
		input = splitString(inRaw, " ");
		
		if(processInput(input, vector<string> {"add", "exit", "clear"}) != -1);
		else if (toLowerCase(input[0]).compare("help") == 0)
		{
			cout << instructionSpace + "Please enter your account name\n";
			cout << instructionSpace + "To add an account type:: add account (name)\n\n\n";
		}
		else if (input[0].compare("") != 0) // If input isn't empty
		{
			string name = combineVecToStr(input, " ");
			int line = findProfile(name, profilePath);
			if (line != -1)
			{
				currentAccount = name;
				cout << "\t\t" + dash + " Signed in as " + name + " " + dash + "\n\n\n";
				help("");
				mostRecentDate = splitString(getLine(profilePath, line), "|")[1];
				return;
			}
			printError("Account ::" + name + ":: not found");
			profiles.close();
		}
		else
		{
			printError("Empty character is not a valid account.");
		}
	}
	cout << "\n\n\n\n" + prmpt;
}

// Command processing methods>>>>>



/** Updated C. A. Acred in 07/6/15
 * This method takes in a vector of the user input and calls the appropriate method 
 * Of the given command. 
 * input is the input commands you wish to process in a string vector.
 * permissions is a string array that specifies which command-heads to process. 
 * pass a null pointer to process all.
 * 
 * Returns -1 if not a valid command.
 */
int processInput(vector<string> input, vector<string> permissions)
{
	if ((toLowerCase(input[0]).compare("get") == 0) && (vecContains(permissions, "get")))
	{
		processGetCom(input, 1);
	}
	else if ((toLowerCase(input[0]).compare("record") == 0) && (vecContains(permissions, "record")))
	{
		processRecordCom(input, 1);
	}
	else if ((toLowerCase(input[0]).compare("remove") == 0) && (vecContains(permissions, "remove")))
	{
		processRemoveCom(input, 1);
	}
	else if ((toLowerCase(input[0]).compare("add") == 0) && (vecContains(permissions, "add")))
	{
		processAddCom(input, 1);
	}
	else if ((toLowerCase(input[0]).compare("exit") == 0) && (vecContains(permissions, "exit")))
	{
		exitSafe();
	}
	else if((toLowerCase(input[0]).compare("help") == 0) && (vecContains(permissions, "help")))
	{
		if (input.size() < 2){help("");}
		else {help(toLowerCase(input[1]));}
	}
	else if((toLowerCase(input[0]).compare("logout") == 0) && (vecContains(permissions, "logout")))
	{
		logOut();
	}
	else if((toLowerCase(input[0]).compare("clear") == 0) && (vecContains(permissions, "clear")))
	{
		cls();
	}
	else
	{
		return -1;
	}
	return 0;
}

/** Updated C. A. Acred on 06/18/15
 * This version process all.
 */
int processInput(vector<string> input)
{
	if ((toLowerCase(input[0]).compare("get") == 0))
	{
		processGetCom(input, 1);
	}
	else if (toLowerCase(input[0]).compare("record") == 0)
	{
		processRecordCom(input, 1);
	}
	else if (toLowerCase(input[0]).compare("remove") == 0)
	{
		processRemoveCom(input, 1);
	}
	else if (toLowerCase(input[0]).compare("add") == 0)
	{
		processAddCom(input, 1);
	}
	else if (toLowerCase(input[0]).compare("exit") == 0)
	{
		exitSafe();
	}
	else if(toLowerCase(input[0]).compare("help") == 0)
	{
		if (input.size() < 2){help("");}
		else {help(toLowerCase(input[1]));}
	}
	else if(toLowerCase(input[0]).compare("logout") == 0)
	{
		logOut();
	}
	else if(toLowerCase(input[0]).compare("clear") == 0)
	{
		cls();
	}
	else
	{
		return -1;
	}
	return 0;
}

/** Updated C. A. Acred on 10/11/15
 * Processes get commands.
 * Offset is the desired starting element.
 */
void processGetCom(vector<string> input, int offset)
{
	if (input.size() > 4)
	{
		printError("To many arguments; Enter a valid command");
		return;
	}
	
	if (input.size() < 2)
	{
		printError("To few arguments; Enter a valid command");
		return;
	}
	
	if (toLowerCase(input[offset]).compare("required") == 0)
	{
		vector<string> exercises = getRoutine(currentAccount);
		
		if (exercises.size() < 1)
		{
			printError("No exercises in your routine");
			return;
		}
		
		struct tm* now = getCurDate();
		string date = intToStr(now->tm_mon + 1) + "/" + intToStr(now->tm_mday) + "/" + intToStr(now->tm_year + 1900);
		if (findLog(date, currentAccount) != -1)
			cout << "\n\t" + dash + " A record for today already exists " + dash + "\n\n";
		else
			cout << "\n";
		for (int i = 0; i < exercises.size(); i++)
		{
			if (vecContains(splitString(getExerItem(exercises[i], 3), "/"), daysOFWeek[now->tm_wday]) && getExerItem(exercises[i], 5).compare("0") == 0)
			{
				string exerName = getExerItem(exercises[i], 1);
				string exerRep = getExerItem(exercises[i], 2);
				if (exerRep.substr(exerRep.length() - 1).compare("r") == 0)
					cout << "\t" + dash + " " + exerName + " " + dash + "\n\tRepeat " + exerRep.substr(0, exerRep.length() - 1) + " times;\n";
				else
					cout << "\t" + dash + " " + exerName + " " + dash + "\n\tDo for " + exerRep.substr(0, exerRep.length() - 1) + " minutes;\n";
			}
		}
		cout << "\n";
	}
	else if (toLowerCase(input[offset]).compare("routine") == 0)
	{
		printRoutine(currentAccount);
	}
	else if (toLowerCase(input[offset]).compare("graph") == 0)
	{
		string graph = "";
		vector<vector<string>> logs = getLogs(currentAccount);
		if (logs.size() < 1)
		{
			printError("No logs to graph");
			return;
		}
		
		vector<string> exercises;
		if (input.size() == 3)
		{
			if (findExercise(input[offset + 1], currentAccount) != -1)
				exercises.push_back(getLine(profilePath, findExercise(input[offset + 1], currentAccount))); // Get entry for the input exercise.
			else
			{
				printError("Exercise does not exist.");
				return;
			}
		}
		else
		{
			exercises = getRoutine(currentAccount);
		}
		int cellSize = 0;
		for (int i = 0; i < exercises.size(); i++) // Get cell size for graph:
		{
			if (getExerItem(exercises[i], 1).length() > cellSize)
				cellSize = getExerItem(exercises[i], 1).length();
			exercises[i] = getExerItem(exercises[i], 1);
		}
		if (cellSize < 6)
			cellSize = 6;
		
		
		
		// Order the dates - Newest on top, oldest on bottom
		vector<string> temp;
		for (int log = 0; log < logs.size(); log++)// For each log, see if any will be bigger than cellSize:
		{
			for (int column = 0; column < exercises.size(); column++)// For each column(exercise):
			{
				for (int item = 1; item < logs[log].size(); item++)  // For each log entry:
				{
					temp = splitString(logs[log][item], ":");
					temp = removeCharFromVec(temp, ' ');
					temp = removeCharFromVec(temp, '\t');
					string weight = "";
					if (temp.size() > 2)
						weight = temp[2] + "lbs";
					string type = temp[1].substr(temp[1].length() - 1);
					temp[1] = temp[1].substr(0, temp[1].length() - 1);
					if (temp[0].compare(exercises[column]) == 0)
					{
						if (weight == "")
						{
							if (cellSize < temp[1].length() + 3)
								cellSize = temp[1].length() + 3;
						}
						else
						{
							if (cellSize < temp[1].length() + 3 + weight.length())
								cellSize = temp[1].length() + 3 + weight.length();
						}
					}
				}
			}
		}
		
		// Organize dates:
		vector<vector<string>> orgLogs;
		vector<string> curRecentLog {"<log>:0/0/2000"};
		int recentLogNum = -1;
		while (logs.size() > 0) 
		{
			for (int i = 0; i < logs.size(); i++)
			{
				if (dateIsMoreRecent(logs[i][0].substr(logHeader.length()), curRecentLog[0].substr(logHeader.length())) == 0)
				{
					curRecentLog = logs[i];
					recentLogNum = i;
				}
			}
			orgLogs.push_back(curRecentLog);
			logs.erase(logs.begin() + recentLogNum);
			curRecentLog = {"<log>:0/0/2000"};
		}
		
		graph = "\t  |";
		int sizeBuf;
		for (int i = 0; i < exercises.size(); i++) // Add header header:
		{
			sizeBuf = cellSize - exercises[i].length();
			graph += exercises[i];
			for (int i = 0; i < sizeBuf; i++)
			{
				graph += " ";
			}
			graph += "|";
		}
		graph += "\n"; // Make line.
		graph += "\t  |";
		for (int j = 0; j < exercises.size(); j++)
		{
			for (int k = 0; k < cellSize; k++)
			{
				graph += "-";
			}
			graph += "|";
		}
		graph += "\n";

		bool empty;
		for (int log = 0; log < orgLogs.size(); log++)// For each log, add it to the graph:
		{
			string date = orgLogs[log][0].substr(logHeader.length());
			int spaceBuffer = 10 - date.length();
			graph += date;
			if (spaceBuffer != 0)
			{
				for (int i = 0; i < spaceBuffer; i++)
				{
					graph += " ";
				}
			}
			graph += "|"; 
			string type;
			for (int column = 0; column < exercises.size(); column++)// For each column(exercise):
			{
				empty = true;
				for (int item = 1; item < orgLogs[log].size(); item++)  // For each log entry:
				{
					temp = splitString(orgLogs[log][item], ":");
					temp = removeCharFromVec(temp, ' ');
					temp = removeCharFromVec(temp, '\t');
					type = temp[1].substr(temp[1].length() - 1);
					string weight = "";
					if (temp.size() > 2)
						weight = temp[2] + "lbs";
					temp[1] = temp[1].substr(0, temp[1].length() - 1);
					if (temp[0].compare(exercises[column]) == 0)
					{
						empty = false;
						graph += temp[1];
						if (type.compare("r") == 0)
						{
							graph += "rep";
						}
						else
						{
							graph += "min";
						}
						if (weight.compare("") != 0)
						{
							graph += weight;
							sizeBuf = cellSize - (temp[1].length() + 3 + weight.length());
						}
						else
							sizeBuf = cellSize - (temp[1].length() + 3);
						
						for (int i = 0; i < sizeBuf; i++) // Add any extra white-space:
						{
							graph += " ";
						}
						graph += "|";
						break;
					}
				}
				if (empty)
				{
					graph += "0";
					for (int i = 0; i < cellSize - 1; i++) // Add a blank column
					{
						graph += " ";
					}
					graph += "|";
				}
			}
			graph += "\n";
			graph += "\t  |";
			for (int j = 0; j < exercises.size(); j++)
			{
				for (int k = 0; k < cellSize; k++)
				{
					graph += "-";
				}
				graph += "|";
			}
			graph += "\n";
		}
		cout << graph;
	}
	else if (toLowerCase(input[offset]).compare("file") == 0) 
	{
		string graph = "";
		vector<vector<string>> logs = getLogs(currentAccount);
		if (logs.size() < 1)
		{
			printError("No logs to output");
			return;
		}
		
		vector<string> exercises = getRoutine(currentAccount);
		int cellSize = 0;
		for (int i = 0; i < exercises.size(); i++) // Get cell size for graph:
		{
			if (getExerItem(exercises[i], 1).length() > cellSize)
				cellSize = getExerItem(exercises[i], 1).length();
			exercises[i] = getExerItem(exercises[i], 1);
		}
		if (cellSize < 6)
			cellSize = 6;
		
		
		
		// Order the dates - Newest on top, oldest on bottom 
		vector<string> temp;
		/* - Not sure if needed
		for (int log = 0; log < logs.size(); log++)// For each log, see if any will be bigger than cellSize:
		{
			for (int column = 0; column < exercises.size(); column++)// For each column(exercise):
			{
				for (int item = 1; item < logs[log].size(); item++)  // For each log entry:
				{
					temp = splitString(logs[log][item], ":");
					temp = removeCharFromVec(temp, ' ');
					temp = removeCharFromVec(temp, '\t');
					string type = temp[1].substr(temp[1].length() - 1);
					temp[1] = temp[1].substr(0, temp[1].length() - 1);
					if (temp[0].compare(exercises[column]) == 0)
					{
						if (cellSize < temp[1].length() + 3)
							cellSize = temp[1].length() + 3;
					}
				}
			}
		}
		*/
		
		// Organize dates:
		vector<vector<string>> orgLogs;
		vector<string> curRecentLog {"<log>:0/0/2000"};
		int recentLogNum = -1;
		while (logs.size() > 0)
		{
			for (int i = 0; i < logs.size(); i++)
			{
				if (dateIsMoreRecent(logs[i][0].substr(logHeader.length()), curRecentLog[0].substr(logHeader.length())) == 0)
				{
					curRecentLog = logs[i];
					recentLogNum = i;
				}
			}
			orgLogs.push_back(curRecentLog);
			logs.erase(logs.begin() + recentLogNum);
			curRecentLog = {"<log>:0/0/2000"};
		}
		
		graph = ",";
		for (int i = 0; i < exercises.size(); i++) // Add header:
		{
			graph += exercises[i];
			graph += ",";
		}
		graph += "\n";

		bool empty;
		for (int log = 0; log < orgLogs.size(); log++)// For each log, add it to the graph:
		{
			string date = orgLogs[log][0].substr(logHeader.length());
			graph += date + ",";
			for (int column = 0; column < exercises.size(); column++)// For each column(exercise):
			{
				empty = true;
				for (int item = 1; item < orgLogs[log].size(); item++)  // For each log entry:
				{
					temp = splitString(orgLogs[log][item], ":");
					temp = removeCharFromVec(temp, ' ');
					temp = removeCharFromVec(temp, '\t');
					string type = temp[1].substr(temp[1].length() - 1);
					string weight = "";
					if (temp.size() > 2)
						weight = temp[2] + "lbs";
					temp[1] = temp[1].substr(0, temp[1].length() - 1);
					if (temp[0].compare(exercises[column]) == 0)
					{
						empty = false;
						graph += temp[1];
						if (type.compare("r") == 0)
							graph += "rep";
						else
							graph += "min";
						if (weight.compare("") != 0)
							graph += weight;
						graph += ",";
						break;
					}
				}
				if (empty)
				{
					graph += " ";
					graph += ",";
				}
			}
			graph += "\n";
		}
		fstream output;
		string file;
		struct tm* now = getCurDate();
		string dateStr = intToStr(now->tm_mon + 1) + "." + intToStr(now->tm_mday) + "." + intToStr(now->tm_year + 1900);
		if (input.size() < 3)
		{
			char* c = getDesktopPath();
			string deskPath(c);
			file = (deskPath + "\\" + currentAccount + "Record" + dateStr + ".csv");
		}
		else
		{
			file = input[offset + 1];
			if (file.substr(file.length() - 4).compare(".csv") != 0)
				file = file.substr(file.length() - 4) + ".csv";
		}
		output.open(file, fstream::out | fstream::trunc);
		output << graph;
		output.close();
		cout << headerSpace + dash + " File created:: " + file + " " + dash +"\n"; 
	}
	else
	{
		string date;
		if (toLowerCase(input[offset]).compare("today") == 0)
		{
			struct tm* now = getCurDate();
			date = intToStr(now->tm_mon + 1) + "/" + intToStr(now->tm_mday) + "/" + intToStr(now->tm_year + 1900);
		}
		else
			date = processDate(input[offset]);
		if (date.compare("") == 0)
			return;
		vector<string> log = getLog(date, currentAccount);
		if (log.size() < 1)
		{
			printError("Not a recorded date");
			return;
		}
		vector<string> elements;
		for (int i = 0; i < log.size(); i++)
		{
			elements = splitString(log[i], ":");
			elements = removeCharFromVec(elements, '\t');
			elements = removeCharFromVec(elements, ' ');

			cout << headerSpace + dash + " " + elements[0] + " " + dash +"\n";
			string repMin = elements[1].substr(elements[1].length() - 1);
			string repMinNum = elements[1].substr(0, elements[1].length() - 1);
			if (repMin.compare("r") == 0) 
			{
				cout << headerSpace + "Repeated " + repMinNum + " times\n";
			}
			else
			{
				cout << headerSpace + "Done for " + repMinNum + " minutes\n";
			}
			
		}
		return;
	}
}

/** Updated C. A. Acred on 10/13/15
 * Processes record commands.
 * Offset is the desired starting element.
 */
void processRecordCom(vector<string> input, int offset)
{
	if (input.size() < 2 || input.size() > 3)
	{
		printError("Incorrect use of record; Be sure to type:: record mm/dd/yy\nOr:: record mm/dd/yy (exercise name)");
		return;
	}
    struct tm* now = getCurDate();
	int curYear = now->tm_year + 1900;
	int curMonth = now->tm_mon + 1;
	int curDay = now->tm_mday;
	
	string log = logHeader; 
	string dateStr = "";
	
	if (toLowerCase(input[offset]).compare("today") == 0)
	{
		dateStr = intToStr(curMonth) + "/" + intToStr(curDay) + "/" + intToStr(curYear);
		log += dateStr + "\n";
	}
	else // User specified date:
	{
		dateStr = processDate(input[offset]);
		if (dateStr.compare("") == 0)
			return;
		log += dateStr + "\n";
	}
	
	bool specified = false;
	string specifiedExercise = "";
	if (input.size() == 3) // If the user wants a specific exercise:
	{
		specifiedExercise = input[offset + 1];
		if ((findExercise(specifiedExercise, currentAccount)) == -1)
		{
			printError("Be sure your specified exercise exists."); // The specifiedExercise does not exist.
			return;
		}
		specified = true;
	}
	
	string curLine;
	int profLine = findProfile(currentAccount, profilePath);
	
	profiles.open(profilePath, fstream::in);
	for (int i = 1; i <= profLine; i++)
	{
		getline(profiles, curLine); // GETLINE NOT WORKING AFTER THIS FOR A SPECIFIED RECORDING
	}
	
	vector<string> curLog = getLog(dateStr, currentAccount);
	vector<string> temp;
	for (int i = 0; i < curLog.size(); i++)
	{
		temp.push_back(splitString(curLog[i], ":")[0]); // Get name of an already existing entry, if any:
	}
	temp = removeCharFromVec(temp, ' ');
	temp = removeCharFromVec(temp, '\t');
	
	bool overWrite = false; // See if there is a current log needing to be removed:
	int logLine;
	if (findLog(dateStr, currentAccount) != -1)
	{
		logLine = findLog(dateStr, currentAccount);
		overWrite = true;
	}
	else
	{
		logLine = findProfile(currentAccount, logPath) + 1;
	}
	
	// Get day of the week:
	vector<string> dowTemp = splitString(dateStr, "/");
	int dow = getDOWFromDate(dowTemp[0], dowTemp[1], dowTemp[2]);
	string rOrM ;
	string inRaw;
	string recordPrmpt = "";
	string repMin = "";
	cout << "\n\n";
	int numExercises = 0;
	int d = dateIsMoreRecent(mostRecentDate, dateStr);
	bool updateRecent = false;
	string lbs = "";
	string recNum = "";
	bool exerciseFound = false;
	bool exerciseAlreadyRecorded = false;
	while (getline(profiles, curLine) && curLine.substr(0, profHeader.length()).compare(profHeader) != 0) // For every exercise in their routine:
	{
		exerciseFound = true;
		profLine++;
		if (specified)
		{
			profiles.close();
			profLine = findExercise(specifiedExercise, currentAccount);
			curLine = getLine(profilePath, profLine);
		}
		if (getExerItem(curLine, 5) == "0" && vecContains(splitString(getExerItem(curLine, 3), "/"), daysOFWeek[dow]))
		{
			rOrM = getExerItem(curLine, 2);
			string rEnd = rOrM.substr(rOrM.length() - 1, 1);
			rOrM = rOrM.substr(0, rOrM.length() - 1);
			inRaw = "";
			if (vecContains(temp, getExerItem(curLine, 1)))
			{
				for (int j = 0; j < curLog.size(); j++) // Remove the save of this entry for the previous log:
				{
					if (temp[j].compare(getExerItem(curLine, 1)) == 0)
					{curLog.erase(curLog.begin() + j); exerciseAlreadyRecorded = true; break;}
					
				}
				cout << "\t\t\t.:: Current Entry for " + getExerItem(curLine, 1) + " will be overwritten::.\n\n";
			}
			if (rEnd == "r")
			{
				
				cout << "\t\tHow many of " + getExerItem(curLine, 1) + " did you do::\n";
				recordPrmpt = "Repetitions(type::cancel to stop)::>";
				repMin = "r";
			}
			else
			{
				cout << "\t\tHow many minutes did you do " + getExerItem(curLine, 1) + "::\n";
				recordPrmpt = "Minutes(type::cancel to stop)::>";
				repMin = "m";
			}

			while (inRaw.compare("") == 0) // Get record:
			{
				inRaw = "";
				cout << recordPrmpt;
				getline(cin, inRaw);
				inRaw = removeCharFromStr(inRaw, ' ');
				if (toLowerCase(inRaw).compare("cancel") == 0)
				{
					cout << headerSpace + dash + " Cancelled " + dash + "\n";
					profiles.close();
					return;
				}
				if (!(strIsNum(inRaw)))
				{
					cout << "\tMake sure your input is a number\n";
					inRaw = "";
				}
			}
			recNum = inRaw;
			numExercises++;
			
			if (getExerItem(curLine, 6).compare("1") == 0)
			{
				inRaw = "";
				cout << "How many pounds did you use doing " + getExerItem(curLine, 1) + "::\n";
				recordPrmpt = "Pounds(type::cancel to stop)::>";
			
				while (inRaw.compare("") == 0) // Get weight
				{
					inRaw = "";
					cout << recordPrmpt;
					getline(cin, inRaw);
					inRaw = removeCharFromStr(inRaw, ' ');
					if (toLowerCase(inRaw).compare("cancel") == 0)
					{
						cout << headerSpace + dash + " Cancelled " + dash + "\n";
						profiles.close();
						return;
					}
					if (!(strIsNum(inRaw)))
					{
						cout << "\tMake sure your input is a number\n";
						inRaw = "";
					}
				}
				lbs = inRaw;
			}
			
			if (!(exerciseAlreadyRecorded) && (d == 1 || d == 2)) // Also compare to see if this entry is more recent than the current recent date. If so do this, otherwise don't.
			{
				if (d == 1)
					updateRecent = true;
				int temp = atoi(rOrM.c_str()) - atoi(getExerItem(curLine, 4).c_str());
				int tempPlus = atoi(rOrM.c_str()) + atoi(getExerItem(curLine, 4).c_str());
				if (temp < 1)
				{
					temp = 1;
				}
				if (atoi(recNum.c_str()) > atoi(rOrM.c_str()))
				{
					tempPlus = atoi(recNum.c_str()) + atoi(getExerItem(curLine, 4).c_str());
				}
				if (atoi(recNum.c_str()) >= atoi(rOrM.c_str()))
				{
					insertAt(profilePath, vector<string> {routineItemSpace + exerNamHead + getExerItem(curLine, 1) + exerSeparator + exerRepHead + intToStr(tempPlus) + rEnd + exerSeparator + exerDowHead + getExerItem(curLine, 3) + exerSeparator + exerIncHead + getExerItem(curLine, 4) + exerSeparator + exerDisHead + getExerItem(curLine, 5) + exerSeparator + exerWeiHead + getExerItem(curLine, 6) + exerSeparator}, vector<int> {profLine});
					removeLine(profilePath, profLine + 1);
				}
				else
				{
					insertAt(profilePath, vector<string> {routineItemSpace + exerNamHead + getExerItem(curLine, 1) + exerSeparator + exerRepHead + intToStr(temp) + rEnd + exerSeparator + exerDowHead + getExerItem(curLine, 3) + exerSeparator + exerIncHead + getExerItem(curLine, 4) + exerSeparator + exerDisHead + getExerItem(curLine, 5) + exerSeparator + exerWeiHead + getExerItem(curLine, 6) + exerSeparator}, vector<int> {profLine}); 
					removeLine(profilePath, profLine + 1);
				}
			}
			if (lbs.compare("") != 0)
			{
				log = log + logItemSpace + getExerItem(curLine, 1) + ": " + recNum + repMin + ": " + lbs + "\n";
			}
			else
				log = log + logItemSpace + getExerItem(curLine, 1) + ": " + recNum + repMin + "\n";
			lbs = "";
		}
		if (specified)
		{
			break;
		}
		exerciseAlreadyRecorded = false;
	}
	if (numExercises < 1 && exerciseFound == false)
	{
		printError("No exercises in your routine");
		return;
	}
	else if (numExercises < 1 && exerciseFound == true)
	{
		printError("No exercises in your routine done on this day.");
		return;
	}
	
	for (int i = 0; i < curLog.size(); i++) // Add in extra log entries:
	{
		log += curLog[i] + "\n";
	}
	
	if (overWrite)
	{
		removeLog(dateStr, currentAccount);
	}
	if (updateRecent) // If this record is more recent than the most recent date, the most recent date for this account needs to be updated.
	{ // This is because date recorded in the past shouldn't effect the incrementation/decrementation of the routine.
		int u = findProfile(currentAccount, profilePath);
		string p = getLine(profilePath, u);
		p = getSubstringTill(p, 0, '|');
		p += "|"+dateStr;
		removeLine(profilePath, u);
		insertAt(profilePath, vector<string> {p}, vector<int> {u});
		mostRecentDate = dateStr;
	}
	insertAt(logPath, vector<string> {logSpace + log}, vector<int> {logLine});
	cout << headerSpace + dash + " Recorded " + dash + "\n";
	cout << "\n\n";
	if (not specified)
		profiles.close();
}

/** Updated C. A. Acred on 7/9/15
 * Processes remove commands.
 * Offset is the desired starting element.
 */
void processRemoveCom(vector<string> input, int offset)
{
	if (input.size() > 3)
	{
		printError("To many arguments; Enter a valid command");
		return;
	}
	
	if (input.size() < 2)
	{
		printError("To few arguments; Enter a valid command");
		return;
	}
	if (toLowerCase(input[offset]).compare("account") == 0)
	{
		string name = combineVecToStr(vector<string>(input.begin() + 2, input.end()), " ");
		if (removeAccount(profilePath, name) == -1 || removeAccount(logPath, name) == -1)
		{
			printError("Account Not found");
			return;
		}
		cout << headerSpace + dash + " Account Deleted " + dash + "\n";
		if (currentAccount.compare(name) == 0)
		{
			cls();
			currentAccount = "no account";
			signIn();
		}
		return;	
	}
	else if (toLowerCase(input[offset]).compare("exercise") == 0) 
	{
		string name = input[offset + 1];
		int line = findExercise(name, currentAccount);
		if (line != -1)
		{
			cout << "Do you wish to remove " + name + " from your logs:: (y/n)\n";
			string inRaw;
			while (inRaw.compare("") == 0)
			{
				inRaw = prompt();
				inRaw = removeCharFromStr(inRaw, ' ');
				if (toLowerCase(inRaw).compare("n") == 0)
				{
					if ((getExerItem(getLine(profilePath, line), 5).compare("0") == 0)) // If it's not already disabled
					{
						string newExercise = "";
						string exercise = removeCharFromStr(getLine(profilePath, line), ' ');
						removeCharFromStr(exercise, '\t');
						vector<string> temp = splitString(exercise, ":");
						for (int k = 0; k < 9; k++)
						{
							newExercise += temp[k] + exerSeparator;
						}
						newExercise = newExercise + "1" + ":";
						removeLine(profilePath, line);
						insertAt(profilePath, vector<string> {newExercise}, vector<int> {line});
					}
				}
				else if (toLowerCase(inRaw).compare("y") == 0)
				{
					removeLogEntry(name, currentAccount);
					removeLine(profilePath, line);
				}
				else 
					inRaw = "";
			}
			cout << headerSpace + dash + " Exercise Deleted " + dash + "\n";
			return;
		}
		printError("Exercise not found; Type:: get routine for reference");
	}
	else if (toLowerCase(input[offset]).compare("logs") == 0) 
	{
		clearLogs(currentAccount);
	}
	else
	{
		string dateStr;
		if (toLowerCase(input[offset]).compare("today") == 0)
		{
			struct tm* now = getCurDate();
			dateStr = intToStr(now->tm_mon + 1) + "/" + intToStr(now->tm_mday) + "/" + intToStr(now->tm_year + 1900);
		}
		else
			dateStr = processDate(input[offset]);
		if (dateStr.compare("") == 0)
			return;
		
		if (removeLog(dateStr, currentAccount) == -1)
		{
			printError("Not a recorded date");
		}
		cout << headerSpace + dash + " Date cleared " + dash + "\n";
	}
}

/** Updated C. A. Acred on 10/9/15
 * Processes add commands.
 * Offset is the desired starting element.
 */
void processAddCom(vector<string> input, int offset)
{
	if (toLowerCase(input[offset]).compare("exercise") == 0) 
	{
		if (input.size() > 7 || input.size() < 6)
		{
			printError("Too many argument; did you use spaces instead of underscores\nin the exercise name or other elements?");
			return;
		}
		int line = findProfile(currentAccount, profilePath) + 1;
		
		// Check rep/min param:
		char rm = tolower(input[offset + 2].at(input[offset + 2].length() - 1));
		string rnum = input[offset + 2].substr(0, (input[offset + 2].length() - 1));
		if (((rm != 'r') && (rm != 'm')) || !(strIsNum(rnum)))
		{
			printError("Not a repetition or minute value. Please in sure you include\nan 'r' or 'm' at the end.");
			return;
		}
		
		// Check dow param:
		string dow  = input[offset + 3];
		vector<string> temp = splitString(dow, "/");
		dow = "";
		
		for (int i = 0; i < temp.size(); i++)
		{
			if (!(vecContains(daysOFWeek, temp[i])))
			{
				printError(temp[i] + " is not a day of the week. Refer to help");
				return;
			}
			dow += temp[i] + "/";
		}
		dow = dow.substr(0, dow.length() - 1);

		// Check inc:
		if (!(strIsNum(input[offset + 4])))
		{
			printError(input[offset + 4] + " is not a number");
			return;
		}
		
		bool weight = false;
		// Check weight if there:
		if (input.size() == 7)
		{
			if (!(strIsNum(input[offset + 5])))
			{
				printError("Must provide a number for the weight parameter.");
				return;
			}
			if (input[offset + 5].compare("1") == 0)
				weight = true;
			else if (input[offset + 5].compare("0") == 0)
				weight = false;
			else
			{
				printError("Must provide a 1 (weight) or a 0 (no weight).");
				return;
			}
		}
		
		vector<string> exercise;
		if (weight)
			exercise = {routineItemSpace + exerNamHead + input[offset + 1] + exerSeparator + exerRepHead + input[offset + 2] + exerSeparator + exerDowHead + dow + exerSeparator + exerIncHead + input[offset + 4] + exerSeparator + exerDisHead + "0" + exerSeparator + exerWeiHead + "1" + exerSeparator};
		else
			exercise = {routineItemSpace + exerNamHead + input[offset + 1] + exerSeparator + exerRepHead + input[offset + 2] + exerSeparator + exerDowHead + dow + exerSeparator + exerIncHead + input[offset + 4] + exerSeparator + exerDisHead + "0" + exerSeparator + exerWeiHead + "0" + exerSeparator};
		
		if (findExercise(input[offset + 1], currentAccount) != -1)
		{
			line = findExercise(input[offset + 1], currentAccount);
			removeLine(profilePath, line);
		}
		insertAt(profilePath, exercise, vector<int> {line});
		
		cout << headerSpace + dash + " Exercise Added " + dash + "\n";
	}
	else if (toLowerCase(input[offset]).compare("account") == 0)
	{
		if (input.size() > 3)
		{
			printError("To many arguments; Enter a valid command");
			return;
		}
		
		if (input.size() < 3)
		{
			printError("To few arguments; Enter a valid command");
			return;
		}
		// Get the name input
		string name = combineVecToStr(vector<string>(input.begin() + 2, input.end()), " ");
		
		// Make sure account is not already created
		string line;
		if (findProfile(name, profilePath) != -1)
		{
			printError("Account already created");
			return;
		}
		logFile.open(logPath, fstream::app | fstream::out);
		profiles.open(profilePath, fstream::app | fstream::out);
		struct tm* now = getCurDate();
		string dateStr = intToStr(now->tm_mon + 1) + "/" + intToStr(now->tm_mday) + "/" + intToStr(now->tm_year + 1900);
		profiles << "<profile>:" + name + "|" + dateStr + "\n";
		logFile << "<profile>:" + name + "\n";
		cout << headerSpace + dash + " Account Added " + dash + "\n";
		profiles.close();
		logFile.close();
		return;
	}
	else
	{
		printError("Invalid command");
	}
}


// Command processing methods>>>>>

/** Updated C. A. Acred on 7/17/15
 * Returns a vector with account's routine.
 */
vector<string> getRoutine(string account)
{
	string curLine;
	vector<string> result;
	int line = findProfile(account, profilePath);
	
	profiles.open(profilePath, fstream::in);
	for (int i = 1; i <= line; i++)
	{
		getline(profiles, curLine);
	}
	while (getline(profiles, curLine) && curLine.substr(0, profHeader.length()).compare(profHeader) != 0)
	{
		if (curLine.compare("") != 0)
			result.push_back(curLine);
	}
	profiles.close();
	
	result = removeCharFromVec(result, ' ');
	result = removeCharFromVec(result, '\t');
	return result;
}

/** Updated C. A. Acred on 5/28/15
 * Prints an error with a wrapper.
 */
void printError(string reason)
{
	cout << dash + "--\nError::\n" + reason + "\nType help for valid commands\n" + dash + "--\n\n";
}


/** Updated C. A. Acred 05/28/2015
 * Prints help about commands.
 * Insert and empty string ("") if you wish all help to be outputted.
 *
 * Returns -1 if there's an error.
 */
int help(string context)
{	
	cout << headerSpace + dash + " .::Commands::. " + dash + "\n\n";
	if (context.empty())
	{
		int i;
		for (i = 0; i < commandHelpGet.size(); i++)
		{
			cout << instructionSpace + commandHelpGet[i];
		}
		for (i = 0; i < commandHelpRec.size(); i++)
		{
			cout << instructionSpace + commandHelpRec[i];
		}
		for (i = 0; i < commandHelpRem.size(); i++)
		{
			cout << instructionSpace + commandHelpRem[i];
		}
		for (i = 0; i < commandHelpAdd.size(); i++)
		{
			cout << instructionSpace + commandHelpAdd[i];
		}
	}
	else if(context.compare("get") == 0)
	{
		int i;
		for (i = 0; i < commandHelpGet.size(); i++)
		{
			cout << instructionSpace + commandHelpGet[i];
		}
	}
	else if(context.compare("remove") == 0)
	{
		int i;
		for (i = 0; i < commandHelpRem.size(); i++)
		{
			cout << instructionSpace + commandHelpRem[i];
		}
	}
	else if(context.compare("record") == 0)
	{
		int i;
		for (i = 0; i < commandHelpRec.size(); i++)
		{
			cout << instructionSpace + commandHelpRec[i];
		}
	}
	else if(context.compare("add") == 0)
	{
		int i;
		for (i = 0; i < commandHelpAdd.size(); i++)
		{
			cout << instructionSpace + commandHelpAdd[i];
		}
	}
	else
	{
		return -1;
	}
	cout << instructionSpace + "To logout type:: logout\n";
	cout << instructionSpace + "To exit type:: exit\n";
	cout << instructionSpace + "To clear the screen type:: clear\n";
	cout << instructionSpace + "-+| date format = m/d/yr |+-\n\t\tExample:: 05/20/2015\n";
	cout << instructionSpace + "-+| daysOFWeek format = day/day/day..etc(up to 7) |+-\n\t\tExample:: mon/tue/wed/thu/fri/sat/sun\n";
	cout << instructionSpace + "-+| rep format = (#)r |+-\n\t\tExample:: 15r\n";
	cout << instructionSpace + "-+| min format = (#)m |+-\n\t\tExample:: 15m\n\n";
	cout << "\n" + instructionSpace + "-+| To edit an exercise in your routine, \n\t    use the add command with the same name |+-\n";
	cout << instructionSpace + "-+| Use underscores for spaces in exercise names, \n\t    or omit spaces completely |+-";
	cout << "\n\n\n\n";
	return 0;
}

/** Updated C. A. Acred on 06/16/2015
 * This method returns a string that the user inputs.
 *
 */
string prompt()
{
	prmpt = "WW(" + currentAccount + "):+|>";
	cout << prmpt; 
	string inRaw = "";
	getline(cin, inRaw);
	return inRaw;
}

/** Updated C. A. Acred on 06/16/2015
 * This method clears the screen.
 *
 */
void cls()
{
	system("CLS");
}


/** Updated C. A. Acred on 06/18/2015
 * This method does anything needed before exiting the program.
 *
 */
void exitSafe()
{
	cout << headerSpace + dash + " Now exiting " + dash + "\n";
	profiles.close();
	logFile.close();
	cls();
	exit(0);
}


/** Updated C. A. Acred on 06/18/2015
 * This method does anything needed for logging out.
 *
 */
void logOut()
{
	currentAccount = "no account";
	cout << headerSpace + dash + " Logged out " + dash + "\n";
	cls();
	signIn();
}

/** Updated C. A. Acred on 06/28/2015
 * Returns the line number in the profiles file that profile is found.
 * -1 if not in file.
 *
 */
int findProfile(string profile, string file)
{
	fstream input;
	input.open(file, fstream::in);
	string curLine;
	int line = 1;
	while (getline(input, curLine)) // For every line in profiles:
	{
		if (getSubstringTill(curLine, profHeader.length(), '|').compare(profile) == 0)
		{
			input.close();
			return line;
		}
		line++;
	}
	input.close();
	return -1;
}

/** Updated C. A. Acred on 07/9/2015
 * Returns the line number in profiles that is the exercise name for account.
 * -1 if not in file.
 *
 */
int findExercise(string name, string account)
{
	string curLine;
	int line = findProfile(currentAccount, profilePath);
	
	profiles.open(profilePath, fstream::in);
	for (int i = 1; i <= line; i++)
	{
		getline(profiles, curLine);
	}
	while (getline(profiles, curLine) && curLine.substr(0, profHeader.length()).compare(profHeader) != 0) // For every line in profiles:
	{
		line++;
		if (getSubstringTill(curLine, exerNamHead.length() + 1, exerSeparator).compare(name) == 0)
		{
			profiles.close();
			return line;
		}
	}
	profiles.close();
	return -1;
}

/** Updated C. A. Acred on 10/12/2015
 * Returns the line number in logPath that is for date in currentAccount.
 * -1 if not in file.
 *
 */
int findLog(string date, string account) // Fix, shouldn't print an error, let caller decide that.
{
	string curLine;
	int line = findProfile(account, logPath);
	if (line == -1) {return -1;}
	
	logFile.open(logPath, fstream::in);
	for (int i = 1; i <= line; i++)
	{
		getline(logFile, curLine);
	}
	while (getline(logFile, curLine) && (curLine.substr(0, profHeader.length()).compare(profHeader) != 0))
	{
		line++;
		if (curLine.compare("\t" + logHeader + date) == 0)
		{
			logFile.close();
			return line;
		}
	}
	logFile.close();
	return -1;
}

/** Updated C. A. Acred on 10/9/2015
 * Couts every routine in account.
 * -1 if no routine is found.
 *
 */
void printRoutine(string account)
{
	vector<string> exercises = getRoutine(account);
	if (exercises.size() < 1)
	{
		printError("No exercises in your routine"); // Bug !!!
		return;
	}
	for (int i = 0; i < exercises.size(); i++)
	{
		if (getExerItem(exercises[i], 5).compare("0") == 0)
		{
			string exerName = getExerItem(exercises[i], 1);
			string exerRep = getExerItem(exercises[i], 2);
			string exerDow = getExerItem(exercises[i], 3);
			string exerInc = getExerItem(exercises[i], 4);
			string exerWei = getExerItem(exercises[i], 6);
			if (exerRep.substr(exerRep.length() - 1).compare("r") == 0)
			{
				if (exerWei.compare("0") == 0)
					cout << "\t" + dash + " " + exerName + " " + dash + "\n\tRepeated " + exerRep.substr(0, exerRep.length() - 1) + " times;\n\tDone on " + exerDow + ";\n\tIncremented " + exerInc + " times per execution;\n\tDecremented " + exerInc + " times per failure to reach repetitions;\n";
			    else
					cout << "\t" + dash + " " + exerName + " " + dash + "\n\tRepeated " + exerRep.substr(0, exerRep.length() - 1) + " times;\n\tDone on " + exerDow + ";\n\tIncremented " + exerInc + " times per execution;\n\tDecremented " + exerInc + " times per failure to reach repetitions;\n\tWeights are in use;\n";
			}
			else
			{
				if (exerWei.compare("0") == 0)
					cout << "\t" + dash + " " + exerName + " " + dash + "\n\tDone for " + exerRep.substr(0, exerRep.length() - 1) + " minutes;\n\tDone on " + exerDow + ";\n\tIncremented " + exerInc + " times per successful execution;\n\tDecremented " + exerInc + " times per failure to reach minutes;\n";
				else
					cout << "\t" + dash + " " + exerName + " " + dash + "\n\tDone for " + exerRep.substr(0, exerRep.length() - 1) + " minutes;\n\tDone on " + exerDow + ";\n\tIncremented " + exerInc + " times per successful execution;\n\tDecremented " + exerInc + " times per failure to reach minutes;\n\tWeights are in use;\n";
			}
		}
	}
}

/** Updated C. A. Acred on 07/10/2015
 * Returns a string of the exercise element in line specified by item.
 * line must contain exercise in proper format.
 * 1 = Name
 * 2 = Rep/Min
 * 3 = Dow
 * 4 = Inc
 * 5 = Dis
 * 6 = Wei
 *
 * Returns empty string if no such item or the string did not have the correct format.
 *
 */
string getExerItem(string line, int item)
{
	if (item > 6 || item < 1)
	{
		printError("No such item in an exercise");
		return "";
	}
	vector<string> details = splitString(line, ":");
	for (int i = 0; i < details.size(); i++)
	{
		if ((i+1)%2 == 0 && (i+1) / 2 == item)
		{
			return details[i];
		}
	}
	return "";
}

/** Updated C. A. Acred on 07/17/2015
 * Removes a log from logFile for account currentAccount with the date date.
 * Returns -1 if not in file.
 *
 */
int removeLog(string date, string account)
{
	int line = findLog(date, account);
	if (line == -1)
	{
		return -1;
	}
	logFile.open(logPath, fstream::in);
	string curLine;
	for (int i = 1; i <= line; i++)
	{
		getline(logFile, curLine);
	}
	removeLine(logPath, line);
	while (getline(logFile, curLine) && ((curLine.substr(0, logHeader.length() + 1).compare(logSpace + logHeader) != 0) && (curLine.substr(0, profHeader.length()).compare(profHeader) != 0)))
	{
		removeLine(logPath, line);
	}
	logFile.close();
	return 0;
}

/** Updated C. A. Acred on 07/17/2015
 * Formats a date string correctly. 
 * Prints error if not in proper date format (mm/dd/yy | mm/dd/yyyy)
 * Prints error if date has not occurred yet
 * 0 padding removed
 * Year adjusted to current century if need be (15 = 2015)
 *
 */
string processDate(string dateStr)
{
	struct tm* now = getCurDate();
	int curYear = now->tm_year + 1900;
	int curMonth = now->tm_mon + 1;
	int curDay = now->tm_mday;
	string curDate = intToStr(curMonth) + "/" + intToStr(curDay) + "/" + intToStr(curYear);
	vector<string> date = splitString(dateStr, "/");
	if (date.size() != 3)
	{
		printError("Must provide a valid date in number format;\nRefer to help");
		return "";
	}
	// Make sure date is a number:
	for (int i = 0; i < 3; i++)
	{
		if (!(strIsNum(date[i])))
		{
			printError("Must provide a valid date in number format;\nRefer to help");
			return "";
		}
	}
	
	// Test date format:
	if ((date[0].length() > 2) || (date[1].length() > 2) || (date[2].length() != 2 && date[2].length() != 4))
	{
		printError("Not a valid date format; Type:: record mm/dd/yy");
		return "";
	}
	if (date[2].length() == 2) {date[2] = intToStr(curYear).substr(0, 2) + date[2];} // Adjust year if needed
	if (date[0].length() == 2 && date[0].substr(0, 1).compare("0") == 0) {date[0] = date[0].substr(1);} // Get rid of 0 padding (style thing)
	if (date[1].length() == 2 && date[1].substr(0, 1).compare("0") == 0) {date[1] = date[1].substr(1);} // Get rid of 0 padding (style thing)
	string result = date[0] + "/" + date[1] + "/" + date[2];
	if (dateIsMoreRecent(curDate, result) == 1)
	{
		printError("Date has not happened yet");
		return "";
	}
	return result;
}

/** Updated C. A. Acred on 07/17/2015
 * Returns all of the log from date date.
 *	Note: Must be in proper date format!
 * Returns empty vector<string> if not in file.
 *
 */
vector<string> getLog(string date, string account)
{
	vector<string> result;
	string curLine;
	int line = findLog(date, account);
	if (line == -1) {return result;}
	
	logFile.open(logPath, fstream::in);
	for (int i = 1; i <= line; i++)
	{
		getline(logFile, curLine);
	}
	while (getline(logFile, curLine) && ((curLine.substr(0, logHeader.length() + 1).compare("\t" + logHeader) != 0) && (curLine.substr(0, profHeader.length()).compare(profHeader) != 0)))
	{
		if (curLine.compare("") != 0)
			result.push_back(curLine);
	}
	logFile.close();
	return result;
}

/** Updated C. A. Acred on 07/17/2015
 * Returns all of the logs for account.
 * Returns empty vector<vector<string>> if the account doesn't exist in file.
 *
 */
vector<vector<string>> getLogs(string account)
{
	vector<vector<string>> result;
	string curLine;
	int line = findProfile(account, logPath);
	if (line == -1) {printError("Log not found"); return result;}
	
	logFile.open(logPath, fstream::in);
	for (int i = 1; i <= line; i++)
	{
		getline(logFile, curLine);
	}
	getline(logFile, curLine);
	vector<string> temp;
	curLine = removeCharFromStr(curLine, ' ');
	curLine = removeCharFromStr(curLine, '\t');
	temp.push_back(curLine);
	while (getline(logFile, curLine) && (curLine.substr(0, profHeader.length()).compare(profHeader) != 0))
	{
		if (curLine.substr(0, logHeader.length() + 1).compare(logSpace + logHeader) == 0)
		{
			result.push_back(temp);
			temp.clear();
		}
		
		if (curLine.compare("") != 0)
		{
			curLine = removeCharFromStr(curLine, ' ');
			curLine = removeCharFromStr(curLine, '\t');
			temp.push_back(curLine);
		}
	}
	if (temp.size() > 1)
	{
		result.push_back(temp);
	}
	logFile.close();
	return result;
}

/** Updated C. A. Acred on 10/12/2015
 * Removes name from all account's logs.
 * Returns -1 if not in logs or account doesn't exist
 *
 */
int removeLogEntry(string name, string account)
{
	string curLine;
	int line = findProfile(account, logPath);
	if (line == -1) {return -1;}
	
	logFile.open(logPath, fstream::in);
	for (int i = 1; i <= line; i++)
	{
		getline(logFile, curLine);
	}
	getline(logFile, curLine); 
	line++;
	curLine = removeCharFromStr(curLine, ' ');
	curLine = removeCharFromStr(curLine, '\t');
	
	vector<string> temp;
	int found = -1;
	while (getline(logFile, curLine) && (curLine.substr(0, profHeader.length()).compare(profHeader) != 0))
	{
		line++;
		curLine = removeCharFromStr(curLine, ' ');
		curLine = removeCharFromStr(curLine, '\t');
		temp = splitString(curLine, ":");
		if (temp[0].compare(name) == 0)
		{
			found = 0;
			removeLine(logPath, line);
			line--;
			string prevLine = getLine(logPath, line);
			string nextLine = getLine(logPath, line + 1);
			prevLine = removeCharFromStr(prevLine, ' ');
			prevLine = removeCharFromStr(prevLine, '\t');
			nextLine = removeCharFromStr(nextLine, ' ');
			nextLine = removeCharFromStr(nextLine, '\t');
			if (prevLine.substr(0, logHeader.length()).compare(logHeader) == 0 && (nextLine.substr(0, logHeader.length()).compare(logHeader) == 0 || nextLine.compare("") == 0 || nextLine.substr(0, profHeader.length()).compare(profHeader) == 0))
			{
				removeLine(logPath, line);
				line--;
			}
		}
	}
	logFile.close();
	return found;
} 

/** Updated C. A. Acred on 07/19/2015
 * Changes name's logs to have newName in account's logs.
 * Returns -1 if not in logs or account doesn't exist.
 *
 */
int editLogEntry(string name, string newName, string account)
{
	string curLine;
	int line = findProfile(account, logPath);
	if (line == -1) {return -1;}
	
	logFile.open(logPath, fstream::in);
	for (int i = 1; i <= line; i++)
	{
		getline(logFile, curLine);
	}
	getline(logFile, curLine); 
	line++;
	curLine = removeCharFromStr(curLine, ' ');
	curLine = removeCharFromStr(curLine, '\t');
	
	vector<string> temp;
	int found = -1;
	while (getline(logFile, curLine) && (curLine.substr(0, profHeader.length()).compare(profHeader) != 0))
	{
		line++;
		if (curLine.compare("") != 0)
		{
			curLine = removeCharFromStr(curLine, ' ');
			curLine = removeCharFromStr(curLine, '\t');
			
			temp = splitString(curLine, ":");
			if (temp[0].compare(name) == 0)
			{
				string newEntry = logItemSpace + newName + ":" + temp[1];
				found = 0;
				removeLine(logPath, line);
				insertAt(logPath, vector<string>{newEntry}, vector<int>{line});
			}
		}
	}
	logFile.close();
	return found;
}

/** Updated C. A. Acred on 07/19/15
 * Removes all logs from account.
 * Returns -1 if account not created.
 */
int clearLogs(string account)
{
	int line = findProfile(account, logPath);
	if (line == -1) {return -1;}
	logFile.open(logPath, fstream::in);
	string curLine;
	for (int i = 1; i <= line; i++)
	{
		getline(logFile, curLine);
	}
	line++;
	while (getline(logFile, curLine) && (curLine.substr(0, profHeader.length()).compare(profHeader) != 0))
	{
		removeLine(logPath, line);
	}
	logFile.close();
	cout << headerSpace + dash + " Logs cleared " + dash + "\n";
	return 0;
}

/** Updated C. A. Acred on 07/20/15
 * Returns 0 if date 1 is more recent, 1 if date2 if more recent. Returns 2 if they are equal.
 * Returns -1 if there's an error.
 * 
 * Date format: mm/dd/yyyy
 */
int dateIsMoreRecent(string date1, string date2)
{
	int one = 0;
	int two = 0;
	vector<string> oneNums;
	vector<string> twoNums;
	
	vector<int> months = {31, 28, 31, 30, 31, 30, 31, 31, 31, 31, 30, 31};
	
	oneNums = splitString(date1, "/");
	twoNums = splitString(date2, "/");
	
	for (int i = 0; i < atoi(oneNums[0].c_str()); i++)
	{
		one += months[i];
	}
	for (int i = 0; i < atoi(twoNums[0].c_str()); i++)
	{
		two += months[i];
	}
	one += atoi(oneNums[1].c_str());
	two += atoi(twoNums[1].c_str());
	
	one += atoi(oneNums[2].c_str()) * 365;
	two += atoi(twoNums[2].c_str()) * 365;
	
	if (one > two)
		return 0;
	else if (one == two)
		return 2;
	return 1;
}

/** Updated C. A. Acred on 07/20/15
 * Removes name's account from path.
 * Returns -1 if not found.
 * 
 */
int removeAccount(string path, string name)
{
	int line;
	if ((line = findProfile(name, path)) == -1)
		return -1;
	fstream input;
	input.open(path, fstream::in);
	string curLine;
	for (int i = 1; i <= line; i++)
	{
		getline(input, curLine);
	}
	removeLine(path, line);
	while (getline(input, curLine) && (curLine.substr(0, profHeader.length()).compare(profHeader) != 0))
	{
		removeLine(path, line);
	}
	input.close();
	return 0;
}

/** Updated C. A. Acred on 07/24/15
 * Returns a char pointer to a c_string of the desktop path.
 *
 * Copied from the internet.
 * 
 */
char* getDesktopPath(void)
{
	ULONG ulDataType;
	HKEY hKey;
	DWORD dwToRead = 100;
	static char strPath[100];
	char strKey[] = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders";
	 
	RegOpenKeyEx(HKEY_CURRENT_USER,strKey,
				 0,KEY_READ,&hKey);
	RegQueryValueEx(hKey,"Desktop",NULL,
				&ulDataType,(LPBYTE)strPath,&dwToRead);
	strPath[dwToRead] = '\0';
	RegCloseKey(hKey);
	return strPath;
}

/** Updated C. A. Acred on 07/27/15
 * Returns an int, 0-6 to represent the day of the week of a given date.
 *
 * Copied from the internet.
 * 
 */
int getDOWFromDate(string month, string day, string year)
{
	tm timeStruct = {};
	timeStruct.tm_year = atoi(year.c_str()) - 1900;
	timeStruct.tm_mon = atoi(month.c_str()) - 1;
	timeStruct.tm_mday = atoi(day.c_str());
	timeStruct.tm_hour = 12;    //  To avoid any doubts about summer time, etc.
	mktime( &timeStruct );
	return timeStruct.tm_wday; 
}



