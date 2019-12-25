// A C++ reader/writer of .ply files.
// Undescribed data objects.
// These contain the actual data.
// Generally, the user will implement subclasses
// for their own object types.
//
//Copyright (C) 2013  INRIA - Sophia Antipolis
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Author(s):      Thijs van Lankveld

#include "PLY/unknown.h"
#include <cstring>

namespace PLY {
// Get the scalar value.
bool PolyValue::get_scalar(const Property& prop, double& value) const {
  if (prop.type != SCALAR)
    return false;
  return to_value(data, prop.data_type, value);
}

// Set the scalar value.
bool PolyValue::set_scalar(const Property& prop, const double& value) {
  if (prop.type != SCALAR)
    return false;
  deinit();
  // Initialise the array and store the scalar.
  data = new char[Type_bytes[prop.data_type]];
  return to_pointer(value, prop.data_type, data);
}

// Get the size of the list value.
bool PolyValue::get_size(const Property& prop, size_t& size) const {
  if (prop.type != LIST)
    return false;
  double value;
  if (!to_value(data, prop.size_type, value))
    return false;
  size = (size_t) value;
  return true;
}

// Get an item of the list value.
bool PolyValue::get_item(const Property& prop, const size_t& num, double& value) const {
  if (prop.type != LIST)
    return false;
  size_t incr = Type_bytes[prop.size_type] + (num * Type_bytes[prop.data_type]);
  return to_value(data + incr, prop.data_type, value);
}

// Prepare the list Value to recieve a number of items.
bool PolyValue::set_size(const Property& prop, const size_t& size) {
  if (prop.type != LIST)
    return false;
  deinit();
  // Initialise the array and store the size.
  size_t len = Type_bytes[prop.size_type] + (size * Type_bytes[prop.data_type]);
  data = new char[len];
  return to_pointer((double) size, prop.size_type, data);
}

// Set an item of the list Value.
bool PolyValue::set_item(const Property& prop, const size_t& num, const double& value) {
  if (prop.type != LIST)
    return false;
  // Store the item.
  size_t incr = Type_bytes[prop.size_type] + (num * Type_bytes[prop.data_type]);
  return to_pointer(value, prop.data_type, data + incr);
}

//  Get the string Value.
bool PolyValue::get_string(const Property& prop, char* str) const {
  if (prop.type != STRING)
    return false;
  std::strcpy(str, data);
  return true;
}

//  Set the string Value.
bool PolyValue::set_string(const Property& prop, const char* str) {
  if (prop.type != STRING)
    return false;
  deinit();
  // Initialise the array (incl. the '\0') and store the string.
  data = new char[std::strlen(str) + 1];
  std::strcpy(data, str);
  return true;
}

// Convert a pointer to a value.
bool PolyValue::to_value(const char* ptr, const Scalar_type& type, double& value) const {
  switch (type) {
  default:
  case StartType:
  case EndType:
    return false;
  case Int8:
    value = *((char*) ptr);
    break;
  case Int16:
    value = *((short*) ptr);
    break;
  case Int32:
    value = *((int*) ptr);
    break;
  case UInt8:
    value = *((unsigned char*) ptr);
    break;
  case UInt16:
    value = *((unsigned short*) ptr);
    break;
  case UInt32:
    value = *((unsigned int*) ptr);
    break;
  case Float32:
    value = *((float*) ptr);
    break;
  case Float64:
    value = *((double*) ptr);
    break;
  }
  return true;
}

// Convert a value to a pointer.
bool PolyValue::to_pointer(const double& value, const Scalar_type& type, char* ptr) const {
  char cval;
  short sval;
  int ival;
  float fval;
  unsigned char ucval;
  unsigned short usval;
  unsigned int uival;

  switch (type) {
  default:
  case StartType:
  case EndType:
    return false;
  case Int8:
    cval = (char) value;
    copy((char*) &cval, ptr, 1);
    break;
  case Int16:
    sval = (short) value;
    copy((char*) &sval, ptr, 2);
    break;
  case Int32:
    ival = (int) value;
    copy((char*) &ival, ptr, 4);
    break;
  case UInt8:
    ucval = (unsigned char) value;
    copy((char*) &ucval, ptr, 1);
    break;
  case UInt16:
    usval = (unsigned short) value;
    copy((char*) &usval, ptr, 2);
    break;
  case UInt32:
    uival = (unsigned int) value;
    copy((char*) &uival, ptr, 4);
    break;
  case Float32:
    fval = (float) value;
    copy((char*) &fval, ptr, 4);
    break;
  case Float64:
    copy((char*) &value, ptr, 8);
    break;
  }
  return true;
}

// Copy some bytes.
void PolyValue::copy(const char* from, char* to, const size_t& num) const {
  for (size_t i = 0; i < num; ++i)
    to[i] = from[i];
}

// Prepare the Item to represent an Element.
void PolyObject::prepare(const Element& elem) {
  deinit();
  init(elem.props.size());
}

// Get a Value.
Value* PolyObject::get_value(const Element& elem, const Property& prop) {
  size_t index;
  if (!elem.find_index(prop.name.c_str(), index))
    return 0;
  if (values[index] == 0)
    values[index] = new PolyValue();
  return values[index];
}

// Initialize the objects.
void PolyObject::init(size_t size) {
  values = new Value*[size];
  num = size;
  for (size_t n = 0; n < size; ++n)
    values[n] = 0;
}

// Deinitialize the objects.
void PolyObject::deinit() {
  if (values) {
    for (size_t n = 0; n < num; ++n)
      if (values[n] != 0)
        delete values[n];
    delete[] values;
    values = 0;
  }
}

// Prepare the array to contain the Objects of an Element.
void PolyArray::prepare(const size_t& size) {
  deinit();
  init(size);
  restart();
}

// Remove all [Objects](\ref Item) from the array.
void PolyArray::clear() {
  for (size_t n = 0; n < num; ++n) {
    if (objects[n] != 0) {
      delete objects[n];
      objects[n] = 0;
    }
  }
}

// Get the next Item.
Item& PolyArray::next_object() {
  if (objects[incr] == 0)
    objects[incr] = new PolyObject;
  return *objects[incr++];
}

// Initialize the objects.
void PolyArray::init(size_t size) {
  objects = new Item*[size];
  num = size;
  for (size_t n = 0; n < size; ++n)
    objects[n] = 0;
}

// Deinitialize the objects.
void PolyArray::deinit() {
  if (objects) {
    clear();
    delete[] objects;
    objects = 0;
  }
}
} // namespace PLY
