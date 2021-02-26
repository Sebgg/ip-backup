// I denna fil läggs definitionerna (implementationen) av de funktioner
// som deklarerats i Time.h

#include "Time.h"
#include <cmath>

bool Time::operator<(const Time& other) const
{
  return (hours < other.get_hours() ||
         (hours == other.get_hours() && minutes < other.get_minutes()) ||
         (hours == other.get_hours() && minutes == other.get_minutes()
         && seconds < other.get_seconds()));
}

bool Time::operator>(const Time& other) const
{
  return other < *this;
}

bool Time::operator<=(const Time& other) const
{
  return (*this < other || *this == other);
}

bool Time::operator>=(const Time& other) const
{
  return (*this > other || *this == other);
}

bool Time::operator==(const Time& other) const
{
  return !(*this < other || *this > other);
}

bool Time::operator!=(const Time& other) const
{
  return (*this > other || *this < other);
}

Time& Time::convert_am_pm()
{
  if(hours > 12)
  {
    hours -= 12;
  }
  return *this;
}

std::string add_zero(int time, std::string& out)
{
  if(time < 10)
  {
    out = out + '0' + std::to_string(time);
  }
  else
  {
    out += std::to_string(time);
  }
  return out;
}

std::string Time::to_string(bool am_pm)
{
  Time t{*this};
  if(am_pm == true)
  {
    t.convert_am_pm();
  }

  std::string out{};
  int hours_copy{t.get_hours()};
  out = add_zero(hours_copy, out);
  out += ':';

  int minutes_copy{t.get_minutes()};
  out = add_zero(minutes_copy, out);
  out += ':';

  int seconds_copy{t.get_seconds()};
  out = add_zero(seconds_copy, out);

  if(hours < 12 && am_pm)
  {
    out += " am";
  }
  else if(hours >= 12 && am_pm)
  {
    out += " pm";
  }

  return out;
}

std::ostream& operator<<(std::ostream& out, Time const& t)
{
  Time other{t};

  std::string output{};
  int hours_copy{other.get_hours()};
  output = add_zero(hours_copy, output);
  output += ':';

  int minutes_copy{other.get_minutes()};
  output = add_zero(minutes_copy, output);
  output += ':';

  int seconds_copy{other.get_seconds()};
  output = add_zero(seconds_copy, output);

  out << output;
  return out;

}

Time& operator>>(std::istream& in, Time& t)
{
  in >> t.hours;
  if(t.get_hours() > 23 || t.get_hours() < 0)
  {
    in.setstate(std::ios_base::failbit);
  }
  in.ignore(1000, ':');

  in >> t.minutes;
  if(t.get_minutes() > 59 || t.get_minutes() < 0)
  {
    in.setstate(std::ios_base::failbit);
  }
  in.ignore(1000, ':');

  in >> t.seconds;
  if(t.get_seconds() > 59 || t.get_seconds() < 0)
  {
    in.setstate(std::ios_base::failbit);
  }

  in.ignore(1000, '\n');

  return t;
}

Time& Time::operator++()
{
  seconds += 1;
  if (hours == 23 && minutes == 59 && seconds == 60)
  {
    hours = 0;
    minutes = 0;
    seconds = 0;
  }
  else if(minutes == 59 && seconds == 60)
  {
    hours += 1;
    minutes = 0;
    seconds = 0;
  }
  else if(seconds == 60)
  {
    minutes += 1;
    seconds = 0;
  }
  return *this;
}

Time Time::operator++(int)
{
  Time tmp{*this};
  ++(*this);
  return tmp;
}

Time& Time::operator--()
{
  seconds -= 1;
  if(hours == 0 && minutes == 0 && seconds < 0)
  {
    hours = 23;
    minutes = 59;
    seconds = 59;
  }
  else if(minutes == 0 && seconds < 0)
  {
    hours -= 1;
    minutes = 59;
    seconds = 59;
  }
  else if(seconds < 0)
  {
    minutes -= 1;
    seconds = 59;
  }

  return *this;
}

Time Time::operator--(int)
{
  Time tmp{*this};
  --(*this);
  return tmp;
}

Time Time::operator+(int right)
{
  Time tmp{*this};
  if(right < 0)
  {
    return tmp - std::abs(right);
  }
  do
  {
    tmp++;
    right -= 1;
  } while(right > 0);
  return tmp;
}

//Kommentar: snygg implementation!
Time Time::operator+(Time& right)
{
  int tmp{};
  tmp = (right.get_hours() * 3600) + (right.get_minutes() * 60) + right.get_seconds();
  Time t{*this + tmp};
  return t;
}

Time Time::operator-(int right)
{
  Time tmp{*this};
  if(right < 0)
  {
    return tmp + std::abs(right);
  }
  do
  {
    tmp--;
    right -= 1;
  } while(right > 0);
  return tmp;
}

Time Time::operator-(Time& right)
{
  int tmp{};
  tmp = (right.get_hours() * 3600) + (right.get_minutes() * 60) + right.get_seconds();
  Time t{*this - tmp};
  return t;
}

/**
* Getter implementations
*/
int Time::get_hours() const
{
  return hours;
}

int Time::get_minutes() const
{
  return minutes;
}

int Time::get_seconds() const
{
  return seconds;
}

/**
* Setter implementations
*/
void Time::set_hours(int new_hour)
{
   hours = new_hour;
}
void Time::set_minutes(int new_minute)
{
   minutes = new_minute;
}
void Time::set_seconds(int new_second)
{
   seconds = new_second;
}
