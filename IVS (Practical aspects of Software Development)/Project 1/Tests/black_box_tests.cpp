//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Tomáš Juhász
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//
class EmptyTree : public :: testing :: Test
{
protected:
    BinaryTree tree;
};
class NonEmptyTree : public :: testing :: Test
{
protected:
    virtual void SetUp() {
        int values[] = {10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55};

        for (int value : values) {
            tree.InsertNode(value);
        }
    }

    BinaryTree tree;

};
class TreeAxioms : public :: NonEmptyTree
{
protected:
    BinaryTree tree;
};

TEST_F(EmptyTree, InsertNode) {
    EXPECT_TRUE(tree.GetRoot() == NULL);

    tree.InsertNode(0);
    ASSERT_TRUE(tree.GetRoot() != NULL);
    ASSERT_TRUE(tree.GetRoot()->pParent == NULL);
    EXPECT_EQ(tree.GetRoot()->key, 0);

    auto result = tree.InsertNode(100);
    ASSERT_TRUE(tree.GetRoot() != NULL);
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second->key,100);

}
TEST_F(EmptyTree, DeleteNode) {
    EXPECT_FALSE(tree.DeleteNode(0));
}
TEST_F(EmptyTree, FindNode) {
    EXPECT_TRUE(tree.FindNode(0) == NULL);
}

TEST_F(NonEmptyTree, InsertNode) {
    auto result = tree.InsertNode(100);
    EXPECT_EQ(result.second->key,100);

    result = tree.InsertNode(0);
    EXPECT_EQ(result.second->key,0);
}
TEST_F(NonEmptyTree, DeleteNode) {
    EXPECT_FALSE(tree.DeleteNode(0));

    int values[] = { 5, 10, 15, 20, 30, 40, 50, 55, 60, 65, 70, 80, 85, 90 };
    for(int i = 0; i <= 13; ++i)
    {
        EXPECT_TRUE(tree.DeleteNode(values[i]));

    }

    EXPECT_TRUE(tree.GetRoot() == NULL);

}
TEST_F(NonEmptyTree, FindNode) {
    EXPECT_TRUE(tree.FindNode(0) == NULL);
    int values[] = { 5, 10, 15, 20, 30, 40, 50, 55, 60, 65, 70, 80, 85, 90 };
    for(int i = 0; i <= 13; ++i)
    {
        auto result = tree.FindNode(values[i]);
        ASSERT_TRUE(result != NULL);

        EXPECT_EQ(result->key, values[i]);


    }

    EXPECT_TRUE(tree.FindNode(0) == NULL);
}
TEST_F (TreeAxioms, Axiom1){
   std::vector<BinaryTree::Node_t *> nodes;
   tree.GetLeafNodes(nodes);

   for (auto node : nodes){
       EXPECT_EQ(node->color ,BinaryTree::Color_t::BLACK);
   }
}
TEST_F(TreeAxioms, Axiom2){
    std::vector<BinaryTree::Node_t *> nodes;
    tree.GetNonLeafNodes(nodes);

    for (auto node : nodes){
        if (node->color == BinaryTree::Color_t::RED){
            EXPECT_EQ(node->pRight->color,BinaryTree::Color_t::BLACK);
            EXPECT_EQ(node->pLeft->color,BinaryTree::Color_t::BLACK);
        }
    }
}
TEST_F(TreeAxioms,Axiom3){
    std::vector<BinaryTree::Node_t *> nodes;
    tree.GetLeafNodes(nodes);
    int refCount=0;
    for (auto node : nodes){
       int count = 0;
       while (node->pParent != NULL){
           if (node->color == BinaryTree::Color_t::BLACK)
                count++;

          node = node->pParent;
       }
       if (refCount == 0)
           refCount = count;
       else
           EXPECT_EQ(refCount,count);
    }
}
/*** Konec souboru black_box_tests.cpp ***/
