//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2021-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Tomáš Juhász
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"
//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//
PriorityQueue::PriorityQueue()
{
    m_pHead = nullptr;
}

PriorityQueue::~PriorityQueue()
{
    Element_t * temp = GetHead();
    Element_t * tempNext;
    while (temp != nullptr){
       tempNext = temp->pNext;
       delete(temp);
       temp = tempNext;
    }
}

void PriorityQueue::Insert(int value)
{
        if (GetHead() == nullptr || GetHead()->value < value){
            m_pHead = new Element_t {
               .pNext = GetHead(),
               .value = value
            };
        }else{
            Element_t * temp = GetHead();
            while (temp != nullptr){
                if (temp->pNext != nullptr){
                    if (temp->pNext->value < value){
                        temp->pNext = new Element_t{
                            .pNext = temp->pNext,
                            .value = value
                        };
                        break;
                    }
                }else{
                    temp->pNext = new Element_t {
                        .pNext = nullptr,
                        .value = value
                    };
                    break;
                }
                temp = temp->pNext;
            }

        }


    }


bool PriorityQueue::Remove(int value)
{

    Element_t * temp = GetHead();
    while (temp != nullptr){
        if (temp ->pNext != nullptr){
            if (temp->pNext->value == value){
                Element_t * tempNext =  temp->pNext->pNext;
                delete (temp->pNext);
                temp->pNext = tempNext;
                return true;

            }else if (temp->value == value){
                m_pHead = temp ->pNext;
                delete (temp);
                return true;
            }
        }else if (temp->pNext == nullptr && temp->value == value){
            delete temp;
            m_pHead = nullptr;
            return true;
        }
        temp = temp->pNext;
    }
    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    Element_t *  head = GetHead();
    while (head != nullptr){
        if (head->value == value)
            return head;
        head= head->pNext;
    } 
    return nullptr;
}

size_t PriorityQueue::Length()
{
    int count =0;
    Element_t * temp = GetHead();
    while (temp != nullptr){
        count++;
        temp = temp->pNext;
    }
	return count;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return m_pHead;
}

/*** Konec souboru tdd_code.cpp ***/
