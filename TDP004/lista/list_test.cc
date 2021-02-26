// This test program uses a test framework supporting TDD and BDD.
// You are not required to use the framework, but encouraged to.
// Documentation:
// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md

// You ARE however required to implement all test cases outlined here,
// even if you do it by way of your own function for each case.  You
// are recommended to solve the cases in order, and rerun all tests
// after you modify your code.

// This define lets Catch create the main test program
// (Must be in only one place!)
#include "catch.hpp"

#include <random>
#include <iostream>
#include <utility>

#include "sorted_list.h"

//=======================================================================
// Test cases
//=======================================================================
// Solve one TEST_CASE or WHEN at a time!
//
TEST_CASE( "Memoryleak special test")
{
  Sorted_List l{};
  Sorted_List l2{};
  l.insert(1);
  l.insert(2);
  l2.insert(3);
  l2.insert(4);

  l2 = std::move(l);
  REQUIRE( l2.get_value_of_index(0) == 1 );


}


TEST_CASE( "Create an empty list" )
{
    Sorted_List l{};

    REQUIRE( l.is_empty() ); //passed
    REQUIRE( l.size() == 0 ); //passed
}


TEST_CASE( "Insert an item in an empty list" )
{
    Sorted_List l{};

    l.insert(5);

    REQUIRE( !l.is_empty() ); //passed
    REQUIRE( l.size() == 1 ); //passed

}

SCENARIO( "Empty lists" )
{

  GIVEN( "An empty list" )
  {
  	Sorted_List l{};

  	REQUIRE( l.is_empty() ); //passed
  	REQUIRE( l.size() == 0 ); //passed

  	WHEN( "an item is inserted" )
  	{

      // insert an item
      l.insert(1);
      THEN( "the size increase and the item is first in the list" )
	    {
    		REQUIRE( !l.is_empty()  ); //passed
    		REQUIRE( l.size() == 1 ); //passed
    		REQUIRE( l.get_value_of_index(0) == 1); //passed
	    }
  	}



  	WHEN( "an item is removed" )
  	{

	    // remove an item

      l.remove(1);
      THEN( "the list is still empty" )
      {
        // add your REQUIRE statements
        REQUIRE( l.is_empty() ); //passed
        REQUIRE( l.size() == 0 ); //passed
      }
    }

  	WHEN( "the list is copied to a new list" )
  	{

      // copy your list to a new variable (copy constructor)
      Sorted_List copy{l};
      THEN( "the new list is also empty" )
      {
  		    // add your REQUIRE statements
          REQUIRE( copy.is_empty() ); //passed
          REQUIRE( copy.size() == 0 ); //passed
  	  }
  	}

  	WHEN( "the list is copied to an existing non-empty list" )
  	{
	    // create and fill a list to act as the existing list
	    // copy (assign) your empty list to the existing
      Sorted_List full{};
      full.insert(1);
      full.insert(3);
      full.insert(5);
      full.insert(7);
      full.insert(9);
      Sorted_List empty{full};
	    THEN( "the new list is the same as the old list" )
	    {
		      // add your REQUIRE statements
          REQUIRE( empty.size() == 5 );//passed
          REQUIRE( !empty.is_empty() );//passed
	    }
	  }
  }
}

SCENARIO( "Single item lists" )
{

  GIVEN( "A list with one item in it" )
  {
    // create the given scenario
    Sorted_List l{};
    l.insert(4);
  	WHEN( "a smaller item is inserted" )
  	{
      l.insert(1);
	    THEN( "and the first item in list is new value" )
	    {
        REQUIRE( l.get_value_of_index(0) == 1 );//passed
        REQUIRE( l.size() == 2 );//passed
        REQUIRE( !l.is_empty() );//passed
	    }
  	}
  	WHEN( "a larger item is inserted" )
  	{
      l.insert(5);
	    THEN( "Larger item is last in the list." )
	    {
        REQUIRE( l.get_value_of_index(1) == 5 );//passed
        REQUIRE( l.size() == 2 );//passed
        REQUIRE( !l.is_empty() );//passed
	    }
  	}
  	WHEN( "an item is removed" )
  	{
      l.remove(4);
	    THEN( "list will be empty" )
	    {
        REQUIRE( l.size() == 0 );//passed
        REQUIRE( l.is_empty() );//passed
	    }
  	}
  	WHEN( "the list is copied to a new list" )
  	{
      Sorted_List _new{l};
	    THEN( "Both lists will have the same size and same values." )
	    {
        REQUIRE( _new.size() == l.size() );//passed
        REQUIRE( !_new.is_empty() );//passed
        REQUIRE( _new.get_value_of_index(0) == l.get_value_of_index(0) );//passed
	    }
  	}

  	WHEN( "the list is copied to an existing non-empty list" )
  	{
      Sorted_List _new{};
      _new.insert(69);
      _new = l;
	    THEN( "the lists should look identical" )
	    {
        REQUIRE( _new.size() == l.size() );//passed
        REQUIRE( !_new.is_empty() );//passed
        REQUIRE( _new.get_value_of_index(0) == l.get_value_of_index(0) );//passed
	    }
  	}
  }
}

SCENARIO( "Multi-item lists" )
{

  GIVEN( "A list with five items in it" )
  {

  	// create the given scenario and all THEN statements
  	// and all REQUIRE statements

    Sorted_List ml{};
    ml.insert(3);
    ml.insert(5);
    ml.insert(7);
    ml.insert(9);
    ml.insert(11);
    WHEN( "an item smaller than all other is inserted" )
    {
      ml.insert(1);
      THEN( "list index 0 should be 1 and list size should be 6" )
      {
        REQUIRE( ml.size() == 6 );//passed
        REQUIRE( !ml.is_empty() );//passed
        REQUIRE( ml.get_value_of_index(0) == 1 );//passed
      }
    }


    WHEN( "an item larger than all other is inserted" )
    {
      ml.insert(69);
      THEN( "last list index should be 69 and list size should be 6" )
      {
        REQUIRE( ml.size() == 6 );//passed
        REQUIRE( !ml.is_empty() );//passed
        REQUIRE( ml.get_value_of_index(5) == 69 );//passed
      }
    }
    WHEN( "an item smaller than all but one item is inserted" )
    {
      ml.insert(4);
      THEN( "index 1 should be 4 and list size should be 6" )
      {
        REQUIRE( ml.size() == 6 );//passed
        REQUIRE( !ml.is_empty() );//passed
        REQUIRE( ml.get_value_of_index(1) == 4 );//passed
      }
    }
    WHEN( "an item larger than all but one item is inserted" )
    {
      ml.insert(10);
      THEN( "second to last index should be 10 and list size should be 6" )
      {
        REQUIRE( ml.size() == 6 );//passed
        REQUIRE( !ml.is_empty() );//passed
        REQUIRE( ml.get_value_of_index(4) == 10 );//passed
      }
    }
    WHEN( "an item is removed" )
    {
      ml.remove(3);
      THEN( "list size should be 4 and index 0 should be 5" )
      {
        REQUIRE( ml.size() == 4 );//passed
        REQUIRE( !ml.is_empty() );//passed
        REQUIRE( ml.get_value_of_index(0) == 5 );//passed
      }
    }
    WHEN( "all items are removed" )
    {
      ml.remove(3);
      ml.remove(7);
      ml.remove(5);
      ml.remove(9);
      ml.remove(11);
      THEN( "Empty list" )
      {
        REQUIRE( ml.size() == 0 );//passed
        REQUIRE( ml.is_empty() );//passed
      }
    }

    WHEN( "the list is copied to a new list" )
    {
      Sorted_List _new{ml};
      THEN( "_new should be same size and contain same values as ml" )
      {
        REQUIRE( ml.get_value_of_index(0) == _new.get_value_of_index(0));//passed
        REQUIRE( ml.get_value_of_index(3) == _new.get_value_of_index(3));//passed
        REQUIRE( ml.size() == _new.size() );//passed
      }
    }

    WHEN( "the list is copied to an existing non-empty list" )
    {
      Sorted_List _new{};
      _new.insert(2);
      _new.insert(4);
      _new.insert(6);
      _new.insert(8);
      _new = ml;

      THEN( "_new should be same size and contain same values as ml" )
      {
        REQUIRE( ml.get_value_of_index(0) == _new.get_value_of_index(0));//passed
        REQUIRE( ml.get_value_of_index(4) == _new.get_value_of_index(4));//passed
        REQUIRE( ml.size() == _new.size() );//passed
      }
    }
  }
}

SCENARIO( "Lists can be copied" )
{

  GIVEN( "A list with five items in it, and a new copy of that list" )
  {
    Sorted_List ml{};
    ml.insert(2);
    ml.insert(4);
    ml.insert(6);
    ml.insert(8);
    ml.insert(10);
    Sorted_List cl{ml};

  	WHEN( "the original list is changed" )
  	{
      ml.remove(2);
      ml.insert(1);
      ml.remove(10);
	    THEN( "the copy remain unmodified" )
	    {
        REQUIRE( cl.get_value_of_index(0) == 2 );//passed
        REQUIRE( ml.size() != cl.size() );//passed
        REQUIRE( cl.size() == 5 );//passed
      }
    }

  	WHEN( "the copied list is changed" )
  	{
      cl.insert(12);
      cl.insert(14);
      cl.remove(6);
      cl.insert(0);
	    THEN( "the original remain unmodified" )
	    {
        REQUIRE( ml.get_value_of_index(0) == 2 );//passed
        REQUIRE( ml.size() != cl.size() );//passed
        REQUIRE( ml.size() == 5 );//passed
        std::cout << ml.to_string() << std::endl;
	    }
  	}
  }
}


SCENARIO( "Lists can be heavily used" )
{

  GIVEN( "A list with 1000 random items in it" )
  {

    Sorted_List big_list{}; // create the given list with 1000 random items
    std::random_device rd;
    std::uniform_int_distribution<int> uniform(1,1000);

    for (int i{0}; i < 1000; ++i)
    {
        int random { uniform(rd) }; // generate a random number
        big_list.insert(random); // insert into list
    }

    WHEN( "the list have 1000 random numbers inserted" )
    {
      for (int i{0}; i < 999; ++i)
      {
          int random { uniform(rd) }; // generate a random number
          big_list.insert(random); // insert into list
      }
      big_list.insert(1);
      THEN( "the list have 2000 items in correct order" )
        REQUIRE( big_list.size() == 2000);//
        big_list.insert(1);
        big_list.remove(1);
        REQUIRE( big_list.size() == 2000 );
        // (assumes unique inserts or duplicates allowed)
    }
    WHEN( "the list have 2000 random numbers removed" )
    {
      Sorted_List _new{big_list};

      for (int i{0}; i < 2000; ++i)
      {
        int random { uniform(rd) }; // generate a random number
        _new.remove(random); // insert into list
      }

      THEN( "the list is empty" )
      {
        // (assumes successful removes)
        REQUIRE( big_list.size() > _new.size() );//passed
        CHECK( !_new.is_empty() );// fails as it should
      }
    }
  }
}

Sorted_List trigger_move(Sorted_List l)
{
    // do some (any) modification to list
    l.insert(3);
    l.remove(1);
    return std::move(l);
}

SCENARIO( "Lists can be passed to functions" )
{

  GIVEN( "A list with 5 items in it" )
  {

    Sorted_List given{};
    // insert 5 items
    given.insert(1);
    given.insert(3);
    given.insert(5);
    given.insert(7);
    given.insert(9);

    REQUIRE( given.size() == 5 ); //passed

    WHEN( "the list is passed to trigger_move()" )
    {

      Sorted_List l{ trigger_move(given) };

      THEN( "the given list remain and the result have the change" )
      {
        REQUIRE( l.size() == 5 );//passed
        REQUIRE( l.get_value_of_index(0) == 3 );//passed
        REQUIRE( !l.is_empty() );//passed
        REQUIRE( l.to_string() == "[3, 3, 5, 7, 9]");//passed
      }
    }
  }
}



// In addition you must of course verify that the list is printed
// correct and that no memory leaks occur during use. You can solve
// the printing part on your own. Here's how to run the (test) program
// when you check for memory leaks:

// You must of course verify that no memory leaks occur during use.
// To do so, an external program must be used to track what memory
// is allocated and free'd, you can run such a program like so:
//
// $ valgrind --tool=memcheck a.out

// Finally you need to check that you can do all operations that make
// sense also on a immutable list (eg all operations but insert):
void use_const_list(Sorted_List const& l)
{
    // perform every operation that do not modify the list here
    std::cout << l << std::endl;
    unsigned int x{ l.size() };
    bool yeet{ l.is_empty() };
    int index_value{ l.get_value_of_index(0) };
    if ( index_value == -1)
    {
      std::cout << "This is correct" << std::endl;
    }
    //return l;
}

// Move this comment and following #if 0 down one case at a time!
// Make sure to close any open braces before this comment.
// The #if 0 will disable the rest of the file.
#if 0
#endif
