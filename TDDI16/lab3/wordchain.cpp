#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <algorithm>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::unordered_map;
using std::queue;
using std::unordered_set;

// Typ som används för ordlistan. Den definieras med en typedef här så att du enkelt kan ändra
// representationen av en ordlista utefter vad din implementation behöver. Funktionen
// "read_questions" skickar ordlistan till "find_shortest" och "find_longest" med hjälp av denna
// typen.

typedef unordered_set<string> Dictionary;
static const string ALPHABET{"abcdefghijklmnopqrstuvwxyz"};

// Finds all neighbours for curr which hasn't been visited, and adds them to queue and vis_nodes.
// Also adds neighbours to 'nodes' with value => previous
void handle_neighbours(const string &curr, const Dictionary &dict, unordered_set<string> &vis_nodes, unordered_map<string, string> &nodes, queue<string> &word_q)
{
  for (unsigned i = 0; i < 4; ++i)
  {
    string temp{curr};
    for (unsigned j = 0; j < ALPHABET.size(); ++j)
    {
      temp[i] = ALPHABET[j];
      if (dict.find(temp) != dict.end()
          && vis_nodes.find(temp) == vis_nodes.end())
      {
        vis_nodes.insert(temp);
        nodes[temp] = curr;
        word_q.push(temp);
      }
    }
  }
}

/**
 * When to is found, creates a vector with the shortest wordchain.
 * Returns the path from -> to
 */
vector<string> create_wordchain(const unordered_map<string, string> &nodes, const string &to)
{
  vector<string> result{to};
  string curr{to};
  while (nodes.at(curr) != "") {
    result.emplace_back(nodes.at(curr));
    curr = nodes.at(curr);
  }
  std::reverse(result.begin(), result.end());
  return result;
}

/**
 * Hitta den kortaste ordkedjan från 'first' till 'second' givet de ord som finns i
 * 'dict'. Returvärdet är den ordkedja som hittats, första elementet ska vara 'from' och sista
 * 'to'. Om ingen ordkedja hittas kan en tom vector returneras.
 */
vector<string> find_shortest(const Dictionary &dict, const string &from, const string &to) {
    vector<string> result(200);

    // set of all visited nodes
    unordered_set<string> vis_nodes(2600);
    vis_nodes.insert(from);

    // value -> previous
    unordered_map<string, string> nodes(2600);
    nodes[from] = "";

    queue<string> word_q{};
    word_q.push(from);

    while(!word_q.empty())
    {
      string curr = word_q.front();
      word_q.pop();

      if(curr == to)
        return create_wordchain(nodes, to);

      handle_neighbours(curr, dict, vis_nodes, nodes, word_q);

      if(word_q.size() == 0 && to == "")
	       return create_wordchain(nodes, curr);
    }

    return result;
}

/**
 * Hitta den längsta kortaste ordkedjan som slutar i 'word' i ordlistan 'dict'. Returvärdet är den
 * ordkedja som hittats. Det sista elementet ska vara 'word'.
 */
vector<string> find_longest(const Dictionary &dict, const string &word)
{
  return find_shortest(dict, word, "");
}

/**
 * Läs in ordlistan och returnera den som en vector av orden. Funktionen läser även bort raden med
 * #-tecknet så att resterande kod inte behöver hantera det.
 */
Dictionary read_dictionary() {
    string line;
    Dictionary result;
    while (std::getline(std::cin, line)) {
        if (line == "#")
            break;

        result.insert(line);
    }

    return result;
}

/**
 * Skriv ut en ordkedja på en rad.
 */
void print_chain(const vector<string> &chain) {
    if (chain.empty())
        return;

    vector<string>::const_iterator i = chain.begin();
    // printf("%s", *i);
    cout << *i;

    for (++i; i != chain.end(); ++i)
      // printf(" -> %s", *i);
      cout << " -> " << *i;

    printf("\n");
    // cout << endl;
}


/**
 * Läs in alla frågor. Anropar funktionerna "find_shortest" eller "find_longest" ovan när en fråga hittas.
 */
void read_questions(const Dictionary &dict) {
    string line;
    while (std::getline(std::cin, line)) {
        size_t space = line.find(' ');
        if (space != string::npos) {
            string first = line.substr(0, space);
            string second = line.substr(space + 1);
            vector<string> chain = find_shortest(dict, first, second);

            // printf("%s %s:", first, second);
            cout << first << " " << second << ": ";
            if (chain.empty()) {
                cout << "ingen lösning" << endl;
            } else {
                cout << chain.size() << " ord" << endl;
                print_chain(chain);
            }
        } else {
            vector<string> chain = find_longest(dict, line);

            cout << line << ": " << chain.size() << " ord" << endl;
            print_chain(chain);
        }
    }
}

int main() {
  Dictionary dict(2600);
  dict = read_dictionary();
  read_questions(dict);

  return 0;
}
