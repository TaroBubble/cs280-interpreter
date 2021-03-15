/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
using std::vector;
using std::map;

extern map<string, Value> symbols;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE, BOOLTYPE };

// a "forward declaration" for a class to hold values
class Value;



class ParseTree {
	int			linenum;

public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}
    ParseTree	*left;
	ParseTree	*right;

	virtual ~ParseTree() {
		delete left;
		delete right;
	}

	int GetLinenum() const { return linenum; }

	virtual NodeType GetType() const { return ERRTYPE; }

	int LeafCount() const {
		int lc = 0;
		if( left ) lc += left->LeafCount();
		if( right ) lc += right->LeafCount();
		if( left == 0 && right == 0 )
			lc++;
		return lc;
	}

	virtual bool IsIdent() const { return false; }
	virtual bool IsString() const { return false; }
    
    

	virtual string GetId() const { return ""; }

	int IdentCount() const {
		int cnt = 0;
		if( left ) cnt += left->IdentCount();
		if( right ) cnt += right->IdentCount();
		if( IsIdent() )
			cnt++;
		return cnt;
	}

	int StringCount() const {
		int cnt = 0;
		if( left ) cnt += left->StringCount();
		if( right ) cnt += right->StringCount();
		if( IsString() )
			cnt++;
		return cnt;
	}

	void GetVars(map<string,int>& var) {
		if( left ) left->GetVars(var);
		if( right ) right->GetVars(var);
		if( IsIdent() )
			var[ this->GetId() ]++;
	}
    
    virtual Value Eval(){
        return Value();
    }
    
};

class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    
    virtual Value Eval()
    {
        left->Eval();
        if (right)
        {
            right->Eval();
        }
        
        return Value();
    }
};

class IfStatement : public ParseTree {
public:
	IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
};

class Assignment : public ParseTree {
public:
	Assignment(int line, ParseTree *lhs, ParseTree *rhs) : ParseTree(line, lhs, rhs) {}
    
    //eval expression then add to symbols
};

class PrintStatement : public ParseTree {
public:
	PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}
    
    //eval then cout
    
};

class PlusExpr : public ParseTree {
public:
	PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
};

class MinusExpr : public ParseTree {
public:
	MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
};

class TimesExpr : public ParseTree {
public:
	TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
};

class DivideExpr : public ParseTree {
public:
	DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
};

class LogicAndExpr : public ParseTree {
public:
	LogicAndExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const { return BOOLTYPE; }
};

class LogicOrExpr : public ParseTree {
public:
	LogicOrExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const { return BOOLTYPE; }
};

class EqExpr : public ParseTree {
public:
	EqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const { return BOOLTYPE; }
};

class NEqExpr : public ParseTree {
public:
	NEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const { return BOOLTYPE; }
};

class LtExpr : public ParseTree {
public:
	LtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const { return BOOLTYPE; }
};

class LEqExpr : public ParseTree {
public:
	LEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const { return BOOLTYPE; }
};

class GtExpr : public ParseTree {
public:
	GtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const { return BOOLTYPE; }
};

class GEqExpr : public ParseTree {
public:
	GEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}

	NodeType GetType() const { return BOOLTYPE; }
};

class IConst : public ParseTree {
	int val;

public:
	IConst(int l, int i) : ParseTree(l), val(i) {}
	IConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}

	NodeType GetType() const { return INTTYPE; }
};

class BoolConst : public ParseTree {
	bool val;

public:
	BoolConst(Token& t, bool val) : ParseTree(t.GetLinenum()), val(val) {}

	NodeType GetType() const { return BOOLTYPE; }
};

class SConst : public ParseTree {
	string val;

public:
	SConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}

	NodeType GetType() const { return STRTYPE; }
	bool IsString() const { return true; }
};

class Ident : public ParseTree {
	string id;

public:
	Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

	bool IsIdent() const { return true; }
	string GetId() const { return id; }
};

#endif /* PARSETREE_H_ */