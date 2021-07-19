#include "gtest/gtest.h"
#include <stdio.h>
#include <stdlib.h>
#include "rw_bp/bit_array.h"
#include <sdsl/bp_support_sada.hpp>
#include "../rmm-tree-classic/rmMTreeClassic.h"

using namespace sdsl;
using namespace std;

/* 
 * Read the parentheses sequence in 'fn' as the bit array 'B'. An opening 
 * parenthesis is represented as a 1 and a closing parentheses is represented as
 * a 0
 */
BIT_ARRAY* parentheses_to_bits(const char* fn, long* n) {
  
  char parenthesis;
  long counter = 0L;
  
  FILE* fp = fopen(fn, "r");
  if (!fp) {
    fprintf(stderr, "Error opening file \"%s\".\n", fn);
    exit(EXIT_FAILURE);
  }

  fseek(fp, 0L, SEEK_END);
  *n = ftell(fp);

  BIT_ARRAY* B = bit_array_create(*n);
  
  fseek(fp, 0L, SEEK_SET);

  while (fread(&parenthesis,sizeof(char),1,fp)==1) {
    if(parenthesis == '(')
      bit_array_set_bit(B, counter);
    counter++;
  }
  
  fclose(fp);
  
  return B;

}

class RMMTreeFixtureTest : public ::testing::Test{
    public:
        RMMTree *t;
        int sizeBlock=4;
        int w=2;
        bp_support_sada<> *bps;
        int size;
        
        void SetUp(){
            //t = new RMMTree(v,sizeBlock,w);
            //bps = new bp_support_sada<>(&(t->bv));
            //size = (int)v.size();
            //t->buildingTree();
        }
        void TearDown(){
            delete t;
        }
};

TEST_F(RMMTreeFixtureTest, fwdSearch_findClose){
    int index[] = {0,1,3,4,7,8,9,10,11,15,13,18,19,20,22,23,24,26,27,30,32};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->findClose(index[i]),bps->find_close(index[i])) << "Resposta errada ao calcular o find_close de i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_findOpen){ 
   int index[] = {3,5,6,12,14,16,17,18,20,21,25,28,29,31,33,34,35,36,38,39};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->findOpen(index[i]),bps->find_open(index[i])) << "Resposta errada ao calcular o find_open de i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, expected_response_to_rmq){ 
    int i[] = {9,1,12,15,17,33,38,39,0,8}; 
    int j[] = {30,6,26,29,32,39,39,39,39,15}; 

    for(int k=8;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->rmq(i[k],j[k]),bps->rmq(i[k],j[k])) << "Resposta errada ao encontrar a posição do excecsso mínimo em [" << i[k] << "," << j[k] << "]";
    }
}

int main(int argc, char **argv){
    long n = 498753914;
    BIT_ARRAY *B = parentheses_to_bits("wiki.par", 498753914);
    printf("%ld parentheses read\n", n);

    //::testing::InitGoogleTest(&argc, argv);
    //testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.*";
    //return RUN_ALL_TESTS();
}
