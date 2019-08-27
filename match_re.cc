/******************************************************************************

Leetcode 10. regular-expression-matching
https://leetcode.com/problems/regular-expression-matching/

*******************************************************************************/
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

using std::string;
using std::vector;

class MatchUnit {
 public:
  int type;
  string string_literal;
  char char_to_repeat;
  // (right_before_start, p.length() - 1]
  bool Match(const string& s, int right_before_start, int* end) {
      if (this->type == 0) {
          cout << "[debug] match fixed string" << endl;
          if (MatchFixedString(s, right_before_start)) {
              *end = right_before_start + this->string_literal.length();
              return true;
          } else {
              return false;
          }
      } else if (this->type == 1) {
          int expand_length = MatchRepeatedPattern(s, right_before_start);
          cout << "[Debug] expand_length: " << expand_length << endl;
          *end = right_before_start + expand_length;
          // Always true, worse case, 0 occurences.
          return true;
      } else {
          cout << "Unknown type" << endl;
          return false;
      }
  }
 private:
  bool MatchFixedString(const string& s, int right_before_start) {
      if (s.length() - right_before_start - 1 < this->string_literal.length()) {
        // Not enough chars remaining.
        cout << "[debug] not enough remaining chars" << endl;
        return false;
      }
      for (int j = 0; j < this->string_literal.length(); ++j) {
          if (this->string_literal[j] != '.' &&
              this->string_literal[j] != s[right_before_start + 1 + j]) {
                  cout << "[debug] mismatch between " << j << " and " << right_before_start + 1 + j << endl;
                  return false;
              }
      }
      return true;
  }

  // Pattern match 0 or more occurrences.
  // Return expand length.
  int MatchRepeatedPattern(const string& s, int right_before_start) {
      // Find the first char which is not char_to_repeat.
      for (int i = right_before_start + 1; i < s.length(); ++i) {
          if (this->char_to_repeat == '.') {
              continue;
          } else {
            if (s[i] != this->char_to_repeat) {
                return i - right_before_start - 1;
            } else {
                continue;
            }
          }
      }
      // Remember to return in this case.
      return s.length() - right_before_start - 1;
  }
};

class Solution {
public:
    bool isMatch(string s, string p) {
        vector<MatchUnit> match_units;
        if (!parsePattern(p, &match_units)) {
            cout << "Invalid pattern!" << endl;
            return false;
        }
        int before_start = -1;
        int end = 0;
        for (int i = 0; i < match_units.size(); ++i) {
            // If at any point, there is a mismatch, return false right away.
            if (!match_units[i].Match(s, before_start, &end)) {
                return false;
            }
            before_start = end;
        }
        // If match_units are exhausted but there are more chars.
        if (end != s.length() - 1) {
            return false;
        }
        return true;
    }

    // Return status.
    bool parsePattern(const string& p, vector<MatchUnit>* match_units) {
        if (p.empty()) {
            return true;
        }
        
        int i = 0;
        int j = 0;
        while (true) {
            // Check whether i has reached the end of the string.
            if (i == p.length()) {
                if (i - j > 0) {  // i - 1 - j + 1 > 0
                    MatchUnit match_unit;
                    match_unit.type = 0;  // fixed string
                    match_unit.string_literal = p.substr(j, i - 1 - j + 1);
                    match_units->push_back(match_unit);
                }
                break;
            }

            // cout << "seen: " << p[i] << endl;
            if (p[i] != '*') {
                ++i;
            } else {  // p[i] == '*'
                if (i - 1 < 0) {
                    cout << "Invalid pattern: No char is found before *"
                         << endl;
                    return false;
                } else {
                    // Process the substring [j, i - 2]
                    if (i - j - 1 > 0) {  // i - 2 - j + 1 > 0
                        MatchUnit match_unit;
                        match_unit.type = 0;  // fixed string
                        match_unit.string_literal = p.substr(j, i - j - 1);
                        match_units->push_back(match_unit);
                    }
                    MatchUnit match_unit;
                    match_unit.type = 1;
                    match_unit.char_to_repeat = p[i - 1];
                    match_units->push_back(match_unit);
                    ++i;
                    j = i;
                }
            }
        }
        return true;
    }

    void printMatchUnit(const MatchUnit& match_unit) {
        if (match_unit.type == 0) {
            cout << "Fixed string: " << match_unit.string_literal << endl;
        } else if (match_unit.type == 1) {
            cout << "Repeated pattern: " << match_unit.char_to_repeat << endl;
        }
    }
};

int main()
{
    cout << "Hello World from Goat Penguinesse." << endl;
    Solution sol;
    // string p = "ab.*";
    // string p = "a.b.*c*";
    // string p = "a*bc.d";
    // string p = "a*";
    // string p = ".*b*";  // This one could be tricky as . matches all.
    // string p = "a*.*";
    string p = "ab.*";
    vector<MatchUnit> match_units;
    
    auto status = sol.parsePattern(p, &match_units);
    if (status) {
        cout << "Successfully parsed the pattern: " << p << endl;
    } else {
        cout << "Failed to parse the pattern: " << p << endl;
        return -1;
    }

    if (match_units.size() == 0) {
        std::cout << "No MatchUnit found!" << std::endl;
    } else {
        for (int i = 0; i < match_units.size(); ++i) {
            sol.printMatchUnit(match_units[i]);
        }
    }
    auto found_match = sol.isMatch("abcccc", p);
    if (found_match) {
        cout << "Found a match!" << endl;
    } else {
        cout << "Mismatch:(" << endl;
    }
    return 0;
}
