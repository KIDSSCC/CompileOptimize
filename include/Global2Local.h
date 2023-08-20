#include "Unit.h"

using namespace std;

class Global2Local{
public:
    Unit* unit;
    //globals：存储每一个全局变量，在哪些函数中被哪些指令使用了
    //usedGlobals：每一个函数中使用了哪些全局变量
    //read：函数中所有的Load指令使用了哪些全局变量
    //write：函数中所有的store指令使用了哪些全局变量
    map<SymbolEntry*, map<Function*, vector<Instruction*>>> globals;
    map<Function*, set<SymbolEntry*>> usedGlobals;
    map<Function*, set<SymbolEntry*>> read;
    map<Function*, set<SymbolEntry*>> write;

    Global2Local(Unit* unit);
    void pass();
    void pass(Function* function);
    void calculateGlobals();
    void unstoreGlobal2Const();
};