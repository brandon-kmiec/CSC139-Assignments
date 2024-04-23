#define GREEN "\x1B[32m"
#define RED "\x1B[31m"
#define NORM "\x1B[0m"

#include <stdio.h>
#include <stdlib.h>

#include "umem.h"
#include "umem.c"

void printResult(char*, int, int);
void printPointerResult(char*, int*, int*);
void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void test8();
void test9();
void test10();
void test11();
void test12();
void test13();

int main(int argc, char* argv[]) {
	int testNum = atoi(argv[1]);
	
	if(argc != 2) {
		printf("Format: ./main [testNum]\n");
	} // end if
	else {
		// run test case based on command line argument
		switch(testNum) {
			case 1:
				test1();
				break;
			case 2:
				test2();
				break;
			case 3:
				test3();
				break;
			case 4:
				test4();
				break;
			case 5:
				test5();
				break;
			case 6:
				test6();
				break;
			case 7:
				test7();
				break;
			case 8:
				test8();
				break;
			case 9:
				test9();
				break;
			case 10:
				test10();
				break;
			case 11:
				test11();
				break;
			case 12:
				test12();
				break;
			case 13:
				test13();
				break;
		} // end switch
	} // end else
	
	return 0;
} // end main


// test 1: umeminit with valid sizeOfRegion and valid allocationAlgo
// test if umeminit returns success
// test 1 is a success if umeminit returns 0, failure otherwise
void test1() {
	int recRes = umeminit(64, BEST_FIT);
	int expRes = 0; // success
	
	printResult("1", expRes, recRes);
} // end test1


// test 2: umeminit with invalid sizeOfRegion and valid allocationAlgo
// test if umeminit returns failure
// test 2 is a success if umeminit returns -1, failure otherwise
void test2() {
	int recRes = umeminit(-1, BEST_FIT);
	int expRes = -1; // failure
	
	printResult("2", expRes, recRes);
} // end test2


// test 3: umeminit with valid sizeOfRegion and invalid allocationAlgo
// test if umeminit return failure
// test 3 is a success if umeminit returns -1, failure otherwise
void test3() {
	int recRes = umeminit(64, -1);
	int expRes = -1; // failure
	
	printResult("3", expRes, recRes);
} // end test3


// test 4: umeminit with multiple calls to umeminit
// test if umeminit returns success the first call and failure the
//	second call
// test 4a is a success if umeminit returns 0, failure otherwise
// test 4b is a success if umeminit returns -1, failure otherwise
void test4() {
	int recRes = umeminit(64, BEST_FIT);
	int expRes = 0; // success
	
	printResult("4a", expRes, recRes);
	
	recRes = umeminit(64, BEST_FIT);
	expRes = -1; // failure
	
	printResult("4b", expRes, recRes);
} // end test4


// test 5: umemdump
// test if umemdump prints memory addresses and sizes
// test 5 is a success if umeminit returns 0 and memory address and memory 
//	size are printed (format is address: size)
void test5() {
	if(umeminit(64, BEST_FIT) == 0) {
		umemdump();
		printResult("5", 0, 0);
	} // end if
	else {
		printResult("5", 0, -1);
	} // end else
} // end test5


// test 6: umalloc with invalid size
// test if umalloc returns NULL
// test 6 is a success if umeminit returns 0 and umalloc returns NULL, 
//	failure otherwise
void test6() {
	if(umeminit(64, BEST_FIT) == 0) {
		int expRes = 0; // NULL = 0
		int recRes;
		
		if(umalloc(-1) == NULL) {
			recRes = 0; // NULL = 0
		} // end if
		else {
			recRes = 1;
		} // end else
		
		printResult("6", expRes, recRes);
	} // end if
	else {
		printResult("6", 0, -1);
	} // end else
} // end test6


// test 7: valid umalloc
// test if umalloc returns a memory address
// test 7 is a success if umeminit returns 0 and umalloc returns a value 
//	other than NULL, failure otherwise
void test7() {
	if(umeminit(64, BEST_FIT) == 0) {
		int *recRes = umalloc(4);
		
		if(recRes != NULL) {
			printPointerResult("7", recRes, recRes);
		} // end if
		else {
			printResult("7", 0, -1);
		} //end else
	} // end if
	else {
		printResult("7", 0, -1);
	} // end else
} // end test7


// test 8: multiple umalloc calls with different size requests
// test if umalloc returns memory address when called multiple times
// test 8a and 8b is a success if umeminit returns 0 and umalloc returns a 
//	value other than NULL, failure otherwise
void test8() {
	if(umeminit(64, BEST_FIT) == 0) {
		int *recRes = umalloc(4);
		
		if(recRes != NULL) {
			printPointerResult("8a", recRes, recRes);
		} // end if
		else {
			printResult("8a", 0, -1);
		} //end else
			
		recRes = umalloc(15);
		
		if(recRes != NULL) {
			printPointerResult("8b", recRes, recRes);
		} // end if
		else {
			printResult("8b", 0, -1);
		} //end else
	} // end if
	else {
		printResult("8a", 0, -1);
		printResult("8b", 0, -1);
	} // end else
} // end test8


// test 9: invalid ufree
// test if ufree returns failure
// test 9 is a success if umeminit returns 0, umalloc returns a value 
//	other than NULL, and ufree returns -1, failure otherwise
void test9() {
	if(umeminit(64, BEST_FIT) == 0) {
		if(umalloc(4) != NULL) {
			int expRes = -1; // failure
			int recRes = ufree(NULL);
		
			printResult("9", expRes, recRes);
		} // end if
	} // end if
	else {
		printResult("9", 0, -1);
	} // end else
} // end test9


// test 10: valid umalloc with valid ufree
// test if ufree returns success
// test 10 is a success if umeminit returns 0, umalloc returns a value 
//	other than NULL, and ufree returns 0, failure otherwise
void test10() {
	if(umeminit(64, BEST_FIT) == 0) {
		int *retPtr = umalloc(4);
		int expRes = 0; // success
		int recRes = ufree(retPtr);
		
		printResult("10", expRes, recRes);
	} // end if
	else {
		printResult("10", 0, -1);
	} // end else
} // end test10


// NOTE: there is no test for umeminit, umalloc, ufree with BEST_FIT
// since BEST_FIT was used in 9 of the 10 previous tests


// test 11: umeminit, umalloc, ufree with WORST_FIT
// tests if WORST_FIT works and results in success from umeminit, umalloc,
//	and ufree
// test 11a is a success if umeminit returns 0
// test 11b is a success if 11a is a success and umalloc is not NULL
// test 11c is a success if 11a and 11b succeed and ufree returns 0
void test11() {
	int recRes = umeminit(64, WORST_FIT);
	int expRes = 0; // success
	
	printResult("11a", expRes, recRes);
	
	if(recRes == expRes) {
		int *recPtr = umalloc(4);
		if(recPtr != NULL) {
			printPointerResult("11b", recPtr, recPtr);
		
			recRes = ufree(recPtr);
			printResult("11c", expRes, recRes);
		} // end if
		else {
			printResult("11b", 0, -1);
			printResult("11c", 0, -1);
		} //end else
	} // end if
	else {
		printResult("11b", 0, -1);
		printResult("11c", 0, -1);
	} // end else
} // end test11


// test 12: umeminit, umalloc, ufree with FIRST_FIT
// tests if FIRST_FIT works and results in success from umeminit, umalloc,
//	and ufree
// test 12a is a success if umeminit returns 0
// test 12b is a success if 12a is a success and umalloc is not NULL
// test 12c is a success if 12a and 12b succeed and ufree returns 0
void test12() {
	int recRes = umeminit(64, FIRST_FIT);
	int expRes = 0; // success
	
	printResult("12a", expRes, recRes);
	
	if(recRes == expRes) {
		int *recPtr = umalloc(4);
		if(recPtr != NULL) {
			printPointerResult("12b", recPtr, recPtr);
		
			recRes = ufree(recPtr);
			printResult("12c", expRes, recRes);
		} // end if
		else {
			printResult("12b", 0, -1);
			printResult("12c", 0, -1);
		} //end else
	} // end if
	else {
		printResult("12b", 0, -1);
		printResult("12c", 0, -1);
	} // end else
} // end test12


// test 13: umeminit, umalloc, ufree with NEXT_FIT
// tests if NEXT_FIT works and results in success from umeminit, umalloc,
//	and ufree
// test 13a is a success if umeminit returns 0
// test 13b is a success if 13a is a success and umalloc is not NULL
// test 13c is a success if 13a and 13b succeed and ufree returns 0
void test13() {
	int recRes = umeminit(64, NEXT_FIT);
	int expRes = 0; // success
	
	printResult("13a", expRes, recRes);
	
	if(recRes == expRes) {
		int *recPtr = umalloc(4);
		if(recPtr != NULL) {
			printPointerResult("13b", recPtr, recPtr);
		
			recRes = ufree(recPtr);
			printResult("13c", expRes, recRes);
		} // end if
		else {
			printResult("13b", 0, -1);
			printResult("13c", 0, -1);
		} //end else
	} // end if
	else {
		printResult("13b", 0, -1);
		printResult("13c", 0, -1);
	} // end else
} // end test13


// helper function to compare and print the result of two integers
//	prints Pass or Fail based on comparison of expected and received result
void printResult(char* testNum, int expResult, int recResult) {
	printf("\nTest #%s: ", testNum);
	
	if(expResult == recResult) {
		printf("%sPass%s\n", GREEN, NORM);
	} // end if
	else {
		printf("%sFail%s\n", RED, NORM);
	} // end else
	
	printf("	Expected Result: %d\n", expResult);
	printf("	Received Result: %d\n\n", recResult);
} // end printResult


// helper function to compare and print the result of two integer pointers
//	prints Pass or Fail based on comparison of expected and received result
void printPointerResult(char* testNum, int* expResult, int* recResult) {
	printf("\nTest #%s: ", testNum);
	
	if(expResult == recResult) {
		printf("%sPass%s\n", GREEN, NORM);
	} // end if
	else {
		printf("%sFail%s\n", RED, NORM);
	} // end else
	
	printf("	Expected Result: %p\n", expResult);
	printf("	Received Result: %p\n\n", recResult);
} // end printPointerResult