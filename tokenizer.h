#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <assert.h>

using namespace std;

typedef unordered_map<string, string> string_map;
typedef unordered_map<string, unordered_set<string> > string_set_map;

bool startswith(string const & a, string const & b) {
    return (bool)!a.compare(0, b.length(), b);
}


//! Tokenizes a text into a vector of strings where each string
//! is either a keyterm of keyterms or is between two keyterms.
vector<string> tokenize(string const & text, unordered_set<string> const & keyterms) {

    vector<string> result;
    string_set_map string_usages;
    string_map usages;
    unordered_set<string> matches;

    string sequence;
    for(string::size_type index = 0; index < text.size(); index++) {
        char character = text[index];
        sequence += character;

        for (auto term : keyterms) {
            string current_string = string(1, character);

            string_map::const_iterator term_usage = usages.find(term);
            if (term_usage != usages.end())
                current_string = term_usage->second + current_string;

            unordered_set<string>::const_iterator term_match = matches.find(term);
            // if term not in matches
            if (term_match == matches.end()) {
                if (startswith(term, current_string)) {
                    // if term in usages
                    if (term_usage != usages.end()) {
                        string_usages[term_usage->second].erase(term);
                        if (string_usages[term_usage->second].empty())
                            string_usages.erase(term_usage->second);
                    }
                    usages[term] = current_string;
                    string_usages[current_string].insert(term);
                }
                else { // failed to match current_string
                    // clean references
                    if (term_usage != usages.end()) {
                        string_usages[term_usage->second].erase(term);
                        if (string_usages[term_usage->second].empty())
                            string_usages.erase(term_usage->second);
                        usages.erase(term);
                    }
                }
            }

            if (term == current_string) {
                matches.insert(term);
            }
        } // end for (auto term : keyterms)

        // select valid candidates, ordered by length
        //! todo: we only need the longest one. Optimize the routine.
        string term = "";
        for (string match : matches) {
            assert(string_usages.count(match) == 1);
            assert(usages.count(match) == 1);
            if (string_usages[match].size() > 1)
                continue;
            bool invalid = false;
            for (auto string_usage: string_usages) {
                if (string_usage.first != match and string_usage.first.find(match) != string::npos) {
                    invalid = true;
                    break;
                }
            }
            if (invalid)
                continue;

            if (match.size() > term.size())
                term = match;
        }

        if (term.size()) {
            // find last occurence of the term in the sequence.
            size_t string_pos = sequence.rfind(term);
            string prefix = sequence.substr(0, string_pos);
            string suffix = sequence.substr(string_pos + term.size(), sequence.size());

            // tokenize prefix and insert it into result.
            if (prefix.size() > 0) {
                vector<string> result_prime = tokenize(prefix, keyterms);
                result.insert(result.end(), result_prime.begin(), result_prime.end());
            }
            result.push_back(term);

            // remove all usages that were part of the yielded in term
            for (auto string_usage: string_set_map(string_usages)) {
                if (term.find(string_usage.first) != string::npos) {
                    for (auto usage: string_usage.second)
                        usages.erase(usage);
                    string_usages.erase(string_usage.first);
                }
                if (matches.count(string_usage.first))
                    matches.erase(string_usage.first);
            }
            sequence = suffix;
        }
    }

    for (auto match : matches) {
        string prefix = sequence.substr(0, sequence.find(match));
        string suffix = sequence.substr(sequence.find(match) + match.size(), sequence.size());
        if (prefix.size() > 0)
            result.push_back(prefix);
        result.push_back(match);

        sequence = suffix;
    }
    if (sequence.size() > 0)
        result.push_back(sequence);

    return result;
}
