#include "component.h"
#include <cmath>

/**
* Component normal constructor
*/
Component::Component(std::string name, Connection* left, Connection* right)
  : name{name}, left{left}, right{right}
{
}

/**
* Battery normal constructor
*/
Battery::Battery(std::string name, Connection* positive, Connection* negative,
                 double charge)
  : Component{name, positive, negative}, charge{charge}
{
}



/**
* Resistor normal constructor
*/
Resistor::Resistor(std::string name, Connection* left, Connection* right,
                   double resistance)
  : Component{name, left, right}, resistance{resistance}
{
}

/**
* Condensator normal constructor
*/
Condensator::Condensator(std::string name, Connection* left, Connection* right,
                         double capacity)
  : Component{name, left, right}, capacity{capacity}, charge{0}
{
}

/**
* Condensator destructor
*/

/**
* Connection default constructor
*/
Connection::Connection()
  : charge{0}
{
}

/**
* Component functions
*/
double Component::get_voltage() const
{
  return fabs(left->charge - right->charge);
}

std::string Component::get_name() const
{
  return name;
}

void Battery::simulate( [[maybe_unused]] double const time )
{
  left->charge = charge;
  right->charge = 0;
}

void Component::set_charges(bool const& positive, double const current)
/**
* Sets the charges of the connections for the component.
*/
{
  if (positive)
  {
    left->charge = (left->charge + current);
    right->charge = (right->charge - current);
  }
  else
  {
    left->charge = (left->charge - current);
    right->charge = (right->charge + current);
  }
}

void Resistor::simulate(double const time)
{
  // Snyggt! 
  if (left->charge > right->charge)
  {
    set_charges(false, get_current() * time);
  }
  else if (left->charge < right->charge)
  {
    set_charges(true, get_current() * time);
  }
}

void Condensator::simulate(double const time)
{
  if (left->charge > right->charge)
  {
    // Kommentar: Denna bör finnas utanför och sättas en gång. 
    double current = get_current() * time;
    set_charges(false, current);
    charge += current;
  }
  else if (left->charge < right->charge)
  {
    double current = get_current() * time;
    set_charges(true, current);
    charge += current;
  }
}

double Battery::get_current() const
{
  return 0;
}

double Resistor::get_current() const
{
  double current{(get_voltage()) / resistance};
  return current;
}

double Condensator::get_current() const
{
  double current{fabs((get_voltage()) - charge) * capacity};
  return current;
}
