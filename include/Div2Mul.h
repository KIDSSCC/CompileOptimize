#include "MachineCode.h"

class Div2Mul {
    MachineUnit* unit;
   public:
    Div2Mul(MachineUnit* unit);
    void pass();
    void div2mul(MachineFunction *);
};