#include <iostream>
#include <set>
#include "field.h"
#include "analyzer.h"

int main() try {
	MSfieldPart1 f2("field2");
	//  012345678            012345678
	// +---------+          +---------+
	//0|121212121|         0|121212121|
	//1|         |         1|msmsmsmsm|
	// +---------+          +---------+
	std::cout << f2 << std::endl;

	//part 1
	Analyzer a2(f2);

	std::cout << ( ( a2.IsMine(0,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(2,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(4,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(6,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(8,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(1,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(3,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(5,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(7,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
                                  
	std::cout << ( ( a2.IsSafe(0,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(2,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(4,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(6,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(8,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(1,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(3,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(5,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(7,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";

	MSfieldPart1 f4("field4");
	//            01234
	//+-----+    +-----+
	//|     |   0|smsms|
	//| 242 |   1|s242s|
	//|     |   2|smsms|
	//+-----+    +-----+
	std::cout << f4 << std::endl;

	//part 1
	Analyzer a4(f4);

	std::cout << ( ( a4.IsMine(1,0) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
  std::cout << ( ( a4.IsMine(3,0) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsMine(1,2) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsMine(3,2) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsSafe(0,0) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsSafe(0,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsSafe(0,2) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsSafe(2,0) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsSafe(2,2) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsSafe(4,0) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsSafe(4,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsSafe(4,2) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";

	std::cout << ( ( a4.IsSafe(1,0) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsSafe(3,0) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsSafe(1,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsSafe(3,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsMine(0,0) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsMine(0,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsMine(0,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsMine(2,0) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsMine(2,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsMine(4,0) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsMine(4,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a4.IsMine(4,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";

	MSfieldPart1 f1("field1");
	//  01234567            01234567  
	// +--------+          +--------+
	//0|23 22 21|         0|23m22m21|
	//1|  4  4 2|         1|mm4  4m2|
	//2|      4 |         2| s    4m|
	//3| 5 6   2|         3|m5 6mm 2|
	//4|2   55 2|         4|2m  55 2|
	//5|134   4 |         5|134 mm4m|
	//6|01 4   3|         6|01m4  m3|
	//7|012 23 2|         7|012m23m2|
	// +--------+          +--------+
	std::cout << f1 << std::endl;

	//part 1
	Analyzer a1(f1);

	std::cout << ( ( a1.IsMine(0,3) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a1.IsMine(2,0) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a1.IsMine(6,3) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a1.IsMine(4,3) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a1.IsSafe(1,2) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";

	std::cout << ( ( a1.IsMine(0,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a1.IsSafe(0,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
} catch ( MSexception& e) {
	std::cerr << e.what() << std::endl;
}
