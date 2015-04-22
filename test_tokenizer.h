#ifndef test_tokenizer_h
#define test_tokenizer_h

#include "gtest/gtest.h"

#include "tokenizer.h"
#include <unordered_set>
#include <string>

using namespace std;

TEST(startswith, a) {
    string a = "the end";
    string b = "the";
    ASSERT_TRUE(startswith(a, b));
    
    b = "he";
    ASSERT_FALSE(startswith(a, b));
}

TEST(Tokenize, basic) {
    unordered_set<string> keyterms = {" "};

    vector<string> expected = {string("the"), string(" "), string("end")};

    ASSERT_EQ(expected, tokenize("the end", keyterms));
};

TEST(Tokenize, dsad) {
    unordered_set<string> keyterms = {" ", "the end"};

    vector<string> expected = {string("the end"), string(" "), string("is")};

    ASSERT_EQ(expected, tokenize("the end is", keyterms));
};

TEST(Tokenize, test_similar_keyterms) {
    unordered_set<string> keyterms = {" ", "the end", "the bar"};

    vector<string> expected = {string("this"), string(" "),
        string("is"), string(" "),string("the"), string(" "), string("foo"),
        string(" "), string("of")};

    ASSERT_EQ(expected, tokenize("this is the foo of", keyterms));
}

TEST(Tokenize, test_shifted_keyterms) {
    unordered_set<string> keyterms = {" ", "the foo stay", "foo is bad"};

    vector<string> expected = {string("the"), string(" "), string("foo is bad")};

    ASSERT_EQ(expected, tokenize("the foo is bad", keyterms));
}

TEST(Tokenize, test_keyterm_in_word) {
    unordered_set<string> keyterms = {".", ".º"};

    vector<string> expected = {string("262"), string(".º")};

    ASSERT_EQ(expected, tokenize("262.º", keyterms));
}

TEST(Tokenize, test_keyterm_subset_of_keyterm) {
    unordered_set<string> keyterms = {"foo"};

    vector<string> expected = {string("foo"), string("-bar")};

    ASSERT_EQ(expected, tokenize("foo-bar", keyterms));

    keyterms = {"foo", "foo-bar"};
    expected = {string("foo-bar")};
    ASSERT_EQ(expected, tokenize("foo-bar", keyterms));

    keyterms = {"foo", "foo-bar"};
    expected = {string("foo"), string("-tar")};
    ASSERT_EQ(expected, tokenize("foo-tar", keyterms));
}

TEST(Tokenize, test_multiple_keyterms) {
    unordered_set<string> keyterms = {"foo", "foo-bar", "-tar"};

    vector<string> expected = {string("foo"), string("-tar"), string("s")};

    ASSERT_EQ(expected, tokenize("foo-tars", keyterms));
}

TEST(Tokenize, test_keyterm_in_begin) {
    unordered_set<string> keyterms = {"pre", "pre-foo-suf"};

    vector<string> expected = {string("pre-foo-suf")};

    ASSERT_EQ(expected, tokenize("pre-foo-suf", keyterms));

    keyterms = {"pre", "pre-foo-suf"};
    expected = {string("d-"), string("pre-foo-suf")};
    ASSERT_EQ(expected, tokenize("d-pre-foo-suf", keyterms));
}

TEST(Tokenize, test_keyterm_in_middle) {
    unordered_set<string> keyterms = {"foo", "pre-foo-suf"};

    vector<string> expected = {string("pre-foo-suf")};

    ASSERT_EQ(expected, tokenize("pre-foo-suf", keyterms));
}

TEST(Tokenize, test_keyterm_in_end) {
    unordered_set<string> keyterms = {"suf", "pre-foo-suf"};

    vector<string> expected = {string("pre-foo-suf")};

    ASSERT_EQ(expected, tokenize("pre-foo-suf", keyterms));
}

TEST(Tokenize, test_keyterms_in_string_end) {
    unordered_set<string> keyterms = {" ", "is solid", "is black"};

    vector<string> expected = {string("the"), string(" "), string("is")};

    ASSERT_EQ(expected, tokenize("the is", keyterms));

    keyterms = {"foo-bar", "bar tal"};
    expected = {string("foo-bar")};
    ASSERT_EQ(expected, tokenize("foo-bar", keyterms));
}

#endif
