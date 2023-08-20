#include "Div_Mod.h"
#include<cmath>
#include "utils.h"
using namespace std;

Div_Mod::Div_Mod(MachineUnit* unit)
{
    this->unit=unit;
}

void Div_Mod::pass()
{
    for(auto func = unit->begin(); func != unit->end(); func++)
        progress(*func);
}

void Div_Mod::progress(MachineFunction* func)
{
    auto all_Mblocks=func->getBlocks();
    for(int i=0;i<all_Mblocks.size();i++)
    {
        vector<MachineInstruction*> insts;
        for(auto Minst:all_Mblocks[i]->getInsts())
            insts.push_back(Minst);
        for(auto m_inst : insts)
        {
            
            if(m_inst->MulOrDivImm())
            {
                int divisor = m_inst->getUse()[1]->getDef()->getUse()[0]->getVal();
                //低位0的数量
                int s = ctz(divisor);
                auto off1 = new MachineOperand(MachineOperand::IMM, 31);
                auto tmp = new MachineOperand(MachineOperand::VREG,SymbolTable::getLabel());
                if(divisor == (int(1) << s))
                {
                    //2的整数次幂
                    auto off = new MachineOperand(MachineOperand::IMM, s);
                    //MachineInstruction* inst1 = new MovMInstruction(nullptr, MovMInstruction::MOVLSR,m_inst->getDef()[0], m_inst->getUse()[0], MachineInstruction::NONE, off1);
                    MachineInstruction* inst2 = new MovMInstruction(nullptr, MovMInstruction::MOVASR,m_inst->getDef()[0], m_inst->getUse()[0], MachineInstruction::NONE, off);                   
                    //MachineInstruction* inst3 = new BinaryMInstruction(nullptr, BinaryMInstruction::ADD,m_inst->getDef()[0], m_inst->getDef()[0], tmp);
                    //all_Mblocks[i]->insertBefore(inst1, m_inst);
                    all_Mblocks[i]->insertBefore(inst2, m_inst);
                    //all_Mblocks[i]->insertBefore(inst3, m_inst);
                    all_Mblocks[i]->removeInst(m_inst);
                }

                else
                {
                    int a = 0;
                    if(divisor % (int(1) << s) == 0)
                    {
                        divisor = divisor / (int(1) << s);
                        a = s;
                    }
                    Multiplier multi = chooseMultiplier(divisor, 32);
                    if(multi.m < ((long long)(1) << 31))
                    {
                        auto m = new MachineOperand(MachineOperand::IMM, multi.m);
                        auto l = new MachineOperand(MachineOperand::IMM, multi.l+a);
                        auto rh = new MachineOperand(MachineOperand::VREG,SymbolTable::getLabel());
                        auto rl = new MachineOperand(MachineOperand::VREG,SymbolTable::getLabel());

                        MachineInstruction* inst1 = new LoadMInstruction( LoadMInstruction::LDR,nullptr, tmp, m);
                        MachineInstruction* inst2 = new SmullMInstruction(nullptr, rl, rh, m_inst->getUse()[0], tmp);
                        MachineInstruction* inst3 = new MovMInstruction(nullptr, MovMInstruction::MOVASR,m_inst->getDef()[0], rh, MachineInstruction::NONE, l);
                        MachineInstruction* inst4 = new MovMInstruction(nullptr, MovMInstruction::MOVLSR,tmp, rh, MachineInstruction::NONE, off1);
                        MachineInstruction* inst5 = new BinaryMInstruction(nullptr, BinaryMInstruction::ADD,m_inst->getDef()[0], m_inst->getDef()[0], tmp);
                        
                        all_Mblocks[i]->insertBefore(inst1, m_inst);
                        all_Mblocks[i]->insertBefore(inst2, m_inst);
                        all_Mblocks[i]->insertBefore(inst3, m_inst);
                        all_Mblocks[i]->insertBefore(inst4, m_inst);
                        all_Mblocks[i]->insertBefore(inst5, m_inst);
                        all_Mblocks[i]->removeInst(m_inst);
                    }
                }
            }
            else if(m_inst->getinstType()==MachineInstruction::BINARY&&m_inst->getop()==4)
            {
                int divisor = m_inst->getUse()[1]->getDef()->getUse()[0]->getVal();
                if((divisor > 0) && ((divisor & (divisor-1)) == 0))
                {
                    auto off = new MachineOperand(MachineOperand::IMM, divisor-1);
                    auto off1 = new MachineOperand(MachineOperand::IMM, 31);
                    auto tmp = new MachineOperand(MachineOperand::VREG,SymbolTable::getLabel());

                    MachineInstruction* inst1 = new BinaryMInstruction(nullptr, BinaryMInstruction::AND,m_inst->getDef()[0], m_inst->getUse()[0], off);
                    MachineInstruction* inst2 = new MovMInstruction(nullptr, MovMInstruction::MOVLSR,tmp, m_inst->getUse()[0], MachineInstruction::NONE, off1);
                    MachineInstruction* inst3 = new BinaryMInstruction(nullptr, BinaryMInstruction::ADD,m_inst->getDef()[0], m_inst->getDef()[0], tmp);
                    all_Mblocks[i]->insertBefore(inst1, m_inst);
                    all_Mblocks[i]->insertBefore(inst2, m_inst);
                    all_Mblocks[i]->insertBefore(inst3, m_inst);
                    all_Mblocks[i]->removeInst(m_inst);
                }
            }
        }
    }
}
