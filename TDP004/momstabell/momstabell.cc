#include <iostream>
#include <iomanip>

using std::cout;
using std::cin;
using std::endl;
using std::cerr;

float tax(float const& price, float const& percent)
{
  return price * percent;
}

float include_tax(float const& price, float const& percent)
{
  return price * (1 + percent);
}

float wrong_first_price(float& first_price)
{
  while(first_price < 0)
  {
    cerr << "FEL: Första pris måste vara minst 0 (noll) kronor" << endl;
    cout << "Mata in första pris: ";
    cin >> first_price;
    cin.ignore(100, '\n');
  }

  return first_price;
}

float wrong_last_price(float& last_price, float& first_price)
{
  while(first_price >= last_price)
  {
    cerr << "FEL: Sista pris måste vara större än första pris" << endl;
    cout << "Mata in sista pris: ";
    cin >> last_price;
    cin.ignore(100, '\n');
  }

  return last_price;
}

float wrong_step_size(float& step)
{
  while(0.009 > step)
  {
    cerr << "FEL: Steglängd måste vara minst 0.01" << endl;
    cout << "Mata in steglängd: ";
    cin >> step;
    cin.ignore(100, '\n');
  }

  return step;
}

float wrong_percent(float& percent)
{
  while(percent < 0 || percent > 100)
  {
    cerr << "FEL: Momsprocent måste vara mellan 0 (noll)"
         << " och 100 (ett hundra) procent." << endl;
    cout << "Mata in momsprocent: ";
    cin >> percent;
    cin.ignore(100, '\n');
  }

  return percent;
}

int main()
{

  cout << "INMATNINGSDEL" << endl << std::setw(12) << std::setfill('=');
  cout << "" << endl;

  float first_price{};
  cout << "Mata in första pris: ";
  cin >> first_price;
  cin.ignore(100, '\n');
  if(first_price < 0)
  {
    wrong_first_price(first_price);
  }

  float last_price{};
  cout << "Mata in sista pris: ";
  cin >> last_price;
  cin.ignore(100, '\n');
  if(first_price >= last_price)
  {
    wrong_last_price(last_price, first_price);
  }

  float step{};
  cout << "Mata in steglängd: ";
  cin >> step;
  cin.ignore(100, '\n');
  if(step < 0.009)
  {
    wrong_step_size(step);
  }

  float percent{};
  cout << "Mata in momsprocent: ";
  cin >> percent;
  cin.ignore(100, '\n');
  if(percent < 0 || percent > 100)
  {
    wrong_percent(percent);
  }
  percent = ( percent / 100 );

  cout << std::setprecision(2) << std::fixed << endl;

  const int price_w{12};
  const int moms_w{17};
  const int total_w{20};

  cout << "MOMSTABELLEN" << endl << std::setw(price_w);
  cout << "" << endl;
  cout << std::setw(price_w) << std::setfill(' ') << "Pris"
       << std::setw(moms_w) << "Moms"
       << std::setw(total_w) << "Pris med moms"
       << std::setw(49) << std::setfill('-') << endl;
  cout << "" << endl;

  while (first_price <= last_price) {
    cout << std::setw(price_w) << std::setfill(' ') << first_price
         << std::setw(moms_w) << tax(first_price, percent)
         << std::setw(total_w) << include_tax(first_price, percent)  << endl;
    first_price += step;
  }

  return 0;
}
