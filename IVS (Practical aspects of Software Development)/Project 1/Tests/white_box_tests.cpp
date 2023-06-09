//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2021-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Tomáš Juhász
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//
class Matrix1x1 : public :: testing :: Test {

    Matrix m{};

};
TEST (BasicTest, MatrixConstructor){
    Matrix x{};
    EXPECT_EQ(x.get(0,0),0);
    Matrix x2{3,3};
    EXPECT_ANY_THROW(new Matrix(0,1));
    EXPECT_ANY_THROW(new Matrix(1,0));

}
TEST (BasicTest, MatrixSetGet) {
    Matrix matrix{};
    EXPECT_TRUE(matrix.set(0,0,0));
    EXPECT_FALSE(matrix.set(-5,0,0));

    EXPECT_EQ(matrix.get(0,0),0);
    EXPECT_ANY_THROW(matrix.get(-5,1));
    EXPECT_TRUE(matrix.set(0,0,1));
    EXPECT_EQ(matrix.get(0,0),1);

    EXPECT_TRUE(matrix.set({{0}}));
    EXPECT_FALSE(matrix.set({{1,1},{1,1}}));
    EXPECT_EQ(matrix.get(0,0),0);

    Matrix matrix2{2,2};
    EXPECT_TRUE(matrix2.set({{1,1},{1,1}}));
    EXPECT_EQ(matrix2.get(1,1),1);



}
TEST (BasicTest, MatrixOperations) {
    Matrix m {2,2};
    m.set({{1,2},{2,1}});
    Matrix m2 {2,2};
    m2.set({{1,2},{2,1}});

    EXPECT_TRUE(m.operator==(m2));
    m2.set(0,0,3);
    EXPECT_FALSE(m.operator==(m2));
    Matrix m3{3,3};
    EXPECT_ANY_THROW(m.operator==(m3));

    EXPECT_ANY_THROW(m.operator+(m3));
    EXPECT_EQ(m.operator+(m2).get(0,0),m.get(0,0)+m2.get(0,0));

    EXPECT_ANY_THROW(m.operator*(m3));
    EXPECT_EQ(m.operator*(m2).get(0,0),7);
    EXPECT_EQ(m.operator*(5).get(0,0),5);
}
TEST (BasicTest,Others){


    Matrix mBadSize{4,3};
    EXPECT_ANY_THROW(mBadSize.solveEquation({5}));
    EXPECT_ANY_THROW(mBadSize.solveEquation({4,2,31}));

    Matrix m1x1 {1,1};
    m1x1.set(0,0,0);
    EXPECT_ANY_THROW(m1x1.solveEquation({0}));
    m1x1.set(0,0,1);
    EXPECT_EQ(m1x1.solveEquation({0})[0],0);
    Matrix m2x2 {2,2};
    m2x2.set({{1,1},{2,1}});
    EXPECT_EQ(m2x2.solveEquation({1,1})[1],1);

    Matrix m3x3 {3,3};
    m3x3.set({{1,1,1},{2,1,3},{3,2,5}});
    EXPECT_EQ(m3x3.solveEquation({1,2,3})[0],1);

    Matrix m4x4{4,4};
    m4x4.set({{1,0,3,4},{2,2,3,5},{3,4,5,6},{2,2,5,8}});
    EXPECT_EQ(m4x4.solveEquation({1,1,1,1})[0],1);
    EXPECT_EQ(m4x4.transpose().get(1,1),2);
    EXPECT_ANY_THROW(m4x4.inverse());


    EXPECT_EQ(m3x3.inverse().get(0,0),1);
    EXPECT_EQ(m2x2.inverse().get(0,0),-1);

    m2x2.set({{0,1},{0,1}});
    EXPECT_ANY_THROW(m2x2.inverse());
}
/*** Konec souboru white_box_tests.cpp ***/
