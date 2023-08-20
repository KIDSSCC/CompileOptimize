#include "MachineCode.h"
#include<iostream>//新加两行
#include<Type.h>
#include<Instruction.h>
extern FILE* yyout;

MachineOperand::MachineOperand(int tp, int val)
{
    this->type = tp;
    if(tp == MachineOperand::IMM)
        this->val = val;
    else 
        this->reg_no = val;
}

MachineOperand::MachineOperand(std::string label)
{
    this->type = MachineOperand::LABEL;
    this->label = label;
}

MachineOperand::MachineOperand(int tp,unsigned long fval,bool if_float)
{
    this->type = tp;
    this->if_float=if_float;
    if(tp == MachineOperand::IMM)
        this->fval = fval;
    else 
        this->reg_no = fval;
}

bool MachineOperand::operator==(const MachineOperand&a) const
{
    if (this->type != a.type)
        return false;
    if (this->type == IMM)
        return this->val == a.val;
    return this->reg_no == a.reg_no;//两个操作数（IMM, VREG, REG）是否相等
}

bool MachineOperand::operator<(const MachineOperand&a) const
{
    if(this->type == a.type)
    {
        if(this->type == IMM)
            return this->val < a.val;
        return this->reg_no < a.reg_no;
    }
    return this->type < a.type;//enum三种类型的int值比较 IMM<VREG<REG

    if (this->type != a.type)//这一段白写吧？
        return false;
    if (this->type == IMM)
        return this->val == a.val;
    return this->reg_no == a.reg_no;
}

void MachineOperand::PrintReg(bool if_float)
{
    if(reg_no>=16)
    {
        int tmp=reg_no-16;
        if(tmp==32)
            fprintf(yyout,"FPSCR");
        else
            fprintf(yyout,"s%d",tmp);
        return ;
    }
    switch (reg_no)
    {
    case 11:
        fprintf(yyout, "fp");
        break;
    case 13:
        fprintf(yyout, "sp");
        break;
    case 14:
        fprintf(yyout, "lr");
        break;
    case 15:
        fprintf(yyout, "pc");
        break;
    default:
        if(if_float)
        {
            fprintf(yyout, "s%d", reg_no);
        }
        else
        {
            fprintf(yyout,"r%d",reg_no);
        }
        break;
    }
}

void MachineOperand::output() 
{
    /* HINT：print operand
    * Example:
    * immediate num 1 -> print #1;
    * register 1 -> print r1;
    * lable addr_a -> print addr_a; */
    switch (this->type)
    {
    case IMM:
        if(if_float)
            fprintf(yyout, "#%ld", this->fval);
        else
            fprintf(yyout,"#%d",this->val);
        break;
    case VREG:
        fprintf(yyout, "v%d", this->reg_no);
        break;
    case REG:
        PrintReg(if_float);
        break;
    case LABEL:
        if (this->label.substr(0, 2) == ".L")
        {
            fprintf(yyout, "%s", this->label.c_str());
        }
        else if (this->label.substr(0, 1) == "@")//新加情况
        {
            fprintf(yyout, "%s", this->label.c_str() + 1);
        }
        else
        {
            fprintf(yyout, "addr_%s%d", this->label.c_str(),parent->getParent()->getParent()->getParent()->getN());//新加%d
        }
    default:
        break;
    }
}

void MachineInstruction::PrintCond()
{
    // TODO
    switch (cond)
    {
    case LT:
        fprintf(yyout, "lt");
        break;
    case LE:
        fprintf(yyout, "le");
        break;
    case GT:
        fprintf(yyout, "gt");
        break;
    case GE:
        fprintf(yyout, "ge");
        break;
    case EQ:
        fprintf(yyout, "eq");
        break;
    case NE:
        fprintf(yyout, "ne");
        break;
    default:    //NONE的情况
        break;
    }
}

//新加两个
void MachineInstruction::insertBefore(MachineInstruction* inst) {
    auto& instructions = parent->getInsts();
    auto it = std::find(instructions.begin(), instructions.end(), this);
    instructions.insert(it, inst);
}

void MachineInstruction::insertAfter(MachineInstruction* inst) {
    auto& instructions = parent->getInsts();
    auto it = std::find(instructions.begin(), instructions.end(), this);
    instructions.insert(++it, inst);
}

int MachineInstruction::MulOrDivImm()
{
    if(type!=BINARY)
        return 0;
    if(op==BinaryMInstruction::DIV)
    {
        if(use_list[1]->getDef() && use_list[1]->getDef()->getUse()[0]->isImm())
        {
            return 1;
        }
        return 0;
    }
    return 0;
    
}

BinaryMInstruction::BinaryMInstruction(MachineBlock* p, int op, MachineOperand* dst, MachineOperand* src1, MachineOperand* src2, int cond)
{
    this->parent = p;
    this->type = MachineInstruction::BINARY;
    this->op = op;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    dst->setParent(this);
    src1->setParent(this);
    src2->setParent(this);
    dst->setDef(this);
}

void BinaryMInstruction::output() 
{
    // TODO: 
    // Complete other instructions
    switch (this->op)
    {
    case BinaryMInstruction::ADD:
        fprintf(yyout, "\tadd ");
        break;
    case BinaryMInstruction::SUB:
        fprintf(yyout, "\tsub ");
        break;
    case BinaryMInstruction::MUL:
        fprintf(yyout, "\tmul ");
        break;
    case BinaryMInstruction::DIV:
        fprintf(yyout, "\tsdiv ");
        break;
    case BinaryMInstruction::VADD:
        fprintf(yyout, "\tvadd.f32 ");
        break;
    case BinaryMInstruction::VSUB:
        fprintf(yyout, "\tvsub.f32 ");
        break;
    case BinaryMInstruction::VMUL:
        fprintf(yyout, "\tvmul.f32 ");
        break;
    case BinaryMInstruction::VDIV:
        fprintf(yyout, "\tvdiv.f32 ");
        break;
    default:
        break;
    }
    this->def_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[1]->output();
    fprintf(yyout, "\n");
}


LoadMInstruction::LoadMInstruction(int op,MachineBlock* p,MachineOperand* dst, MachineOperand* src1, MachineOperand* src2,int cond)
{
    this->op=op;
    this->parent = p;
    this->type = MachineInstruction::LOAD;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src1);
    if (src2)
        this->use_list.push_back(src2);
    dst->setParent(this);
    src1->setParent(this);
    if (src2)
        src2->setParent(this);
    dst->setDef(this);
}

void LoadMInstruction::output()
{
    if(this->op==LoadMInstruction::LDR)
    {
        fprintf(yyout, "\tldr ");
    }
    else
    {
        fprintf(yyout, "\tvldr.32 ");
    }
    this->def_list[0]->output();
    fprintf(yyout, ", ");

    // Load immediate num, eg: ldr r1, =8
    if(this->use_list[0]->isImm())
    {
        if(this->use_list[0]->ifFloat())
        {
            fprintf(yyout, "=%d\n", this->use_list[0]->getFval());
            return;
        }
        else
        {
            fprintf(yyout, "=%d\n", this->use_list[0]->getVal());
            return;
        }
        
    }
    // Load address,eg: ldr r1, [src1 {,src2}]
    if(this->use_list[0]->isReg()||this->use_list[0]->isVReg())
        fprintf(yyout, "[");

    this->use_list[0]->output();
    if( this->use_list.size() > 1 )//src2不为空
    {
        fprintf(yyout, ", ");
        this->use_list[1]->output();
    }

    if(this->use_list[0]->isReg()||this->use_list[0]->isVReg())
        fprintf(yyout, "]");
    fprintf(yyout, "\n");
}

StoreMInstruction::StoreMInstruction(int op,MachineBlock* p,MachineOperand* src1, MachineOperand* src2, MachineOperand* src3, int cond)
{
    // TODO
    this->op=op;
    this->type = MachineInstruction::STORE;
    this->parent = p;
    this->cond = cond;
    this->use_list.push_back(src1);
    src1->setParent(this);
    this->use_list.push_back(src2);
    src2->setParent(this);
    if (src3!=nullptr)
    {
        this->use_list.push_back(src3);
        src3->setParent(this);
    }
}

void StoreMInstruction::output()
{
    // TODO
    if(op==StoreMInstruction::STR)
    {
        fprintf(yyout, "\tstr ");
    }
    else
    {
        fprintf(yyout, "\tvstr.32 ");
    }

    //eg: str src1, [src2 {,src3}]
    this->use_list[0]->output();
    fprintf(yyout, ", ");
    if(this->use_list[1]->isReg()||this->use_list[1]->isVReg())
    {
        fprintf(yyout, "[");
    }
    this->use_list[1]->output();
    if( this->use_list.size() > 2 )//src3不为空
    {
        fprintf(yyout, ", ");
        this->use_list[2]->output();
    }

    if(this->use_list[1]->isReg()||this->use_list[1]->isVReg())
    {
        fprintf(yyout, "]");
    }
    fprintf(yyout, "\n");
}



MovMInstruction::MovMInstruction(MachineBlock* p, int op,MachineOperand* dst, MachineOperand* src,int cond,MachineOperand* num)
{
    // TODO
    this->type = MachineInstruction::MOV;
    this->parent = p;
    this->op = op;
    this->cond = cond;
    this->def_list.push_back(dst);
    dst->setParent(this);
    dst->setDef(this);
    this->use_list.push_back(src);
    src->setParent(this);
    if (num!=nullptr) {
        this->use_list.push_back(num);
        num->setParent(this);
    }
}

void MovMInstruction::output() 
{
    // TODO mov dst,src或mvn dst,src
    //加：判断mov还是mvn
    if(this->op==MovMInstruction::MOV||this->op==MovMInstruction::MOVLSR||this->op==MovMInstruction::MOVASR||this->op==MovMInstruction::MOVLSL)
    {
        fprintf(yyout, "\tmov");
        
    }
    else if(this->op==MovMInstruction::VMOV)
    {
        fprintf(yyout, "\tvmov");
    }
    PrintCond();
    fprintf(yyout, " ");
    this->def_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[0]->output();
    if (op == MOVLSR) 
    {
        fprintf(yyout, ", LSR");
        this->use_list[1]->output();
    }
    if (op == MOVASR) 
    {
        fprintf(yyout, ", ASR");
        this->use_list[1]->output();
    }
    if (op == MOVLSL) {
        fprintf(yyout, ", LSL");
        this->use_list[1]->output();
    }
    fprintf(yyout, "\n");
}

NeonVcvtInstruction::NeonVcvtInstruction(MachineBlock*p,int op,MachineOperand*dst,MachineOperand*src,int cond)
{
    this->type = MachineInstruction::NEONVCVT;
    this->parent = p;
    this->op = op;
    this->cond = cond;
    this->def_list.push_back(dst);
    dst->setParent(this);
    dst->setDef(this);
    this->use_list.push_back(src);
    src->setParent(this);
}

void NeonVcvtInstruction::output()
{
    if(op==NeonVcvtInstruction::IntToFloat)
    {
        fprintf(yyout, "\tvcvt.f32.s32 ");
    }
    else if(op==NeonVcvtInstruction::FloatToInt)
    {
        fprintf(yyout, "\tvcvt.s32.f32 ");
    }
    this->def_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[0]->output();
    fprintf(yyout, "\n");
}
BranchMInstruction::BranchMInstruction(MachineBlock* p, int op, MachineOperand* dst, int cond)
{
    // TODO
    this->type = MachineInstruction::BRANCH;
    this->parent = p;
    this->cond = cond;
    this->op = op;
    this->use_list.push_back(dst);//注意这个dst能往def_list加！
    dst->setParent(this);
    dst->setDef(this);
}

void BranchMInstruction::output()
{
    // TODO 
    switch (op) {
        case B:
            fprintf(yyout, "\tb");
            PrintCond();
            fprintf(yyout, " ");
            this->use_list[0]->output();
            fprintf(yyout, "\n");
            break;
        case BX:
            fprintf(yyout, "\tbx");
            PrintCond();
            fprintf(yyout, " ");
            this->use_list[0]->output();
            fprintf(yyout, "\n");
            break;
        case BL:
            fprintf(yyout, "\tbl");
            PrintCond();
            fprintf(yyout, " ");
            this->use_list[0]->output();
            fprintf(yyout, "\n");
            break;
    }
}

CmpMInstruction::CmpMInstruction(int op,MachineBlock* p, MachineOperand* src1, MachineOperand* src2, int cond)
{
    // TODO
    this->type = MachineInstruction::CMP;
    this->parent = p;
    this->cond = cond;
    this->op=op;
    p->setCmpCond(cond);//爹块块的条件设置？？
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    src1->setParent(this);
    src2->setParent(this);
}

void CmpMInstruction::output()
{
    // TODO
    // Jsut for reg alloca test
    // delete it after test
    switch (op)
    {
        case CMP:
            fprintf(yyout, "\tcmp ");
            break;
        case VCMP:
            fprintf(yyout,"\tvcmp.f32 ");
            break;
        case VCMPE:
            fprintf(yyout,"\tVCMPE.f32");
            break;
    }
    this->use_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[1]->output();
    fprintf(yyout, "\n");
    if(op==VCMP||op==VCMPE)
    {
        fprintf(yyout,"\tvmrs	APSR_nzcv, FPSCR\n");
    }
}

StackMInstrcuton::StackMInstrcuton(MachineBlock* p, int op, std::vector<MachineOperand*> srcs,MachineOperand* dst,MachineOperand* src ,int cond)//改了参数
{
    // TODO
    this->type = MachineInstruction::STACK;
    this->parent = p;
    this->op = op;
    this->cond = cond;
    
    if(srcs.size()!=0)
    {
        for(long unsigned int i=0;i<srcs.size();i++)
        {
            this->use_list.push_back(srcs[i]);
        }
    }
    if(dst!=nullptr)
    {
        this->use_list.push_back(dst);
        dst->setParent(this);
    }
    if(src!=nullptr)
    {
        this->use_list.push_back(src);
        src->setParent(this);
    }

}

void StackMInstrcuton::output()
{
    // TODO
    if(use_list.size()==0)
        return;
    if(op==PUSH) 
    {
        fprintf(yyout, "\tpush ");
    }
    else if(op==POP)
    {   
        fprintf(yyout, "\tpop ");
    }
    else if(op==VPUSH)
    {
        fprintf(yyout,"\tvpush ");
    }
    else
    {
        fprintf(yyout,"\tvpop ");
    }
    fprintf(yyout, "{");
    for (long unsigned int i = 0; i < use_list.size(); i++)
    {
        this->use_list[i]->output();
        if(i!=use_list.size()-1)
            fprintf(yyout, ", ");
    }
    fprintf(yyout, "}\n");
}

SmullMInstruction::SmullMInstruction(MachineBlock* p,MachineOperand* dst,MachineOperand* dst1,MachineOperand* src1,MachineOperand* src2,int cond) {
    this->parent = p;
    this->type = MachineInstruction::SMULL;
    this->cond = cond;

    this->def_list.push_back(dst);
    dst->setParent(this);
    dst->setDef(this);
    this->def_list.push_back(dst1);
    dst1->setParent(this);
    dst1->setDef(this);

    this->use_list.push_back(src1);
    src1->setParent(this);
    this->use_list.push_back(src2);
    src2->setParent(this);
}

void SmullMInstruction::output() 
{
    fprintf(yyout, "\tumull ");
    this->def_list[0]->output();
    fprintf(yyout, ", ");
    this->def_list[1]->output();
    fprintf(yyout, ", ");
    this->use_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[1]->output();
    fprintf(yyout, "\n");
}



MachineFunction::MachineFunction(MachineUnit* p, SymbolEntry* sym_ptr) 
{ 
    this->parent = p; 
    this->sym_ptr = sym_ptr; 
    this->stack_size = 0;
    this->paramsNum =((FunctionType*)(sym_ptr->getType()))->getParams().size();
};

int MachineBlock::blockseparate = 0;
void MachineBlock::output()
{
    
    int offset = (parent->getSavedRegs().size()+parent->getSavedSRegs().size() + 2) * 4;
    int intparamCnt=dynamic_cast<FunctionType*>(parent->getSym_ptr()->getType())->getIntparamCount();
    int floatparamCnt=dynamic_cast<FunctionType*>(parent->getSym_ptr()->getType())->getFloatparamCount();
    int firstforint = 0;
    int firstforfloat = 1;
    if(floatparamCnt>4&&(floatparamCnt+intparamCnt)%2==1)
        firstforfloat=0;
    int inst_cnt=0;
    if(inst_list.size()!=0)
    {
        fprintf(yyout, ".L%d:\n", this->no);
        for(long unsigned int i=0;i<inst_list.size();i++)
        {
            //当前指令inst_list[i];
            if(inst_list[i]->getinstType()==MachineInstruction::BRANCH)
            {
                if(dynamic_cast<BranchMInstruction*>(inst_list[i])->getop()==BranchMInstruction::BX)
                {
                    auto fp = new MachineOperand(MachineOperand::REG, 11);
                    auto lr = new MachineOperand(MachineOperand::REG, 14);
                    auto cur_inst=new StackMInstrcuton(this,StackMInstrcuton::VPOP,parent->getSavedSRegs());
                    cur_inst->output();
                    cur_inst =new StackMInstrcuton(this, StackMInstrcuton::POP,parent->getSavedRegs(), fp, lr);
                    cur_inst->output();
                    
                }
            }
            //函数参数
            if((inst_list[i]->getinstType()==MachineInstruction::STORE)&&(intparamCnt>4||floatparamCnt>4))
            {
                MachineOperand* operand = inst_list[i]->getUse()[0];
                if (operand->isReg() && operand->getReg() == 3)
                {
                    //寄存器编号等于3，肯定是整型参数
                    if (firstforint<1) 
                    {
                        firstforint++;
                    } 
                    else 
                    {
                        auto fp = new MachineOperand(MachineOperand::REG, 11);
                        auto r3 = new MachineOperand(MachineOperand::REG, 3);
                        auto off =new MachineOperand(MachineOperand::IMM, offset);
                        auto cur_inst = new LoadMInstruction(LoadMInstruction::LDR,this, r3, fp, off);
                        cur_inst->output();
                        offset += 4;
                    }
                }
                if(operand->isReg() && operand->getReg() == 20)
                {
                    if (firstforfloat<1) 
                    {
                        firstforfloat++;
                    }
                    else
                    {
                        auto fp = new MachineOperand(MachineOperand::REG, 11);
                        auto s4 = new MachineOperand(MachineOperand::REG, 20,true);
                        auto off =new MachineOperand(MachineOperand::IMM, offset);
                        auto cur_inst = new LoadMInstruction(LoadMInstruction::VLDR,this, s4, fp, off);
                        cur_inst->output();
                        offset+=4;
                    } 
                }
            }
            if(inst_list[i]->getinstType()==MachineInstruction::BINARY) //2-5 115成功
            {
                if(dynamic_cast<BinaryMInstruction*>(inst_list[i])->getop()==BinaryMInstruction::ADD)
                {   
                    auto dst = inst_list[i]->getDef()[0];
                    auto src1 = inst_list[i]->getUse()[0];
                    if (dst->isReg() && dst->getReg() == 13 && src1->isReg() &&src1->getReg() == 13)
                    {   
                        if((inst_list[i+1]->getinstType()==MachineInstruction::BRANCH)&&(dynamic_cast<BranchMInstruction*>(inst_list[i+1])->getop()==BranchMInstruction::BX))
                        {   
                            int size = parent->AllocSpace(0);
                            auto r1 = new MachineOperand(MachineOperand::REG, 1);
                            auto off =new MachineOperand(MachineOperand::IMM, size);
                            auto curr_inst=new LoadMInstruction(LoadMInstruction::LDR,nullptr, r1, off);
                            curr_inst->output();
                            inst_list[i]->getUse()[1]->setReg(1);
                        }
                    }
                }
            }
            inst_list[i]->output();
            inst_cnt++;
            if(inst_cnt>=500)
            {
                fprintf(yyout, "\tb .B%d\n", blockseparate);
                fprintf(yyout, ".LTORG\n");
                parent->getParent()->printGlobal();
                fprintf(yyout, ".B%d:\n", blockseparate++);
                inst_cnt=0;
            }
        }
    }
}

void MachineBlock::insertBefore(MachineInstruction* a, MachineInstruction* b) 
{
    //机器指令A插在机器指令B的前面
    auto it = find(inst_list.begin(), inst_list.end(), b);
    if (it != inst_list.end()){
        inst_list.insert(it, a);
        a->setParent(b->getParent());
    }        
}
void MachineBlock::removeInst(MachineInstruction* inst) 
{
    auto it = find(inst_list.begin(), inst_list.end(), inst);
    if (it != inst_list.end())
        inst_list.erase(it);
}

std::vector<MachineOperand*> MachineFunction::getSavedRegs() 
{
    std::vector<MachineOperand*> regs;
    for (auto it = saved_regs.begin(); it != saved_regs.end(); it++) 
    {
        auto reg = new MachineOperand(MachineOperand::REG, *it);
        regs.push_back(reg);
    }
    return regs;
}
std::vector<MachineOperand*> MachineFunction::getSavedSRegs() 
{   
    std::vector<MachineOperand*> regs;
    if(saved_sregs.empty())
        return regs;
    int Smin=*saved_sregs.begin();
    int Smax=*(--saved_sregs.end());
    Smin=20;
    if((Smax-Smin+1)%2!=0)
        Smax++;
    for (int i=Smin;i<=Smax;i++) 
    {
        auto reg = new MachineOperand(MachineOperand::REG, i,true);
        regs.push_back(reg);
    }
    return regs;
}

void MachineFunction::output()
{   
    //const char * func_name= this->sym_ptr->toStr().c_str() + 1;   //不然2-4 094函数名会乱码？
    fprintf(yyout, "\t.global %s\n",this->sym_ptr->toStr().c_str() + 1);  
    fprintf(yyout, "\t.type %s , %%function\n",this->sym_ptr->toStr().c_str() + 1);
    fprintf(yyout, "%s:\n", this->sym_ptr->toStr().c_str() + 1);
    // TODO
    /* Hint:
    *  1. Save fp
    *  2. fp = sp
    *  3. Save callee saved register
    *  4. Allocate stack space for local variable */
    
    // Traverse all the block in block_list to print assembly code.
    auto fp = new MachineOperand(MachineOperand::REG, 11);
    auto sp = new MachineOperand(MachineOperand::REG, 13);
    auto lr = new MachineOperand(MachineOperand::REG, 14);
    auto stack=new StackMInstrcuton(nullptr,StackMInstrcuton::PUSH,getSavedRegs(),fp,lr);
    stack->output();
    
    stack=new StackMInstrcuton(nullptr,StackMInstrcuton::VPUSH,getSavedSRegs());
    stack->output();
    auto mov=new MovMInstruction(nullptr,MovMInstruction::MOV,fp,sp);
    mov->output();
    int off = AllocSpace(0);
    if (off % 8 != 0) 
    {
        off = AllocSpace(4);
    }
    auto size = new MachineOperand(MachineOperand::IMM, off);
    auto interval_reg=new MachineOperand(MachineOperand::REG, 4);

    auto load_inst=new LoadMInstruction(LoadMInstruction::LDR,nullptr, interval_reg, size);
    load_inst->output();
    auto stackchange=new BinaryMInstruction(nullptr, BinaryMInstruction::SUB, sp, sp, interval_reg);
    stackchange->output();
    int count = 0;
    for(long unsigned int i=0;i<block_list.size();i++)
    {
        block_list[i]->output();
        count += block_list[i]->getSize();
        if(count > 100)
        {
            fprintf(yyout, "\tb .F%d\n", parent->getN());
            fprintf(yyout, ".LTORG\n");
            parent->printGlobal();
            fprintf(yyout, ".F%d:\n", parent->getN()-1);
            count = 0;
        }
    }
}

void MachineUnit::PrintGlobalDecl()
{
    // TODO:
    // You need to print global variable/const declarition code;
    std::vector<int> constIdx;
    std::vector<int> zeroIdx;
    if (!global_id_list.empty())
    {
        fprintf(yyout, "\t.data\n");
    }
    for (long unsigned int i = 0; i < global_id_list.size(); i++) 
    {
        IdentifierSymbolEntry* se = (IdentifierSymbolEntry*)global_id_list[i];
        auto seType=se->getType();
        if(seType->isInt()&&(seType->whetherConst()))//浮点
        {
            //const常量
            constIdx.push_back(i);
        }
        else if (se->ifquan0()) 
        {
            zeroIdx.push_back(i);
        }
        else
        {
            fprintf(yyout, "\t.global %s\n", se->toStr().c_str()); 
            fprintf(yyout, "\t.align 4\n");
            fprintf(yyout, "\t.size %s, %d\n", se->toStr().c_str(),se->getType()->getOSize() / 8);
            fprintf(yyout, "%s:\n", se->toStr().c_str());
            if (!se->getType()->isArray())
            {
                if(se->getType()->isFloat())
                {
                    FloattoUint32 coverter;
                    coverter.floatnumber=(float)(se->getValue());
                    fprintf(yyout, "\t.word %d\n", (int)coverter.IEEEnumber);

                }
                else
                {
                    fprintf(yyout, "\t.word %d\n", (int)(se->getValue()));
                }
            }
            else 
            {  /*
                
                int* p = se->getIntArrayValue();//浮点
                for (int i = 0; i < n; i++) {
                    fprintf(yyout, "\t.word %d\n", p[i]);
                }
                */
                int n = se->getType()->getOSize() / 32;
                if(dynamic_cast<ArrayType*>(seType)->getFinalType()->isInt())
                {
                    int* p = se->getIntArrayValue();//浮点
                    for (int i = 0; i < n; i++) {
                        fprintf(yyout, "\t.word %d\n", p[i]);
                    }
                }
                else
                {
                    float *p=se->getFloatArrayValue();
                    for(int i=0;i<n;i++)
                    {
                        FloattoUint32 coverter;
                        coverter.floatnumber=(float)(p[i]);
                        fprintf(yyout,"\t.word %d\n",(int)coverter.IEEEnumber);
                    }
                }
            }
        }
    }
    //here
    if(constIdx.size()!=0)
    {
        fprintf(yyout, "\t.section .rodata\n");
        for(long unsigned int i=0;i<constIdx.size();i++)
        {
            IdentifierSymbolEntry* se = (IdentifierSymbolEntry*)global_id_list[constIdx[i]];
            fprintf(yyout, "\t.global %s\n", se->toStr().c_str()); 
            fprintf(yyout, "\t.align 4\n");
            fprintf(yyout, "\t.size %s, %d\n", se->toStr().c_str(),se->getType()->getOSize() / 8);//换
            fprintf(yyout, "%s:\n", se->toStr().c_str());
            if (!se->getType()->isArray()) 
            {
                fprintf(yyout, "\t.word %d\n", (int)(se->getValue()));//浮点
            } 
            else 
            {
                int n = se->getType()->getOSize() / 32;
                int* p = se->getIntArrayValue();//浮点
                for (int i = 0; i < n; i++) 
                {
                    fprintf(yyout, "\t.word %d\n", p[i]);
                }
            }
        }
    }
    if (zeroIdx.size()!=0) {

        for(long unsigned int i=0;i<zeroIdx.size();i++)
        {
            IdentifierSymbolEntry* se = dynamic_cast<IdentifierSymbolEntry*>(global_id_list[zeroIdx[i]]);
            if(se->getType()->isArray())
            {
                fprintf(yyout, "\t.comm %s, %d, 4\n", se->toStr().c_str(),se->getType()->getOSize() / 8);
            }
        }
    }
}
void MachineUnit::printGlobal(){//新加
    for (auto s : global_id_list) {
        IdentifierSymbolEntry* se = (IdentifierSymbolEntry*)s;
        fprintf(yyout, "addr_%s%d:\n", se->toStr().c_str(), n);
        fprintf(yyout, "\t.word %s\n", se->toStr().c_str());
    }
    n++;
}
void MachineUnit::output()
{
    // TODO
    /* Hint:
    * 1. You need to print global variable/const declarition code;
    * 2. Traverse all the function in func_list to print assembly code;
    * 3. Don't forget print bridge label at the end of assembly code!! */
    fprintf(yyout, "\t.cpu cortex-a72\n");
    fprintf(yyout, "\t.arch armv8-a\n");
    fprintf(yyout, "\t.fpu vfpv3-d16\n");
    fprintf(yyout, "\t.arch_extension crc\n");
    
    fprintf(yyout, "\t.arm\n");
    PrintGlobalDecl();
    fprintf(yyout, "\t.text\n");//加
    for(auto iter : func_list)
        iter->output();
    printGlobal();//新加
}
