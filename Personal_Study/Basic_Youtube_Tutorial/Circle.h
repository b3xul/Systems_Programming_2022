// Makes sure header files aren't included
// more than once
#ifndef CIRCLE_H
#define CIRCLE_H

class Circle : public Shape {
 public:
  Circle();
  Circle(const Circle &orig);
  Circle(double width);
  virtual ~Circle();
  // Override Area()
  // You should use override to force the
  // compiler to check if the base class
  // virtual fucntion is the same as
  // the subclass
  double Area() override;
 private:

};

#endif /* CIRCLE_H */