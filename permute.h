/**
  ******************************************************************************
  * @file    permute.h
  * @author  Brett Brotherton
  * @version V1.0.0
  * @date    2013-11-03
  * @brief   Smartly iterate throug permutations of a string of numbers
  ******************************************************************************
  * @attention
  * Copyright 2013 Brett Brotherton http://www.bbrotherton.com
  *
  *   Licensed under the Apache License, Version 2.0 (the "License");
  *   you may not use this file except in compliance with the License.
  *   You may obtain a copy of the License at
  *
  *       http://www.apache.org/licenses/LICENSE-2.0
  *
  *   Unless required by applicable law or agreed to in writing, software
  *   distributed under the License is distributed on an "AS IS" BASIS,
  *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  *   See the License for the specific language governing permissions and
  *   limitations under the License.
  * ******************************************************************************
*/
#ifndef PERMUTE_H
#define PERMUTE_H


/*
 * nextPermutation:
 * This function computes the next permutation of a string of digits 0-9 
 * with the restrictions based on the given arguments
 * 
 * guess: the string of digits you wish to modfiy to produce the next guess
 * this will be modified on return
 * 
 * exclusions: a string with digits to exclude from possible permutations
 * 
 * norepeats: if non-zero then any permutation that has a digit repeated
 * will be excluded 
 * 
 * return: none
 * 
 * example: guess = "00" exclusions = "1234568", norepeats = 1
 * will give the follwing sequence after subsequent calls:
 * 00, 07, 09, 70, 79, 00, ....
*/
void nextPermutation(char *guess, char *exclusions, int norepeats);

#endif  //PERMUTE_H
