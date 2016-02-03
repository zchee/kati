// Copyright 2015 Google Inc. All rights reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// +build ignore

#include "strutil.h"

#include <assert.h>

#include <string>
#include <vector>

#include "string_piece.h"
#include "testutil.h"

using namespace std;

namespace {

void TestWordScanner() {
  vector<StringPiece> ss;
  for (StringPiece tok : WordScanner("foo bar baz hogeeeeeeeeeeeeeeee")) {
    ss.push_back(tok);
  }
  assert(ss.size() == 4LU);
  ASSERT_EQ(ss[0], "foo");
  ASSERT_EQ(ss[1], "bar");
  ASSERT_EQ(ss[2], "baz");
  ASSERT_EQ(ss[3], "hogeeeeeeeeeeeeeeee");
}

void TestHasPrefix() {
  assert(HasPrefix("foo", "foo"));
  assert(HasPrefix("foo", "fo"));
  assert(HasPrefix("foo", ""));
  assert(!HasPrefix("foo", "fooo"));
}

void TestHasSuffix() {
  assert(HasSuffix("bar", "bar"));
  assert(HasSuffix("bar", "ar"));
  assert(HasSuffix("bar", ""));
  assert(!HasSuffix("bar", "bbar"));
}

string SubstPattern(StringPiece str, StringPiece pat, StringPiece subst) {
  string r;
  Pattern(pat).AppendSubst(str, subst, &r);
  return r;
}

void TestSubstPattern() {
  ASSERT_EQ(SubstPattern("x.c", "%.c", "%.o"), "x.o");
  ASSERT_EQ(SubstPattern("c.x", "c.%", "o.%"), "o.x");
  ASSERT_EQ(SubstPattern("x.c.c", "%.c", "%.o"), "x.c.o");
  ASSERT_EQ(SubstPattern("x.x y.c", "%.c", "%.o"), "x.x y.o");
  ASSERT_EQ(SubstPattern("x.%.c", "%.%.c", "OK"), "OK");
  ASSERT_EQ(SubstPattern("x.c", "x.c", "OK"), "OK");
  ASSERT_EQ(SubstPattern("x.c.c", "x.c", "XX"), "x.c.c");
  ASSERT_EQ(SubstPattern("x.x.c", "x.c", "XX"), "x.x.c");
}

void TestNoLineBreak() {
  assert(NoLineBreak("a\nb") == "a\\nb");
  assert(NoLineBreak("a\nb\nc") == "a\\nb\\nc");
}

void TestHasWord() {
  assert(HasWord("foo bar baz", "bar"));
  assert(HasWord("foo bar baz", "foo"));
  assert(HasWord("foo bar baz", "baz"));
  assert(!HasWord("foo bar baz", "oo"));
  assert(!HasWord("foo bar baz", "ar"));
  assert(!HasWord("foo bar baz", "ba"));
  assert(!HasWord("foo bar baz", "az"));
  assert(!HasWord("foo bar baz", "ba"));
  assert(!HasWord("foo bar baz", "fo"));
}

static string NormalizePath(string s) {
  ::NormalizePath(&s);
  return s;
}

void TestNormalizePath() {
  ASSERT_EQ(NormalizePath(""), "");
  ASSERT_EQ(NormalizePath("."), "");
  ASSERT_EQ(NormalizePath("/"), "/");
  ASSERT_EQ(NormalizePath("/tmp"), "/tmp");
  ASSERT_EQ(NormalizePath("////tmp////"), "/tmp");
  ASSERT_EQ(NormalizePath("a////b"), "a/b");
  ASSERT_EQ(NormalizePath("a//.//b"), "a/b");
  ASSERT_EQ(NormalizePath("a////b//../c/////"), "a/c");
  ASSERT_EQ(NormalizePath("../foo"), "../foo");
  ASSERT_EQ(NormalizePath("./foo"), "foo");
  ASSERT_EQ(NormalizePath("x/y/..//../foo"), "foo");
  ASSERT_EQ(NormalizePath("x/../../foo"), "../foo");
  ASSERT_EQ(NormalizePath("/../foo"), "/foo");
  ASSERT_EQ(NormalizePath("/../../foo"), "/foo");
  ASSERT_EQ(NormalizePath("/a/../../foo"), "/foo");
  ASSERT_EQ(NormalizePath("/a/b/.."), "/a");
  ASSERT_EQ(NormalizePath("../../a/b"), "../../a/b");
  ASSERT_EQ(NormalizePath("../../../a/b"), "../../../a/b");
  ASSERT_EQ(NormalizePath(".././../a/b"), "../../a/b");
  ASSERT_EQ(NormalizePath("./../../a/b"), "../../a/b");
}

}  // namespace

int main() {
  TestWordScanner();
  TestHasPrefix();
  TestHasSuffix();
  TestSubstPattern();
  TestNoLineBreak();
  TestHasWord();
  TestNormalizePath();
  assert(!g_failed);
}
