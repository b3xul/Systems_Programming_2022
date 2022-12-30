#include "Shape.h"
#include "Circle.h"
#include <cmath>

Circle::~Circle() = default;

Circle::Circle(double width) : Shape(width) { // calls the Shape constructor

}

double Circle::Area(){
  return 3.14159 * pow((width / 2), 2);
}
