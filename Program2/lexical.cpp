/*Yu Chen CS280 Fall 2018 */

#include "tokens.h"
#include <iostream>
#include <fstream>
#include <map>
#include <cctype>

using namespace std;



//tokens
static map<TokenType, string> token_map = 
{
    {PLUS,"PLUS"},
    {MINUS,"MINUS"},
    {STAR,"STAR"},
    {SLASH,"SLASH"},
    {ASSIGN,"ASSIGN"},
    {SCONST,"SCONST"},
    {ICONST,"ICONST"},
    {IDENT, "IDENT"},
    {EQ, "EQ"},
    {NEQ,"NEQ"},
    {LT, "LT"},
    {GT,"GT"},
    {GEQ, "GEQ"},
    {LEQ, "LEQ"},
    {LOGICAND, "LOGICAND"},
    {LOGICOR,"LOGICOR"},
    {LPAREN,"LPAREN"},
    {RPAREN,"RPAREN"},
    {ERR,"ERR"},
    {SC,"SC"},

};


Token keywords(const string& lexeme, int linenum)
{
    TokenType tt = IDENT;
    if(lexeme == "print")
    {
        tt = PRINT;
    }
    if(lexeme == "if")
    {
        tt = IF;
    }
    if(lexeme == "then")
    {
        tt = THEN;
    }
    if(lexeme == "true")
    {
        tt = TRUE;
    }
    if(lexeme == "false")
    {
        tt = FALSE;
    }
    return Token(tt, lexeme, linenum);
}


/*Token
keyorident(const string& lexeme, int linenum)
{
    //iterator
    TokenType tt = IDENT;
    auto it = keywords.find(lexeme);
    if (it != keywords.end())
    {
        tt = it->second;
    }
    return Token(tt, lexeme, linenum);
}*/

ostream& operator<<(ostream& out, const Token& tok)
{
    TokenType tt = tok.GetTokenType();
    string conString = tok.GetLexeme();
    conString.erase(conString.begin()+0);
    if (tt == PLUS || tt == MINUS || tt == STAR || tt == SLASH || tt == SC || tt == ASSIGN || tt == PRINT)
    {
        out << token_map[tt];
    }
    if(tt == IDENT || tt == ICONST || tt == ERR || tt == SCONST)
    {
        out <<token_map[tt];
        out << '('<<tok.GetLexeme()<<')';
    }

    return out;
}
// states
Token
getNextToken(istream *in, int *linenum)
{
    enum LexState { BEGIN, INID, INSTRING, INCOMMENT, ININT, INEQ, INNEQ, INLT, INGT, INOR, INAND /* others */ };
    LexState lexstate = BEGIN;
    string lexeme;
    char ch;
    while(in->get(ch)) 
    {
        if( ch == '\n' )
        {
            (*linenum)++;
        }
        switch(lexstate)
        {
            case BEGIN:
                if( isspace(ch) )
                    continue;
                lexeme = ch;
                if( isalpha(ch) ) 
                {
                    lexstate = INID;
                }
                else if(isdigit(ch))
                {
                    lexstate = ININT;
                }
                else if( ch == '"' )
                {
                    lexstate = INSTRING;
                }
                else if(ch == '#')
                {
                    lexstate = INCOMMENT;
                }
                else if(ch == '!')
                {
                    lexstate = INNEQ;
                }
                else if(ch == '=')
                {
                    lexstate = INEQ;
                }
                else if(ch == '>')
                {
                    lexstate = INGT;
                }
                else if(ch == '<')
                {
                    lexstate = INLT;
                }
                else if(ch == '|')
                {
                    lexstate = INOR;
                }
                else if(ch == '&')
                {
                    lexstate = INAND;
                }
                else
                {
                    TokenType tt = ERR;
                    switch(ch)
                    {
                        case '+':
                            tt = PLUS;
                            break;
                        case '-':
                            tt = MINUS;
                            break;
                        case '*':
                            tt = STAR;
                            break;
                        case '/':
                            tt = SLASH;
                            break;
                        case '=':
                            tt = ASSIGN;
                            break;
                        case '<':
                            tt = LT;
                            break;
                        case '>':
                            tt = GT;
                            break;
                        case '(':
                            tt = LPAREN;
                            break;
                        case ')':
                            tt = RPAREN;
                            break;
                        case ';':
                            tt = SC;
                            break; 
                    }
                    return Token(tt, lexeme, *linenum);
                }
                break;
                
            case INID:
                if(isalpha(ch) || isdigit(ch))
                {
                    lexeme += ch;
                    break;
                }
                //can be followed by zero or more alphas
                else
                {
                    if(ch == '\n')
                    {
                        (*linenum)--;
                    }  
                    in->putback(ch);
                    return keywords(lexeme, *linenum);
                }
                
                
            case ININT:
                if(isdigit(ch))
                   {
                      lexeme += ch;
                   }
                else if(isalpha(ch))
                   {
                      lexeme += ch; 
                      return Token(ERR, lexeme, *linenum);
                   }
                else
                   {
                       if(ch == '\n')
                       {
                           (*linenum)--;
                       }
                       in->putback(ch);
                       return Token(ICONST, lexeme, *linenum);
                   }
                   break;
  
            case INSTRING:
                lexeme += ch;
                if(ch == '\n')
                {
                    return Token(ERR, lexeme, *linenum);
                }
                if(ch == '"')
                {
                    lexeme = lexeme.substr(1, lexeme.length()-2);
                    return Token(SCONST, lexeme, *linenum);
                }
                break;
                
            /*case INHASH:
                if(ch != '#')
                {
                    in->putback(ch);
                }
                lexstate = INCOMMENT;
                break;*/
                
            case INCOMMENT:
                if(ch == '\n')
                   {
                       //self loop
                       lexstate = BEGIN;
                   }
                break;
            case INNEQ:
                if(ch == '=' )
                {
                    lexeme += ch;
                    return Token(NEQ, lexeme, *linenum);
                }
                else
                {
                    if(ch == '\n')
                    {
                        (*linenum)--;
                    }
                    in->putback(ch);
                    return Token(ERR, lexeme, *linenum);
                }
            case INEQ:
                if(ch == '=' )
                {
                    lexeme += ch;
                    return Token(EQ, lexeme, *linenum);
                }
                else
                {
                    in->putback(ch);
                    if(ch == '\n')
                        (*linenum)--;
                    return Token(ASSIGN, lexeme, *linenum);
                }
            case INGT:
                if(ch == '=')
                {
                    return Token(GEQ, lexeme, *linenum);
                }
                else
                {
                    if(ch == '\n')
                    {
                        (*linenum)--;
                    }
                    in->putback(ch);
                    return Token(GT, lexeme, *linenum);
                }
            case INLT:
                if(ch == '<')
                {
                    return Token(LEQ, lexeme, *linenum);
                }
                else
                {
                    if(ch == '\n')
                    {
                        (*linenum)--;
                    }
                    in->putback(ch);
                    return Token(LT, lexeme, *linenum);
                }
            case INOR:
                if(ch == '|' )
                {
                    lexeme += ch;
                    return Token(LOGICOR, lexeme, *linenum);
                }
                else
                {
                    if(ch == '\n')
                    {
                        (*linenum)--;
                    }
                    in->putback(ch);
                    return Token(ERR, lexeme, *linenum);
                }
            case INAND:
                if(ch == '&')
                {
                    lexeme += ch;
                    return Token(LOGICAND, lexeme, *linenum);
                }
                else
                {
                    if(ch == '\n')
                    {
                        (*linenum)--;
                    }
                    in->putback(ch);
                    return Token(ERR, lexeme, *linenum);
                }
        }
    }
        //end of file
        if(in->eof())
        {
            return Token(DONE, "", *linenum);
        }
    return Token(ERR, "error", *linenum);
}

