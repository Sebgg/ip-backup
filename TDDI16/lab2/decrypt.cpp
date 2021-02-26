#include <iostream>
#include <chrono>
#include <string>
#include "key.h"
#include <math.h>
#include <vector>
#include <unordered_map>

using namespace std;

namespace std
{
  template<> struct hash<Key>
  {
    typedef Key argument_type;
    typedef size_t result_type;
    result_type operator()(argument_type const& k) const noexcept
    {
      result_type total{0};
      for(int i=1 ; i < (C + 1) ; i++)
      {
        total += (int)k.digit[i - 1] * pow(10, i);
      }
      return total;
    }
  };
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage:" << endl;
        cout << argv[0] << " <hashed password> < rand8.txt" << endl;
        return 1;
    }

    // Hashed password.
    Key hashed{argv[1]};
    string pw{argv[1]};

    // Table.
    Key table[N];

    // Read table.
    for (int i{0}; i < N; i++) {
        char buffer[C + 1];
        if (scanf("%s", buffer) != 1) {
            cerr << "Too short table!" << endl;
            return 1;
        }
        table[i] = Key{buffer};
    }

    auto begin = chrono::high_resolution_clock::now();

    // Find all possible passwords that hash to 'hashed' and print them.

    // Key == hashed candidate, Value = unhashed candidate
    unordered_map<Key, vector<Key>> pw2_table;

    Key candidate{};
    Key k_end{string(C, 'a').replace(C/2, 1, C/2 + (C % 2), '5').c_str()};

    do {
      Key enc = subset_sum(candidate, table);
      pw2_table[enc].push_back(candidate);
      ++candidate;
    } while (candidate != k_end);

    Key k_end2{string(C, 'a').replace(0, C/2, C/2, '5').c_str()};
    Key candidate2{};
    Key inc_key{string(C, 'a').replace(C/2 - 1, 1, 1, 'b').c_str()};

    do {
      Key enc = subset_sum(candidate2, table);
      if (pw2_table.find(hashed - enc) != pw2_table.end())
      {
      	for(auto &c : pw2_table[hashed - enc])
        {
          Key test{candidate2 + c};
      	  cout << "match: " << test << endl;
      	}
      }

      candidate2 += inc_key;

    } while (candidate2 != k_end2);

    auto end = chrono::high_resolution_clock::now();
    cout << "Decryption took "
         << std::chrono::duration_cast<chrono::seconds>(end - begin).count()
         << " seconds." << endl;

    return 0;
}
