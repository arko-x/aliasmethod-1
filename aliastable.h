
/*
・確率分布を元にした乱数生成に関するアルゴリズム
LINEの論文でエッジサンプリングの高速化に流用していた．
リファレンスで引いていたReducing the Sampling Complexity of Topic Modelsはトピックモデルのサンプリング高速化に関する論文．
ただ，掲載しているGenerate Aliasアルゴリズムは間違っているような印象だった．
このコードの６４行目，６６行目に該当する．
LINEのコードでは正しく動作するように修正されていた．

author: m-ochi
*/

#ifndef INCLUDED_aliastable_h_
#define INCLUDED_aliastable_h_

#include <iostream>
#include <random>
#include <memory>
#include <cstdint>
#include <iostream>
#include <random>

class AliasTable {

public:
    AliasTable(int64_t nums_a, double* weights_a);
    ~AliasTable();
    int64_t sampling(double rand1=0, double rand2=0, int64_t seed=131);
    
    int64_t nums;
    std::unique_ptr<double[]> weights;
    std::unique_ptr<int64_t[]> alias;
    std::unique_ptr<double[]> probs;

private:
    void initialize();

};

#endif
