#ifndef __TYPE_H__
#define __TYPE_H__
#include <vector>
#include <string>
#include<iostream>
#include <SymbolTable.h>

class Type
{
protected:
    //类型系统，包含bool型(1位整数)，int(32位)，void，func，ptr，float和array
    enum {BOOL,INT, VOID, FUNC, PTR,FLOAT,ARRAY};    
public:
    int kind;
    Type(int kind) : kind(kind) {};
    virtual ~Type() {};
    virtual std::string toStr() = 0;

    bool isInt()const {return kind==INT;};
    bool isFloat() const {return kind==FLOAT;};
    bool isBool() const {return kind==BOOL;};
    bool isVoid() const {return kind == VOID;};
    bool isFunc() const {return kind == FUNC;};
    bool isArray() const {return kind == ARRAY;};
    bool isPtr() const { return kind == PTR; };
    int getkind(){return kind;};

    //getOSize用于在不确定具体类型时，通过让基类type来确定类型并返回位数 bit
    int getOSize() ;
    
    virtual bool whetherConst(){return false;};
    bool isIndirectInt()const;
    bool isIndirectFloat() const ;
};

class IntType : public Type
{
private:
    int size;
    bool ifconst;
public:
    IntType(int size,bool ifconst=false) : Type(Type::INT), size(size),ifconst(ifconst){};
    std::string toStr();

    int getSize(){return size;};
    bool whetherConst(){return ifconst;}
};

class FloatType : public Type
{
private:
    int size;
    bool ifconst;
public:
    FloatType(int size, bool ifconst = false) : Type(Type::FLOAT), size(size), ifconst(ifconst){};
    std::string toStr();
    int getSize(){return size;};
    bool whetherConst(){return ifconst;}
};

//BoolType类型并不直接继承自Type基类，而是一种特殊的int
class BoolType:public IntType
{
public:
    BoolType():IntType(1){kind=Type::BOOL;};
    std::string toStr();
};

class VoidType : public Type
{
public:
    VoidType() : Type(Type::VOID){};
    std::string toStr();
};

class FunctionType : public Type
{
    
private:
    //函数类型中细分函数的返回值类型和函数的参数类型，最后一个参数列表怀疑冗余
    Type *returnType;
    std::vector<Type*> paramsType;
    std::vector<SymbolEntry*> params;
    int IntparamCount=0;
    int FloatparamCount=0;
public:
    //两种构造函数，区别在于是否有参数列表
    FunctionType(Type* returnType, std::vector<Type*> paramsType) : Type(Type::FUNC), returnType(returnType), paramsType(paramsType){};
    FunctionType(Type* returnType, std::vector<Type*> paramsType,std::vector<SymbolEntry*> params) : Type(Type::FUNC), returnType(returnType), paramsType(paramsType),params(params){};
    Type* getRetType() {return returnType;};
    std::string toStr();
    
    //对于参数类型列表和参数列表的getset
    void setparamsType(std::vector<Type*> paramsType){this->paramsType=paramsType;}
    std::vector<Type*> getparamsType(){return paramsType;} 
    void setparams(std::vector<SymbolEntry*> params){this->params=params;}
    std::vector<SymbolEntry*> getParams(){return params;}
    void setIntparamCount(int x){this->IntparamCount=x;};
    int getIntparamCount(){return IntparamCount;};
    void setFloatparamCount(int x){this->FloatparamCount=x;};
    int getFloatparamCount(){return FloatparamCount;};
};

class ArrayType : public Type
{
private:
    Type* artype;  //数组类型（int、float），还是多维数组，多维数组第二维及以上的话，artype是array
    int num;        //数组元素个数
    bool ifconst;   //是否常量数组
    int size=0;       //数组总位数，
    Type* parenttype = nullptr;//在嵌套数组中，其向上一层的类型
public:
    ArrayType( Type* artype,int num,bool ifconst=false) : Type(Type::ARRAY), artype(artype), num(num),ifconst(ifconst) 
    {
        this->size=artype->getOSize()*num;
        /*if(artype->isInt()) 
            this->size=((IntType *)artype)->getSize() * num;
        if(artype->isFloat()) 
            this->size=((FloatType *)artype)->getSize() * num;
        if(artype->isArray()) 
            this->size=((ArrayType *)artype)->getArraySize() * num;
        */
    };
    std::string toStr();
    Type* getArrayType() const { return artype; };
    int getArrayNum() const {return num;};
    bool ifArrayConst() const { return ifconst; };
    int getArraySize() const {return size;}

    //parentType的getset
    Type* getParentType() const { return parenttype; };
    void setParentType(Type* parenttype) { this->parenttype = parenttype; };

    //在多维数组中，确定其最终到底是一个整型还是浮点
    Type* getFinalType() 
    {
        Type * temp=this;
        while(temp->isArray()){
            temp=((ArrayType *)temp)->getArrayType() ;
        }
        return temp;
    }
};

class PointerType : public Type
{
private:
    Type *valueType;
public:
    PointerType(Type* valueType) : Type(Type::PTR) 
    {
        this->valueType = valueType;
    };
    std::string toStr();
    Type* getType() const { return valueType; };
};

class TypeSystem
{
private:
    static IntType commonInt;
    static IntType commonIntConst;
    static BoolType commonBool;
    static VoidType commonVoid;
    static FloatType commonFloat;   
    static FloatType commonFloatConst;
public:
    static Type *intType;
    static Type *intconstType;
    static Type *voidType;
    static Type *boolType;
    static Type *floatType;
    static Type *floatconstType;
};

#endif
