#include "Global2Local.h"
#include <numeric>
#include "Type.h"
using namespace std;


Global2Local::Global2Local(Unit *unit)
{
    this->unit=unit;
}

void Global2Local::pass() {
    calculateGlobals();
    auto iter = unit->begin();
    while (iter != unit->end())
    {
        pass(*iter++);
    }
    unstoreGlobal2Const();
}
void Global2Local::pass(Function* func)
{

    auto all_globals = usedGlobals[func];
    if (all_globals.empty())
        return;
    map<SymbolEntry*, Operand*> g2l;
    for(auto ever_var:all_globals)
    {
        auto virtul_type = ((PointerType*)(ever_var->getType()))->getType();
        if(virtul_type->whetherConst())
        {
            continue;
        }
        if(virtul_type->isArray())
            continue;
        //当前的全局变量在当前函数中应用的所有语句
        auto insts=this->globals[ever_var][func];
        auto newSymbol = new TemporarySymbolEntry(new PointerType(virtul_type),SymbolTable::getLabel());
        auto newAddr = new Operand(newSymbol);
        g2l[ever_var] = newAddr;
        //向入口块进行插入,将addr中的数据加载上来，存储在newAddr中
        auto entry_block = func->getEntry();
        auto alloc =new AllocaInstruction(newAddr, new TemporarySymbolEntry(virtul_type, 0));
        alloc->setParent(entry_block);
        entry_block->insertFront(alloc);

        SymbolEntry* addr_se=new IdentifierSymbolEntry(*((IdentifierSymbolEntry*)ever_var));
        addr_se->setType(new PointerType(virtul_type));
        auto addr = new Operand(addr_se);
        auto dst = new Operand(new TemporarySymbolEntry(virtul_type, SymbolTable::getLabel()));
        auto load_inst = new LoadInstruction(dst, addr);
        load_inst->setParent(entry_block);
        for (auto inst = entry_block->begin(); inst != entry_block->end(); inst = inst->getNext())
        {
            if (!inst->isAlloc()) {
                entry_block->insertBefore(load_inst, inst);
                break;
            }
        }
        auto store_inst = new StoreInstruction(newAddr, dst);
        store_inst->setParent(entry_block);
        entry_block->insertAfter(store_inst, load_inst);
        for (auto inst : insts) 
        {
            if (inst->isLoad()) 
            {
                auto addr = inst->getUse()[0];
                inst->replaceUse(addr, newAddr);
            } else if (inst->isStore()) 
            {
                auto addr = inst->getUse()[0];
                inst->replaceUse(addr, newAddr);
            } 
        }
    }
    //函数返回前，将所有进行过写操作的变量从局部写回全局
    for(auto block:func->getBlockList())
    {
        auto inst = block->rbegin();
        if(inst->isRet())
        {
            for (auto itea : write[func])
            {
                auto virtul_type = ((PointerType*)(itea->getType()))->getType();
                if (!g2l[itea])
                    continue;
                if (virtul_type->isArray())
                    continue;
                auto addr_se =new IdentifierSymbolEntry(*((IdentifierSymbolEntry*)itea));
                addr_se->setType(new PointerType(virtul_type));
                auto addr = new Operand(addr_se);

                auto dst = new Operand(new TemporarySymbolEntry(virtul_type, SymbolTable::getLabel()));
                auto load_inst = new LoadInstruction(dst, g2l[itea]);
                load_inst->setParent(block);
                block->insertBefore(load_inst, inst);

                auto store_inst = new StoreInstruction(addr, dst);
                store_inst->setParent(block);
                block->insertBefore(store_inst, inst);
            } 
        }
    }

    for(auto block : func->getBlockList())
    {
        for (auto inst = block->begin(); inst != block->end(); inst = inst->getNext())
        {
            if (inst->isFuncCall())
            {
                auto target_func = ((IdentifierSymbolEntry*)(((FuncCallInstruction*)inst)->getFuncEntry()))->getFunction();
                if (!target_func)
                    continue;
                //被调函数所将要读取的全局变量进行写回
                for (auto ever_var : read[target_func])
                {
                    auto virtul_type = ((PointerType*)(ever_var->getType()))->getType();
                    if (virtul_type->isArray())
                        continue;
                    if (!g2l[ever_var])
                        continue;
                    auto addr_se =new IdentifierSymbolEntry(*((IdentifierSymbolEntry*)ever_var));
                    addr_se->setType(new PointerType(virtul_type));
                    auto addr = new Operand(addr_se);

                    auto dst = new Operand(new TemporarySymbolEntry(virtul_type, SymbolTable::getLabel()));
                    auto load_inst = new LoadInstruction(dst, g2l[ever_var]);
                    load_inst->setParent(block);
                    block->insertBefore(load_inst, inst);
                    auto store_inst = new StoreInstruction(addr, dst);
                    store_inst->setParent(block);
                    block->insertBefore(store_inst, inst);
                }
                for (auto ever_var : write[target_func]) {
                    auto virtul_type = ((PointerType*)(ever_var->getType()))->getType();
                    if (virtul_type->isArray())
                        continue;
                    if (!g2l[ever_var])
                        continue;
                    auto addr_se =new IdentifierSymbolEntry(*((IdentifierSymbolEntry*)ever_var));
                    addr_se->setType(new PointerType(virtul_type));
                    auto addr = new Operand(addr_se);

                    auto dst = new Operand(new TemporarySymbolEntry(virtul_type, SymbolTable::getLabel()));
                    auto load_inst = new LoadInstruction(dst, addr);
                    load_inst->setParent(block);
                    auto store = new StoreInstruction(g2l[ever_var], dst);
                    store->setParent(block);
                    block->insertAfter(store, inst);
                    block->insertAfter(load_inst, inst);
                }
            }
        }
    }
    return ;
}

void Global2Local::calculateGlobals()
{
    //统计所有函数中使用的全局变量
    map<Function*, int> func2index;
    int index = 0;
    for (auto curr_func = unit->begin(); curr_func != unit->end(); curr_func++)
    {
        func2index[*curr_func] = index;
        index+=1;
        for (auto block : (*curr_func)->getBlockList())
        {
            for (auto inst = block->begin(); inst != block->end();inst = inst->getNext())
            {
                for (auto used_oper : inst->getUse())
                {
                    if (used_oper->isGlobal())
                    {
                        //entry是全局变量的指针标签地址，一个指针类型的entry
                        auto entry = used_oper->getEntry();
                        globals[entry][*curr_func].push_back(inst);
                        usedGlobals[*curr_func].insert(entry);
                        if (inst->isLoad())
                            read[*curr_func].insert(entry);
                        else if (inst->isStore())
                            write[*curr_func].insert(entry);
                    }
                }
            }
        }
    }
    //以函数数量形成的二维矩阵,每一行是主调，每一列是被调
    vector<vector<int>> matrix(index, vector<int>(index));
    for (auto curr_func = unit->begin(); curr_func != unit->end(); curr_func++)
    {
        //all_preds是function——>std::vector<Instruction*>的映射
        auto all_preds=(*curr_func)->getPreds();
        for(auto itea2 :all_preds)
        {
            matrix[func2index[itea2.first]][func2index[*(curr_func)]]=1;
        }

    }
    //计算每个函数的出度,每个函数调用了多少函数
    vector<int> outDeg(index, 0);
    for (int i = 0; i < index; i++) 
    {
        matrix[i][i] = 0;
        for(int j=0;j<index;j++)
        {
            outDeg[i]+=matrix[i][j];
        }
    }
    //拓扑排序，更新了各个函数所读写的全局变量的范围
    int finish = 0;
    while (finish < index) {
        //找到出度为0，没有调用过其他函数的函数
        int i;
        for (i = 0; i < index; i++)
        {
            if (outDeg[i] == 0)
                break;
        }
        outDeg[i] = -1;
        finish++;
        auto func = *(unit->begin() + i);
        for (auto itea : func->getPreds()) {
            read[itea.first].insert(read[func].begin(), read[func].end());
            write[itea.first].insert(write[func].begin(), write[func].end());
            outDeg[func2index[itea.first]]--;
        }
    }
}

void Global2Local::unstoreGlobal2Const()
{
    for(auto pair:globals)
    {
        auto virtul_type = ((PointerType*)(pair.first->getType()))->getType();
        if (virtul_type->isArray())
        {
            bool store = false;
            for(auto itea :pair.second)
            {
                //itea:map<Function*, vector<Instruction*>>
                for (auto inst : itea.second)
                {
                    if(inst->isGep())
                    {
                        auto def = inst->getDef();
                        for (auto itea2 = def->use_begin(); itea2 != def->use_end();itea2++)
                        {
                            if ((*itea2)->isGep())
                            {
                                auto gepDef = (*itea2)->getDef();
                                for (auto itea3 = gepDef->use_begin();itea3 != gepDef->use_end(); itea3++)
                                {
                                    if ((*itea3)->isGep() || (*itea3)->isStore()) {
                                        // 最多考虑二维数组吧
                                        store = true;
                                        break;
                                    }
                                }
                                if (store)
                                    break;
                            }
                            if ((*itea2)->isStore()) 
                            {
                                store = true;
                                break;
                            }
                        }
                        if (store)
                            break;
                    }
                    if (!store)
                    {
                        auto name = pair.first->toStr();
                        auto entry = identifiers->lookup(name);
                        vector<Instruction*> rmvList;
                        for (auto it1 : pair.second)
                        {
                            for (auto in : it1.second)
                            {
                                if (in->isGep())
                                {
                                    auto def = in->getDef();
                                    auto idx = in->getUse()[1];
                                    if (!idx->getEntry()->getType()->whetherConst())
                                        continue;
                                    int i = idx->getConstVal();
                                    auto elementType =((ArrayType*)virtul_type)->getArrayType();
                                    for (auto it2 = def->use_begin();it2 != def->use_end(); it2++)
                                    {
                                        if ((*it2)->isGep())
                                        {
                                            i *= ((ArrayType*)elementType)->getArrayNum();
                                            auto idx = (*it2)->getUse()[1];
                                            if (!idx->getEntry()->getType()->whetherConst())
                                                break;
                                            i += idx->getConstVal();
                                            elementType = ((ArrayType*)elementType)->getArrayType();
                                            auto gepDef = (*it2)->getDef();
                                            for (auto it3 = gepDef->use_begin();it3 != gepDef->use_end(); it3++) 
                                            {
                                                if ((*it3)->isLoad())
                                                {
                                                    auto loadDst = (*it3)->getDef();
                                                    float* valArr =((IdentifierSymbolEntry*)entry)->getFloatArrayValue();
                                                    double val = 0;
                                                    if (valArr!=nullptr)
                                                        val = valArr[i];
                                                    auto operand = new Operand(new ConstantSymbolEntry(elementType, val));
                                                    while (loadDst->use_begin() !=loadDst->use_end()) 
                                                    {
                                                        auto use =*(loadDst->use_begin());use->replaceUse(loadDst,operand);
                                                    }
                                                    rmvList.push_back(*it3);
                                                }
                                            }
                                        }
                                        if ((*it2)->isLoad()) {
                                            auto loadDst = (*it2)->getDef();
                                            float* valArr =((IdentifierSymbolEntry*)entry)->getFloatArrayValue();
                                            double val = 0;
                                            if (valArr)
                                                val = valArr[i];
                                            auto operand =new Operand(new ConstantSymbolEntry(elementType, val));
                                            while (loadDst->use_begin() !=loadDst->use_end()) 
                                            {
                                                auto use = *(loadDst->use_begin());
                                                use->replaceUse(loadDst, operand);
                                            }
                                            rmvList.push_back(*it2);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }
        else {
            bool store = false;
            for (auto it1 : pair.second)
                for (auto in : it1.second)
                    if (in->isStore()) {
                        store = true;
                        break;
                    }
            if (!store) 
            {
                auto name = pair.first->toStr();
                auto entry = identifiers->lookup(name);
                auto operand = new Operand(new ConstantSymbolEntry(virtul_type, ((IdentifierSymbolEntry*)entry)->getValue()));
                for (auto it1 : pair.second)
                {
                    for (auto in : it1.second) 
                    {
                        auto def = in->getDef();
                        while (def->use_begin() != def->use_end()) 
                        {
                            auto use = *(def->use_begin());
                            use->replaceUse(def, operand);
                        }
                        in->getParent()->remove(in);
                    }
                }
            }
        }
    }
    /*
    for (auto it : globals)
    {
        auto type = ((PointerType*)(it.first->getType()))->getType();
        if (type->isArray())
        {
            bool store = false;
            for (auto it1 : it.second)
            {
                for (auto in : it1.second)
                {
                    if (in->isGep())
                    {
                        auto def = in->getDef();
                        for (auto it2 = def->use_begin(); it2 != def->use_end();it2++)
                        {
                            if ((*it2)->isGep())
                            {
                                auto gepDef = (*it2)->getDef();
                                for (auto it3 = gepDef->use_begin();it3 != gepDef->use_end(); it3++)
                                {
                                    if ((*it3)->isGep() || (*it3)->isStore()) {
                                        // 最多考虑二维数组吧
                                        store = true;
                                        break;
                                    }
                                }
                                if (store)
                                    break;
                            }
                            if ((*it2)->isStore()) {
                                store = true;
                                break;
                            }
                        }
                        if (store)
                            break;
                    }
                    if (!store)
                    {
                        auto name = it.first->toStr();
                        auto entry = identifiers->lookup(name);
                        vector<Instruction*> rmvList;
                        for (auto it1 : it.second)
                        {
                            for (auto in : it1.second)
                            {
                                if (in->isGep())
                                {
                                    auto def = in->getDef();
                                    auto idx = in->getUse()[1];
                                    if (!idx->isConst())
                                        continue;
                                    int i = idx->getConstVal();
                                }
                            }
                        }
                    }
                }
            }
        }
    }*/
}