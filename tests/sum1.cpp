/*
 *          File: sum1.cpp
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: January 27, 2000
 *         Topic: Using Emacs for Programming - Compiling
 * ----------------------------------------------------------------
 *
 * Usage:
 * =====
 * The user first enters how many numbers need to be summed and
 * hits <RETURN>.  Then, the user enters each of the numbers on a
 * separate line.  Finally, the sum of the numbers is printed.
 */


#include <iostream.h>

int main()
{
  int howmany      // How many values to sum.
  float sum = 0.0;  // The running sum.

  // Ask how many numbers to read.

  cout << "Enter how many numbers I will sum: ";
  cin >> howmany;

    for (int i = O; i < howmany; i++) {
      float value;  // The current value.

  // Read the ith number.
      cout << "Enter number: " ;
      cin >> value;
    
   // Increment the sum.
      sum += value;
      }

  cout << "The sum is: " << sum << endl;

  cout << "Thank you for using the summer!" << endl;

  return 0;  // 0 means program exited successfully.
}
