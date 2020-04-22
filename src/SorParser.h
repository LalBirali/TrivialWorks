#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <iostream>
#include "Entry.h"
#include <map>
#include <string>
#include <algorithm>
#include "string.h"
#include "dataframe.h"

using namespace std;

/**
SorParser::
This class contains all of the parsing used to contruct a dataframe.
*/
class SorParser {
public:
    map<int, map<int, Entry*>> m;
	map<int, TYPE> types;

    SorParser()  {}
    ~SorParser() {}

    /**
    * Determines if a string is a boolean (0 or 1)
    * @arg s String to be determined if it is a boolean.
    */
    bool isBool(string s) {

	    if (s.compare("0") == 0 || s.compare("1") == 0) {
		    return true;
	    } else {
		    return false;
	    }

    }

    /**
    * Determines if a string is an Integer
    * @arg s String to be determined if it is an integer
    */
    bool isInt(string s) {
	    size_t i = 0;

	    if (s[0] == '+' || s[0] == '-') {

		    i = 1;

	    }

	    for (; i < s.length(); i++) {

		    if (!isdigit(s[i])) {

			    return false;

		    }

	    }

	    return true;

    }

    /**
    * Determines if a string is a float
    * @arg s String to be determined if it is a float
    */
    bool isFloat(string s) {

	    size_t i = 0;

	    if (s[0] == '+' || s[0] == '-') {

		    i = 1;

	    }

	    bool periodFound = false;

	    for (; i < s.length(); i++) {

		    if (!isdigit(s[i]) && (s[i] != '.' || periodFound)) {

			    return false;

		    } else if (s[i] == '.') {

			    periodFound = true;

		    }

	    }

	    return true;

    }

    /** Checks to see if every char in the array is a digit
    * @arg number char array that may or may not be a number
    */
    bool isNumber(char number[]) {
        for (int i = 0; number[i] != 0; i++) {
            if (!isdigit(number[i]))
                return false;
        }
        return true;
    }

    /** Verifies that check and against are equals (same characters in the each array)
    * @arg check one array to check is equal to the other array
    * @arg against other array to check is equal to the first
    */
    bool verifyArg(char* check, char* against) {
	    if (strlen(check) != strlen(against)) {
		    return false;
	    } else {
		    for (size_t i = 0; i < strlen(check); i++) {
			    if (check[i] != against[i]) {
				    return false;
			    }
		    }
	    }
	    return true;
    }

    /** Exits the program and outputs malformed command.
    */
    void illegalCommand() {
	    std::cout << "Malformed Command" << std::endl;
	    exit(1);
    }

    /**
    * Determines if y has higher priority in the Type list
    * Empty < Bool < Int < Float < String
    * @arg x first type to compare
    * @arg y second type to compare
    */
    bool priority(TYPE x, TYPE y) {

	    if (x == STRING) {

		    return false;

	    } else if (x == FLOAT) {

		    if (y == STRING) {

			    return true;

		    } else {

			    return false;

		    }
	    } else if (x == INT) {
		    if (y == STRING || y == FLOAT) {
			    return true;
		    } else {
			    return false;
		    }
	    } else if (x == BOOL) {
		    if (y == STRING || y == FLOAT || y == INT) {
			    return true;
		    } else {
			    return false;
		    }
	    } else {
		    return true;
	    }
    }

    /**
    * Determines if s is a valid input
    * @arg s string to be tested if it is valid.
    */
    bool isValid(string s) {

	    bool quotes = (s[0] == '"' && s[s.size() - 1] == '"');
	    if (isBool(s) || isInt(s) || isFloat(s)) {
		    return true;
	    } else {
		    if (quotes) {
			    return true;
		    } else {
			    for (unsigned int i = 0; i < s.size(); i++) {

				    if (s[i] == ' ') {

					    return false;

				    }
			    }
			    return true;
		    }
	    }
    }

    /**
    * Add an entry to the vector of vectors of entries
    * @arg v vector of vector of entries containing the parsed data
    * @arg s string containing the data to be put in the new entry
    * @arg row current row of the query
    * @arg column current column of the query
    * @arg types vector of types of the columns
    */
    void addEntry(map<int, map<int, Entry*>> & m, string s, size_t row, size_t column, map<int, TYPE> & types) {

	    Entry * e;
	    if (s == "") {

		    e = new Entry();
	    } else if (isBool(s)) {

		    if (s.compare("0") == 0) {

			    e = new Entry(false, BOOL);

		    } else {

			    e = new Entry(true, BOOL);
		    }

	    } else if (isInt(s)) {

		    int i = atoi(s.c_str());

		    e = new Entry(i, INT);

	    } else if (isFloat(s)) {

		    float f = stof(s);
		    e = new Entry(f, FLOAT);

	    } 
	    else {
		
		    if (s != "") {
			    string* s2 = new string(s);
			    e = new Entry(s2, STRING);
		    } else {
			    e = new Entry();
		    }
	    }

	    if (types.size() < column + 1) {

		    types[column] = (e->type);

	    } else {

		    if (priority(types[column], e->type)) {

			    types[column] = e->type;
		    }

	    }

	    m[row][column] = e;

    }

    /**
    * Parses the file containing all of the data and outputs the results to the inputted commands.
    * @arg docName document name containing the name of the document
    * @arg from Starting byte to start reading at (default: 0)
    * @arg len total number of bytes to read (default: 500)
    */
    void parseDocFile(char* docName, int from, int len) {
	    std::ifstream ifs;

	    ifs.open(docName, std::ifstream::in);

	    if (!ifs.is_open()) {

		    std::cout << "Incorrect doc name" << std::endl;
		    exit(1);

	    }
	    bool newEntry = false;
	    char c = ifs.get();
	    string s = "";
	    Entry e;
	    int place = 1;
	    int row = 0;
	    int column = 0;

	    for (;place < from; place++) {

		    c = ifs.get();
		    place++;

	    }

	    if (c != '\n' && from != 0) {

		    while (c != '\n') {
			    c = ifs.get();
			    place++;
		    }

	    }

	    while(ifs.good() && place < from+len) {

		    if (c == '<' && !newEntry) {

			    newEntry = true;

		    } else if (c == '>' && newEntry) {
			
			    string WHITESPACE = " ";
			    size_t start = s.find_first_not_of(WHITESPACE);
			    s = (start == std::string::npos) ? "" : s.substr(start);

			    size_t end = s.find_last_not_of(WHITESPACE);
			    s = (end == std::string::npos) ? "" : s.substr(0, end + 1);

			    if (isValid(s)) {
				    addEntry(m, s, row, column, types);
			    } else {
				    addEntry(m, "", row, column, types);
			    }
			    newEntry = false;
			    s = "";
			    column++;

		    } else if (c == '\n') {

			    newEntry = false;
			    s = "";
			    row++;
			    column = 0;

		    } else {
			    if (newEntry) {
				    s += c;
			    }
		    }

		    c = ifs.get();
		
		    place++;

	    }
    	ifs.close();

	    if (c != std::ifstream::traits_type::eof()) {
		    if (c != '\n' && m[m.size() - 1].size() != types.size()) {
			    m.erase(m.size() - 1);
		    }
	    }
	
	    for (size_t i = 0; i < m.size(); i++) {
		    while (m[i].size() < types.size()) {
			    Entry* e = new Entry();
			    m[i][m[i].size()] = e;

		    }
	    }
	
	    for (size_t i = 0; i < m.size(); i++) {
		    for (size_t j = 0; j < m[i].size(); j++) {
			    if (m[i][j]->type != EMPTY) {
				    m[i][j]->changeType(types[j]);
			    }
		    }
	    }
	
    }
	StrBuff * getSchema() {
    	StrBuff * stb = new StrBuff();
    	for (unsigned int i = 0; i < types.size(); i++) {
        	switch(types.at(i)) {
            	case INT: {
              	  	stb->c("I");
                	break;
            	}
            	case BOOL: {
                	stb->c("B");
                	break;
            	}
            	case FLOAT: {
                	stb->c("F");
                	break;
            	}
            	case STRING: {
                	stb->c("S");
                	break;
            	}
            	case EMPTY: {
                	stb->c("B");
                	break;
            	}
        
        	}
    	}
    	return stb;
	}

	void setUpDF(Schema * newSchema, DataFrame * df) {
		Row r(*newSchema);
		for (unsigned int i = 0; i < m.size(); i++) {
        	for (unsigned int j = 0; j < m.at(i).size(); j++) {
            	switch(types.at(j)) {
                	case INT: {
                    	r.set(j, m.at(i).at(j)->getIntValue());
                    	break;
                	}
                	case BOOL: {
                    	r.set(j, m.at(i).at(j)->getBoolValue());
                    	break;
                	}
                	case FLOAT: {
                    	r.set(j, m.at(i).at(j)->getFloatValue());
                    	break;
                	}
                	case STRING: {
                    	String * str = new String(m.at(i).at(j)->getStringValue()->c_str());
                    	r.set(j, str);
                    	break;
                	}
                	case EMPTY: {
                    	r.set(j, m.at(i).at(j)->getBoolValue());
                    	break;
                	}
            	}
        	}
        	df->add_row(r);
    	}
	}
};