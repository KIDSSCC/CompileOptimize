#include "Instruction.h"
#include "BasicBlock.h"
#include <iostream>
#include "Function.h"
#include "Type.h"
extern FILE* yyout;

Instruction::Instruction(unsigned instType, BasicBlock *insert_bb)
{
    //上下指令都指向自己（在插入到基本块中的时候会进行调整）
    prev = next = this;
    //操作码是-1
    opcode = -1;
    this->instType = instType;
    if (insert_bb != nullptr)
    {
        insert_bb->insertBack(this);
        parent = insert_bb;
    }
}

Instruction::~Instruction()
{
    //把这条指令从基本块中除去
    parent->remove(this);
}

BasicBlock *Instruction::getParent()
{
    //获取所属的基本块
    return parent;
}

void Instruction::setParent(BasicBlock *bb)
{
    //设置所属的基本块（因为在构造的时候可能没有直接给出）
    parent = bb;
}

void Instruction::setNext(Instruction *inst)
{
    //设置下一条指令
    next = inst;
}

void Instruction::setPrev(Instruction *inst)
{
    //设置上一条指令
    prev = inst;
}

Instruction *Instruction::getNext()
{
    //获取下一条指令
    return next;
}

Instruction *Instruction::getPrev()
{
    //获取上一条指令
    return prev;
}

BinaryInstruction::BinaryInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb) : Instruction(BINARY, insert_bb)
{
    this->opcode = opcode;
    operands.push_back(dst);
    operands.push_back(src1);
    operands.push_back(src2);
    dst->setDef(this);
    src1->addUse(this);
    src2->addUse(this);
}

BinaryInstruction::~BinaryInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void BinaryInstruction::output() const
{
    std::string s1, s2, s3, op, type;
    s1 = operands[0]->toStr();
    s2 = operands[1]->toStr();
    s3 = operands[2]->toStr();
    type = operands[0]->getType()->toStr();
    switch (opcode)
    {
    case ADD:
        if(type == "float") op = "fadd";
        else op = "add";
        break;
    case SUB:
        if(type == "float") op = "fsub";
        else op = "sub";
        break;
    case MUL:
        if(type == "float") op = "fmul";
        else op="mul";
        break;
    case DIV:
        if(type == "float") op = "fdiv";
        else op="sdiv";
        break;
    case MOD:
        op="srem";
        break;
    case AND:
        op = "and";     //新加
            break;
    case OR:
        op = "or";     //新加 
    default:
        break;
    }
    
    fprintf(yyout, "  %s = %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(), s2.c_str(), s3.c_str());
}

CmpInstruction::CmpInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb): Instruction(CMP, insert_bb){
    this->opcode = opcode;
    operands.push_back(dst);
    operands.push_back(src1);
    operands.push_back(src2);
    dst->setDef(this);
    src1->addUse(this);
    src2->addUse(this);
}

CmpInstruction::~CmpInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void CmpInstruction::output() const
{
    std::string s1, s2, s3, op, type;
    s1 = operands[0]->toStr();
    s2 = operands[1]->toStr();
    s3 = operands[2]->toStr();
    type = operands[1]->getType()->toStr();
    switch (opcode)
    {
    case E:
        op = "eq";
        break;
    case NE:
        op = "ne";
        break;
    case L:
        if(operands[1]->getType()->isFloat()) op="olt";
        else op = "slt";
        break;
    case LE:
        if(operands[1]->getType()->isFloat()) op="ole";
        else op = "sle";
        break;
    case G:
        if(operands[1]->getType()->isFloat()) op="ogt";
        else op = "sgt";
        break;
    case GE:
        op = "sge";
        break;
    default:
        op = "";
        break;
    }
    if(operands[1]->getType()->isFloat())   fprintf(yyout, "  %s = fcmp %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(), s2.c_str(), s3.c_str());
    else fprintf(yyout, "  %s = icmp %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(), s2.c_str(), s3.c_str());
}

UncondBrInstruction::UncondBrInstruction(BasicBlock *to, BasicBlock *insert_bb) : Instruction(UNCOND, insert_bb)
{
    branch = to;
}

void UncondBrInstruction::output() const
{
    fprintf(yyout, "  br label %%B%d\n", branch->getNo());
}

void UncondBrInstruction::setBranch(BasicBlock *bb)
{
    branch = bb;
}

BasicBlock *UncondBrInstruction::getBranch()
{
    return branch;
}

CondBrInstruction::CondBrInstruction(BasicBlock*true_branch, BasicBlock*false_branch, Operand *cond, BasicBlock *insert_bb) : Instruction(COND, insert_bb){
    this->true_branch = true_branch;
    this->false_branch = false_branch;
    cond->addUse(this);
    operands.push_back(cond);
}

CondBrInstruction::~CondBrInstruction()
{
    operands[0]->removeUse(this);
}

void CondBrInstruction::output() const
{
    std::string cond, type;
    cond = operands[0]->toStr();
    type = operands[0]->getType()->toStr();
    int true_label = true_branch->getNo();
    int false_label = false_branch->getNo();
    fprintf(yyout, "  br %s %s, label %%B%d, label %%B%d\n", type.c_str(), cond.c_str(), true_label, false_label);
}

void CondBrInstruction::setFalseBranch(BasicBlock *bb)
{
    false_branch = bb;
}

BasicBlock *CondBrInstruction::getFalseBranch()
{
    return false_branch;
}

void CondBrInstruction::setTrueBranch(BasicBlock *bb)
{
    true_branch = bb;
}

BasicBlock *CondBrInstruction::getTrueBranch()
{
    return true_branch;
}

RetInstruction::RetInstruction(Operand *src, BasicBlock *insert_bb) : Instruction(RET, insert_bb)
{
    if(src != nullptr)
    {
        operands.push_back(src);
        src->addUse(this);
    }
    
}

RetInstruction::~RetInstruction()
{
    if(!operands.empty())
        operands[0]->removeUse(this);
}

void RetInstruction::output() const
{

    if(operands.empty())
    {

        fprintf(yyout, "  ret void\n");
    }
    else
    {
        std::string ret, type;
        ret = operands[0]->toStr();
        type = operands[0]->getType()->toStr();
        fprintf(yyout, "  ret %s %s\n", type.c_str(), ret.c_str());
    }
}

AllocaInstruction::AllocaInstruction(Operand *dst, SymbolEntry *se, BasicBlock *insert_bb) : Instruction(ALLOCA, insert_bb)
{
    operands.push_back(dst);
    dst->setDef(this);
    this->se = se;
}

AllocaInstruction::~AllocaInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
}

void AllocaInstruction::output() const
{
    std::string dst, type;
    dst = operands[0]->toStr();
    type = se->getType()->toStr();
    fprintf(yyout, "  %s = alloca %s, align 4\n", dst.c_str(), type.c_str());
}

LoadInstruction::LoadInstruction(Operand *dst, Operand *src_addr, BasicBlock *insert_bb) : Instruction(LOAD, insert_bb)
{
    operands.push_back(dst);
    operands.push_back(src_addr);
    dst->setDef(this);
    src_addr->addUse(this);
}

LoadInstruction::~LoadInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
}

void LoadInstruction::output() const
{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    std::string src_type;
    std::string dst_type;
    dst_type = operands[0]->getType()->toStr();
    src_type = operands[1]->getType()->toStr();
    fprintf(yyout, "  %s = load %s, %s %s, align 4\n", dst.c_str(), dst_type.c_str(), src_type.c_str(), src.c_str());
}

StoreInstruction::StoreInstruction(Operand *dst_addr, Operand *src, BasicBlock *insert_bb) : Instruction(STORE, insert_bb)
{
    operands.push_back(dst_addr);
    operands.push_back(src);
    dst_addr->addUse(this);
    src->addUse(this);
}

StoreInstruction::~StoreInstruction()
{
    operands[0]->removeUse(this);
    operands[1]->removeUse(this);
}

void StoreInstruction::output() const
{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    std::string dst_type = operands[0]->getType()->toStr();
    std::string src_type = operands[1]->getType()->toStr();

    fprintf(yyout, "  store %s %s, %s %s, align 4\n", src_type.c_str(), src.c_str(), dst_type.c_str(), dst.c_str());
}

//通过declstmt节点过来，收到三个主要参数，一个指针类型的临时节点，数组基地址（大概），和一个常数0
GepInstruction::GepInstruction(Operand* dst,
                               Operand* arr,
                               Operand* idx,
                               BasicBlock* insert_bb,
                               bool paramFirst)
    : Instruction(GEP, insert_bb), paramFirst(paramFirst) {
    operands.push_back(dst);
    operands.push_back(arr);
    operands.push_back(idx);
    dst->setDef(this);
    arr->addUse(this);
    idx->addUse(this);
    first = false;
    init = nullptr;
    last = false;
}

void GepInstruction::output() const {
    Operand* dst = operands[0];
    Operand* arr = operands[1];
    Operand* idx = operands[2];
    std::string arrType = arr->getType()->toStr();
    // Type* type = ((PointerType*)(arr->getType()))->getType();
    // ArrayType* type1 = (ArrayType*)(((ArrayType*)type)->getArrayType());
    // if (type->isInt() || (type1 && type1->getLength() == -1))
    if (paramFirst)
        fprintf(yyout, "  %s = getelementptr inbounds %s, %s %s, i32 %s\n",
                dst->toStr().c_str(),arrType.substr(0, arrType.size() - 1).c_str(), arrType.c_str(),arr->toStr().c_str(), idx->toStr().c_str());
    else
        fprintf(yyout, "  %s = getelementptr inbounds %s, %s %s, i32 0, i32 %s\n",
                dst->toStr().c_str(), arrType.substr(0, arrType.size() - 1).c_str(),arrType.c_str(), arr->toStr().c_str(), idx->toStr().c_str());
}

GepInstruction::~GepInstruction() {
    operands[0]->setDef(nullptr);
    if (operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}
MachineOperand* Instruction::genMachineOperand(Operand* operand)
{
    /*
    为一个中间代码操作数产生其汇编指令操作数，只有可能是立即数，虚拟寄存器，或地址标签
    */
    /*
    se是符号表项
    根据这一操作数的类型，创建对应的机器指令操作数单元mope
    分为常量，临时变量和变量
    对于常量，获取其值
    对于临时变量，获取其临时标签
    对于变量，通过id_se讲宽泛的symbolentry转换成了IdentifierSymbolEntry
    这里还需要考虑函数的参数，
    */
    auto se = operand->getEntry();
    MachineOperand* machine_operand=nullptr;
    if(se->getType()->isFloat())
    {
        //浮点操作数
        if(se->isConstant())
        {
            FloattoUint32 coverter;
            coverter.floatnumber=(float)dynamic_cast<ConstantSymbolEntry*>(se)->getValue();
            machine_operand = new MachineOperand(MachineOperand::IMM, coverter.IEEEnumber,true);
        }
        else if(se->isTemporary())
        {
            machine_operand = new MachineOperand(MachineOperand::VREG, dynamic_cast<TemporarySymbolEntry*>(se)->getLabel(),true);
        }
        else if(se->isVariable())
        {
            IdentifierSymbolEntry* id = dynamic_cast<IdentifierSymbolEntry*>(se);
            if(id->isGlobal())
            {
                machine_operand = new MachineOperand(id->toStr().c_str());
            }
            else if(id->isParam())
            {
                //函数参数需要映射的虚拟寄存器是特定的r0-r3
                if (id->getAsParamNum() < 4)
                {
                    machine_operand = new MachineOperand(MachineOperand::REG,id->getAsParamNum()+16,true);
                }
                else
                {
                    machine_operand = new MachineOperand(MachineOperand::REG, 20,true);
                }
            }
        }
    }
    else
    {
        //整型操作数
        if(se->isConstant())
        {
            machine_operand = new MachineOperand(MachineOperand::IMM, (int)dynamic_cast<ConstantSymbolEntry*>(se)->getValue());
        }
        else if(se->isTemporary())
        {
            machine_operand = new MachineOperand(MachineOperand::VREG, dynamic_cast<TemporarySymbolEntry*>(se)->getLabel());
        }
        else if(se->isVariable())
        {
            IdentifierSymbolEntry* id = dynamic_cast<IdentifierSymbolEntry*>(se);
            if(id->isGlobal())
            {
                //全局变量先去找它的地址，再进行加载
                machine_operand = new MachineOperand(id->toStr().c_str());
            }
            else if(id->isParam())  //新加的情况
            {
                //函数参数需要映射的虚拟寄存器是特定的r0-r3
                if (id->getAsParamNum() < 4)
                {
                    //4个参数以内，以r0-r3解决
                    machine_operand = new MachineOperand(MachineOperand::REG,id->getAsParamNum());
                }
                else
                {
                    machine_operand = new MachineOperand(MachineOperand::REG, 3);
                }
            }
        }
    }
    return machine_operand;
}


MachineOperand* Instruction::genMachineReg(int reg,bool is_float=false) 
{
    if(is_float)
    {
        return new MachineOperand(MachineOperand::REG,reg,true);
    }
    else
        return new MachineOperand(MachineOperand::REG, reg);
}

MachineOperand* Instruction::genMachineVReg(bool is_float) 
{
    if(is_float)
        return new MachineOperand(MachineOperand::VREG, SymbolTable::getLabel(),true);
    else
        return new MachineOperand(MachineOperand::VREG, SymbolTable::getLabel());
   
}

MachineOperand* Instruction::genMachineImm(int val) 
{
    return new MachineOperand(MachineOperand::IMM, val);
}

MachineOperand* Instruction::genMachineLabel(int block_no)
{
    std::ostringstream buf;
    buf << ".L" << block_no;
    std::string label = buf.str();
    return new MachineOperand(label);
}

void AllocaInstruction::genMachineCode(AsmBuilder* builder)
{
    /* HINT:
    * Allocate stack space for local variabel
    * Store frame offset in symbol entry */
   /*
   分配的空间可能需要重新计算(数组)
   offset分配空间，给当前函数栈的大小增加了4，是一个int，并获取到了栈的位置
   寻址方式采用fp寻址，即第一个参数位置在fp-4
   把这一参数在栈中的信息保存在了符号表项内
   */
    auto cur_func = builder->getFunction();
    int size = se->getType()->getOSize() / 8;
    if (size < 0)
        size = 4;
    //debug，数组参数（从右往左压栈），由于数组形参尺寸为0，没有分配空间
    if (se->getType()->isArray() && ((IdentifierSymbolEntry *)se)->isParam()) {
        size = 4;
    }
    int offset = cur_func->AllocSpace(size);//改!!否则数组WA！！
    dynamic_cast<TemporarySymbolEntry*>(operands[0]->getEntry())->setOffset(-offset);
}

void LoadInstruction::genMachineCode(AsmBuilder* builder)
{
    /*
    对加载指令产生机器指令
    cur_block获取到了当前基本块
    加载一个全局变量时，先将其地址加载到一个虚拟寄存器，然后再从该虚拟寄存器中加载出值到目标dst上
    加载一个变量时，需要确定其已经被声明了同时也是通过alloc语句进行声明的。然后通过11号寄存器进行加载。
    加载一个临时变量时，直接进行加载即可
    */
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst=nullptr;
    // Load global operand
    if(operands[1]->getEntry()->isVariable()&& dynamic_cast<IdentifierSymbolEntry*>(operands[1]->getEntry())->isGlobal())
    {
        /*
        dst是对应中间代码加载结果的操作数的汇编代码操作数
        internal_reg1在这里只是获得了一个临时的汇编代码操作数（虚拟寄存器）
        internal_reg2是internal_reg1的一个深拷贝
        二者代表同一个虚拟寄存器
        src在这里对应了加载源的汇编代码操作数
        */
        auto dst = genMachineOperand(operands[0]);
        auto internal_reg1 = genMachineVReg();
        auto internal_reg2 = new MachineOperand(*internal_reg1);
        auto src = genMachineOperand(operands[1]);
        // example: load r0, addr_a
        cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg1, src);
        cur_block->InsertInst(cur_inst);
        // example: load r1, [r0]
        if(operands[0]->getEntry()->getType()->isFloat())
            cur_inst = new LoadMInstruction(LoadMInstruction::VLDR,cur_block, dst, internal_reg2);
        else
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, dst, internal_reg2);
        cur_block->InsertInst(cur_inst);
    }
    // Load local operand
    else if(operands[1]->getEntry()->isTemporary()&& operands[1]->getDef()&& operands[1]->getDef()->isAlloc())
    {

        auto dst=genMachineOperand(operands[0]);
        auto src1 = genMachineReg(11);
        auto src2 = genMachineImm(dynamic_cast<TemporarySymbolEntry*>(operands[1]->getEntry())->getOffset());
        int off=dynamic_cast<TemporarySymbolEntry*>(operands[1]->getEntry())->getOffset();
        if (off > 255 || off < -255) 
        {
            auto operand = genMachineVReg();
            cur_block->InsertInst((new LoadMInstruction(LoadMInstruction::LDR,cur_block, operand, src2)));
            src2 = operand;
        }
        if(operands[0]->getEntry()->getType()->isFloat())
            cur_inst = new LoadMInstruction(LoadMInstruction::VLDR,cur_block, dst, src1, src2);
        else
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, dst, src1, src2);
        cur_block->InsertInst(cur_inst);
    }
    // Load operand from temporary variable
    else
    {
        // example: load r1, [r0]
        auto dst = genMachineOperand(operands[0]);
        auto src = genMachineOperand(operands[1]);
        if(dst->ifFloat())
            cur_inst = new LoadMInstruction(LoadMInstruction::VLDR,cur_block, dst, src);
        else
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, dst, src);
        cur_block->InsertInst(cur_inst);
    }
}

void StoreInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst=nullptr;
    MachineOperand* src=nullptr;
    //src:要存的是什么东西
    src = genMachineOperand(operands[1]);
    MachineOperand* dst1;
    if(src->isImm())
    {
        if(operands[1]->getType()->isFloat())
        {
            dst1 = genMachineVReg();
            cur_inst=new LoadMInstruction(LoadMInstruction::LDR,cur_block, dst1, src);
            cur_block->InsertInst(cur_inst);
            auto dst2=genMachineVReg(true);
            cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,dst2,dst1);
            cur_block->InsertInst(cur_inst);
            src = new MachineOperand(*dst2);
        }
        else
        {
            dst1 = genMachineVReg();
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, dst1, src);
            cur_block->InsertInst(cur_inst);
            src = new MachineOperand(*dst1);
        }
        
    }
    if(operands[0]->getEntry()->isTemporary() && operands[0]->getDef() &&operands[0]->getDef()->isAlloc())
    {
        //存储一个变量
        //超过一定范围的操作数可能需要重新加载到寄存器中。
        auto fp = genMachineReg(11);
        auto offset = genMachineImm(dynamic_cast<TemporarySymbolEntry*>(operands[0]->getEntry())->getOffset());
        if(operands[1]->getType()->isFloat())
        {
            cur_inst=new StoreMInstruction(StoreMInstruction::VSTR,cur_block, src, fp, offset);
        }
        else
        {
            auto operand = genMachineVReg();
            cur_inst=new LoadMInstruction(LoadMInstruction::LDR,cur_block, operand, offset);
            cur_block->InsertInst(cur_inst);
            offset=operand;
            cur_inst = new StoreMInstruction(StoreMInstruction::STR,cur_block, src, fp, offset);
        }
        cur_block->InsertInst(cur_inst);
    }
    else if(operands[0]->getEntry()->isVariable() &&dynamic_cast<IdentifierSymbolEntry*>(operands[0]->getEntry())->isGlobal())
    {
        //全局变量，先将dst存储到一个虚拟寄存器中，再存入src
        auto dst = genMachineOperand(operands[0]);
        auto internal_reg1 = genMachineVReg();
        cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg1, dst);
        cur_block->InsertInst(cur_inst);
        cur_inst = new StoreMInstruction(StoreMInstruction::STR,cur_block, src, internal_reg1);
        cur_block->InsertInst(cur_inst);
    }
    else 
    {
        auto dst = genMachineOperand(operands[0]);
        if(src->ifFloat())
        {
            cur_inst = new StoreMInstruction(StoreMInstruction::VSTR,cur_block, src, dst);
        }
        else
        {
            cur_inst = new StoreMInstruction(StoreMInstruction::STR,cur_block, src, dst);
        }
        cur_block->InsertInst(cur_inst);
    }

}

void BinaryInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO:
    // complete other instructions
    auto cur_block = builder->getBlock();
    MachineOperand* dst=nullptr,*src1=nullptr,*src2=nullptr;
    bool whetherFloat=operands[0]->getType()->isFloat();
    dst=genMachineOperand(operands[0]);
    src1=genMachineOperand(operands[1]);
    src2=genMachineOperand(operands[2]);
    /* HINT:
    * The source operands of ADD instruction in ir code both can be immediate num.
    * However, it's not allowed in assembly code.
    * So you need to insert LOAD/MOV instrucrion to load immediate num into register.
    * As to other instructions, such as MUL, CMP, you need to deal with this situation, too.*/
    MachineInstruction* cur_inst=nullptr;//设为nullptr？？
    MachineOperand* operand_temp;
    if(src1->isImm())
    {
        if(operands[1]->getType()->isFloat())
        {
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, src1);
            cur_block->InsertInst(cur_inst);
            auto float_reg=genMachineVReg(true);
            cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,float_reg,internal_reg);
            cur_block->InsertInst(cur_inst);
            src1 = new MachineOperand(*float_reg);
        }
        else
        {
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, src1);
            cur_block->InsertInst(cur_inst);
            src1 = new MachineOperand(*internal_reg);
        }
    }
    if(src2->isImm())
    {
        if(operands[2]->getType()->isFloat())
        {
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, src2);
            cur_block->InsertInst(cur_inst);
            auto float_reg=genMachineVReg(true);
            cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,float_reg,internal_reg);
            cur_block->InsertInst(cur_inst);
            src2 = new MachineOperand(*float_reg);
        }
        else
        {
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, src2);
            cur_block->InsertInst(cur_inst);
            src2 = new MachineOperand(*internal_reg);
        }
    }
    /*
    经过上面的操作，src1和src2保证了都是寄存器的值或者由寄存器存储的地址
    */
    switch (opcode)
    {
    case ADD:
        if(whetherFloat)
            cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::VADD, dst, src1, src2);
        else
            cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD, dst, src1, src2);
        break;
    case SUB:
        if(whetherFloat)
            cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::VSUB, dst, src1, src2);
        else
            cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::SUB, dst, src1, src2);
        break;
    case MUL:
        if(whetherFloat)
            cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::VMUL, dst, src1, src2);
        else
            cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MUL, dst, src1, src2);
        break;
    case DIV:
        if(whetherFloat)
            cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::VDIV, dst, src1, src2);
        else
            cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::DIV, dst, src1, src2);
        break;
    case MOD:
        /*
        9%2=9-（9/2）*2
        dst先存储了src1/src2的结果
        operand_temp保存了dst*src2的结果
        dst最后变成src1-operand_temp的结果。
        */
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::DIV, dst, src1, src2);
        operand_temp=new MachineOperand(*dst);
        cur_block->InsertInst(cur_inst);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MUL, operand_temp, dst, src2);
        cur_block->InsertInst(cur_inst);
        dst = new MachineOperand(*operand_temp);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::SUB, dst, src1, operand_temp);
        break;
    case AND:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::AND, dst, src1, src2);
        break;
    case OR:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::OR, dst, src1, src2);
        break;
    default:
        break;
    }
    cur_block->InsertInst(cur_inst);
}

void CmpInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst=nullptr;
    MachineOperand*src1,*src2;
    src1=genMachineOperand(operands[1]);
    src2=genMachineOperand(operands[2]);
    if (src1->isImm())
    {  
        if(operands[1]->getType()->isFloat())
        {
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, src1);
            cur_block->InsertInst(cur_inst);
            auto float_reg=genMachineVReg(true);
            cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,float_reg,internal_reg);
            cur_block->InsertInst(cur_inst);
            src1 = new MachineOperand(*float_reg);
        }
        else
        {
            
            auto internal_reg = genMachineVReg();
            
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, src1);
            cur_block->InsertInst(cur_inst);
            src1 = new MachineOperand(*internal_reg);
        }
    }
    if(src2->isImm())
    {  
        if(operands[2]->getType()->isFloat())
        {
            
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, src2);
            cur_block->InsertInst(cur_inst);
            auto float_reg=genMachineVReg(true);
            cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,float_reg,internal_reg);
            cur_block->InsertInst(cur_inst);
            src2 = new MachineOperand(*float_reg);
        }
        else
        {
            auto internal_reg = genMachineVReg();
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, src2);
            cur_block->InsertInst(cur_inst);
            src2 = new MachineOperand(*internal_reg);
        }
    }
    if(operands[1]->getType()->isFloat())
    {
        //默认已经完成了类型转换，此时这两个操作数的类型是相同的，判断谁都可以
        cur_inst = new CmpMInstruction(CmpMInstruction::VCMP,cur_block, src1, src2, opcode);
    }
    else
        cur_inst = new CmpMInstruction(CmpMInstruction::CMP,cur_block, src1, src2, opcode);//debug!!enum的数值要对应上！！顺序一致才可！！
    cur_block->InsertInst(cur_inst);
    
    if (opcode >= L && opcode <= LE) //debug:GE改LE
    {
        //对于L，LE，G，GE，创建分支
        auto dst = genMachineOperand(operands[0]);
        auto trueOperand = genMachineImm(1);
        auto falseOperand = genMachineImm(0);
        /*
        对应操作为真
        相反操作为假
        */
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst,trueOperand, opcode);
        cur_block->InsertInst(cur_inst);
        switch(opcode)
        {
            case L:
                cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst,falseOperand, GE);
                break;
            case LE:
                cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst,falseOperand, G);
                break;
            case G:
                cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst,falseOperand, LE);
                break;
            case GE:
                cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst,falseOperand, L);
                break;

        }
        cur_block->InsertInst(cur_inst);
    }
}

void UncondBrInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst;

    std::stringstream buffer;
    buffer << ".L" << branch->getNo();
    MachineOperand* dst = new MachineOperand(buffer.str());
    cur_inst =new BranchMInstruction(cur_block, BranchMInstruction::B, dst);
    cur_block->InsertInst(cur_inst);

}

void CondBrInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst;
    std::stringstream buffer;

    buffer << ".L" << true_branch->getNo();
    MachineOperand* dst = new MachineOperand(buffer.str());
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::B,dst, cur_block->getCmpCond());
    cur_block->InsertInst(cur_inst);

    buffer.str("");
    buffer << ".L" << false_branch->getNo();
    dst = new MachineOperand(buffer.str());
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::B, dst);
    cur_block->InsertInst(cur_inst);
}

void RetInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    /* HINT:
    * 1. Generate mov instruction to save return value in r0
    * 2. Restore callee saved registers and sp, fp
    * 3. Generate bx instruction */

   /*有返回值的话额外进行一次mov指令*/
    auto cur_block = builder->getBlock();
    auto cur_func = builder->getFunction();
    MachineInstruction* cur_inst;
    if (!operands.empty()) 
    {
        Type* dstrtnType=dynamic_cast<FunctionType*>(cur_func->getSym_ptr()->getType())->getRetType();
        auto src = genMachineOperand(operands[0]);
        if(dstrtnType->isFloat())
        {
            if(src->isImm())
            {
                auto internal_reg = genMachineVReg();
                cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, src);
                cur_block->InsertInst(cur_inst);
                src=internal_reg;
            }
            auto dst = new MachineOperand(MachineOperand::REG, 0,true);
            cur_inst =new MovMInstruction(cur_block, MovMInstruction::VMOV, dst, src);
        }
        else
        {
            //通过r0寄存器传回返回值
            auto dst = new MachineOperand(MachineOperand::REG, 0);
            cur_inst =new MovMInstruction(cur_block, MovMInstruction::MOV, dst, src);
        }
        cur_block->InsertInst(cur_inst);
    }
    //当前函数准备返回了
    
    //当前的栈大小与sp的值
    auto sp = new MachineOperand(MachineOperand::REG, 13);
    auto size =new MachineOperand(MachineOperand::IMM, cur_func->AllocSpace(0));
    //转移栈帧
    cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,sp, sp, size);
    cur_block->InsertInst(cur_inst);

    auto lr = new MachineOperand(MachineOperand::REG, 14);
    cur_inst =new BranchMInstruction(cur_block, BranchMInstruction::BX, lr);
    cur_block->InsertInst(cur_inst);
}
XorInstruction::XorInstruction(Operand*dst,Operand*src,BasicBlock*insert_bb):Instruction(XOR,insert_bb)
{
    operands.push_back(dst);
    operands.push_back(src);
    dst->setDef(this);
    src->setDef(this);
}

void XorInstruction::output()const
{
    Operand* dst = operands[0];
    Operand* src = operands[1];
    fprintf(yyout, "  %s = xor %s %s, true\n", dst->toStr().c_str(),src->getType()->toStr().c_str(), src->toStr().c_str());
}

FuncCallInstruction::FuncCallInstruction(Operand* dst,SymbolEntry* func,std::vector<Operand*> params,BasicBlock* insert_bb = nullptr):Instruction(FUNCCALL,insert_bb)
{
    this->dst=dst;//新加属性
    this->func=func;
    operands.push_back(dst);
    if(dst)
    {
        dst->setDef(this);
    }

    
    for(int i=0;i<(int)params.size();i++)
    {
        operands.push_back(params[i]);
        params[i]->addUse(this);
    }
}

void FuncCallInstruction::output() const
{
    fprintf(yyout, "  ");
    if (operands[0])
    {
        fprintf(yyout, "%s = ", operands[0]->toStr().c_str());
    }
    FunctionType* type = (FunctionType*)(func->getType());
    fprintf(yyout, "call %s %s(", type->getRetType()->toStr().c_str(),func->toStr().c_str());
    for (long unsigned int i = 1; i < operands.size(); i++) {
        if (i != 1)
        {
            fprintf(yyout, ", ");
        }
        fprintf(yyout, "%s %s", operands[i]->getType()->toStr().c_str(),operands[i]->toStr().c_str());
    }
    fprintf(yyout, ")\n");
}

ZextInstruction::ZextInstruction(Operand*dst,Operand*src,BasicBlock*bb):Instruction(ZEXT,bb)
{
    operands.push_back(dst);
    operands.push_back(src);
    dst->setDef(this);
    src->addUse(this);
}

void ZextInstruction::output() const
{
    Operand* dst = operands[0];
    Operand* src = operands[1];
    fprintf(yyout, "  %s = zext %s %s to i32\n", dst->toStr().c_str(),src->getType()->toStr().c_str(), src->toStr().c_str());
}

I2FInstruction::I2FInstruction(Operand* dst,Operand* src,BasicBlock* insert_bb): Instruction(I2F, insert_bb), dst(dst), src(src) {
    operands.push_back(dst);
    operands.push_back(src);
    dst->setDef(this);
    src->addUse(this);
}
F2IInstruction::F2IInstruction(Operand* dst,Operand* src,BasicBlock* insert_bb): Instruction(F2I, insert_bb), dst(dst), src(src) {
    operands.push_back(dst);
    operands.push_back(src);
    dst->setDef(this);
    src->addUse(this);
}
I2FInstruction::~I2FInstruction() {
    operands[0]->setDef(nullptr);
    if (operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
}
F2IInstruction::~F2IInstruction() {
    operands[0]->setDef(nullptr);
    if (operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
}
void I2FInstruction::output() const {
    Operand* dst = operands[0];
    Operand* src = operands[1];
    fprintf(yyout, "  %s = sitofp %s %s to %s\n", dst->toStr().c_str(),
            src->getType()->toStr().c_str(), src->toStr().c_str(),
            dst->getType()->toStr().c_str());
}
void F2IInstruction::output() const {
    Operand* dst = operands[0];
    Operand* src = operands[1];
    fprintf(yyout, "  %s = fptosi %s %s to %s\n", dst->toStr().c_str(),
            src->getType()->toStr().c_str(), src->toStr().c_str(),
            dst->getType()->toStr().c_str());
}

void XorInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst;
    auto dst = genMachineOperand(operands[0]);
    auto trueOperand = genMachineImm(1);
    auto falseOperand = genMachineImm(0);
    cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst,trueOperand, MachineInstruction::EQ);
    cur_block->InsertInst(cur_inst);
    cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst,falseOperand, MachineInstruction::NE);
    cur_block->InsertInst(cur_inst);
}

void FuncCallInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    //函数调用语句
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst;
    MachineOperand* operand;
    int intParamIndex=0;
    int floatParamIndex=0;
    int intParamCnt=0;
    int floatParamCnt=0;
    for(int i=1;i<(int)operands.size();i++)
    {
        if(operands[i]->getEntry()->getType()->isFloat())
            floatParamCnt++;
        else
            intParamCnt++;
    }
    //先遍历一遍，主要是确定浮点参数的个数,以此确定是否需要对齐，启用s4寄存器。
    bool whetherAlign=false;
    if(floatParamCnt>4&&floatParamCnt%2==1)
        whetherAlign=true;
    for(int i=1;i<(int)operands.size();i++)
    {
        if(operands[i]->getEntry()->getType()->isFloat())
        {
            //浮点参数
            if(floatParamIndex>4)
            {
                continue;
            }
            else if(floatParamIndex==4)
            {
                if(!whetherAlign)
                {
                    floatParamIndex++;
                    continue;
                }
                else
                {
                    auto param = genMachineOperand(operands[i]);
                    operand = genMachineReg(floatParamIndex++,true);
                    if(param->isImm())
                    {
                        auto dst1=genMachineVReg();
                        cur_inst=new LoadMInstruction(LoadMInstruction::LDR,cur_block,dst1,param);
                        cur_block->InsertInst(cur_inst);
                        cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,operand,dst1);
                        cur_block->InsertInst(cur_inst);
                    }
                    else
                    {
                        cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,operand,param);
                        cur_block->InsertInst(cur_inst);
                    }
                }
            }
            else
            {
                auto param = genMachineOperand(operands[i]);
                operand = genMachineReg(floatParamIndex++,true);
                if(param->isImm())
                {
                    auto dst1=genMachineVReg();
                    cur_inst=new LoadMInstruction(LoadMInstruction::LDR,cur_block,dst1,param);
                    cur_block->InsertInst(cur_inst);
                    cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,operand,dst1);
                    cur_block->InsertInst(cur_inst);
                }
                else
                {
                    cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,operand,param);
                    cur_block->InsertInst(cur_inst);
                }
            }
            
        }
        else
        {
            //整型参数
            if(intParamIndex>=4)
                continue;
            else
            {
                auto param = genMachineOperand(operands[i]);
                operand = genMachineReg(intParamIndex++);
                if(param->isImm())
                {
                    cur_inst=new LoadMInstruction(LoadMInstruction::LDR,cur_block,operand,param);
                    cur_block->InsertInst(cur_inst);
                }
                else
                {
                    cur_inst=new MovMInstruction(cur_block,MovMInstruction::MOV,operand,param);
                    cur_block->InsertInst(cur_inst);
                }
            }
        }
    }
    //至此完成初步参数的扫描，需要通过寄存器传参的部分完成。开始补充需要压栈的部分,逆序压栈
    int intNeedTopush=0;
    if(intParamCnt>4)
        intNeedTopush=intParamCnt-4;
    int floatNeedTopush=0;
    if(whetherAlign)
    {
        //whetherAlign为真时，此时浮点参数数量一定多于4，至少为5
        floatNeedTopush=floatParamCnt-5;
    }
    else
    {
        if(floatParamCnt>4)
            floatNeedTopush=floatParamCnt-4;
    }
    //offset用作后续重新调整栈顶
    int offsize=intNeedTopush+floatNeedTopush;
    int index=operands.size()-1;
    //压栈时候的一个动态数组。
    std::vector<MachineOperand*> vec;
    while(intNeedTopush>0||floatNeedTopush>0)
    {
        //二者有一个大于零就可以开始逆序压栈了
        if(operands[index]->getEntry()->getType()->isFloat())
        {
            //浮点参数压栈操作
            if(floatNeedTopush==0)
            {
                index--;
                continue;
            }
            floatNeedTopush--;
            operand = genMachineOperand(operands[index]);
            if(operand->isImm())
            {
                //立即数先加载，再vmov，再压栈
                auto internal_reg = genMachineVReg();
                cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, operand);
                cur_block->InsertInst(cur_inst);
                auto float_reg=genMachineVReg(true);
                cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,float_reg,internal_reg);
                cur_block->InsertInst(cur_inst);
                cur_inst = new StackMInstrcuton(cur_block, StackMInstrcuton::VPUSH, vec,float_reg);
                cur_block->InsertInst(cur_inst);
            }
            else
            {
                //非立即数，直接压栈
                cur_inst = new StackMInstrcuton(cur_block, StackMInstrcuton::VPUSH, vec,operand);
                cur_block->InsertInst(cur_inst);
            }
        }
        else
        {
            //整型参数压栈
            if(intNeedTopush==0)
            {
                index--;
                continue;
            }
            intNeedTopush--;
            operand = genMachineOperand(operands[index]);
            if(operand->isImm())
            {
                //立即数先加载，再vmov，再压栈
                auto internal_reg = genMachineVReg();
                cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, operand);
                cur_block->InsertInst(cur_inst);
                cur_inst = new StackMInstrcuton(cur_block, StackMInstrcuton::PUSH, vec,internal_reg);
                cur_block->InsertInst(cur_inst);
            }
            else
            {
                //非立即数，直接压栈
                cur_inst = new StackMInstrcuton(cur_block, StackMInstrcuton::PUSH, vec,operand);
                cur_block->InsertInst(cur_inst);
            }
        }
        index--;
    }
    auto func_addr = new MachineOperand(func->toStr().c_str());
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::BL, func_addr);
    cur_block->InsertInst(cur_inst);
    if(offsize!=0)
    {
        //有压栈的参数，调整栈顶
        auto off = genMachineImm(offsize * 4);
        auto sp = new MachineOperand(MachineOperand::REG, 13);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,sp, sp, off);
        cur_block->InsertInst(cur_inst);
    }
    if (dst) 
    {
        operand = genMachineOperand(dst);
        MachineOperand* internal_reg;
        if(dst->getEntry()->getType()->isFloat())
        {
            internal_reg= new MachineOperand(MachineOperand::REG, 0,true);
            cur_inst =new MovMInstruction(cur_block, MovMInstruction::VMOV, operand, internal_reg);
        }
        else
        {
            internal_reg= new MachineOperand(MachineOperand::REG, 0);
            cur_inst =new MovMInstruction(cur_block, MovMInstruction::MOV, operand, internal_reg);
        }
        cur_block->InsertInst(cur_inst);
    }
    
}

void ZextInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src = genMachineOperand(operands[1]);
    auto cur_inst =new MovMInstruction(cur_block, MovMInstruction::MOV, dst, src);
    cur_block->InsertInst(cur_inst);
}

void GepInstruction::genMachineCode(AsmBuilder* builder){
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst;
    //第一次到这里的时候，dst是指针类型的临时变量，idx是常量0，init为空
    auto dst = genMachineOperand(operands[0]);
    auto idx = genMachineOperand(operands[2]);
    if(init)
    {
        if(last)
        {
            auto base = genMachineOperand(init);
            cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD, dst, base, genMachineImm(4));
            cur_block->InsertInst(cur_inst);
        }
        return;
    }
    MachineOperand* base = nullptr;
    int size;
    auto idx1 = genMachineVReg();
    if (idx->isImm()) 
    {
        if (idx->getVal() < 255) 
        {
            cur_inst =new MovMInstruction(cur_block, MovMInstruction::MOV, idx1, idx);
        } 
        else 
        {
            cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, idx1, idx);
        }
        idx = new MachineOperand(*idx1);
        cur_block->InsertInst(cur_inst);
    }
    if (paramFirst) 
    {
        size =((PointerType*)(operands[1]->getType()))->getType()->getOSize() / 8;
    } 
    else 
    {
        if (first) 
        {
            base = genMachineVReg();
            if (operands[1]->getEntry()->isVariable() &&((IdentifierSymbolEntry*)(operands[1]->getEntry()))->isGlobal()) 
            {
                //全局数组？
                auto src = genMachineOperand(operands[1]);
                cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, base, src);
            } 
            else 
            {
                //局部数组？
                int offset = ((TemporarySymbolEntry*)(operands[1]->getEntry()))->getOffset();
                if (offset > -255 && offset < 255) 
                {
                    cur_inst =new MovMInstruction(cur_block, MovMInstruction::MOV,base, genMachineImm(offset));
                } 
                else 
                {
                    cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, base,genMachineImm(offset));
                }
            }
            cur_block->InsertInst(cur_inst);
        }
        //operand[1]是数组基址，其操作数被设置为了指针类型，内核为数组类型
        ArrayType* type =(ArrayType*)(((PointerType*)(operands[1]->getType()))->getType());
        //size是元素粒度，对于一维数组，其每一个元素是int或者float，32位，占4个字节。多维数组在这里可能更大
        size = type->getArrayType()->getOSize() / 8;
    }
    auto size1 = genMachineVReg();
    if (size > -255 && size < 255) 
    {
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, size1,genMachineImm(size));
    } 
    else 
    {
        cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, size1, genMachineImm(size));
    }
    cur_block->InsertInst(cur_inst);
    auto off = genMachineVReg();
    cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MUL, off,idx, size1);
    off = new MachineOperand(*off);
    cur_block->InsertInst(cur_inst);
    if (paramFirst || !first) 
    {
        auto arr = genMachineOperand(operands[1]);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,dst, arr, off);
        cur_block->InsertInst(cur_inst);
    } 
    else 
    {
        auto addr = genMachineVReg();
        auto base1 = new MachineOperand(*base);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,addr, base1, off);
        cur_block->InsertInst(cur_inst);
        addr = new MachineOperand(*addr);
        if (operands[1]->getEntry()->isVariable() &&((IdentifierSymbolEntry*)(operands[1]->getEntry()))->isGlobal()) 
        {
            cur_inst =new MovMInstruction(cur_block, MovMInstruction::MOV, dst, addr);
        }
        else 
        {
            auto fp = genMachineReg(11);
            cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD, dst, fp, addr);
        }
        cur_block->InsertInst(cur_inst);
    }
}

void I2FInstruction::genMachineCode(AsmBuilder* builder){
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst;
    auto dst = genMachineOperand(operands[0]);
    auto src = genMachineOperand(operands[1]);
    if(src->isImm())
    {
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, src);
        cur_block->InsertInst(cur_inst);
        src=new MachineOperand(*internal_reg);
    }
    auto float_reg=genMachineVReg(true);
    cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,float_reg,src);
    cur_block->InsertInst(cur_inst);
    src=new MachineOperand(*float_reg);
    cur_inst=new NeonVcvtInstruction(cur_block,NeonVcvtInstruction::IntToFloat,dst,src);
    cur_block->InsertInst(cur_inst);

}
void F2IInstruction::genMachineCode(AsmBuilder* builder){
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst;
    auto dst = genMachineOperand(operands[0]);
    auto src = genMachineOperand(operands[1]);
    if(src->isImm())
    {
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(LoadMInstruction::LDR,cur_block, internal_reg, src);
        cur_block->InsertInst(cur_inst);
        auto float_reg = genMachineVReg(true);
        cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,float_reg,internal_reg);
        cur_block->InsertInst(cur_inst);
        src=new MachineOperand(*float_reg);
    }
    auto float_reg=genMachineVReg(true);
    cur_inst=new NeonVcvtInstruction(cur_block,NeonVcvtInstruction::FloatToInt,float_reg,src);
    cur_block->InsertInst(cur_inst);
    src=new MachineOperand(*float_reg);
    cur_inst=new MovMInstruction(cur_block,MovMInstruction::VMOV,dst,src);
    cur_block->InsertInst(cur_inst);
}
