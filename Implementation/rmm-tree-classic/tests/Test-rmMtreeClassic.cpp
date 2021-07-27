#include "gtest/gtest.h"

#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../rmMTreeClassic.h"
#include <sdsl/bits.hpp>
/*
* desabilitar um teste inserindo esse prefixo em seu nome
*testing::GTEST_FLAG(filter) = "-MyLibrary.TestWriting"; usar o nome do teste precedido de "-" desabilita ele no main,
a ausência desse mesmo elemento "-", torna o teste o único a ser executado.
*/


/**TODO: adicionar leitura de arquivos. Aumentar nossa capacidade de testes
*/
using namespace sdsl;
using namespace std;


class RMMTreeFixtureTest : public ::testing::Test{
    public:
        RMMTree *t;
        int sizeBlock=4;
        int w=2;
        bp_support_sada<> *bps;
        int size;
        //int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,1,0,0,0};
        int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};

        void SetUp(){
            t = new RMMTree(v,sizeBlock,w);
            bps = new bp_support_sada<>(&(t->bv));
            size = (int)v.size();
            t->buildingTree();
        }
        void TearDown(){
            delete t;
        }
};

TEST_F(RMMTreeFixtureTest, GET_INT){
    int index[] = {9,17,30,36};
    int len = 2;
    int expected[]={3,0,2,1};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bitsread(index[i],index[i]+len-1),expected[i])<< "Resposta errada ao tentar devolver o inteiro correspondente aos bits [" << index[i] << ","<<(index[i]+len-1)<<"]";
    }
}

TEST_F(RMMTreeFixtureTest, expected_log2_ceil){
    int index[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
    int expected[] = {0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5}; 

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ((int)ceil(t->cLog_2(index[i])),expected[i])<< "Resposta errada ao tentar calcular o teto do logarítmo na base 2 de " << index[i];
    }
}

TEST_F(RMMTreeFixtureTest, expected_log2_floor){
    int index[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
    int expected[] = {0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){ 
        EXPECT_EQ((int)floor(t->fLog_2(index[i])),expected[i])<< "Resposta errada ao tentar calcular o chão do logarítmo na base 2 de " << index[i];
    }
}

TEST_F(RMMTreeFixtureTest, index_in_bp_leaf_index_on_tree){
    int index[] = {2,7,10,14,19,23,26,30,35,37};//para b=4
    int expected[] = {15,16,17,18,9,10,11,12,13,14};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        int k = floor((double)(index[i])/sizeBlock);
        EXPECT_EQ(t->leafInTree(k),expected[i]) << "Resposta errada ao tentar obter o índice da RMMTree em que se encontra a " << index[i] <<"-th folha";
    }
}

TEST_F(RMMTreeFixtureTest, given_a_node_v_returns_the_leaf_order){
    int index[] = {15,16,17,18,9,10,11,12,13,14};//para b=4
    int expected[] = {0,1,2,3,4,5,6,7,8,9};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->numLeaf(index[i]),expected[i]) << "Resposta errada ao tentar descobrir a ordem do nó v=" << index[i];
    }

}
 
TEST_F(RMMTreeFixtureTest, fwdSearch_findClose){
    int index[] = {0,1,3,4,7,8,9,10,11,15,13,18,19,20,22,23,24,26,27,30,32};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->findClose(index[i]),bps->find_close(index[i])) << "Resposta errada ao calcular o find_close de i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, fwdSearch_general){
    int index[] = {9,10,17,22,12,21,0,15,19,23,25,0,0,0,36,18,8,0,9,12,0};
    int d[] = {-2,1,-1,-1,-1,-1,2,-3,1,2,0,0,3,5,-1,-1,3,5,-1,-4,4};
    int expected[] = {21,11,18,35,17,36,2,18,24,27,29,6,9,11,39,21,11,11,18,36,10};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fwdSearch(index[i],d[i]),expected[i]) << "Resposta errada ao calcular um FwdSearch genérico, para i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, fwdSearch_answer_not_found){
    int index[] = {9,17,22};
    int d[] = {10,-8,57};
    
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fwdSearch(index[i],d[i]),size) << "Resposta errada ao tentar obter 'answer not found' para FwdSearch para i=" << index[i];
    } 
} 

TEST_F(RMMTreeFixtureTest, bwdSearch_findOpen){ 
   int index[] = {3,5,6,12,14,16,17,18,20,21,25,28,29,31,33,34,35,36,38,39};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->findOpen(index[i]),bps->find_open(index[i])) << "Resposta errada ao calcular o find_open de i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_enclose){
    int index[] = {2,5,9,10,12,15,17,18,19,21,22,23,25,30,31,32,36};
  
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->enclose(index[i]),bps->enclose(index[i])) << "Resposta errada ao calcular o enclose de i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_general){ 
    int index[] = {9,17,22,12,15,19,36,37,39,18,37};
    int d[] = {-2,-1,-1,-4,0,2,0,3,2,-1,-2};
    int expected[] = {7,8,21,6,13,15,6,32,37,7,-1};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bwdSearch(index[i],d[i]),expected[i]) << "Resposta errada ao calcular um BwdSearch genérico, para i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_answer_not_found){ 
    int index[] = {9,17,22};
    int d[] = {5,10,-4};
    
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bwdSearch(index[i],d[i]),-1) << "Resposta errada ao tentar obter 'answer not found' para BwdSearch  com  i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, minExcess_i_and_j_in_the_same_block){ 
    int i[] = {0,0,8,13,16,20,22,24,25,29,33,36}; 
    int j[] = {3,2,11,15,19,22,23,27,26,30,34,39};
    int expected[] = {1,1,1,0,-3,-2,1,0,-1,-1,-2,-2};

    for(int k=0;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->minExcess(i[k],j[k]),expected[k]) << "Resposta errada ao contabilizar o excesso mínimo no intervalo [" << i[k] << "," << j[k] << "]";
    }
}

TEST_F(RMMTreeFixtureTest, minExcess_i_and_j_in_the_different_block){ 
    int i[] = {9,1,12,15,17,33,38,39,0,0}; 
    int j[] = {30,6,26,29,32,39,39,39,39,10};
    int expected[] = {-1,0,-4,-3,-3,-5,-2,-1,0,1};

    for(int k=0;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->minExcess(i[k],j[k]),expected[k]) << "Resposta errada ao contabilizar o excesso mínimo no intervalo [" << i[k] << "," << j[k] << "]";
    }
}

TEST_F(RMMTreeFixtureTest, expected_response_to_rmq){ 
    int i[] = {9,1,12,15,17,33,38,39,8}; 
    int j[] = {30,6,26,29,32,39,39,39,15}; 

    for(int k=0;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->rmq(i[k],j[k]),bps->rmq(i[k],j[k])) << "Resposta errada ao encontrar a posição do excecsso mínimo em [" << i[k] << "," << j[k] << "]";
    }
}

TEST_F(RMMTreeFixtureTest, maxExcess_i_and_j_in_the_same_block){ 
    int i[] = {0,0,8,13,16,20,22,24,25,29,33,36,0}; 
    int j[] = {3,2,11,15,19,22,23,27,26,30,34,39,39};
    int expected[] = {3,3,4,1,-1,-1,2,2,0,0,-1,0,6};

    for(int k=0;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->maxExcess(i[k],j[k]),expected[k]) << "Resposta errada ao contabilizar o excesso máximo no intervalo [" << i[k] << "," << j[k] << "]";
    }
}

TEST_F(RMMTreeFixtureTest, maxExcess_i_and_j_in_the_different_block){ 
    int i[] = {9,1,12,15,17,33,38,39,0,0}; 
    int j[] = {30,6,26,29,32,39,39,39,39,10};
    int expected[] = {3,2,0,1,1,-1,-1,-1,6,5};
    
    for(int k=0;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->maxExcess(i[k],j[k]),expected[k]) << "Resposta errada ao contabilizar o excesso máximo no intervalo [" << i[k] << "," << j[k] << "]";
    }
}


TEST_F(RMMTreeFixtureTest, expected_response_to_rMq){ 
    int i[] = {9,1,12,15,17,33,38,39,0}; 
    int j[] = {30,6,26,29,32,39,39,39,39};
    int expected[] ={11,2,13,15,27,33,38,39,11}; 

    for(int k=0;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->rMq(i[k],j[k]),expected[k]) << "Resposta errada ao encontrar a posição do excecsso máximo em [" << i[k] << "," << j[k] << "]";
    }
}

TEST_F(RMMTreeFixtureTest, number_of_times_excess_min_appears_with_i_and_j_in_the_same_block){ 
    int i[] = {12,16,21,28,28,29,32,36,36};
    int j[] = {15,18,23,31,30,31,35,38,39};  
    
    int expected[] = {2,1,1,2,1,2,1,2,1};
    
    for(int k=0;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->minCount(i[k],j[k]),expected[k]) << "Resposta errada ao calcular a quantidade de vezes que o excesso mínimo aparece em [" << i[k] << " , "<< j[k]<<"]"; 
    }
}

TEST_F(RMMTreeFixtureTest, number_of_times_excess_min_appears_with_i_and_j_in_the_different_block){ 
    int i[] = {0,0,24,0};
    int j[] = {15,23,39,39};  
    
    int expected[] = {2,2,1,1};
    
    for(int k=0;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->minCount(i[k],j[k]),expected[k]) << "Resposta errada ao calcular a quantidade de vezes que o excesso mínimo aparece em [" << i[k] << " , "<< j[k]<<"]"; 
    }
}

TEST_F(RMMTreeFixtureTest, ith_time_the_minimum_excess_appears_in_the_same_block){ 
    int i[] = {0,4,12,12,28,28,28,29,29,32,32,36,36,36}; 
    int j[] = {3,7,15,15,31,31,29,31,31,33,35,38,38,39};
    int q[] = {1,1,1,2,1,2,1,1,2,1,1,1,2,1};
    int expected[] = {0,6,12,14,29,31,29,29,31,33,35,36,38,39};
    
    for(int k=0;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->minSelectExcess(i[k],j[k],q[k]),expected[k]) << "Resposta errada ao tenta encontrar a "<< q[k] << "-th vez em que o excesso mínimo aparece em [" << i[k] << " , "<< j[k]<<"]"; 
    }
}

TEST_F(RMMTreeFixtureTest, ith_time_the_minimum_excess_appears_in_the_different_block){ 
    int i[] = {13,11,0,24,34,33,33,33}; 
    int j[] = {34,16,7,33,38,38,38,39};
    int q[] = {1,3,2,4,2,1,2,1};
    int expected[] = {21,16,6,33,38,36,38,39};
    
    for(int k=0;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->minSelectExcess(i[k],j[k],q[k]),expected[k]) << "Resposta errada ao tenta encontrar a "<< q[k] << "-th vez em que o excesso mínimo aparece em [" << i[k] << " , "<< j[k]<<"]"; 
    }
}

TEST_F(RMMTreeFixtureTest, check_if_i_is_a_leaf){
    int index[] = {2,8,11,16,17,19,21,25,34,37,37}; 
    bool expected[] ={true,false,true,false,false,true,false,false,false,true,true}; 

    for(int k=0;k<(int)(sizeof(index)/sizeof(index[0]));k++){
        EXPECT_EQ(t->isLeaf(index[k]),expected[k]) << "Resposta errada ao verificar se o nó codificado em [" << index[k] << " , close(i)] é uma folha."; 
    }
}

TEST_F(RMMTreeFixtureTest, check_if_x_is_ancestor_of_y){ 
    int x[] = {1,1,0,13,17,22,26,0,0,0}; 
    int y[] = {2,9,9,14,19,24,30,27,37,39}; 
    bool expected[] ={true,false,true,false,false,true,false,true,true,false}; 

    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->isAncestor(x[k],y[k]),expected[k]) << "Resposta errada ao verificar se o nó x= " << x[k] << " é ancestral do nó y= " << y[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, node_depth_x){ 
    int index[] = {2,7,11,17,23,27,34}; 
    int expected[] ={2,1,5,5,3,5,4}; 

    for(int k=0;k<(int)(sizeof(index)/sizeof(index[0]));k++){
        EXPECT_EQ(t->depth(index[k]),expected[k]) << "Resposta errada ao verificar a profunidade do nó codificado em [" << index[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, returns_the_index_j_that_encodes_the_parent_of_node_x){ 
    int x[] = {0,2,7,12,15,18,20,25,32,33,37,39}; 
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->parent(x[k]),bps->enclose(x[k])) << "Resposta errada ao buscar o pai do nó codificado em bv[" << x[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, rigth_sibling_of_x){ 
    int x[] = {2,8,13,19,24,30,37};

    int expected[] = {4,22,15,size,26,32,size};
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->nextSibling(x[k]),expected[k]) << "Resposta errada ao buscar o irmão direito do nó codificado em bv[" << x[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, left_sibling_of_x){ 
    int x[] = {2,8,13,19,24,30,37}; 
    
    int expected[] = {size,size,11,9,size,26,7};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->prevSibling(x[k]),expected[k]) << "Resposta errada ao buscar o irmão esquerdo do nó codificado em bv[" << x[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, return_lastChild_of_x){ 
    int x[] = {0,37,1,7,10,8,23,4};
    
    int expected[] = {37,size,4,22,15,19,32,size};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->lastChild(x[k]),expected[k]) << "Resposta errada ao buscar o último filho do nó codificado em bv[" << x[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, return_firstChild_of_x){ 
    int x[] = {0,37,15,22,26,9,23};

    int expected[] = {1,size,size,23,27,10,24};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->firstChild(x[k]),expected[k]) << "Resposta errada ao buscar o primeiro filho do nó codificado em bv[" << x[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, subtree_size){ 
    int x[] = {0,1,2,7,8,9,22,27,29,39};
    
    int expected[] = {20,3,1,15,7,5,7,1,size,size};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->subtreeSize(x[k]),expected[k]) << "Resposta errada ao calcular o tamannho da subarvore enraizada em bv[" << x[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, ancestor_of_x_that_is_d_levels_above_x){ 
    int x[] = {19,2,11,15,24,27,37};
    int d[] = {2,2,4,4,2,3,1};  
    
    int expected[] = {7,0,7,7,22,22,0};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->levelAncestor(x[k],d[k]),expected[k]) << "Resposta errada ao calcular o ancestral de bv[" << x[k] << " , close(i)] que está " << d[k] << " níveis acima."; 
    }
}

TEST_F(RMMTreeFixtureTest, lowest_common_ancestor_of_x_and_y){ 
    int x[] = {2,9,8,1,10,23};
    int y[] = {4,19,22,37,13,27};  
    
    int expected[] = {1,8,7,0,10,23};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->lca(x[k],y[k]),expected[k]) << "Resposta errada ao calcular o menor ancestral comum de " << x[k] << " e "<< y[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, expected_level_next){ 
    int x[] = {4,7,10,15};
    int expected[] = {8,37,24,27};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->levelNext(x[k]),expected[k]) << "Resposta errada ao calcular o próximo nó a direita de  " << x[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, expected_level_prev){ 
    int x[] = {22,8,7,30};
    int expected[] = {8,4,1,26};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->levelPrev(x[k]),expected[k]) << "Resposta errada ao calcular o próximo nó a direita de  " << x[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, leftmost_node_with_depth_d){ 
    int d[] = {1,3,4,5,6};
    int expected[] = {0,2,9,10,11};
    
    for(int k=0;k<(int)(sizeof(d)/sizeof(d[0]));k++){
        EXPECT_EQ(t->levelLeftMost(d[k]),expected[k]) << "Resposta errada ao calcular o nó mais a esquerda com profundidade  " << d[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, rightmost_node_with_depth_d){ 
    int d[] = {1,2,3,4,5};
    int expected[] = {0,37,22,23,32};
    //6
    for(int k=0;k<(int)(sizeof(d)/sizeof(d[0]));k++){
        EXPECT_EQ(t->levelRightMost(d[k]),expected[k]) << "Resposta errada ao calcular o nó mais a direita com profundidade  " << d[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, deepest_and_leftmost_child_of_x){ 
    int x[] = {0,1,7,22,37};
    int expected[] = {11,2,11,27,37};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->deepestNode(x[k]),expected[k]) << "Resposta errada ao buscar o filho de B[" << x[k] << " , close(x)] com maior profundidade"; 
    }
}

TEST_F(RMMTreeFixtureTest, number_of_children_of_node_x){ 
    int x[] = {0,1,7,10,23,26};
    int expected[] = {3,2,2,3,4,1};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->degree(x[k]),expected[k]) << "Resposta errada ao computar o número de filhos do nó codificado em B[" << x[k] << " , close(x)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, position_of_tth_son_of_x){ 
    int x[] = {0,1,7,9,10,22,23,23,32};
    int q[] = {3,2,1,2,2,1,4,2,1};
    
    int expected[] = {37,4,8,size,13,23,32,26,size};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->child(x[k],q[k]),expected[k]) << "Resposta errada ao buscar o " << q[k] << "-th filho  do nó codificado em B[" << x[k] << " , close(x)]"; 
    }
} 

TEST_F(RMMTreeFixtureTest, number_of_siblings_left_of_node_x){ 
    int x[] = {37,7,1,22,32,30,15};
    int expected[] = {2,1,1,1,3,2,2};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->childRank(x[k]),expected[k]) << "Resposta errada ao computar o número de irmãos  a esquerda do nó codificado em B[" << x[k] << " , close(x)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, number_of_leaves_to_the_left_of_node_x){ 
    int x[] = {0,8,22,24,27,32,37};
    int expected[] = {0,2,6,6,7,9,10};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->leafRank(x[k]),expected[k]) << "Resposta errada ao computar o número de folhas a esquerda do nó codificado em B[" << x[k] << " , close(x)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, tth_leaf_index){ 
    int x[] = {1,2,3,4,5,6,7,8,9,10,11};
    int expected[] = {2,4,11,13,15,19,24,27,30,32,37};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->leafSelect(x[k]),expected[k]) << "Resposta errada ao buscar o índice da " << x[k] << "-th folha"; 
    }
}

TEST_F(RMMTreeFixtureTest, leftmost_leaf_of_x){ 
    int x[] = {4,8,22,24,30,37,7,32,27};
    int expected[] = {4,11,24,24,30,37,11,32,27};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->leftMostLeaf(x[k]),expected[k]) << "Resposta errada ao buscar a folha mais a esquerda de B[" << x[k] << ", close(x)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, rightmost_leaf_of_x){ 
    int x[] = {0,8,22,23,26};
    int expected[] = {37,19,32,32,27};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->rightMostLeaf(x[k]),expected[k]) << "Resposta errada ao buscar a folha mais a direita de B[" << x[k] << ", close(x)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, pre_rank_x){ 
    int x[] = {11,15};
    int expected[] = {8,10};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->preRank(x[k]),expected[k]) << "Resposta errada ao realizar o percurso pré-ordem de X= " << x[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, post_rank_x){ 
    int x[] = {11,16};
    int expected[] = {3,5};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->postRank(x[k]),expected[k]) << "Resposta errada ao calcular postrank de x=" << x[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, pre_select){ 
    int x[] = {3,9};
    int expected[] = {2,11};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->preSelect(x[k]),expected[k]) << "Resposta errada ao calcular a posição do " << x[k] << "-th parênteses de abertura."; 
    }
}

TEST_F(RMMTreeFixtureTest, post_select){ 
    int x[] = {8,12,18};
    int expected[] = {9,27,7};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->postSelect(x[k]),expected[k]) << "Resposta errada ao calcular o parênteses de abertura correspondente ao  " << x[k] << "-th parênteses de fechamento."; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_print_tree){
    t->printInfoTree();
    t->printTree();
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    
    testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.*";
    return RUN_ALL_TESTS();
}
