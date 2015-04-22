# tokenize

## Description
Tokenize is a C++ function that tokenizes a `std::string` into keytokens set by the user:

     >>> tokenize(“foo-bar”, {“foo”, “bar”})
     >>> {“foo”,”-“,”bar”} // a std::vector<string>

Tokenize is a function of two arguments, a string (call it text) and a set (call key-terms). It returns a vector<string> (call it result) which fulfils 4 conditions:

1. a string in the result can only be either a key-term or a string between two keyterms;
2. the concatenation of the result is always the initial string;
3. a keyterm containing other keyterms has priority over the latter;
4. a keyterm overlapping other has priority based on its position in the text (see example below)

A simple split with a delimiter is a subset of this interface with a single key-term, the delimiter. However, the conditions above generalize the concept to *overlapping key-terms*.

Some examples:

     tokenize("the end", {}) --> {"the end"}  // (condition 1 + condition 2)
     tokenize("the end", {" "}) --> {"the", " ", "end"}  // (condition 1 + condition 2)
     tokenize("foo-bar", {"foo", "foo-bar"}) --> {"foo-bar"}  // (condition 1 + condition 2 + condition 3)
     tokenize("the end", {"the e", " ", "end"}) --> {"the e", "nd"}  // (condition 4)
     tokenize("foo-tars ds", {"foo", "foo-bar", "-tar"}) --> {"foo", "-tar", "s ds"}  // (corollary of condition 2)

The main use of tokenize is in tokenization respecting keyterms that you want to identify as unique tokens.
For example:

    string text = "The PhD student n.º 10 must fill the PhD application.";
    unsorted_set<string> keys = {" ", ".", "n.º", "PhD student"};
    vector<string> result = tokenize(text, keys);

`result` is:

    {"The", " ", "PhD student", " ", 
     "n.º", " ", "10", " ",
     "must", " ", "fill", " ", 
     "the", " ", "PhD", " ", "application", "."}

1. `"PhD student"` is a key-term, `"PhD application"` is not.
2. A simple tokenization would return `"application."` or would split "`n.º`" in `{"n", ".", "º"}`

## Dependencies

C++11 for the function and googletests for running the tests.

## Tests

Tokenize is tested for probably all edge cases. Check `test*` for details.

## Licence

This code is licenced under MIT licence. Check LICENCE for details.
