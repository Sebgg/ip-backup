// Denna fil ska innehålla deklarationer för de typer och funktioner
// som behövs

#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <string>

struct Time
{
  /**
  * Normal constructor
  */
  Time(int hours = 0, int minutes = 0, int seconds = 0)
    : hours{hours}, minutes{minutes}, seconds{seconds}
  {
  }

  /**
  * Copy constructor
  */
  Time(const Time& other)
    : hours{other.hours}, minutes{other.minutes},
      seconds{other.seconds}
  {
  }
  /**
  *Logical comparison operators
  */
    //Kommentar: Bra att ni gjort dessa const
  bool operator<(const Time&) const;
  bool operator>(const Time&) const;

  bool operator<=(const Time&) const;
  bool operator>=(const Time&) const;

  bool operator==(const Time&) const;
  bool operator!=(const Time&) const;


  /**
  *input/output operators
  */
  friend std::ostream& operator<<(std::ostream&, Time const&);
  friend Time& operator>>(std::istream&, Time&);

  /**
  * preincremential operators
  */
  Time& operator++();
  Time& operator--();

  /**
  * postincremential operators
  */
  Time operator++(int);
  Time operator--(int);

  /**
  * Addition and subtraction operators with time and int
  */
  Time operator+(int);
  Time operator-(int);

  /**
  * Addition and subtraction operators with time and time
  */
  Time operator+(Time&);
  Time operator-(Time&);

  std::string to_string(bool);

  /**
  * Getters
  */
  int get_hours() const;
  int get_minutes() const;
  int get_seconds() const;

  /**
  * Setters
  */
  void set_hours(int);
  void set_minutes(int);
  void set_seconds(int);

private:
  int hours;
  int minutes;
  int seconds;

  /**
  * Converts time object for am/pm output.
  */
  Time& convert_am_pm();
};

#endif
