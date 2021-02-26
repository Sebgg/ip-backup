#include "catch.hpp"

#include "sorted_array.h"
#include "less.h"
#include "distance.h"

#include <sstream>
#include <string>

TEST_CASE("default initialization")
{
  Sorted_Array a;
  CHECK (a.size() == 0);
  CHECK_THROWS (a.at(0));
  CHECK_THROWS (a.erase(0));
  CHECK_THROWS (a.erase(-5));
  CHECK_THROWS (a.erase(5));
}

TEST_CASE("insertion")
{
  Sorted_Array<double> a;
  a.insert(5);
  CHECK (a.size() == 1);
  CHECK (a.at(0) == 5);
  CHECK (a[0] == 5);

  a.insert(1);
  CHECK (a.size() == 2);
  CHECK (a[0] == 1);
  CHECK (a[1] == 5);
}

TEST_CASE("initialization")
{
  Sorted_Array a{17, 9, 5, 2, -100};
  CHECK (a.size() == 5);
  CHECK (a[0] == -100);
  CHECK (a[1] == 2);
  CHECK (a[2] == 5);
  CHECK (a[3] == 9);
  CHECK (a[4] == 17);

  a.insert(1);
  CHECK (a.size() == 6);
  CHECK (a[0] == -100);
  CHECK (a[1] == 1);
  CHECK (a[2] == 2);
  CHECK (a[3] == 5);
  CHECK (a[4] == 9);
  CHECK (a[5] == 17);

  a.erase(5);
  CHECK (a.size() == 5);
  CHECK (a[0] == -100);
  CHECK (a[1] == 1);
  CHECK (a[2] == 2);
  CHECK (a[3] == 5);
  CHECK (a[4] == 9);

  CHECK_THROWS(a.erase(5));

  a.erase(2);
  CHECK (a.size() == 4);
  CHECK (a[0] == -100);
  CHECK (a[1] == 1);
  CHECK (a[2] == 5);
  CHECK (a[3] == 9);
}

TEST_CASE("const array")
{
  Sorted_Array const a{1, 2, 3, 4, 5};

  CHECK (a.size() == 5);
  CHECK (a[0] == 1);
  CHECK (a[0] == a.at(0));
  CHECK (a[1] == 2);
  CHECK (a[1] == a.at(1));
  CHECK (a[2] == 3);
  CHECK (a[2] == a.at(2));
  CHECK (a[3] == 4);
  CHECK (a[3] == a.at(3));
  CHECK (a[4] == 5);
  CHECK (a[4] == a.at(4));
  CHECK_THROWS (a.at(5));
}

TEST_CASE("print empty")
{
  std::ostringstream oss;
  Sorted_Array<int> empty;
  oss << empty;
  CHECK (oss.str() == "{}");
}

TEST_CASE("print")
{
  std::ostringstream oss;
  Sorted_Array<int> array{2, 3, 5, 7, 11, 13};
  oss << array;
  CHECK (oss.str() == "{2, 3, 5, 7, 11, 13}");
}

SCENARIO("Trying to make a list with std::string")
{
  GIVEN("An empty list")
  {
    Sorted_Array<std::string> a{};

    WHEN("A string is inserted")
    {
      a.insert("Hej hej");
      THEN("List size should be one, and value at index 0 \"hej hej\"")
      {
        CHECK (a[0] == "Hej hej");//passed
        CHECK (a.size() == 1);//passed
      }
    }
  }

  GIVEN("A list with 5 strings in it")
  {
    Sorted_Array<std::string> a{"hej", "svej", "på", "dig", "ju"};

    WHEN("A string smaller than all but one is inserted")
    {
      a.insert("efter");

      THEN("the value at index 1 should be \"efter\"")
      {
        CHECK (a[1] == "efter");//passed
        CHECK (a.size() == 6);//passed
      }
    }
    WHEN("A item is removed")
    {
      a.erase(1);

      THEN("the list size should be one less")
      {
        CHECK (a[1] == "ju");//passed
        CHECK (a.size() == 4);//passed
      }
    }
  }
}

SCENARIO ("Trying to make a list with double")
{
  GIVEN ("An empty list")
  {
    Sorted_Array<double> a{};

    WHEN ("A double is inserted")
    {
      a.insert(0.05);
      THEN("List size should be one, and value at index 0 should be 0.05")
      {
        CHECK (a[0] == 0.05);
        CHECK (a.size() == 1);
      }
    }
  }
  GIVEN ("A list with 5 doubles in it")
  {
    Sorted_Array<double> a{0.02, 0.07, 69.69, 32.2, 133.7};

    WHEN ("A double is inserted")
    {
      a.insert(10.11);

      THEN ("The value at index 2 should be 10.11")
      {
        CHECK (a[2] == 10.11);
      }
    }
    WHEN ("A double is erased")
    {
      a.erase(a[0]);

      THEN ("The value at index 0 should be 0.07")
      {
        CHECK (a[0] == 0.07);
      }
    }
  }
}

SCENARIO("Testing the less than comparator")
{
  GIVEN("a Less<int> comparator")
  {
    Less<int> a{};

    WHEN("two integers are compared low < high")
    {
      int greater{10};
      int less{5};
      bool result = a.compare(less, greater);
      THEN("result should be true")
      {
        CHECK (result);
      }
    }
    WHEN("two integers are compared high < low")
    {
      int greater{10};
      int less{5};
      bool result = a.compare(greater, less);
      THEN("result should be false")
      {
        CHECK (!result);
      }
    }
  }
  GIVEN("a Less<string> comparator")
  {
    Less<std::string> a{};

    std::string low{"abcde"};
    std::string high{"edcba"};
    WHEN("two strings are compared low < high")
    {
      bool result = a.compare(low, high);
      THEN("result should be true")
      {
        CHECK (result);
      }
    }
    WHEN("two strings are compared high < low")
    {
      bool result = a.compare(high, low);
      THEN("result should be false")
      {
        CHECK (!result);
      }
    }
  }
  GIVEN("a Less<double> comparator")
  {
    Less<double> a{};

    double high{133.7};
    double low{69.69};
    WHEN("two doubles are compared low < high")
    {
      bool result = a.compare(low, high);
      THEN ("result should be true")
      {
        CHECK (result);
      }
    }
    WHEN("two doubles are compared high < low")
    {
      bool result = a.compare(high, low);
      THEN("result should be false")
      {
        CHECK (!result);
      }
    }
  }
}

TEST_CASE("Testing the distance comparator")
{
  Distance<int> d{10};
  CHECK (d.compare(9, 13));
  CHECK (!d.compare(0, 15));
}

TEST_CASE("Testing 2c")
{
  Distance<int> d{10};
  Sorted_Array<int, Distance<int>> a{{5, 14, 0, -15, 19, 10, 100}, d};

  CHECK (a[0] == 10);
  CHECK (a[6] == 100);
  CHECK (a[3] == 19);
}
