#include "catch.hpp"
#include "Time.h"
#include <iostream>

// här lägger ni era testfall.
// Jobba enligt TDD;
//  1. Lägg till testfall
//  2. Testa
//  3. Lägg till (minsta möjliga) implementation
//  4. Testa alla testfall
//  5. Refaktorera (skriv om) så att allt ser bra ut

Time t1{12, 5, 30};
Time t2{12, 0, 59};
Time t3{9, 12, 18};
Time t4{9, 12, 18};
Time t5{12, 1, 0};
Time t8{1,2,3};
Time t9{};
Time te{11, 40, 00};
Time tf{12, 10, 00};

TEST_CASE( "I/O tests" )
{
  std::cout << "Enter a time: ";
  std::cin >> t9;
  CHECK(std::cin.fail()); //Should fail on correct input, does
  CHECK(std::cin.good()); //passed

  std::string temp{t1.to_string(true)};
  CHECK( temp == "12:05:30 pm"); //passed
}

TEST_CASE( "Comparison tests" )
{

  REQUIRE( t1 > t2 ); //passed
  CHECK( t1 < t2 ); //failed as expected
  REQUIRE( t3 < t2 ); //passed

  REQUIRE( t3 == t4 ); //passed
  CHECK( t3 != t4 ); //failed as expected
  REQUIRE( t1 != t2 ); //passed

  REQUIRE( t1 >= t2 ); //passed
  CHECK( t2 >= t1); //failed as expected
  REQUIRE( t3 <= t2 ); //passed
}

TEST_CASE( "Step tests" )
{
  t2++;
  REQUIRE( t2 == t5 ); //passed
  --t2;
  t5--;
  REQUIRE( t5 == t2 ); //passed
  t5++;

  CHECK( (t2++) == t5 ); //failed as expected
  CHECK( (--t5) <= t2 ); //passed
}

TEST_CASE( "Mathematical operators")
{
  /**
  * Testing with positive integers
  */
  Time t6{t2 + 270};
  REQUIRE( t1 == t6 ); //passed

  Time t7{t1 - 270};
  REQUIRE( t7 == t2 ); //passed

  /**
  * Testing with negative integers
  */

  Time nt1{tf + -1800};
  REQUIRE( nt1 == te); //passed

  Time nt2{ te - -1800 };
  Time ntt1{12, 10, 00};
  REQUIRE( nt2 == ntt1 ); //passed

  /**
  * Testing time - time
  */
  Time tb{t3 + t4};
  Time tc{18, 24, 36};
  REQUIRE( tb == tc); //passed

  Time ta{t3 - t4};
  Time tt{0, 0, 0};
  REQUIRE( ta == tt ); //passed
}

TEST_CASE( "AM_PM" )
{
  REQUIRE( t1.to_string(true) == "12:05:30 pm" ); //passed

  REQUIRE( t3.to_string(true) == "09:12:18 am" ); //passed


  Time td{11, 59, 59};
  ++td;
  REQUIRE( td.to_string(true) == "12:00:00 pm" ); //passed

}
