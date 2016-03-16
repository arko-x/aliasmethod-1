#include "aliastable.h"
//#include <gsl/gsl_rng.h>

//std::random_device  AliasTable::rd;
//std::mt19937        AliasTable::mt(rd());
//std::uniform_real_distribution<double> AliasTable::r_uni(0, 1.0);

AliasTable::AliasTable(int64_t nums_a, double* weights_a)
: nums(nums_a), alias(new int64_t[nums]), probs(new double[nums]), weights(new double[nums]){
    for(int i=0; i < nums; ++i)
        weights[i] = weights_a[i];

    if (!alias || !probs) {
        std::cout << "Error: memory allocation failed!" << std::endl;
        exit(1);
    }
    initialize();
//    mt(rd());

    // 乱数生成器の初期化
//    const gsl_rng_type* gsl_T = gsl_rng_rand48;
//    gsl_r = gsl_rng_alloc(gsl_T);
//    gsl_rng_env_setup();
//    gsl_rng_set(gsl_r, 314159265);
//    std::random_device rd;
//    rd();
//    mt(rd());
//    gsl_rng_set(gsl_r, rd());

}

AliasTable::~AliasTable() {}

void AliasTable::initialize() {
    std::unique_ptr<double[]> norm_probs(new double[nums]);
    std::unique_ptr<int64_t[]> large_block(new int64_t[nums]);
    std::unique_ptr<int64_t[]> small_block(new int64_t[nums]);

    if (norm_probs == NULL || large_block == NULL || small_block == NULL) {
        std::cout << "Error: memory allocation failed!" << std::endl;
        exit(1);
    }

    double sum = 0;
    int64_t cur_small_block;
    int64_t cur_large_block;
    int64_t num_small_block = 0;
    int64_t num_large_block = 0;

    for (int64_t k = 0; k != nums; k++)
        sum += weights[k];

    // norm_probの総和はnum_edgeとなる
    for (int64_t k = 0; k != nums; k++)
        norm_probs[k] = weights[k] * nums / sum;

    // 逆順．list[n++]はlist[n]に値を入れてからnをインクリメント
    for (int64_t k = nums - 1; k >= 0; k--) {
        // 1/num_edgeの割合よりそのエッジが大きいか,小さいか
        if (norm_probs[k]<1)
            small_block[num_small_block++] = k;
        else
            large_block[num_large_block++] = k;
    }

    // num_small_block>0かつnum_large_block>0
    while (num_small_block && num_large_block) {
        // list[--n]はnをn-1してから処理
        cur_small_block = small_block[--num_small_block];
        cur_large_block = large_block[--num_large_block];
        probs[cur_small_block] = norm_probs[cur_small_block];
        alias[cur_small_block] = cur_large_block;
        norm_probs[cur_large_block] = norm_probs[cur_large_block] + norm_probs[cur_small_block] - 1;
        if (norm_probs[cur_large_block] < 1)
            small_block[num_small_block++] = cur_large_block;
        else
            large_block[num_large_block++] = cur_large_block;
    }

    while (num_large_block)
        probs[large_block[--num_large_block]] = 1;

    while (num_small_block)
        probs[small_block[--num_small_block]] = 1;
}

int64_t AliasTable::sampling(double rand1, double rand2, int64_t seed) {
    if (rand1 == 0 && rand2 == 0) {
        std::random_device rd;
        std::mt19937 mt(rd());
//        std::mt19937 mt;
        std::uniform_real_distribution<double> r_uni(0, 1.0);

        rand1 = r_uni(mt);
        rand2 = r_uni(mt);
    }

    //std::cout << "rand1:"<< rand1 << ", rand2:" << rand2 << std::endl;

    //小数点以下切り捨て
    int64_t k = static_cast<int64_t>(nums * rand1);
    return rand2 < probs[k] ? k : alias[k];
}

/*
int main() {
    // sampling for test
    int64_t nums  = 13;
    double* weights = new double[nums];
    for(int64_t i=0; i<nums; i++) {
        weights[i] = static_cast<double>(i+1);
    }
    AliasTable* as1 = new AliasTable(nums,weights);
    AliasTable* as2 = new AliasTable(nums,weights);
    std::cout << "as1->nums:"       << as1->nums       << std::endl;

    for(int64_t i=0; i<nums; i++) {
        std::cout << "as1->weights["<< i << "]:" << as1->weights[i] << std::endl;
        std::cout << "as2->weights["<< i << "]:" << as2->weights[i] << std::endl;
        std::cout << "as1->alias["  << i << "]:" << as1->alias[i]   << std::endl;
        std::cout << "as2->alias["  << i << "]:" << as2->alias[i]   << std::endl;
        std::cout << "as1->probs["  << i << "]:" << as1->probs[i]   << std::endl;
        std::cout << "as2->probs["  << i << "]:" << as2->probs[i]   << std::endl;
    }

    for(int i=0; i<10; i++) {
        std::cout << "as1->sampling():" << as1->sampling(0,0) << std::endl;
    }

    delete[] weights;
    delete as1;

    return 0;
}
*/


