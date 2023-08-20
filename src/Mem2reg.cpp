#include "Mem2reg.h"

Mem2reg::Mem2reg(Unit* unit)
{
    this->unit=unit;
}

void Mem2reg::pass()
{
    auto iter = unit->begin();
    while(iter!=unit->end())
    {
        pass(*iter);
        iter++;
    }
}

void Mem2reg::pass(Function* function)
{
    checkCondBranch(function);

}

void Mem2reg::checkCondBranch(Function* func) {
    for (auto block : func->getBlockList()) {
        auto in = block->rbegin();
        if (in->isCond()) {
            auto cond = (CondBrInstruction*)in;
            auto trueBlock = cond->getTrueBranch();
            auto falseBlock = cond->getFalseBranch();
            if (trueBlock == falseBlock) {
                std::cout<<"here\n";
                block->removeSucc(trueBlock);
                trueBlock->removePred(block);
                new UncondBrInstruction(trueBlock, block);
                block->remove(in);
            }
        }
    }
}