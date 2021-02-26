#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>

struct Connection
{
  Connection();
  ~Connection() = default;
  double charge{};
};

class Component
{
public:
  Component(std::string name, Connection* left, Connection* right);
  virtual ~Component() = default;

  virtual void simulate(double const) = 0;

  double get_voltage() const;
  virtual double get_current() const = 0;
  std::string get_name() const;

  void set_charges(bool const&, double const);

protected:
  std::string name;
  Connection* left;
  Connection* right;
};

class Battery : public Component
{
public:
  Battery(std::string name, Connection* positive,
          Connection* negative, double charge);
  ~Battery() = default;
  void simulate(double const) override;

  double get_current() const override;
private:
  double charge{};
};

class Resistor : public Component
{
public:
  Resistor(std::string name, Connection* left, Connection* right,
           double resistance);
  ~Resistor() = default;
  void simulate(double const) override;

  double get_current() const override;
private:
  double resistance{};
};

class Condensator : public Component
{
public:
  Condensator(std::string name, Connection* left, Connection* right,
              double capacity);
  ~Condensator() = default;
  void simulate(double const) override;

  double get_current() const override;
private:
  double capacity{};
  double charge{};
};

#endif
