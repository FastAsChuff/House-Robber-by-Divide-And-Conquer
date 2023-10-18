#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>
#include <time.h>

// gcc robhousednc.c -o robhousednc.bin -O3 -march=native -Wall

// Inspired by question https://stackoverflow.com/questions/77285596/dynamic-programming-with-memoization-on-the-rob-house-problem

uint64_t findmaxsumsubset(uint8_t robi, uint32_t i, uint32_t j, uint16_t *money) {
  // Requires j >= i+2;
  uint64_t maxsumm2 = 0;
  uint64_t maxsumm1 = 0;
  uint64_t maxsum = 0;
  if (robi) {
    maxsumm2 = money[i];
    maxsumm1 = money[i];
  } else {
    maxsumm1 = money[i+1];
  }
  i += 2;
  for (; i<=j; i++) {
    if (maxsumm1 <= maxsumm2 + money[i]) {
      maxsum = maxsumm2 + money[i];
    } else {
      maxsum = maxsumm1;
    }
    maxsumm2 = maxsumm1;
    maxsumm1 = maxsum;
  }
  return maxsum;
}


uint64_t findmaxsum(uint8_t robi, uint8_t robj, uint32_t i, uint32_t j, uint16_t *money)  {
  // Find max sum of subset of houses from i to j inc. given robi and robj
  // Requires j > i
  uint8_t robip1, robip2;
  if (j == i+1) {
    if (robj == robi) return 0;
    if (robi) return money[i];
    if (robj) return money[j];
    return 0;
  }
  if (j == i+2) {
    if (robi == robj) {
      robip1 = 1-robi;
    } else {
      robip1 = 0;
    }
    return robi*money[i] + robip1*money[i+1] + robj*money[j];
  }
  if (j == i+3) {
    if (robi == 1) {
      robip1 = 0;
      robip2 = 1-robj;
    } else {
      if (robj == 1) {
        robip1 = 1;
        robip2 = 0;
      } else {
        if (money[i+1] > money[i+2]) {
          robip1 = 1;
          robip2 = 0;
        } else {
          robip1 = 0;
          robip2 = 1;
        }
      }
    }
    return robi*money[i] + robip1*money[i+1] + robip2*money[i+2] + robj*money[j];
  }
  if (robj) return money[j] + findmaxsumsubset(robi, i, j-2, money);
  return findmaxsumsubset(robi, i, j-1, money);
}

void findends(uint8_t *robi, uint8_t *robj, uint32_t maxj, uint16_t *money)  {
  uint64_t maxsum1, maxsum2, maxsum3, maxsum4;
  maxsum1 = findmaxsum(0, 0, 0, maxj, money);
  maxsum2 = findmaxsum(0, 1, 0, maxj, money);
  maxsum3 = findmaxsum(1, 0, 0, maxj, money);
  maxsum4 = findmaxsum(1, 1, 0, maxj, money);
  if ((maxsum1 >= maxsum2) && (maxsum1 >= maxsum3) && (maxsum1 >= maxsum4)) {
    *robi = 0;
    *robj = 0;
    printf("Max sum is %lu (0...0)\n", maxsum1);
    return;
  }
  if ((maxsum2 >= maxsum1) && (maxsum2 >= maxsum3) && (maxsum2 >= maxsum4)) {
    *robi = 0;
    *robj = 1;
    printf("Max sum is %lu (0...1)\n", maxsum2);
    return;
  }
  if ((maxsum3 >= maxsum1) && (maxsum3 >= maxsum2) && (maxsum3 >= maxsum4)) {
    *robi = 1;
    *robj = 0;
    printf("Max sum is %lu (1...0)\n", maxsum3);
    return;
  }
  *robi = 1;
  *robj = 1;
  printf("Max sum is %lu (1...1)\n", maxsum4);
  return; 
}

void findmaxselection(const uint8_t robi, const uint8_t robj, const uint32_t i, const uint32_t j, uint16_t* restrict money , uint8_t *rob)  {
  uint64_t n, mid, sum1, sum2, sum3, maxsumitomid0, maxsumitomid1, maxsummidp1toj0, maxsummidp1toj1;
  n = 1+j-i;  
  memset(rob + i, 0, n*sizeof(rob[0]));
  rob[i] = robi;
  rob[j] = robj;
  if (n <= 2) return;
  if (n > 4) {
    mid = ((uint64_t)i + j)/2;
    maxsumitomid0 = findmaxsum(robi, 0, i, mid, money);
    maxsumitomid1 = findmaxsum(robi, 1, i, mid, money);
    maxsummidp1toj0 = findmaxsum(0, robj, mid+1, j, money);
    maxsummidp1toj1 = findmaxsum(1, robj, mid+1, j, money);
    sum1 = maxsumitomid1 + maxsummidp1toj0;
    sum2 = maxsumitomid0 + maxsummidp1toj1;
    sum3 = maxsumitomid0 + maxsummidp1toj0;
    if ((sum3 > sum2) && (sum3 > sum1)) {
      findmaxselection(robi, 0, i, mid, money, rob);
      findmaxselection(0, robj, mid+1, j, money, rob);
    } else {
      if (sum1 > sum2) {
        findmaxselection(robi, 1, i, mid, money, rob);
        findmaxselection(0, robj, mid+1, j, money, rob);
      } else {
        findmaxselection(robi, 0, i, mid, money, rob);
        findmaxselection(1, robj, mid+1, j, money, rob);
      }
    }
  } else {
    // n == 3
    // 010 001 100 101
    // n == 4
    // 0010 0100 0101 1010 1001
    if (n == 3) {
      if (rob[i] == rob[j]) {
        rob[i+1] = 1-rob[i];
      } else {
        rob[i+1] = 0;
      }
      return;
    }
    if (rob[i] == 1) {
      rob[i+1] = 0;
      rob[i+2] = 1-rob[j];
      return;
    }
    if (rob[j] == 1) {
      rob[i+1] = 1;
      rob[i+2] = 0;
      return;
    }
    if (money[i+1] > money[i+2]) {
      rob[i+1] = 1;
      rob[i+2] = 0;
    } else {
      rob[i+1] = 0;
      rob[i+2] = 1;
    }
  }
}

void printloot(uint16_t *money, uint8_t *rob, uint64_t maxj) {
  uint64_t j, robsum = 0;
  for (j = 0; j<=maxj; j++) {
    printf("%i", rob[j]);
    robsum += rob[j]*money[j];
  }
  printf("  Total = %lu\n", robsum);
} 


int main(int argc, char **argv) {
  uint64_t i, maxj, streetlength = 5000;
  if (argc > 1) {
    streetlength = atol(argv[1]);
  } else {
    printf("This program solves the 'House Robber Problem' using a divide and conquer technique. ");
    printf("Each house on a street has an amount of money stashed in it, and the robber wants to steal as much as he can in one night. ");
    printf("However, the houses have connected security which alerts the police if adjacent homes are burgled. ");
    printf("This program finds an optimal selection of houses to rob without alerting the police. ");
    printf("The houses are given a random amount of cash each.\n");
    printf("Usage %s streetlength\n", argv[0]);
    printf("e.g. %s 10\n", argv[0]);
    printf("Output. 54865, 22063, 49828, 34740, 14676, 40586, 48906, 23116, 63710, 5083\n");
    printf("Max sum is 231985 (1...0)\n");
    printf("1010101010  Total = 231985\n");
    exit(0);
  }
  if (streetlength > 0xffffffff) {
    printf("Maximum streetlength supported is 4294967295.\n");
    exit(1);
  }
  if (streetlength < 3) {
    printf("Minimum streetlength supported is 3.\n");
    exit(1);
  }
  maxj = streetlength - 1;
  srand(time(0));
  uint16_t *money = malloc(streetlength*sizeof(uint16_t));
  uint8_t *rob = malloc(streetlength);
  if ((money == NULL) || (rob == NULL)) {
    printf("Insufficient Memory.\n");
    exit(1);
  }
  for (i=0; i<maxj; i++) {
    money[i] = 1+rand() % 65535;
    printf("%u, ", money[i]);
  }
  money[maxj] = 1+rand() % 65535;
  printf("%u\n", money[maxj]);
  findends(rob, rob+maxj, maxj, money);
  findmaxselection(rob[0], rob[maxj], 0, maxj, money, rob);
  printloot(money, rob, maxj);
}
