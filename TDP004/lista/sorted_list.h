#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>

class Sorted_List
{
  /**
  * Implementation of Node class inside Sorted_List class
  */
  class Node
  {
  public:
    Node(int value = 0, Node* next = nullptr)
    : value{value}, next{next}
    {
    }

    ~Node()
    {
      delete next;
    }

    int value;
    Node* next;

  };

public:
  /**
  * default constructor
  */
  Sorted_List()
    : first{nullptr}, list_size{0}
  {
  }

  /**
  * Copy constructor
  */
  Sorted_List(const Sorted_List& original)
   : first{copy(original.first)}, list_size{original.list_size}
  {
  }

  /**
  * Move constructor
  */
  Sorted_List(Sorted_List&& original)
   : first{original.first}, list_size{original.list_size}
   {
     original.first = nullptr;
     original.list_size = 0;
   }

  ~Sorted_List()
  {
    delete first;
    first = nullptr;
    list_size = 0;
  }


  /**
  * Operator overloads
  */
  Sorted_List& operator =(Sorted_List const&); // Kopieringstilldelningoperator
  Sorted_List& operator =(Sorted_List&&); // Flyttilldelningsoperator

  friend std::ostream& operator<<(std::ostream&, Sorted_List const&);

  /**
  * To string function
  */
  std::string to_string() const;

  /**
  * Insert functions
  */
  void insert(int);

  /**
  * Remove functions
  */
  void remove(int);

  /**
  * Checker functions
  */
  bool is_empty() const;

  /**
  * Getters
  */
  int get_value_of_index(unsigned int) const;
  unsigned int size() const;


private:
  Node* first;
  unsigned int list_size;

  Node* insert_helper(Node*, int);

  int get_index_of(Node* const) const;
  Node* get_node(unsigned int) const;

  /**
  * Copy functions
  */
  Node* copy(Node* const) const;

};

#endif
