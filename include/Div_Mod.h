#include "MachineCode.h"

class Div_Mod {
    MachineUnit* unit;
   public:
    Div_Mod(MachineUnit* unit);
    void pass();
    void progress(MachineFunction *);
};