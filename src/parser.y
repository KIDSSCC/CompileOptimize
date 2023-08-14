%code top{
    #include <iostream>
    #include <cstring>
    #include <assert.h>
    #include "parser.h"

    //来自main.cpp
    extern Ast ast;
    int yylex();
    int yyerror( char const * );

    std::string tempType;

    //为while循环中的break和continue做的准备。
    std::vector<int> new_WhileStmt_Pos;
    std::vector<loopctl*> loop_ctrl;

    //记录while句子个数，如果为0的时候遇见了break或continue要报错
    int whilenum = 0;
    bool if_FuncDefStmt_IsNull=0;

    ArrayType* arraytype;
    //数组索引
    int myindex;
    //存储进的等价一维int数组，重点计算size
    int* equalintarray;
    float* equalfloatarray;
    std::stack<Initlist*> initstack;
    Initlist* top;
    int bracenum= 0;
    
    int intParam_num_for_func=0;
    int floatParam_num_for_func=0;
    bool functionhasRet=false;
}

%code requires {
    #include "Ast.h"
    #include "SymbolTable.h"
    #include "Type.h"    
}
%union {
    int itype;
    float ftype;        
    char* strtype;
    StmtNode* stmttype;
    ExprNode* exprtype;
    Type* type;
    SymbolEntry* se;    
}
%start Program
%token <strtype> ID 
%token <itype> INTEGER
%token <ftype> FLOATNUMBER    
%token IF ELSE  WHILE
%token INT VOID FLOAT
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE SEMICOLON COMM
%token ADD SUB MUL DIV MOD OR AND NOT LESS GREAT EQ NE LE GE ASSIGN
%token CONST
%token RETURN CONTINUE BREAK

%nterm <stmttype> Stmts Stmt AssignStmt BlockStmt IfStmt ReturnStmt DeclStmt FuncDef 
%nterm <stmttype> WhileStmt BrkStmt CtnueStmt ConstDecl ConstDefList ConstDef VarDecl VarDefList VarDef FuncParams FuncParam  BlankStmt ExprStmt
%nterm <exprtype> Exp AddExp Cond LOrExp PrimaryExp LVal RelExp LAndExp UnaryExp MulExp EqExp FuncCall ParaList ArrayIndex ConstExp ConstInitVal VarInitVal ConstInitValList VarInitValList FunArrayIndex
%nterm <type> Type

%precedence THEN
%precedence ELSE
%%
Program
    : Stmts {
        ast.setRoot($1);
    }
    ;
Stmts
    : Stmt {$$=$1;}
    | Stmts Stmt{
        $$ = new SeqNode($1, $2);
    }
    ;
Stmt
    : AssignStmt { $$=$1;}
    | BlockStmt {$$=$1;}
    | IfStmt {$$=$1;}
    | ReturnStmt {$$=$1;}
    | DeclStmt {$$=$1; }
    | FuncDef {$$=$1;}
    | WhileStmt {$$=$1;}
    | BrkStmt {$$=$1;}
    | CtnueStmt {$$=$1;}
    | BlankStmt {$$=$1;}    
    | ExprStmt {$$=$1;}  
    ;
LVal
    : ID {  
        SymbolEntry *se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier %s is undeclared\n", (char*)$1);
            delete [](char*)$1;
            exit(EXIT_FAILURE); //报错退出
        }
        $$ = new Id(se);
        delete []$1;
    }
    | ID ArrayIndex{
        SymbolEntry* se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier %s is undeclared\n", (char*)$1);
            delete [](char*)$1;
            exit(EXIT_FAILURE); 
        }
        //找到了对应的符号表项
        $$ = new Id(se, $2);
        delete []$1;
    }
    ;
AssignStmt
    :
    LVal ASSIGN Exp SEMICOLON {
        $$ = new AssignStmt($1, $3);
    }
    ;
BlockStmt
    :   LBRACE 
        {identifiers = new SymbolTable(identifiers);} 
        Stmts RBRACE 
        {
            //遇见左大括号时，创建新的作用域，其中的语句中涉及的变量作用域都会随之更新，直至遇见右大括号，恢复原来的作用域
            $$ = new CompoundStmt($3);
            SymbolTable *top = identifiers;
            identifiers = identifiers->getPrev();
            delete top;
        }
        |   LBRACE RBRACE{
            $$=new CompoundStmt();
            //
            if_FuncDefStmt_IsNull=1;
        }
    ;
BlankStmt
    : SEMICOLON {
        $$ = new BlankStmt();
    }
    ;
IfStmt
    : IF LPAREN Cond RPAREN Stmt %prec THEN {
        $$ = new IfStmt($3, $5);
    }
    | IF LPAREN Cond RPAREN Stmt ELSE Stmt {
        $$ = new IfElseStmt($3, $5, $7);
    }
    ;
ReturnStmt
    :RETURN Exp SEMICOLON{
        functionhasRet=true;
        $$ = new ReturnStmt($2);
    }
    | RETURN SEMICOLON {
        functionhasRet=true;
        $$ = new ReturnStmt();
    }   
    ;
WhileStmt
    : WHILE
    {
        //这个地方涉及到两个动态数组，一个是newWhileStmtPos，int数组，一个是loopctrl，loopctl的数组
        //核心还是loopctrl数组，newWhileStmtPos每一次代表了一个新的循环开始，从当前的loopctrl.size()之后的break和continue语句都是属于这个while的
        //采用动态数组的方式进行控制是因为考虑到有while循环嵌套的问题
        whilenum++;
        new_WhileStmt_Pos.push_back(loop_ctrl.size());
    }   
     LPAREN Cond RPAREN Stmt {
        $$ = new WhileStmt($4, $6);
        int lastloop=new_WhileStmt_Pos[new_WhileStmt_Pos.size()-1];
        new_WhileStmt_Pos.erase(new_WhileStmt_Pos.end()-1);
        for(int i=lastloop;i<(int)loop_ctrl.size();i++)
        {
            loop_ctrl[i]->setWhileStmt(dynamic_cast<WhileStmt*>($$));
        }
        loop_ctrl.erase(loop_ctrl.begin()+lastloop,loop_ctrl.end());
        whilenum--;
    }
    ;
BrkStmt
    : BREAK SEMICOLON {
        $$ = new BreakStmt();
        loop_ctrl.push_back(dynamic_cast<loopctl*>($$));
        if(whilenum==0)
        {
            fprintf(stderr, "BrkStmt not in WhileStmt！\n");
            exit(EXIT_FAILURE);
        }

    }
    ;
CtnueStmt
    : CONTINUE SEMICOLON {
        $$ = new ContinueStmt();
        loop_ctrl.push_back(dynamic_cast<loopctl*>($$));
        if(whilenum==0)
        {
            fprintf(stderr, "CtnueStmt not in WhileStmt！\n");
            exit(EXIT_FAILURE); 
        }
    }
    ;
ExprStmt
    : Exp SEMICOLON {
        $$ = new ExprStmt($1);
    }
    ; 
Exp
    :AddExp {$$ = $1;}
    ;
Cond
    :
    LOrExp {$$ = $1;}
    ;
PrimaryExp
    :LPAREN LOrExp RPAREN { 
        $$ = $2;
    }
    | LVal {
        $$ = $1;
    }
    | INTEGER {
        SymbolEntry* se = new ConstantSymbolEntry(TypeSystem::intType, $1);    
        $$ = new Constant(se);
    }
    | FLOATNUMBER {
        SymbolEntry *se = new ConstantSymbolEntry(TypeSystem::floatType, $1); 
        $$ = new Constant(se);
    } 
    | FuncCall {$$=$1;}     //加(从exp那里移到这里)
    ;
UnaryExp 
    : PrimaryExp {$$ = $1;}
    | ADD UnaryExp {
        $$=$2;
    }
    | SUB UnaryExp {
        SymbolEntry* se;
        if($2->getSymPtr()->getType()->isIndirectFloat())
        {
            se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());
        }
        else if(($2->getSymPtr()->getType()->isIndirectInt())||($2->getSymPtr()->getType()->isBool()))
        {
            se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        }

        $$ = new UnaryExpr(se, UnaryExpr::SUB, $2);
    }
    | NOT UnaryExp {
        SymbolEntry* se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new UnaryExpr(se, UnaryExpr::NOT, $2);
    }
    ;
MulExp
    : UnaryExp {$$ = $1;}
    | MulExp MUL UnaryExp {
        SymbolEntry* se;
        se= new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::MUL, $1, $3);
    }
    | MulExp DIV UnaryExp {
        SymbolEntry* se;
        se= new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::DIV, $1, $3);
    }
    | MulExp MOD UnaryExp {
        SymbolEntry* se;
        se= new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::MOD, $1, $3);
    }
    ;
AddExp
    :
    MulExp {$$ = $1;}
    |
    AddExp ADD MulExp
    {
        SymbolEntry* se;
        se= new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::ADD, $1, $3);
    }
    |
    AddExp SUB MulExp
    {
        SymbolEntry* se;
        se= new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());     
        $$ = new BinaryExpr(se, BinaryExpr::SUB, $1, $3);
    }
    ;
RelExp
    : Exp {$$ = $1;}
    | RelExp LESS AddExp {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::LT, $1, $3);
    }
    | RelExp GREAT AddExp {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::GT, $1, $3);
    }
    | RelExp LE AddExp {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::LE, $1, $3);
    }
    | RelExp GE AddExp {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::GE, $1, $3);
    }
    ;
EqExp
    : RelExp {$$ = $1;}
    | EqExp EQ RelExp {
        SymbolEntry* se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::EQ, $1, $3);
    }
    | EqExp NE RelExp {
        SymbolEntry* se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::NE, $1, $3);
    }
    ;
LAndExp
    : EqExp {$$ = $1;}
    | LAndExp AND EqExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::AND, $1, $3);
    }
    ;
LOrExp
    : LAndExp {$$ = $1;}
    | LOrExp OR LAndExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::OR, $1, $3);
    }
    ;
ConstExp
    : AddExp {$$ = $1;}
    ;
Type
    : INT {
        tempType="INT";
        $$ = TypeSystem::intType;
    }
    | FLOAT {
        tempType="FLOAT";
        $$ = TypeSystem::floatType;
    }
    | VOID {
        $$ = TypeSystem::voidType;
    }
    ;
DeclStmt
    : ConstDecl SEMICOLON {$$=$1;}
    | VarDecl SEMICOLON {$$=$1;}
    ;
ConstDecl
    : CONST Type ConstDefList {$$ = $3;}
    ;
ConstDefList
    : ConstDefList COMM ConstDef {
        $$ = $1;
        $1->setNext($3);    //通过next指针将不同的节点串联起来，暂时只用于变量批量声明时，在同一级打印出各个声明语句
    }
    | ConstDef {$$ = $1;}
    ;
ConstDef
    : ID ASSIGN ConstInitVal {
        SymbolEntry *se;
        if(tempType=="INT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::intconstType, $1, identifiers->getLevel());
            dynamic_cast<IdentifierSymbolEntry*>(se)->setValue((int)($3->getValue()));
        }
        if(tempType=="FLOAT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::floatconstType, $1, identifiers->getLevel());
            dynamic_cast<IdentifierSymbolEntry*>(se)->setValue($3->getValue());
        }
        if(!identifiers->install1($1, se))
        {
            fprintf(stderr, "ConstID %s is already defined\n", (char*)$1);
            exit(EXIT_FAILURE);
        }
        $$ = new DeclStmt(new Id(se),$3);
        delete []$1;
    }
    | ID ArrayIndex ASSIGN {
        //nums中存储了从最高维开始每一维的长度，const int a[5][2][7]=>nums:5,2,7
        std::vector<int> nums;
        ExprNode* eachdimension = $2;
        while(eachdimension)
        {
            nums.push_back(eachdimension->getValue());
            eachdimension = (ExprNode*)(eachdimension->getNext());
        }
        Type* type;
        Type* temp1;
        if(tempType=="INT")  type = TypeSystem::intconstType;
        if(tempType=="FLOAT") type = TypeSystem::floatconstType;
        //形成了一个ArrayType的链，互相之间以artype和parent保持联系
        for(auto it = nums.rbegin(); it != nums.rend(); it++) 
        {
            temp1 = new ArrayType(type, *it);
            if(type->isArray())
            {
                ((ArrayType*)type)->setParentType(temp1);
            }
            type = temp1;
        }
        //arraytype，当前的最顶层的ArrayType
        arraytype = (ArrayType*)type;

        myindex = 0;
        std::stack<Initlist*>().swap(initstack);
        SymbolEntry* se = new IdentifierSymbolEntry(arraytype, $1, identifiers->getLevel());
        $<se>$ = se;

        //这里的size是已经计算了整型或者浮点的位数了，const int a[5]=>equalintarray=5*32=160;
        int len_of_array=arraytype->getArraySize()/32;
        if(tempType=="INT")  equalintarray =new int[len_of_array];
        if(tempType=="FLOAT") equalfloatarray =new float[len_of_array];
    }
    ConstInitVal{
        if(tempType=="INT")   ((IdentifierSymbolEntry*)$<se>4)->setIntArrayValue(equalintarray);
        if(tempType=="FLOAT")  ((IdentifierSymbolEntry*)$<se>4)->setFloatArrayValue(equalfloatarray);  
        identifiers->install($1, $<se>4);
        $$ = new DeclStmt(new Id($<se>4,$2), $5);
        delete []$1;
    }
    ;
ConstInitVal
    : ConstExp {
        $$ = $1;
        //initstack为空，是声明一个常量，不为空，是声明一个常量数组
        if(!initstack.empty())
        {
            if(tempType=="INT")
            {
                equalintarray[myindex++] =(int)($1->getValue());
                Type* artp = initstack.top()->getSymPtr()->getType();
                if(artp == TypeSystem::intconstType)
                {
                    initstack.top()->AddExp($1);
                }
                else
                {
                    while(artp)
                    {
                        if(((ArrayType*)artp)->getArrayType() != TypeSystem::intconstType)
                        {
                            //int a[2][4]={1,2,3,4,5,6,7,8}
                            artp = ((ArrayType*)artp)->getArrayType();
                            SymbolEntry* se = new ConstantSymbolEntry(artp);
                            Initlist* initlist = new Initlist(se);
                            initstack.top()->AddExp(initlist);
                            initstack.push(initlist);
                        }
                        else
                        {
                            initstack.top()->AddExp($1);
                            while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                            {
                                artp = ((ArrayType*)artp)->getParentType();
                                initstack.pop();
                            }
                            break;
                        }
                    }
                }    
            } 
            if(tempType=="FLOAT")
            {
                equalfloatarray[myindex++] =$1->getValue();
                Type* artp = initstack.top()->getSymPtr()->getType();
                if(artp == TypeSystem::floatconstType)
                {
                    initstack.top()->AddExp($1);
                }
                else
                {
                    while(artp)
                    {
                        if(((ArrayType*)artp)->getArrayType() != TypeSystem::floatconstType)
                        {
                            artp = ((ArrayType*)artp)->getArrayType();
                            SymbolEntry* se = new ConstantSymbolEntry(artp);
                            Initlist* initlist = new Initlist(se);
                            initstack.top()->AddExp(initlist);
                            initstack.push(initlist);
                        }
                        else
                        {
                            initstack.top()->AddExp($1);
                            while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                            {
                                artp = ((ArrayType*)artp)->getParentType();
                                initstack.pop();
                            }
                            break;
                        }
                    }
                }
            } 
        }   
    }
    | LBRACE RBRACE {
        if(tempType=="INT")
        {
            SymbolEntry* se;
            ExprNode* initlist;
            if(initstack.empty())
            {
                // 如果只用一个{}初始化数组，那么栈一定为空
                // 此时也没必要再加入栈了
                int num=arraytype->getArraySize()/32;
                memset(equalintarray, 0, num*4);
                myindex += num;
                se = new ConstantSymbolEntry(arraytype);
                initlist = new Initlist(se);
            }
            else
            {
                // 栈不空说明肯定不是只有{}
                // 此时需要确定{}到底占了几个元素
                int size = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArraySize()/32;
                memset(equalintarray + myindex, 0, size*4);
                myindex += size;
                Type* type = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType();
                se = new ConstantSymbolEntry(type);
                initlist = new Initlist(se);
                initstack.top()->AddExp(initlist);
                while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                {
                    initstack.pop();
                }
            }
            $$ = initlist;
        }

        if(tempType=="FLOAT"){
            SymbolEntry* se;
            ExprNode* initlist;
            if(initstack.empty()){
                // 如果只用一个{}初始化数组，那么栈一定为空
                // 此时也没必要再加入栈了
                memset(equalfloatarray, 0, arraytype->getArraySize());
                myindex += arraytype->getArraySize()/32;
                se = new ConstantSymbolEntry(arraytype);
                initlist = new Initlist(se);
            }else{
                // 栈不空说明肯定不是只有{}
                // 此时需要确定{}到底占了几个元素
                int size = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArraySize();
                int len = size / ((FloatType *)TypeSystem::floatconstType)->getSize();
                memset(equalfloatarray + myindex, 0, size);
                myindex += len;
                Type* type = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType();
                se = new ConstantSymbolEntry(type);
                initlist = new Initlist(se);
                initstack.top()->AddExp(initlist);
                while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum){
                    initstack.pop();
                }
            }
            $$ = initlist;
        }
        
    }
    | LBRACE {
        //bracenum是用来确认当前是第几层大括号
        bracenum++;
        //第一层的时候，这个栈必定是空的
        if(!initstack.empty())
        {
            arraytype = (ArrayType*)(((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType());
        }
        //如果不经过上面的分支，即第一次进入这里，arraytype还是当前最顶层的数组类型
        SymbolEntry* se= new ConstantSymbolEntry(arraytype);
        Initlist* expr = new Initlist(se);
        if(!initstack.empty())
            initstack.top()->AddExp(expr);
        initstack.push(expr);
        $<exprtype>$ = expr;
    } 
      ConstInitValList RBRACE {
        bracenum--;
        while(initstack.top() != $<exprtype>2 && initstack.size() > (long unsigned int)(bracenum + 1))
        {
            initstack.pop();
        }
        if(initstack.top() == $<exprtype>2)
        {
            initstack.pop();
        }
        $$ = $<exprtype>2;
        if(!initstack.empty())
        {
            while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
            {
                initstack.pop();
            }
        }
        
        if(tempType=="INT")
        {
            int size=((ArrayType*)($$->getSymPtr()->getType()))->getArraySize();
            while(myindex % (size/32) !=0 )
                equalintarray[myindex++] = 0;
        }
        if(tempType=="FLOAT")
        {
            while(myindex % (((ArrayType*)($$->getSymPtr()->getType()))->getArraySize()) !=0 )
                equalfloatarray[myindex++] = 0.0;
        }
        if(!initstack.empty())
            arraytype = (ArrayType*)(((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType());
    }
    ;
ConstInitValList
    : ConstInitVal {
        $$ = $1;
    }
    | ConstInitValList COMM ConstInitVal {
        $$ = $1;
    }
    ;
ArrayIndex
    : LBRACK ConstExp RBRACK {
        $$ = $2;
    }
    | ArrayIndex LBRACK ConstExp RBRACK {
        $$ = $1;
        $1->setNext($3);//把每个维度串起来
    }
    ;
VarDecl
    : Type VarDefList {$$ = $2;}
    ;
VarDefList 
    : VarDefList COMM VarDef {
        $$ = $1;
        $1->setNext($3);
    }
    | VarDef { $$ = $1;}
    ;
VarDef
    : ID {
        SymbolEntry *se;
        if(tempType=="INT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::intType, $1, identifiers->getLevel());
        }
         if(tempType=="FLOAT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::floatType, $1, identifiers->getLevel());
        }   
        if(!identifiers->install1($1, se))
        {
            fprintf(stderr, "VarID %s is already defined\n", (char*)$1);
            exit(EXIT_FAILURE);
        }
        $$ = new DeclStmt(new Id(se));
        delete []$1;
    }
    | ID ASSIGN VarInitVal {
        SymbolEntry *se;
        if(tempType=="INT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::intType, $1, identifiers->getLevel());
            dynamic_cast<IdentifierSymbolEntry*>(se)->setValue((int)($3->getValue()));
        }
        if(tempType=="FLOAT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::floatType, $1, identifiers->getLevel());
            dynamic_cast<IdentifierSymbolEntry*>(se)->setValue($3->getValue());   
        }
        if(!identifiers->install1($1, se))
        {
            fprintf(stderr, "VarID %s is already defined\n", (char*)$1);
            exit(EXIT_FAILURE);
        }
        $$ = new DeclStmt(new Id(se),$3);
        delete []$1;
    }
    | ID ArrayIndex {
        std::vector<int> nums;//存储各维大小
        ExprNode* eachdimension = $2;
        while(eachdimension)
        {
            nums.push_back(eachdimension->getValue());
            eachdimension = (ExprNode*)(eachdimension->getNext());
        }
        Type *type;
        Type* temp1;
        if(tempType=="INT")
        {
            type = TypeSystem::intType;
        }  
        if(tempType=="FLOAT") 
        {
             type = TypeSystem::floatType;
        }
        for(auto it = nums.rbegin(); it != nums.rend(); it++) 
        {
            temp1 = new ArrayType(type, *it);
            if(type->isArray())
            {
                ((ArrayType*)type)->setParentType(temp1);
            }
            type = temp1;
        }
        arraytype = (ArrayType*)type;
        SymbolEntry* se = new IdentifierSymbolEntry(arraytype, $1, identifiers->getLevel());
        ((IdentifierSymbolEntry*)se)->setquan0();
        identifiers->install($1, se);
        $$ = new DeclStmt(new Id(se,$2));
        delete []$1;

    }
    | ID ArrayIndex ASSIGN {
        std::vector<int> nums;
        ExprNode* eachdimension = $2;
        while(eachdimension)
        {
            nums.push_back(eachdimension->getValue());
            eachdimension = (ExprNode*)(eachdimension->getNext());
        }
        Type* type;
        Type* temp1;
        //确定基础类型是int还是float
        if(tempType=="INT") type = TypeSystem::intType; 
        if(tempType=="FLOAT") type = TypeSystem::floatType;

        for(auto it = nums.rbegin(); it != nums.rend(); it++) 
        {
            temp1 = new ArrayType(type, *it);
            if(type->isArray())
            {
                ((ArrayType*)type)->setParentType(temp1);
            }
            type = temp1;
        }
        arraytype = (ArrayType*)type;
        myindex = 0;
        std::stack<Initlist*>().swap(initstack);
        SymbolEntry* se = new IdentifierSymbolEntry(arraytype, $1, identifiers->getLevel());
        $<se>$ = se;
        int len_of_array=arraytype->getArraySize()/32;
        if(tempType=="INT")  equalintarray =new int[len_of_array];
        if(tempType=="FLOAT") equalfloatarray =new float[len_of_array];
        
    }
      VarInitVal{ 
        if(tempType=="INT")
        {
            ((IdentifierSymbolEntry*)$<se>4)->setIntArrayValue(equalintarray);
        }
        if(tempType=="FLOAT")
        {
            ((IdentifierSymbolEntry*)$<se>4)->setFloatArrayValue(equalfloatarray);  
        }  
        if(((Initlist*)$5)->ifempty())
            ((IdentifierSymbolEntry*)$<se>4)->setquan0();
        identifiers->install($1, $<se>4);
        $$ = new DeclStmt(new Id($<se>4,$2), $5);
        delete []$1;
    }
    ;
VarInitVal 
    : Exp {  
        $$ = $1;
        //initstack栈不为空，说明当前是一个数组
        if(!initstack.empty())
        {
            if(tempType=="INT")
            {
                equalintarray[myindex++] =(int)($1->getValue());
                Type* artp = initstack.top()->getSymPtr()->getType();
                while(artp)
                {
                    if(((ArrayType*)artp)->getArrayType() != TypeSystem::intType)
                    {
                        artp = ((ArrayType*)artp)->getArrayType();
                        SymbolEntry* se = new ConstantSymbolEntry(artp);
                        Initlist* initlist = new Initlist(se);
                        initstack.top()->AddExp(initlist);
                        initstack.push(initlist);
                    }
                    else
                    {
                        initstack.top()->AddExp($1);
                        while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                        {
                            artp = ((ArrayType*)artp)->getParentType();
                            initstack.pop();
                        }
                        break;
                    }
                }    
            } 

            if(tempType=="FLOAT"){
                equalfloatarray[myindex++] =$1->getValue();
                Type* artp = initstack.top()->getSymPtr()->getType();
                if(artp == TypeSystem::floatType)
                {
                    initstack.top()->AddExp($1);
                }
                else
                {
                    while(artp)
                    {
                        if(((ArrayType*)artp)->getArrayType() != TypeSystem::floatType)
                        {
                            artp = ((ArrayType*)artp)->getArrayType();
                            SymbolEntry* se = new ConstantSymbolEntry(artp);
                            Initlist* initlist = new Initlist(se);
                            initstack.top()->AddExp(initlist);
                            initstack.push(initlist);
                        }
                        else
                        {
                            initstack.top()->AddExp($1);
                            while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                            {
                                artp = ((ArrayType*)artp)->getParentType();
                                initstack.pop();
                            }
                            break;
                        }
                    }
                }
                    
            } 
        }       
    }
    | LBRACE RBRACE {
        if(tempType=="INT")
        { 
            SymbolEntry* se;
            ExprNode* initlist;
            if(initstack.empty())
            {
                // 如果只用一个{}初始化数组，那么栈一定为空
                // 此时也没必要再加入栈了
                int num=arraytype->getArraySize()/32;
                memset(equalintarray, 0, num*4);
                myindex += num;
                se = new ConstantSymbolEntry(arraytype);
                initlist = new Initlist(se);
            }
            else
            {
                // 栈不空说明肯定不是只有{}
                // 此时需要确定{}到底占了几个元素
                int size = ((ArrayType*)(((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType()))->getArraySize()/32;
                memset(equalintarray + myindex, 0, size*4);
                myindex += size;
                Type* type = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType();
                se = new ConstantSymbolEntry(type);
                initlist = new Initlist(se);
                initstack.top()->AddExp(initlist);
                while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                {
                    initstack.pop();
                }
            }
            $$ = initlist;
        }

        if(tempType=="FLOAT"){
            SymbolEntry* se;
            ExprNode* initlist;
            if(initstack.empty())
            {
                // 如果只用一个{}初始化数组，那么栈一定为空
                // 此时也没必要再加入栈了
                memset(equalfloatarray, 0, arraytype->getArraySize());
                myindex += arraytype->getArraySize()/32 ;
                se = new ConstantSymbolEntry(arraytype);
                initlist = new Initlist(se);
            }
            else
            {
                // 栈不空说明肯定不是只有{}
                // 此时需要确定{}到底占了几个元素
                int size = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArraySize();
                int len = size/32 ;
                memset(equalfloatarray + myindex, 0, size);
                myindex += len;
                Type* type = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType();
                se = new ConstantSymbolEntry(type);
                initlist = new Initlist(se);
                initstack.top()->AddExp(initlist);
                while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                {
                    initstack.pop();
                }
            }
            $$ = initlist;
        }
        
    }
    | LBRACE {
        bracenum++;
        SymbolEntry* se;
        if(!initstack.empty())
        {
            arraytype = (ArrayType*)(((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType());
        }
        se = new ConstantSymbolEntry(arraytype);
        Initlist* expr = new Initlist(se);
        if(!initstack.empty())
        {
            initstack.top()->AddExp(expr);
        }
        initstack.push(expr);
        $<exprtype>$ = expr;
        
    } 
      VarInitValList RBRACE {
        bracenum--;
        while(initstack.top() != $<exprtype>2 && initstack.size() > (long unsigned int)(bracenum + 1))
        {
            initstack.pop();
        }
        if(initstack.top() == $<exprtype>2)
        {
            initstack.pop();
        }
        $$ = $<exprtype>2;
        if(!initstack.empty())
        {
            while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
            {
                initstack.pop();
            }
        }
        if(tempType=="INT")
        {
            int size=((ArrayType*)($$->getSymPtr()->getType()))->getArraySize();
            while(myindex % (size/32) !=0 )
            {
                equalintarray[myindex++] = 0;
            }
        }
        if(tempType=="FLOAT")
        {
            while(myindex % (((ArrayType*)($$->getSymPtr()->getType()))->getArraySize() !=0 ))
            {
                equalfloatarray[myindex++] = 0.0;
            }
        }
        
        if(!initstack.empty())
        {
            arraytype = (ArrayType*)(((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType());
        }
        
    }
    ;


VarInitValList
    : VarInitVal {
        $$ = $1;
    }
    | VarInitValList COMM VarInitVal {
        $$ = $1;
    }
    ;

FuncDef 
    :Type ID {
        identifiers = new SymbolTable(identifiers);
        intParam_num_for_func=0;
        floatParam_num_for_func=0;
        functionhasRet=false;
    }
    LPAREN FuncParams RPAREN
    {
        FunctionType* functype=new FunctionType($1,{}); 
        if($5)
        {
            std::vector<Type*> paramType;
            std::vector<SymbolEntry*> params;
            DeclStmt* tmp=dynamic_cast<DeclStmt*>($5);
            
            while(tmp)
            {
                params.push_back(dynamic_cast<IdentifierSymbolEntry*>(tmp->getID()->getSymPtr()));
                paramType.push_back(dynamic_cast<IdentifierSymbolEntry*>(tmp->getID()->getSymPtr())->getType());
                tmp=dynamic_cast<DeclStmt*>(tmp->getNext());
            }
            functype->setparams(params);
            functype->setparamsType(paramType);
            functype->setIntparamCount(intParam_num_for_func);
            functype->setFloatparamCount(floatParam_num_for_func);
        }   
        SymbolEntry *se = new IdentifierSymbolEntry(functype, $2, identifiers->getPrev()->getLevel());
        if(!identifiers->getPrev()->install2($2, se))
        {
            fprintf(stderr, "func %s is already defined\n", (char*)$2);
            exit(EXIT_FAILURE);
        }    
    }
    BlockStmt
    {
        SymbolEntry *se;
        se = identifiers->lookup($2);
        assert(se != nullptr);
        if(($1==TypeSystem::voidType)&&(!functionhasRet))
        {
            auto tmpnode=new SeqNode($8,new ReturnStmt());
            $8=tmpnode;
        }
        FunctionDef* newFunc=new FunctionDef(se,$5, $8);
        $$ = newFunc;
        //恢复原来的作用域，和函数本身关系不大
        SymbolTable *top = identifiers;
        identifiers = identifiers->getPrev();
        delete top;
        delete []$2;
    }
    ;

FuncParams
    : FuncParams COMM FuncParam {
        $$ = $1;
        $1->setNext($3);        //设置参数的next
    }
    | FuncParam {
        $$ = $1;
    }
    | %empty { $$=nullptr;}     //epsilon
    ;
FuncParam
    : Type ID {
        SymbolEntry *se;
        if(tempType=="INT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::intType, $2, identifiers->getLevel(),intParam_num_for_func++);
        }
        if(tempType=="FLOAT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::floatType, $2, identifiers->getLevel(),floatParam_num_for_func++);
        }
        //将这一符号表项存入当前作用域的符号表中
        identifiers->install($2, se);
        dynamic_cast<IdentifierSymbolEntry*>(se)->setLabel(SymbolTable::getLabel());
        dynamic_cast<IdentifierSymbolEntry*>(se)->setAddr(new Operand(se));
        $$ = new DeclStmt(new Id(se));
        delete []$2;    
    }
    | Type ID FunArrayIndex{
        if(tempType=="FLOAT")  
        {
            SymbolEntry* se;
            ExprNode* temp = $3;
            Type* arr = TypeSystem::floatType;
            Type* arr1;
            std::stack<ExprNode*> stk;
            while(temp)
            {
                stk.push(temp);
                temp = (ExprNode*)(temp->getNext());
            }
            while(!stk.empty())
            {
                arr1 = new ArrayType(arr, stk.top()->getValue());
                if(arr->isArray())
                    ((ArrayType*)arr)->setParentType(arr1);
                arr = arr1;
                stk.pop();
            }
            se = new IdentifierSymbolEntry(arr, $2, identifiers->getLevel(),intParam_num_for_func++);
            identifiers->install($2, se);
            ((IdentifierSymbolEntry*)se)->setLabel(SymbolTable::getLabel());
            ((IdentifierSymbolEntry*)se)->setAddr(new Operand(se));
            $$ = new DeclStmt(new Id(se));
            delete []$2;
        }
        else
        {
            SymbolEntry* se;
            ExprNode* temp = $3;
            Type* arr = TypeSystem::intType;
            Type* arr1;
            std::stack<ExprNode*> stk;
            while(temp)
            {
                stk.push(temp);
                temp = (ExprNode*)(temp->getNext());
            }
            while(!stk.empty())
            {
                arr1 = new ArrayType(arr, stk.top()->getValue());
                if(arr->isArray())
                    ((ArrayType*)arr)->setParentType(arr1);
                arr = arr1;
                stk.pop();
            }
            se = new IdentifierSymbolEntry(arr, $2, identifiers->getLevel(),intParam_num_for_func++);
            identifiers->install($2, se);
            ((IdentifierSymbolEntry*)se)->setLabel(SymbolTable::getLabel());
            ((IdentifierSymbolEntry*)se)->setAddr(new Operand(se));
            $$ = new DeclStmt(new Id(se));
            delete []$2;
        }
        
    }
    ;
FunArrayIndex 
    : LBRACK RBRACK {
        $$ = new ExprNode(nullptr);
    }
    | FunArrayIndex  LBRACK Exp RBRACK {
        $$ = $1;
        $$->setNext($3);
    }
    ;
FuncCall
    : ID LPAREN ParaList RPAREN {
        //要做的是，新建一个函数调用节点，需要包含的信息是函数名的符号表项，以及各个参数的符号表项
        SymbolEntry *se;
        se = identifiers->lookup($1);
        //函数没有被声明过
        if(se == nullptr)
        {
            fprintf(stderr, "function %s is undeclared\n", (char*)$1);
            exit(EXIT_FAILURE);
        }
        $$=new FunctionCall(se,$3);
    }
    ;
ParaList
    : ParaList COMM Exp {
        $$ = $1;
        $1->setNext($3);        //设置next
    }
    | Exp { $$=$1;}
    | %empty { $$=nullptr;}
    ;

%%

int yyerror(char const* message)
{
    std::cerr<<message<<std::endl;
    return -1;
}
