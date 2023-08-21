#include <iostream>
#include <string.h>
#include <unistd.h>
#include "Ast.h"
#include "Unit.h"
#include "MachineCode.h"
#include "LinearScan.h"
#include "Mem2reg.h"
#include "Global2Local.h"
#include "Div_Mod.h"
using namespace std;

Ast ast;
Unit unit;
MachineUnit mUnit;
extern FILE *yyin;
extern FILE *yyout;

int yyparse();

char outfile[256] = "a.out";
bool dump_tokens;//应该是打印词法分析结果的设置，在这一版main中已经取消了这一项
bool dump_ast;//是否打印节点树
bool dump_ir;//是否打印中间代码
bool dump_asm;//是否打印汇编代码

int main(int argc, char *argv[])
{
    // for(int i=0;i<argc;i++)
    // {
    //     std::cout<<"argv["<<i<<"] is "<<argv[i]<<std::endl;
    // }
    int opt;
    while ((opt = getopt(argc, argv, "Siato:O:")) != -1)
    {
        switch (opt)
        {
        case 'o':
            strcpy(outfile, optarg);
            break;
        case 'a':
            dump_ast = true;
            break;
        case 't':
            dump_tokens = true;
            break;
        case 'i':
            dump_ir = true;
            break;
        case 'S':
            dump_asm = true;
            break;
        case 'O':
            break;
        default:
            fprintf(stderr, "Usage: %s [-o outfile] infile\n", argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }
    
    //异常处理
    if (optind >= argc)
    {
        fprintf(stderr, "no input file\n");
        exit(EXIT_FAILURE);
    }
    if (!(yyin = fopen(argv[optind], "r")))
    {
        fprintf(stderr, "%s: No such file or directory\nno input file\n", argv[optind]);
        exit(EXIT_FAILURE);
    }
    if (!(yyout = fopen(outfile, "w")))
    {
        fprintf(stderr, "%s: fail to open output file\n", outfile);
        exit(EXIT_FAILURE);
    }

    //词法分析，语法分析
    yyparse();
    ast.typeCheck(nullptr); 
    if(dump_ast)
    {
        ast.output();
    }
    //生成中间代码
    ast.genCode(&unit);
    
    // Mem2reg mem2regPass(&unit);
    // mem2regPass.pass();
    Global2Local g2l(&unit);
    g2l.pass();


    if(dump_ir)
    {
        unit.output();
    }
    
    //生成汇编代码
    unit.genMachineCode(&mUnit);
    // Div_Mod d_m(&mUnit);
    // d_m.pass();


    //寄存器分配
    LinearScan linearScan(&mUnit);
    linearScan.allocateRegisters();
    if(dump_asm)
    {
        mUnit.output();
    }
    return 0;
}
