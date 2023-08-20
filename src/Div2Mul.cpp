#include "Div2Mul.h"
#include<cmath>
using namespace std;

Div2Mul::Div2Mul(MachineUnit* unit)
{
    this->unit=unit;
}

void Div2Mul::pass()
{
    for(auto func = unit->begin(); func != unit->end(); func++)
        div2mul(*func);
}

void Div2Mul::div2mul(MachineFunction* func)
{
    auto all_Mblocks=func->getBlocks();
    for(int i=0;i<all_Mblocks.size();i++)
    {
        vector<MachineInstruction*> insts;
        for(auto Minst:all_Mblocks[i]->getInsts())
            insts.push_back(Minst);
        for(auto m_inst : insts)
        {
            /*
            if(m_inst->isDivConst())
            {

            }*/
        }
    }
}
