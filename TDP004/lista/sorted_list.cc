#include "sorted_list.h"

Sorted_List& Sorted_List::operator =(Sorted_List const& other)
{
  Sorted_List temp{other};
  std::swap(first, temp.first);
  list_size = other.list_size;
  return *this;
}

Sorted_List& Sorted_List::operator =(Sorted_List&& other)
{
  delete first;

  first = other.first;
  list_size = other.list_size;

  other.first = nullptr;

  return *this;
}

std::string Sorted_List::to_string() const
{
  std::string output{};
  output += "[";

  if (list_size == 0)
  {
    output += "]";
    return output;
  }

  Node* current{first};
  while (current->next != nullptr)
  {
    output = output + std::to_string(current->value) + ", ";
    current = current->next;
  }

  output = output + std::to_string(current->value) + "]";
  return output;
}

std::ostream& operator <<(std::ostream& outstream, Sorted_List const& list)
{
  outstream << list.to_string();
  return outstream;
}

Sorted_List::Node* Sorted_List::insert_helper(Sorted_List::Node* current, int value)
{
  if (current->next == nullptr || current->next->value > value)
  {
    current->next = new Sorted_List::Node{value, current->next};
    return current;
  }
  else
  {
     current->next = insert_helper(current->next, value);
     return current;
  }
}

void Sorted_List::insert(int value)
{
  if (first == nullptr)
  {
    first = new Sorted_List::Node{value};
    list_size += 1;
  }
  else if (first->value > value)
  {
    first = new Sorted_List::Node{value, first};
    list_size += 1;
  }
  else
  {
    first = insert_helper(first, value);
    list_size += 1;
  }
}

void Sorted_List::remove(int value)
{
  if (first != nullptr)
  {
    Sorted_List::Node* current{first};
    if (first->value == value)
    {
      first = current->next;
      current->next = nullptr;
      delete current;
      list_size -= 1;
    }
    else
    {
      Sorted_List::Node* next{current->next};

      while (next != nullptr)
      {
        if (next->value == value)
        {
          break;
        }
        else
        {
          current = next;
          next = next->next;
        }
      }
      if (next != nullptr && next->value == value)
      {
        current->next = next->next;
        next->next = nullptr;
        delete next;
        list_size -= 1;
      }
    }
  }
}

bool Sorted_List::is_empty() const
{
  return ( first == nullptr );
}

int Sorted_List::get_value_of_index(unsigned int index) const
{
  if (size() < index)
  {
    return -1;
  }
  else
  {
    Sorted_List::Node* current{first};
    while (index > 0)
    {
      current = current->next;
      --index;
    }
    return current->value;
  }
}

int Sorted_List::get_index_of(Sorted_List::Node* const n) const
{
  if (first == nullptr)
  {
    return -1;
  }

  Sorted_List::Node* current{first};
  int index{1};
  while (current->value != n->value)
  {
    ++index;
    current = current->next;
  }
  return index;
}

Sorted_List::Node* Sorted_List::get_node(unsigned int index) const
{
  if (index > list_size)
  {
    return nullptr;
  }

  Sorted_List::Node* current{first};
  while (index > 0)
  {
    current = current->next;
    --index;
  }

  return current;
}

unsigned int Sorted_List::size() const
{
  return list_size;
}

Sorted_List::Node* Sorted_List::copy(Sorted_List::Node* const current) const
{
  if (current != nullptr)
  {
    return new Sorted_List::Node{current->value, copy(current->next)};
  }
  else
  {
    return nullptr;
  }
}
