/*
 * main.cpp
 */

#include "tokens.h"
#include "parse.h"
#include <fstream>
#include <cctype>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>



using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

int
main(int argc, char *argv[])
{
	ifstream file;
	istream *in;
	int linenum = 0;
    in = &cin;
    vector<string> filevec;

	// CHECK ARGUMENTS, OPEN FILES
    
    for(int i = 1; i<argc; i++)
    {
        filevec.push_back(argv[i]);
                 
    }
    if(argc == 2)
    {
        file.open(filevec.front());
        if(file.is_open())
        {
            if(file.peek()==ifstream::traits_type::eof())
            {
                //nothing
                cout << "0: No statements in program" <<endl;
                return 0;
            }
        }
        else
        {
            cout << "COULD NOT OPEN " << argv[1] << endl;
            return 0;
        }
    }
    if(filevec.size()>1)
    {
        cout << "TOO MANY FILENAMES" << endl;
        return 0;
    }


    ParseTree *prog = Prog(in, &linenum);
    if( prog == 0 )
        return 0; // quit on error

        // GENERATE OUTPUT


	return 0;
 
}