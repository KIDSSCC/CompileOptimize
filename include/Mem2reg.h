#include "Unit.h"
#include <stack>
class Mem2reg{
public:
    Unit* unit;
    std::vector<AllocaInstruction*> allocaIns;
    std::map<Operand*, std::stack<Operand*>> stacks;
    std::vector<BinaryInstruction*> addZeroIns;

    Mem2reg(Unit* unit);
    void pass();
    void pass(Function* function);
    void checkCondBranch(Function* function);
};