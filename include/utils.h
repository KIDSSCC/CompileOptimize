#include<string>
using namespace std;

bool isSysy(std::string name);
inline int clz(int x) { return __builtin_clz(x); }
inline int ctz(int x) { return __builtin_ctz(x); }

struct Multiplier {
    long long  m;
    int l;
};

Multiplier chooseMultiplier(int d, int p);