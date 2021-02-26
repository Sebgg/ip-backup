#include <iostream>
#include <map>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <iomanip>
#include <vector>

using namespace std;

bool check_legit(string word)
{
  if (word.substr(word.size()-2, word.size()-1) == "'s")
  {
    word = word.substr(0, word.size()-3);
  }

  if (word.find("--") != string::npos)
  {
    return false;
  }

  if (all_of(word.cbegin(),
              word.cend(),
              [](char c){return c == '-' || isalpha(c);}))
  {
    return true;
  }
  return false;
}

//Kommentar: Bra att ni skickar word som referens
bool find_word(string& word)
{
    //Kommentar: Nu kallar ni på find_first_not_of... två gånger vilket är
    //mycket onödigt, det vore bättre att utföra detta en gång och spara
    //värdet i en variabel. Ni får godkänt endå eftersom detta var det enda
    //ni missat.
  if (word.find_first_not_of("\"'(") != string::npos)
  {
    word = word.substr(word.find_first_not_of("\"'("), word.size());
  }

  if (isalpha(word.front()))
  {
      //Kommentar: Finurligt sätt at rensa bakre delen av ordet, det vore dock
      //snyggare och effektivare att använda erease() istället
    string trash_end{"!?;,:.\"')"};
    reverse(word.begin(), word.end());
    word = word.substr(word.find_first_not_of(trash_end), word.size());
    reverse(word.begin(), word.end());

    if (word.size() > 2)
    {
      bool legit{check_legit(word)};
      if (legit)
      {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
	//Kommentar: Kommentaren ovan gäller även här
        if (word.find("'s") != string::npos)
        {
          word.erase(word.find("'s"));
        }
      }
      return legit;
    }
  }
  return false;
}

//Kommentar: Snyggt att ni tar map som en const referens
void print_a(map<string, int> const& word_map, unsigned int width)
{
  for(auto it = word_map.cbegin(); it != word_map.cend(); ++it)
  {
    cout << setw(width) << left << it->first << right
    << ' ' << it->second << "\n";
  }
}
void print_f(map<string, int> const& word_map, unsigned int width)
{
  vector<pair<string, int>> output{};
  copy(word_map.begin(), word_map.end(),
      back_inserter<vector<pair<string, int>>>(output));

  sort(output.begin(), output.end(),
            [](const pair<string, int>& left, const pair<string, int>& right)
                {
                  return left.second > right.second;
                });
  for(auto it = output.cbegin(); it != output.cend(); ++it)
  {
    cout << setw(width) << right << it->first << ' ' << it->second << "\n";
  }
}

void print_o(unsigned int line_width, vector<string>const& timeline)
{
  string out{};
  for_each(timeline.begin(), timeline.end()+1,
          [&](string word)
          {
            if ((out.size() + word.size()) < line_width)
            {
              out.append(word + " ");
            }
            else
            {
              cout << out << endl;
              out = word + " ";
            }
          });

  if (out != " ") //prints the last word if it's on a seperate line
  {
    cout << out << endl;
  }
}

int main(int argc, char const *argv[]) {
  //error handling
  string program_name{argv[0]};
  string usage_out{"Usage: " + program_name + " FILE [-a] [-f] [-o N]\n"};
  if (argc == 1)
  {
    cerr << "Error: No arguments given." << endl << usage_out;
    return -1;
  }
  else if (argc == 2)
  {
    cerr << "Error: Second argument missing or invalid." << endl << usage_out;
    return -1;
  }
  else if (argv[2][1] == 'o' && argc != 4)
  {
    cerr << "Error: Need to specify a linewidth when using the -o flag."
         << endl << usage_out;
    return -1;
  }

  ifstream in{argv[1]};

  if (!in)
  {
    cerr << "Error: Seems like the file you tried to open can't be opened."
              << endl;
    return -1;
  }

  map<string, int> word_map{};
  vector<string> timeline{};
  string word;
  unsigned int largest_word_size{};

  while (in >> word)
  {
    if (find_word(word))
    {
      word_map[word] += 1;
      timeline.push_back(word);
      largest_word_size = max(largest_word_size,
        static_cast<unsigned int>(word.length()));
    }
  }

  switch (argv[2][1]) {
    case 'a':
      print_a(word_map, largest_word_size);
      break;
    case 'f':
      print_f(word_map, largest_word_size);
      break;
    case 'o':
      print_o(atoi(argv[3]), timeline);
      break;
    default: cerr << "Error: Second argument is invalid." <<  endl << usage_out;
             in.close();
             return -1;
  }

  in.close();
  return 0;
}
