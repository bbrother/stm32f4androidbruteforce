/**
  ******************************************************************************
  * @file    permute.c
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
#include <string.h>
#include "permute.h"
#ifdef TESTPERMUTE
#include <stdio.h>
#endif

void nextPermutation(char *guess, char *exclusions, int numrepeats, int onlyrepeats) {
  int done = 0;
  int charcnts[10];
  int i;
  int idx;
  int repeat;
  while (done == 0) {
    done = 1;
    idx = strlen(guess) - 1;
    while (idx >= 0) {
      repeat = 0;
      //guess[idx]++;
      //if (guess[idx] > '9') {
        //guess[idx] = '0';
      //}
      guess[idx]++;
      if (guess[idx] > '9') {
        guess[idx] = '0';
        repeat = 1;
      }
      while (strchr(exclusions, (int)guess[idx]) != NULL) {
        guess[idx]++;
        if (guess[idx] > '9') {
          guess[idx] = '0';
          repeat = 1;
        }
      }
      if (repeat) {
        idx--;
      } else {
        break;
      }
    }
    
    if (numrepeats) {
      for (i = 0; i < 10; i++) {
        charcnts[i] = 0;
      }
      for (i = 0; i < (int)strlen(guess); i++) {
        charcnts[guess[i] - '0']++;
        if (charcnts[guess[i] - '0'] > numrepeats) {
          done = 0;
        }
      }
    }
    if (onlyrepeats) {
      repeat = 0;
      for (i = 0; i < 10; i++) {
        if (charcnts[i] > 1) {
          repeat = 1;
        }
      }
      if (repeat != 1) {
        done = 0;
      }
    }
  }
}

#ifdef TESTPERMUTE
int main(void) {
    char guess[7];
    strcpy(guess, "40000000");
    while(guess[0] == '4') {
        printf("%s\n", guess);
        nextPermutation(guess, "123", 2, 0);
    }
    /*strcpy(guess, "700000");
    while(guess[0] == '7') {
        printf("%s\n", guess);
        nextPermutation(guess, "123", 1);
    }*/
    
}
#endif
