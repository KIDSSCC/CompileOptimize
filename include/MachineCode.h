#ifndef __MACHINECODE_H__
#define __MACHINECODE_H__
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <fstream>
#include<iostream>
#include "SymbolTable.h"

/* Hint:
* MachineUnit: Compiler unit
* MachineFunction: Function in assembly code
* MachineBlock: current machine code block;   
* MachineInstruction: Single assembly instruction
* MachineOperand: Operand in assembly instruction, such as immediate number, (v)register, address label */

/* Todo:
* We only give the example code of "class BinaryMInstruction" and "class AccessMInstruction" (because we believe in you !!!),
* You need to complete other the member function, especially "output()" ,
* After that, you can use "output()" to print assembly code . */

/*
编译单元
编译函数
编译基本块
编译指令
*/
class MachineUnit;
class MachineFunction;
class MachineBlock;
class MachineInstruction;



class MachineOperand
{
private:
    MachineInstruction* parent;
    int type;
    int val;  // value of immediate number 
    unsigned long fval;//存储浮点数的IEEE754格式
    int reg_no; // register no
    std::string label; // address label
    bool if_float=false;
public:
    /*
    立即数，虚拟寄存器，物理寄存器，地址标签
    地址标签：主要为BranchMInstruction以及LoadMInstruction的操作数
    两种构造函数，根据类型的不同，val可能是传入的是立即数的值，也可能是寄存器的编号
    重载了==，当两个操作数是立即数或者寄存器时可以进行比较
    重载了<,此处存疑，目前和==一样，只比较了立即数和寄存器
    一系列get&set
    PrintReg：11-15号为特殊寄存器
    output：操作数的打印
    */
    enum { IMM, VREG, REG, LABEL };//四种操作数类型
    MachineOperand(int tp, int val);//tp为IMM, VREG, REG的时候
    MachineOperand(std::string label);//tp为LABEL的时候
    MachineOperand(int tp,unsigned long fval,bool if_float);
    bool operator == (const MachineOperand&) const;//两个操作数（IMM, VREG, REG）是否相等
    bool operator < (const MachineOperand&) const;//两个操作数（IMM, VREG, REG）大小关系

    bool isImm() { return this->type == IMM; };
    bool isReg() { return this->type == REG; };
    bool isVReg() { return this->type == VREG; };
    bool isLabel() { return this->type == LABEL; };
    int getVal() {return this->val; };
    unsigned int getFval(){return this->fval;};
    void setVal(int value){this->type = IMM; this->val = value; };//加
    int getReg() {return this->reg_no; };
    void setReg(int regno) {this->type = REG; this->reg_no = regno;};
    std::string getLabel() {return this->label; };
    void setParent(MachineInstruction* p) { this->parent = p; };
    MachineInstruction* getParent() { return this->parent;};
    void PrintReg(bool if_float);
    void output();
    bool ifFloat(){return if_float;};
};

class MachineInstruction
{
protected:
    //应该是机器指令的基类
    /*
    所属的基本块
    序号
    指令类型
    一个可选的指令条件
    操作码
    按照操作数出现顺序排列的操作数列表def_list，use_list，以及其对应的add
    关于条件指令的打印函数
    指令类型暂时完成了二元运算，load，store，mov，分支指令，比较指令，STACK存疑
    条件语句的类型暂时完成了等于，不等，小于，小于等于，大于，大于等于，以及一个默认的空
    */
    MachineBlock* parent;
    int no;
    int type;  // Instruction type
    int cond = MachineInstruction::NONE;  // Instruction execution condition, optional !!
    int op;  // Instruction opcode
    // Instruction operand list, sorted by appearance order in assembly instruction
    std::vector<MachineOperand*> def_list;
    std::vector<MachineOperand*> use_list;
    void addDef(MachineOperand* ope) { def_list.push_back(ope); };
    void addUse(MachineOperand* ope) { use_list.push_back(ope); };
    // Print execution code after printing opcode
    void PrintCond();
public:
    /*
    output留作具体的指令去实现
    no的get&set
    def_list与use_list的get
    */
    enum instType { BINARY, LOAD, STORE, MOV, BRANCH, CMP, STACK,NEONCMP,NEONVCVT,NEONMOV};//挪这里
    enum condType { EQ, NE, LT, GE , GT, LE, NONE,DEBUG};//这个地方WA debug
    virtual void output() = 0;
    void setNo(int no) {this->no = no;};
    int getNo() {return no;};
    std::vector<MachineOperand*>& getDef() {return def_list;};
    std::vector<MachineOperand*>& getUse() {return use_list;};
    //以下新加
    MachineBlock* getParent() const {return parent;};

    void insertBefore(MachineInstruction*);
    void insertAfter(MachineInstruction*);
    int getinstType(){return type;}
    int getop(){return op;}
    void setCond(int cond){this->cond=cond;};
};

class BinaryMInstruction : public MachineInstruction
{
public:
    /*
    二元运算指令，加减乘除与或
    */
    enum opType { ADD, SUB, MUL, DIV, MOD,AND, OR ,VADD,VSUB,VMUL,VDIV};
    BinaryMInstruction(MachineBlock* p, int op, MachineOperand* dst, MachineOperand* src1, MachineOperand* src2, int cond = MachineInstruction::NONE);
    void output();
};


class LoadMInstruction : public MachineInstruction
{
public:
    //加载指令
    enum optype{LDR,VLDR};
    LoadMInstruction(int op,MachineBlock* p,MachineOperand* dst, MachineOperand* src1, MachineOperand* src2 = nullptr, int cond = MachineInstruction::NONE);
    void output();
};

class StoreMInstruction : public MachineInstruction
{
public:
    //存储指令
    enum opType{STR,VSTR};
    StoreMInstruction( int op,MachineBlock* p,MachineOperand* src1, MachineOperand* src2, MachineOperand* src3 = nullptr, int cond = MachineInstruction::NONE);
    void output();
};

class MovMInstruction : public MachineInstruction
{
public:
    /*
    movm指令，暂时存疑
    */
    enum opType { MOV,VMOV};
    MovMInstruction(MachineBlock* p, int op, MachineOperand* dst, MachineOperand* src,int cond = MachineInstruction::NONE);
    void output();
};

class BranchMInstruction : public MachineInstruction
{
public:
    /*
    分支指令
    关于操作码的类型暂时存疑
    */
    enum opType { B, BL, BX };
    BranchMInstruction(MachineBlock* p, int op, 
                MachineOperand* dst, 
                int cond = MachineInstruction::NONE);
    void output();
};

class CmpMInstruction : public MachineInstruction
{
public:
    /*
    比较指令
    */
    enum opType { CMP,VCMP,VCMPE };
    CmpMInstruction(int op,MachineBlock* p, 
                MachineOperand* src1, MachineOperand* src2, 
                int cond = MachineInstruction::NONE);
    void output();
};

class StackMInstrcuton : public MachineInstruction
{
public:
    /*
    压栈与弹栈的操作
    */
    enum opType { PUSH, POP,VPUSH,VPOP};
    StackMInstrcuton(MachineBlock* p, int op, std::vector<MachineOperand*> srcs,MachineOperand* dst=nullptr,MachineOperand* src = nullptr,int cond = MachineInstruction::NONE);
    void output();
};

class NeonVcvtInstruction: public MachineInstruction
{
public:
    enum{IntToFloat,FloatToInt};
    NeonVcvtInstruction(MachineBlock* p,int op,MachineOperand*dst,MachineOperand*src,int cond=MachineInstruction::NONE);
    void output();
};

class MachineBlock
{
private:
    /*
    基本块，包含所属的函数单元
    no：一个序号，代表当前基本块的序号
    前驱块与后继块
    指令列表
    live_in&live_out：两个操作数集合，暂时存疑
    */
    MachineFunction* parent;
    int no;  
    std::vector<MachineBlock *> pred, succ;
    std::vector<MachineInstruction*> inst_list;
    std::set<MachineOperand*> live_in;//???
    std::set<MachineOperand*> live_out;
    static int blockseparate;
    int cmpCond;
public:
    /*
    关于指令列表的迭代
    构造函数，包含一个所属的函数以及初始化序号
    指令的插入
    前驱与后继的get&set
    两个操作数集合的设置
    */
    std::vector<MachineInstruction*>& getInsts() {return inst_list;};
    std::vector<MachineInstruction*>::iterator begin() { return inst_list.begin(); };
    std::vector<MachineInstruction*>::iterator end() { return inst_list.end(); };
    MachineBlock(MachineFunction* p, int no) { this->parent = p; this->no = no; };
    void InsertInst(MachineInstruction* inst) { this->inst_list.push_back(inst); };
    void addPred(MachineBlock* p) { this->pred.push_back(p); };
    void addSucc(MachineBlock* s) { this->succ.push_back(s); };
    std::set<MachineOperand*>& getLiveIn() {return live_in;};
    std::set<MachineOperand*>& getLiveOut() {return live_out;};
    std::vector<MachineBlock*>& getPreds() {return pred;};
    std::vector<MachineBlock*>& getSuccs() {return succ;};
    void output();
    //以下新加
    MachineFunction* getParent() const {return parent;};
    int getSize() const {return inst_list.size();};
    std::vector<MachineInstruction*>::reverse_iterator rbegin() {return inst_list.rbegin();};
    int getCmpCond() const { return cmpCond; };
    void setCmpCond(int cond) { cmpCond = cond; };

};

class MachineFunction
{
private:
    /*
    函数，包含其所属的编译单元，组成函数的若干基本块，
    stack_size：栈的大小，暂时存疑
    saved_regs：set容器，
    sym_ptr：符号表项，应该是代表函数的信息
    */
    MachineUnit* parent;
    std::vector<MachineBlock*> block_list;
    int stack_size;
    std::set<int> saved_regs;
    std::set<int> saved_sregs;
    SymbolEntry* sym_ptr;
    int paramsNum;//新加
public:
    std::vector<MachineBlock*>& getBlocks() {return block_list;};
    std::vector<MachineBlock*>::iterator begin() { return block_list.begin(); };
    std::vector<MachineBlock*>::iterator end() { return block_list.end(); };
    MachineFunction(MachineUnit* p, SymbolEntry* sym_ptr);
    /* HINT:
    * Alloc stack space for local variable;
    * return current frame offset ;
    * we store offset in symbol entry of this variable in function AllocInstruction::genMachineCode()
    * you can use this function in LinearScan::genSpillCode() */
    int AllocSpace(int size) { this->stack_size += size; return this->stack_size; };
    void InsertBlock(MachineBlock* block) { this->block_list.push_back(block); };
    void addSavedRegs(int regno) 
    {
        if(regno<=16)
        {
            saved_regs.insert(regno);
            if(regno<=12&&regno%2!=0)
            {
                saved_regs.insert(regno + 1);
            }
            else if(regno<12&&regno>0&&regno%2==0)
            {
                saved_regs.insert(regno - 1);
            }
        }
        else
        {
            saved_sregs.insert(regno);
        }
    };
    void output();
    //以下新加
    MachineUnit* getParent() const {return parent;};
    std::vector<MachineOperand*> getSavedRegs();
    std::vector<MachineOperand*> getSavedSRegs();
    int getParamsNum() const { return paramsNum; };
    SymbolEntry* getSym_ptr(){return sym_ptr;};
};

class MachineUnit
{
private:
    /*
    编译单元由函数构成
    与中间代码生成类似，需要提前管理全局变量声明
    此处存疑，应该还要管理io函数
    */
    std::vector<MachineFunction*> func_list;
    void PrintGlobalDecl();
    int n;//新加
public:
    std::vector<SymbolEntry*> global_id_list;//新加
    std::vector<MachineFunction*>& getFuncs() {return func_list;};
    std::vector<MachineFunction*>::iterator begin() { return func_list.begin(); };
    std::vector<MachineFunction*>::iterator end() { return func_list.end(); };
    void InsertFunc(MachineFunction* func) { func_list.push_back(func);};
    void output();
    //以下新加
    int getN() const { return n; };
    void printGlobal();
};

#endif