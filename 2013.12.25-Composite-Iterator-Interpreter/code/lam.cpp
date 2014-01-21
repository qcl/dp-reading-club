/*
 * Interpretr:
 *   an lambda-calculus interpreter (with memory leaks)
 */

#include<cstdio>
#include<map>
#include<string>
#include<functional>
#include<stdexcept>
#include<memory>

using std::map;
using std::string;
using std::function;
using std::runtime_error;
using std::shared_ptr;
using std::dynamic_pointer_cast;

class Value {
public:
  Value() {}
  virtual ~Value() {}
};

class IntValue : public Value {
  const int n;
public:
  IntValue(int n_) : n(n_) {}
  ~IntValue() {}
  int getInt() { return n; }
};

class FunValue : public Value {
  function<shared_ptr<Value>(shared_ptr<Value>)> fn;
public:
  FunValue(function<shared_ptr<Value>(shared_ptr<Value>)> fn_) : fn(fn_) {}
  ~FunValue() {}
  shared_ptr<Value> apply(shared_ptr<Value> v) { return fn(v); }
};

class Expr {
  Expr(Expr&&) = delete;
public:
  Expr() {}
  Expr(const Expr&) {}
  virtual ~Expr() {}
  virtual shared_ptr<Value> eval(const map<string,shared_ptr<Value>>&) const {
    throw runtime_error("no evaluation possible");
  }
};

class Var : public Expr {
  const string var;
public:
  Var(const string& var_) : var(var_) {}
  Var(const Var& v) : var(v.var) {}
  ~Var() {}
  shared_ptr<Value> eval(const map<string,shared_ptr<Value>>& context) const {  return context.at(var); }
};

class Val : public Expr {
  const shared_ptr<Value> val;
public:
  Val(Value* val_) : val(val_) {}
  Val(const Val& v) : val(v.val) {}
  ~Val() {}
  shared_ptr<Value> eval(const map<string,shared_ptr<Value>>&) const { return val; }
};

class Lam : public Expr {
  const string x;
  const Expr* const e;
public:
  Lam(const string& x_, const Expr* const e_) : x(x_), e(e_) {}
  Lam(const Lam& lam) : x(lam.x), e(lam.e) {}
  ~Lam() {}
  shared_ptr<Value> eval(const map<string,shared_ptr<Value>>& context) const {
    const string x_(x);
    const Expr * const e_(e);
    return shared_ptr<Value>(new FunValue([x_,e_,context](shared_ptr<Value> v) {
      map<string,shared_ptr<Value>> new_context(context);
      new_context.insert(make_pair(x_, v));
      return e_->eval(new_context);
    }));
  }
};

class Ap : public Expr {
  const Expr * const e1, * const e2;
public:
  Ap(const Expr* const e1_, const Expr* const e2_) : e1(e1_), e2(e2_) {}
  Ap(const Ap& ap) : e1(ap.e1), e2(ap.e2) {}
  ~Ap() {}
  shared_ptr<Value> eval(const map<string,shared_ptr<Value>>& context) const {
    shared_ptr<Value> v1 = e1->eval(context);
    shared_ptr<Value> v2 = e2->eval(context);
    // throw exception upon type mismatch
    return dynamic_pointer_cast<FunValue>(v1)->apply(v2);
  }
};

int main() {
  Expr * e =
    new Ap(
      new Ap(
        new Lam("x",
          new Lam("y",
            new Var("y"))),
        new Val(new IntValue(5))),
      new Val(new IntValue(3)));
  shared_ptr<Value> v = e->eval({});
  printf("%d\n", dynamic_pointer_cast<IntValue>(v)->getInt());
  return 0;
}
