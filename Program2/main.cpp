#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include <fstream>
#include <vector>
#include <stdbool.h>
#include <algorithm>
#include <set>

using namespace std;
int main(int argc, char const *argv[])
{
    bool isV = false;
    bool isID = false;
    bool isSum = false;
    bool readFile = false;
    
    int tokencounter = 0;
    int stringcounter = 0;
    int idCount =0;
    Token tok;
    int lineNumber=0;
    
    vector<string> idents;
    vector<string> filevec;
    
    for(int i = 1; i<argc; i++)
    {
        if(argv[i][0]=='-')
        {
            if(strcmp(argv[i],"-v")==0)
            {
                isV = true;
            }
            else if(strcmp(argv[i],"-allids")==0)
            {
                isID = true;
            }
            else if(strcmp(argv[i],"-sum")==0)
            {
                isSum = true;
            }
            else
            {
                cout << "INVALID FLAG "<<argv[i]<<endl;
                return 0;
            }
        }
        else
        {
            filevec.push_back(argv[i]);
        }
    }   
    ifstream infile;
    
        if (filevec.size()>1)
        {
            cout << "TOO MANY FILE NAMES" << endl;
            return 0;
        }
        else if(filevec.size()==0)
        {
            return 0;
        }
        else
        {

            infile.open(filevec.front());

            if(infile.is_open())
            {
                readFile = true;
                istream *in= readFile ? &infile : &cin;
                
                
                //good file
                while((tok=getNextToken(in, &lineNumber))!= DONE && tok != ERR)
                {
                    ++tokencounter;
                    
                    if(tok == IDENT)
                    {
                        //do something
                        idCount++;
                        idents.push_back(tok.GetLexeme());
                    }
                    if(tok == SCONST)
                    {
                        //do something
                        stringcounter++;
                    }
                    if(isV)
                    {
                        cout << tok << endl;
                    }


                }
               

                if(isID)
                {
                    cout << "IDENTIFIERS: ";
                    std::sort(idents.begin(),idents.end());
                    //remove duplicates later
                    for(int i = 0; i<idents.size(); i++)
                    {
                        if(i < idents.size()-1)
                        {
                            cout<< idents[i] << ", ";
                        }
                        else
                        {
                            cout << idents[i] << endl;
                        }
                    }

                }
                
                
                if(isSum)
                {
                    cout << "Total lines: " << lineNumber << endl;
                    cout << "Total tokens: " << tokencounter << endl;
                    cout << "Total identifiers: " << idCount << endl;
                    cout << "Total strings: " << stringcounter << endl; 
                }
                
                
                if (tok == ERR)
                {
                    cout << "Error on line " << tok.GetLinenum() << " ("<< tok.GetLexeme() << ")" << endl; ;
                    return 0;
                }
                

                
            }
            else
            {
                cout << "UNABLE TO OPEN " << filevec.front() << endl;
                return 0;
            }

        }
    
    return 0;
}