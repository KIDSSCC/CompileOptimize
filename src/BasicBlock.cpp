#include "BasicBlock.h"
#include "Function.h"
#include <algorithm>
#include<Type.h>

extern FILE* yyout;

// insert the instruction to the front of the basicblock.
void BasicBlock::insertFront(Instruction *inst)
{
    //head->getNext()是真正的第一条指令，相当于插在了整个指令列表的最前面
    insertBefore(inst, head->getNext());
}

// insert the instruction to the back of the basicblock.
void BasicBlock::insertBack(Instruction *inst) 
{
    //head其实是指令列表结束的地方，相当于插入在了整个指令列表的最后面
    insertBefore(inst, head);
}

// insert the instruction dst before src.
void BasicBlock::insertBefore(Instruction *dst, Instruction *src)
{
    //dst是新来的，src是原有的，这里需要做的是从原有的列表中找到src的位置，修改之间的关系。
    // Todo

    dst->setNext(src);
    dst->setPrev(src->getPrev());
    src->getPrev()->setNext(dst);
    src->setPrev(dst);

    dst->setParent(this);
}

//dst插入在src的后面
void BasicBlock::insertAfter(Instruction* dst, Instruction* src) {
    dst->setNext(src->getNext());
    src->getNext()->setPrev(dst);

    dst->setPrev(src);
    src->setNext(dst);

    dst->setParent(this);
}

// remove the instruction from intruction list.
void BasicBlock::remove(Instruction *inst)
{
    //删除一条指令。
    inst->getPrev()->setNext(inst->getNext());
    inst->getNext()->setPrev(inst->getPrev());
}

void BasicBlock::output() const
{
    fprintf(yyout, "B%d:", no);
    //打印前驱和指令列表，不打印后继吗？
    if (!pred.empty())
    {
        fprintf(yyout, "%*c; preds = %%B%d", 32, '\t', pred[0]->getNo());
        for (auto i = pred.begin() + 1; i != pred.end(); i++)
            fprintf(yyout, ", %%B%d", (*i)->getNo());
    }
    fprintf(yyout, "\n");
    for (auto i = head->getNext(); i != head; i = i->getNext())
        i->output();
}

void BasicBlock::addSucc(BasicBlock *bb)
{
    succ.push_back(bb);
}

// remove the successor basicclock bb.
void BasicBlock::removeSucc(BasicBlock *bb)
{
    succ.erase(std::find(succ.begin(), succ.end(), bb));
}

void BasicBlock::addPred(BasicBlock *bb)
{
    pred.push_back(bb);
}

// remove the predecessor basicblock bb.
void BasicBlock::removePred(BasicBlock *bb)
{
    pred.erase(std::find(pred.begin(), pred.end(), bb));
}

void BasicBlock::genMachineCode(AsmBuilder* builder) 
{
    /*
    对中间代码基本块进行汇编代码的生成
    和对函数的操作类似，创建新的汇编代码基本块单元并设置为正在翻译
    */
    
    auto cur_func = builder->getFunction();
    auto cur_block = new MachineBlock(cur_func, no);
    builder->setBlock(cur_block);
    /*
    head是中间代码基本块所属的指令入口，（head本身为空），因此从head->next开始。
    为每一条指令生成其汇编代码
    */

    for (auto i = head->getNext(); i != head; i = i->getNext())
    {
        i->genMachineCode(builder);
    }
    /*
    完成上面的工作，将当前的汇编代码基本块插入到当前函数的基本块列表中。
    */
    cur_func->InsertBlock(cur_block);
}
//构造函数
BasicBlock::BasicBlock(Function *f)
{
    this->no = SymbolTable::getLabel();
    f->insertBlock(this);
    parent = f;
    head = new DummyInstruction();
    head->setParent(this);
    
}

BasicBlock::~BasicBlock()
{
    //删除所有指令和前驱后继关系。 
    Instruction *inst;
    inst = head->getNext();
    while (inst != head)
    {
        Instruction *t;
        t = inst;
        inst = inst->getNext();
        delete t;
    }
    for(auto &bb:pred)
        bb->removeSucc(this);
    for(auto &bb:succ)
        bb->removePred(this);
    parent->remove(this);
}
