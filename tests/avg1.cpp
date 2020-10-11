/*
 *          File: avg1.cpp
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: February 2, 2000
 *         Topic: Using Emacs for Programming - Debugging
 * ----------------------------------------------------------------
 *
 * Usage:
 * =====
 * The user first enters how many numbers need to be averaged and
 * hits <RETURN>.  Then, the user enters each of the numbers on a
 * separate line.  Finally, the average of the numbers is printed.
 */

#include <iostream.h>


/************************ Function Prototypes *********************/

// avgNumbers: Returns the average of the numbers.
float avgNumbers(float numbers[], int howmany);

// sumNumbers: Returns the sum of the numbers.
float sumNumbers(float numbers[], int howmany);

/*************************** Main Function *************************/

int main()
{
  const int MAX_NUMS = 10;  // Can deal with at most MAX_NUMS numbers.
  int howmany;              // How many numbers entered.
  float numbers[MAX_NUMS];  // Array to hold the numbers.

  // Ask how many numbers to read.

  cout << "Enter how many numbers I will average: ";
  cin >> howmany;  // Should error check "howmany"!

  // Store the numbers in array.

  for (int i; i < howmany; i++) {
    // Read the ith number.
    cout << "Enter number: " ;
    cin >> numbers[i];
  }

  cout << "The average is: " << avgNumbers(numbers, howmany) << endl;

  cout << "Thank you for using the averager!" << endl;

  return 0;  // 0 means program exited successfully.
}


/************************ Function Definitions *********************/

float avgNumbers(float numbers[], int howmany)
{
  float sum = sumNumbers(numbers, howmany);
  return sum / howmany;
}

float sumNumbers(float numbers[], int howmany)
{
  float sum = 0;

  for (int i = 0; i < howmany; i++) {
    numbers[i] += sum;
  }

  return sum;
}
