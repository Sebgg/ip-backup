#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int main()
{

  int input_i{};
  cout << "Skriv in ett heltal: ";
  cin >> input_i;
  cout << "Du skrev in talet: " << input_i << '\n' << endl;
  cin.ignore(1000, '\n');

  cout << "Skriv in fem heltal: ";
  cin >> input_i;
  cout << "Du skrev in talen: " << input_i << ", ";
  cin >> input_i;
  cout << input_i << ", ";
  cin >> input_i;
  cout << input_i << ", ";
  cin >> input_i;
  cout << input_i << ", ";
  cin >> input_i;
  cout << input_i << endl;
  cin.ignore(1000, '\n');

  double input_d{};
  cout << "Skriv in ett flyttal: ";
  cin >> input_d;
  cout << setprecision(3) << fixed;
  cout << "Du skrev in flyttalet: " << input_d << '\n' << endl;
  cin.ignore(1000, '\n');

  cout << "Skriv in ett heltal och ett flyttal: ";
  cin >> input_i >> input_d;
  cout << "Du skrev in heltalet: " << setw(10) << input_i << endl;
  cout << "Du skrev in flyttalet: " << setw(9) << input_d << '\n' << endl;
  cin.ignore(1000, '\n');

  cout << "Skriv in ett flyttal och ett heltal: ";
  cin >> input_d >> input_i;
  cout << "Du skrev in heltalet: " << setw(10)
       << setfill('-') << input_i << endl;
  cout << "Du skrev in flyttalet: " << setw(9) << input_d << '\n' << endl;
  cin.ignore(1000, '\n');

  char input_c{};
  cout << "Skriv in ett tecken: ";
  cin >> input_c;
  cout << "Du skrev in tecknet: " << input_c << '\n' << endl;
  cin.ignore(1000, '\n');

  string input_str{};
  cout << "Skriv in en sträng: ";
  cin >> input_str;
  cout << "Strängen '" << input_str << "' har " << input_str.length()
       << " tecken." << '\n' << endl;
  cin.ignore(1000, '\n');

  cout << "Skriv in ett heltal och en sträng: ";
  cin >> input_i >> input_str;
  cout << "Du skrev in talet |" << input_i
       << "| och strängen |" << input_str << "|.\n" << endl;
  cin.ignore(1000, '\n');

  cout << "Skriv in en sträng och ett flyttal: ";
  cin >> input_str >> input_d;
  cout << "Du skrev in \"" << input_d
       << "\" och \"" << input_str << "\".\n" << endl;
  cin.ignore(1000, '\n');

  cout << "Skriv in en hel rad med text: ";
  getline(cin, input_str);
  cout << "Du skrev in textraden: '" << input_str << "'\n" << endl;

  cout << "Skriv in en textrad som slutar med ett negativt heltal: ";
  getline(cin, input_str, '-');
  cin >> input_i;
  cout << "Du skrev in textraden : '" << input_str
       << "' och heltalet '-" << input_i << "'\n" << endl;
  cin.ignore(1000, '\n');

  return 0;
}
