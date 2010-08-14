// function.h
// Author: Allen Porter <allen@thebends.org>
//
// Some types modeled after the base collection types used in the Gaua library.
// http://code.google.com/p/google-collections/

namespace ysynth {

template<typename X>
class Supplier {
 public:
  virtual ~Supplier() { }

  virtual X GetValue() = 0;

 protected:
  Supplier() { }
};

template<typename X>
class ConstantSupplier : public Supplier<X> {
 public:
  ConstantSupplier(X value) : value_(value) { }
  virtual ~ConstantSupplier() { }

  virtual X GetValue() { return value_; }

 private:
  X value_;
};

template<typename X, typename Y>
class Function {
 public:
  virtual ~Function() { }

  virtual Y GetValue(X x) = 0;

 protected:
  Function() { }
};

template<typename X>
class Predicate : Function<X, bool> {
 public:
  virtual ~Predicate() { }

  virtual bool GetValue(X x) = 0;

 protected:
  Predicate() { }
};

}  // namespace ysynth
