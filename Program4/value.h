/*
 * value.h
 */

#ifndef VALUE_H_
#define VALUE_H_

#include <string>
#include <iostream>
using namespace std;

extern void RunTimeError (string);

// object holds boolean, integer, or string, and remembers which it holds
class Value {
	bool		bval;
	int		ival;
	string	sval;
	enum VT { isBool, isInt, isString, isTypeError } type;

public:
	Value() : bval(false), ival(0), type(isTypeError) {}
	Value(bool bval) : bval(bval), ival(0), type(isBool) {}
	Value(int ival) : bval(false), ival(ival), type(isInt) {}
	Value(string sval) : bval(false), ival(0), sval(sval), type(isString) {}

	// in the case of an error, I use the value to hold the error message
	Value(string sval, bool isError) : bval(false), ival(0), sval(sval), type(isTypeError) {}

	bool isBoolType() const { return type == VT::isBool; }
	bool isIntType() const { return type == VT::isInt; }
	bool isStringType() const { return type == VT::isString; }
	bool isError() const { return type == VT::isTypeError; }
	bool hasMessage() const { return isError() && sval.size() > 0; }

	bool isTrue() const { return isBoolType() && bval; }
	bool getBoolean() const {
		if( !isBoolType() )
			throw "Not boolean valued";
		return bval;
	}

	int getInteger() const {
		if( !isIntType() )
			throw "Not integer valued";
		return ival;
	}

	string getString() const {
		if( !isStringType() )
			throw "Not string valued";
		return sval;
	}

	string getMessage() const {
		if( !hasMessage() )
			throw "No message";
		return sval;
	}

	friend ostream& operator<<(ostream& out, const Value& v) {
		if( v.type == VT::isBool ) out << (v.bval ? "True" : "False");
		else if( v.type == VT::isInt ) out << v.ival;
		else if( v.type == VT::isString ) out << v.sval;
		else if( v.sval.size() > 0 ) out << "RUNTIME ERROR " << v.sval;
		else out << "TYPE ERROR";
		return out;
	}

	Value operator+(const Value& v){
        if(v.isIntType() && this->isIntType()){
            int val = this->getInteger()+ v.getInteger();
            return val;
        }
        if(v.isStringType() && this->isStringType()){
            string val = this->getString()+ v.getString();
            return val;
        }
        else{
            RunTimeError("Cannot add these two values");
            return Value();
        }
    }
	Value operator-(const Value& v){
        if(v.isIntType() && this->isIntType()){
            int val = this->getInteger()+ v.getInteger();
            return val;
        }
        if(v.isStringType() && this->isStringType()){
            string string1 = v.getString();
            string substring = this->getString();
            auto posit = string1.find(substring);
            
            string1.replace(posit, substring.length(),"");
            return string1;
        }
        else{
            RunTimeError("Cannot subtract these two values");
            return Value();
        }
    }
	Value operator*(const Value& v){
        if(v.isIntType() && this->isIntType()){
            int val = this->getInteger()* v.getInteger();
            return val;
        }
        if(v.isStringType() && this->isStringType()){
            RunTimeError("Cannot multiply these two strings");
            return Value();
        }
        if(v.isIntType() && this->isStringType()){
            if(v.getInteger()>0){
                string string1;
                for(int i = 0; i<v.getInteger(); i++){
                    string1 += this->getString();
                }
                return string1;
            }
            else{
                RunTimeError("Cannot multiply string with a negative number");
                return Value();
            }
            
        }
        else{
            RunTimeError("Cannot multiply these two values");
            return Value();
        }
    }
	Value operator/(const Value& v){
        if(v.isIntType() && this->isIntType()){
            int val = this->getInteger() + v.getInteger();
            return val;
        }
        if(v.isStringType() && this->isStringType()){
            string val = this->getString() + v.getString();
            return val;
        }
        else{
            RunTimeError("Cannot divide these two values");
            return Value();
        }
        
    }
	Value operator<(const Value& v){
        if(v.isIntType() && this->isIntType()){
             return Value(v.getInteger() < this->getInteger());  
        }
        if(v.isStringType() && this->isStringType()){
            return Value(v.isStringType() < this->getInteger());
        }
        else{
            RunTimeError("Type mismatch in <");
            return Value();
        }
    }
	Value operator<=(const Value& v){
        if(v.isIntType() && this->isIntType()){
            return Value(v.getInteger() <= this->getInteger());
        }
        if(v.isStringType() && this->isStringType()){
            return Value(v.getString() <= v.getString());
        }
        else{
            RunTimeError("Type mismatch in <=");
            return Value();
        }
    }
	Value operator>(const Value& v){
        if(v.isIntType() && this->isIntType()){
            return Value(v.getInteger() > this->getInteger());
        }
        if(v.isStringType() && this->isStringType()){
            return Value(v.getString() > this->getString());
        }
        else{
            RunTimeError("Type mismatch in >");
            return Value();
        }
    }
	Value operator>=(const Value& v){
        if(v.isIntType() && this->isIntType()){
            return Value(v.getInteger() >= this->getInteger());
        }
        else{
            RunTimeError("Type mismatch in >=");
            return Value();
        }                
    }
	Value operator==(const Value& v){
        if(v.isIntType() && this->isIntType()){
            return Value(v.getInteger() == this->getInteger());
        }
        if(v.isStringType() && this->isStringType()){
            return Value(v.getString() == this->getString());
        }
        if(v.isBoolType() && this->isBoolType()){
            return Value(v.getBoolean() == this->getBoolean());
        }
        else{
            RunTimeError("Type mismatch in ==");
            return Value();
        }
    }
	Value operator!=(const Value& v) {
		Value ans = this->operator==(v);
		ans.bval = !ans.bval;
		return ans;
	}
};

#endif /* VALUE_H_ */
