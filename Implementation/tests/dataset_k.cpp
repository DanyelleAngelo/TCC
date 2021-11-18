#include "gtest/gtest.h"
#include <vector>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../read_bp/read_bp.h"
#include "../rmm-tree-optimized/rmMTreeOptimized.h"
#include "../rmm-tree-classic/rmMTreeClassic.h"
#include <sdsl/bits.hpp>

/*
* Os testes desabilitados (com excessão de print_tree), estão assim porque a resposta esperada (vetor expected[]) foi construída com base na
* sequência de parenteses balanceadas especifica.
*/


using namespace sdsl;
using namespace std;

int iterations;
int_vector<1> v;
int order;

class RMMTreeFixtureTest : public ::testing::Test{
    public:
        RMMTree_Kary *t;
        RMMTree_Bin *tBin;
        size_t sizeBlock=32;
        size_t w=16;
        vector<int> argsI;
        vector<int> argsII;
        vector<int> argsSelect;
        vector<int> argsFindClose;
        vector<int> argsFindOpen;
        vector<int> argsDepth;
        vector<int> argsDepthBwd;
        
        void SetUp(){
            t = new RMMTree_Kary(v,sizeBlock,w,order);
            tBin = new RMMTree_Bin(v,sizeBlock,w);
            t->buildingTree();
            tBin->buildingTree();
            ArgumentsI();
            ArgumentsII();
		    srand(t->size/18);
            ArgumentsSelect();
            ArgumentsFindClose();
            ArgumentsFindOpen();
        }
        void TearDown(){
            delete t;
        }

        void ArgumentsI(){
        	int k,i=0;
		    srand(t->size/38);
            while(i<iterations){
                k = rand()%(t->size-2);
                argsI.push_back(k);
                i++;
            }
        }
        
        void ArgumentsII(){
        	int k,i=0;
		    srand(t->size/200);
            while(i<iterations){
                k = rand()%(t->size-2);
                argsII.push_back(k);
                i++;
            }
        }

        void ArgumentsSelect(){
            int k,i=0;
            while(i<(t->size)/2){
                k = rand()%((v.size()/2)-1);
                argsSelect.push_back(k);
                i++;
            }
        }

        void ArgumentsFindClose(){
        	int k,i=0;
            while(i<iterations){
                k = rand()%(t->size-1);
                if(t->bv[k]==1){
                    argsFindClose.push_back(k);
                    i++;
                }
            }
        }

        void ArgumentsFindOpen(){
        	int k,i=0;
            int depth, close, excessMax;
            while(i<iterations){
                k = rand()%(t->size-1);
                if(t->bv[k]==0){
                    argsFindOpen.push_back(k);

                    //para as funções que precisam do argumento de profundidade
                    depth = tBin->depth(argsFindOpen[i]);
                    close = tBin->findClose(argsFindOpen[i]);
                    if(argsFindOpen[i]+1 <= close-1 ){
                        excessMax = tBin->maxExcess(argsFindOpen[i]+1, close-1);
                    }
                    else{
                        excessMax = depth;
                    }
                    argsDepthBwd.push_back((rand()% depth+depth) - depth);
                    argsDepth.push_back( (rand()% excessMax+depth) - depth);

                    i++;
                }
            }
        }
};

TEST_F(RMMTreeFixtureTest, fwdSearch_general){
    for(size_t i=0;i<argsFindOpen.size();i++){
        EXPECT_EQ(t->fwdSearch(argsFindOpen[i],argsDepth[i]),tBin->fwdSearch(argsFindOpen[i],argsDepth[i])) << "Resposta errada ao calcular um FwdSearch genérico, para i=" << argsFindOpen[i];
    } 
}

TEST_F(RMMTreeFixtureTest, fwdSearch_findClose){
    for(size_t i=0;i<argsFindClose.size();i++){
        EXPECT_EQ(t->findClose(argsFindClose[i]),tBin->findClose(argsFindClose[i])) << "Resposta errada ao calcular o find_close de i=" << argsFindClose[i];
    }
}

TEST_F(RMMTreeFixtureTest, bwdSearch_general){ 
    for(size_t i=0;i<argsFindOpen.size();i++){
       EXPECT_EQ(t->bwdSearch(argsFindOpen[i],argsDepthBwd[i]),tBin->bwdSearch(argsFindOpen[i],argsDepthBwd[i])) << "Resposta errada ao calcular um BwdSearch genérico, para i=" << argsFindOpen[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_findOpen){
    for(size_t i=0;i<argsFindOpen.size();i++){
        EXPECT_EQ(t->findOpen(argsFindOpen[i]),tBin->findOpen(argsFindOpen[i])) << "Resposta errada ao calcular o find_open de i=" << argsFindOpen[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_enclose){
    int k=0;
    for(int i=0;i<(t->size)/2;i++){
        k = rand()%(t->size);
        EXPECT_EQ(t->enclose(k),tBin->enclose(k)) << "Resposta errada ao calcular o enclose de i=" << k;
    } 
}

TEST_F(RMMTreeFixtureTest, check_if_i_is_a_leaf){
    int k=0;
    for(int i=0;i<(t->size)/2;i++){
        k = rand()%(t->size);
        EXPECT_EQ(t->isLeaf(k),tBin->isLeaf(k)) << "Resposta errada ao verificar se o nó codificado em [" << k << " , close(i)] é uma folha."; 
    }
}

TEST_F(RMMTreeFixtureTest, check_if_x_is_ancestor_of_y){ 
    for(size_t i=0;i<argsI.size();i++){
        EXPECT_EQ(t->isAncestor(argsI[i],argsII[i]),tBin->isAncestor(argsI[i],argsII[i])) << "Resposta errada ao verificar se o nó x= " << argsI[i] << " é ancestral do nó y= " << argsII[i]; 
    }
}

TEST_F(RMMTreeFixtureTest, node_depth_x){ 
    for(size_t i=0;i<argsI.size();i++){
        EXPECT_EQ(t->depth(argsI[i]),tBin->depth(argsI[i])) << "Resposta errada ao verificar a profunidade do nó codificado em [" << argsI[i] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, returns_the_index_j_that_encodes_the_parent_of_node_x){ 
    int k=0;
    for(int i=0;i<(t->size)/2;i++){
        k = rand()%(t->size);
        EXPECT_EQ(t->parent(k),tBin->enclose(k)) << "Resposta errada ao buscar o pai do nó codificado em bv[" << k << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, rigth_sibling_of_x){ 
    for(size_t i=0;i<argsII.size();i++){
        EXPECT_EQ(t->nextSibling(argsII[i]),tBin->nextSibling(argsII[i])) << "Resposta errada ao buscar o irmão direito do nó codificado em bv[" << argsII[i] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, left_sibling_of_x){ 
    for(size_t i=0;i<argsI.size();i++){
        EXPECT_EQ(t->prevSibling(argsI[i]),tBin->prevSibling(argsI[i])) << "Resposta errada ao buscar o irmão esquerdo do nó codificado em bv[" << argsI[i] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, return_lastChild_of_x){ 
   for(size_t i=0;i<argsII.size();i++){
        EXPECT_EQ(t->lastChild(argsII[i]),tBin->lastChild(argsII[i])) << "Resposta errada ao buscar o último filho do nó codificado em bv[" << argsII[i] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, return_firstChild_of_x){
    for(size_t i=0;i<argsI.size();i++){    
        EXPECT_EQ(t->firstChild(argsI[i]),tBin->firstChild(argsI[i])) << "Resposta errada ao buscar o primeiro filho do nó codificado em bv[" << argsI[i] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, subtree_size){ 
    for(size_t i=0;i<argsII.size();i++){   
        EXPECT_EQ(t->subtreeSize(argsII[i]),tBin->subtreeSize(argsII[i])) << "Resposta errada ao calcular o tamannho da subarvore enraizada em bv[" << argsII[i] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest,ancestor_of_x_that_is_d_levels_above_x){ 
    for(size_t i=0;i < argsFindOpen.size() ;i++){
        EXPECT_EQ(t->levelAncestor(argsFindOpen[i],argsDepthBwd[i]),tBin->levelAncestor(argsFindOpen[i],argsDepthBwd[i])) << "Resposta errada ao calcular o ancestral de bv[" << argsFindOpen[i] << " , close(i)] que está " << argsDepthBwd[i] << " níveis acima."; 
    }
}

TEST_F(RMMTreeFixtureTest, expected_level_next){ 
    for(size_t i=0;i<argsI.size();i++){ 
        EXPECT_EQ(t->levelNext(argsI[i]),tBin->levelNext(argsI[i])) << "Resposta errada ao calcular o próximo nó a direita de  " << argsI[i]; 
    }
}

TEST_F(RMMTreeFixtureTest, expected_level_prev){ 
    for(size_t i=0;i<argsII.size();i++){ 
        EXPECT_EQ(t->levelPrev(argsII[i]),tBin->levelPrev(argsII[i])) << "Resposta errada ao calcular o próximo nó a direita de  " << argsII[i]; 
    }
}

TEST_F(RMMTreeFixtureTest, leftmost_node_with_depth_d){ 
    for(size_t i=0;i<argsDepth.size();i++){ 
        EXPECT_EQ(t->levelLeftMost(argsDepth[i]),tBin->levelLeftMost(argsDepth[i])) << "Resposta errada ao calcular o nó mais a esquerda com profundidade  " << argsDepth[i]; 
    }
}

TEST_F(RMMTreeFixtureTest, rightmost_node_with_depth_d){ 
    for(size_t i=0;i<argsDepth.size();i++){ 
        EXPECT_EQ(t->levelRightMost(argsDepth[i]),tBin->levelRightMost(argsDepth[i])) << "Resposta errada ao calcular o nó mais a direita com profundidade  " << argsDepth[i]; 
    }
}

TEST_F(RMMTreeFixtureTest, number_of_leaves_to_the_left_of_node_x){ 
    for(size_t i=0;i<argsI.size();i++){ 
        EXPECT_EQ(t->leafRank(argsI[i]),tBin->leafRank(argsI[i])) << "Resposta errada ao computar o número de folhas a esquerda do nó codificado em B[" << argsI[i] << " , close(x)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, tth_leaf_index){  
    for(int i=1;i<t->getNumberLeaves();i++){ 
        EXPECT_EQ(t->leafSelect(i),tBin->leafSelect(i)) << "Resposta errada ao buscar o índice da " << i << "-th folha"; 
    }
}

TEST_F(RMMTreeFixtureTest, leftmost_leaf_of_x){ 
    for(size_t i=0;i<argsI.size();i++){ 
        EXPECT_EQ(t->leftMostLeaf(argsI[i]),tBin->leftMostLeaf(argsI[i])) << "Resposta errada ao buscar a folha mais a esquerda de B[" << argsI[i] << ", close(x)]"; 
    }
}

TEST_F(RMMTreeFixtureTest,rightmost_leaf_of_x){ 
    for(size_t i=0;i<argsI.size();i++){ 
        EXPECT_EQ(t->rightMostLeaf(argsI[i]),tBin->rightMostLeaf(argsI[i])) << "Resposta errada ao buscar a folha mais a direita de B[" << argsI[i] << ", close(x)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, pre_rank_x){ 
    for(size_t i=0;i<argsFindClose.size();i++){ 
        EXPECT_EQ(t->preRank(argsFindClose[i]),tBin->preRank(argsFindClose[i])) << "Resposta errada ao realizar o percurso pré-ordem de X= " << argsFindClose[i]; 
    }
}

TEST_F(RMMTreeFixtureTest, post_rank_x){ 
    for(size_t i=0;i<argsFindClose.size();i++){ 
        EXPECT_EQ(t->postRank(argsFindClose[i]),tBin->postRank(argsFindClose[i])) << "Resposta errada ao calcular postrank de x=" << argsFindClose[i]; 
    }
}

TEST_F(RMMTreeFixtureTest, pre_select){ 
    for(size_t i=0;i<argsSelect.size();i++){ 
        EXPECT_EQ(t->preSelect(argsSelect[i]),tBin->preSelect(argsSelect[i])) << "Resposta errada ao calcular a posição do " << argsSelect[i] << "-th parênteses de abertura."; 
    }
}

TEST_F(RMMTreeFixtureTest, post_select){ 
    for(size_t i=0;i<argsSelect.size();i++){
        EXPECT_EQ(t->postSelect(argsSelect[i]),tBin->postSelect(argsSelect[i])) << "Resposta errada ao calcular o parênteses de abertura correspondente ao  " << argsSelect[i] << "-th parênteses de fechamento."; 
    }
}

int main(int argc, char *argv[]){
	if(argc<4){
		cout << "Número de argumentos inválidos" << endl;
		exit(EXIT_FAILURE);
	}
	parentheses_to_bits(argv[1],v);
	iterations = atoi(argv[2]);
	order = atoi(argv[3]);	

    ::testing::InitGoogleTest(&argc, argv);
    testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.*";
	return RUN_ALL_TESTS();
}