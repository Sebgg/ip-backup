#include "component.h"
#include <vector>
#include <iostream>
#include <iomanip>

void print_circuit(std::vector<Component*> const& circuit)
{
  for (auto * component : circuit)
  {
    std::cout << std::setw(6) << std::setprecision(2)
              << std::right << std::fixed
              << component->get_voltage();
    std::cout << std::setw(6) << component->get_current();
  }
  std::cout << std::endl;
}

void print_header(std::vector<Component*> const& circuit)
{
  for (auto * component : circuit)
  {
    std::cout << std::setw(12) << std::right;
    std::cout << component->get_name();
  }
  std::cout << std::endl;

  for (int i{}; i < circuit.size(); ++i)
  {
    std::cout << "  volt  curr";
  }
  std::cout << std::endl;
}

void simulate_circuit(std::vector<Component*>& circuit, int iterations,
  int print_steps, double const timeframe)
{
  print_header(circuit);

  int print{iterations / print_steps};
  int print_copy{print};
  while (iterations >= 0)
  {
    for (auto * component : circuit )
    {
      component->simulate(timeframe);
    }
    if (print_copy == 0)
    {
      print_circuit(circuit);
      print_copy = print;
    }
    --print_copy;
    --iterations;
  }
}

void tests(int print_steps, int iterations,
           double const timeframe, char* battery)
{
  /* Test 1
  Connection* p{new Connection{}};
  Connection* n{new Connection{}};
  Connection* r124{new Connection{}};
  Connection* r23{new Connection{}};

  std::vector<Component*> circuit{};

  circuit.push_back(new Battery{"b", p, n, battery});

  circuit.push_back(new Resistor{"r1", p, r124, 6});
  circuit.push_back(new Resistor{"r2", r124, r23, 4});
  circuit.push_back(new Resistor{"r3", r23, n, 8});
  circuit.push_back(new Resistor{"r4", r124, n, 12});
  */

  /* Test 2
  Connection* p{new Connection{}};
  Connection* n{new Connection{}};
  Connection* r{new Connection{}};
  Connection* l{new Connection{}};

  std::vector<Component*> circuit{};

  circuit.push_back(new Battery{"b", p, n, battery});

  circuit.push_back(new Resistor{"r1", p, l, 150});
  circuit.push_back(new Resistor{"r2", p, r, 50});
  circuit.push_back(new Resistor{"r3", l, r, 100});
  circuit.push_back(new Resistor{"r4", l, n, 300});
  circuit.push_back(new Resistor{"r5", r, n, 250});
  */

  /* Test 3 */
  Connection a{};
  Connection b{};
  Connection c{};
  Connection d{};

  Connection* p{&a};
  Connection* n{&b};
  Connection* r{&c};
  Connection* l{&d};

  std::vector<Component*> circuit{};

  circuit.push_back(new Battery{"bat", p, n, std::stod(battery)});

  circuit.push_back(new Resistor{"r1", p, l, 150});
  circuit.push_back(new Resistor{"r2", p, r, 50});
  circuit.push_back(new Condensator{"c3", l, r, 1.0});
  circuit.push_back(new Resistor{"r4", l, n, 300});
  circuit.push_back(new Condensator{"c5", r, n, 0.75});

  simulate_circuit(circuit, iterations, print_steps, timeframe);

  for (auto * c : circuit) // clears the vector, ensures no memleaks
  {
    delete c;
  }
}

int main(int argc, char* argv[])
{
  // Kommentar: Ni bör returnera olika värden för olika fel
  if (argc != 5)
  {
    std::cerr << "Ojojoj! Fel antal argument, försök igen :)" << std::endl;
    return -1;
  }

  for (int i{}; i < argc; ++i) //checks for negative numbers in input
  {
    if (argv[i][0] == '-')
    {
      std::cerr << "Hej svej du får inte skriva in negativa tal." << std::endl;
      return -1;
    }
  }

  int print_steps{};
  double timeframe{};
  int iterations{};
  try
  {
    iterations = std::stoi(argv[1]);
    print_steps = std::stoi(argv[2]);
    timeframe = std::stod(argv[3]);
    double i{std::stod(argv[4])}; // to check that battery voltage is correct
  } catch(std::exception& e)
  {
    std::cerr << "Det verkar ha blivit fel i argumenten." << std::endl
              << "Tänk på att det är"
              << " \"./a.out <iterationer> <utskriftssteg>"
              << " <tidssteg> <batterispänning>\"" << std::endl;
    return -1;
  }

  tests(print_steps, iterations, timeframe, argv[4]);

  return 0;
}
