# tokenize

## Description
Tokenize is a C++ function that tokenizes a `std::string` into key-tokens
set by the user::

     >>> tokenize(“foo-bar”, {“foo”, “bar”})
     >>> {“foo”,”-“,”bar”} // a std::vector<string>

`tokenize` guarantees that:

1. All key-tokens in the string are in the list; 
2. no key-token is contained in any element of the list;
2. the concatenation of the list is always the initial string;
3. A keytoken containing other keytokens has priority over the latter.

The main use of tokenize is tokenization respecting keyterms that you
want to identify as unique tokens. For example::

    string text = "The PhD student n.º 10 must fill the PhD application.";
    unsorted_set<string> keys = {" ", ".", "n.º", "PhD student"};
    vector<string> result = tokenize(text, keys);

`result` is::

    {"The", " ", "PhD student", " ", 
     "n.º", " ", "10", " ",
     "must", " ", "fill", " ", 
     "the", " ", "PhD", " "application", "."}

1. `"PhD student"` is a key-term, `"PhD application"` is not.
2. A simple tokenization would return `"application."` or `{"n", ".", "º"}`.
3. tokenize reduces to a simple split when `keys = {" "}`.

## Dependencies

C++11 for the function and googletests for running the tests.

## Tests

Tokenize is tested for probably all edge cases. Check `test*` for details.

## Licence

This code is licenced under MIT licence. Check LICENCE for details.

