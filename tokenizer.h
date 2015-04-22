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

    vector<string> result;  // the final result to be returned.
    
    // these two are references keyterms<>strings
    string_set_map string_usages;  // a map of the strings in use to a set of keyterms using then.
    string_map usages;  // inverse of `string_usages`: keyterm using string.

    unordered_set<string> matches;  // key terms that have an exact match.

    string sequence;  // memory of string being tokenized but not yet decided.

    // for each character
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
                // if term startswith current_string
                if (startswith(term, current_string)) {
                    // if term in usages
                    if (term_usage != usages.end()) {
                        // clean old references to replace for the new ones.
                        string_usages[term_usage->second].erase(term);
                        if (string_usages[term_usage->second].empty())
                            string_usages.erase(term_usage->second);
                    }
                    // set references
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

            if (term == current_string)
                matches.insert(term);
        } // auto term : keyterms

        // select valid candidates, ordered by length
        string term = "";
        for (string match : matches) {
            // continue if candidate is not suitable.
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

            // suitable candidate, maximize the size of the candidate.
            if (match.size() > term.size())
                term = match;
        }

        // if we have a candidate
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
            // finally, store the term
            result.push_back(term);

            // remove all references that used the term
            for (auto string_usage: string_set_map(string_usages)) {
                if (term.find(string_usage.first) != string::npos) {
                    for (auto usage: string_usage.second)
                        usages.erase(usage);
                    string_usages.erase(string_usage.first);
                }
                if (matches.count(string_usage.first))
                    matches.erase(string_usage.first);
            }

            // sequence becomes what is left of the previous sequence.
            sequence = suffix;
        }
    }

    // clean sequence after last character
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
