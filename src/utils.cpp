#include "utils.h"
#include<string>
#include<cmath>
bool isSysy(string name)
{
    string sysy_func[13]={"getint","getch","getarray","getfloat","getfarray","putint","putch","putarray","putfloat","putfarray","putf","_sysy_starttime","_sysy_stoptime"};
    for(int i=0;i<13;i++)
    {
        if(name==sysy_func[i])
        {
            return true;
        }
    }
    return false;
}

Multiplier chooseMultiplier(int d, int p) {
    int l = floor(log2(d));
    long long  high = ceil((double((long long)(1) << (l+32)) / d));
    return {high, l};
}