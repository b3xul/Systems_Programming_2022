// Refer to the declarations in the header
#include "Shape.h"

// This file is called the implementation file

Shape::Shape(double length) {
  this->height = length;
  this->width = length;
  // Refer to class fields and methods with ::
  Shape::numOfShapes++;
}

Shape::Shape(double height, double width) {
  this->height = height;
  this->width = width;
  Shape::numOfShapes++;
}

Shape::~Shape() = default;

// We can protect access to our fields using getters
// and setters. We could block certain values
// from being assigned, or we could provide
// custom output like adding inches or centimeter
// to the height, or width

// Because we create the class before naming an
// object we don't know its name. We can use this
// to refer to objects.

// The pointer operator is used to access an
// objects fields and methods
void Shape::SetHeight(double height) { this->height = height; }
double Shape::GetHeight() { return height; }

void Shape::SetWidth(double width) { this->width = width; }
double Shape::GetWidth() { return width; }

int Shape::GetNumOfShapes() { return numOfShapes; }

// A method marked as virtual can be used
// polymorphically.
// Polymorphism is a feature in which similar
// objects can be treated the same, but also
// utilize their differences automatically
// for any methods marked as virtual
double Shape::Area() {
  return height * width;
}

int Shape::numOfShapes = 0;
