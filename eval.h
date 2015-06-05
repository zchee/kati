#ifndef EVAL_H_
#define EVAL_H_

#include <unordered_map>
#include <vector>

#include "loc.h"
#include "string_piece.h"

using namespace std;

class AssignAST;
class CommandAST;
class Makefile;
class Rule;
class RuleAST;
class Var;
class Vars;

struct EvalResult {
  ~EvalResult();
  vector<Rule*> rules;
  Vars* vars;
  unordered_map<StringPiece, Vars*> rule_vars;
  // TODO: read_mks
  unordered_map<StringPiece, bool> exports;
};

class Evaluator {
 public:
  Evaluator(const Vars* vars);
  ~Evaluator();

  void EvalAssign(const AssignAST* ast);
  void EvalRule(const RuleAST* ast);
  void EvalCommand(const CommandAST* ast);

  Var* LookupVar(StringPiece name);
  // For target specific variables.
  Var* LookupVarInCurrentScope(StringPiece name);

  EvalResult* GetEvalResult();

#if 0
  const vector<Rule*>& rules() const { return rules_; }
  const Vars* vars() const { return vars_; }
  const unordered_map<StringPiece, Vars*>& rule_vars() const {
    return rule_vars_;
  }
#endif

 private:
  const Vars* in_vars_;
  Vars* vars_;
  unordered_map<StringPiece, Vars*> rule_vars_;
  vector<Rule*> rules_;
  Rule* last_rule_;

  Loc loc_;
};

#endif  // EVAL_H_
