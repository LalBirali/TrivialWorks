#pragma once
#include "object.h"
#include "int.h"
#include "bool.h"
#include "float.h"
#include "string.h"
#include "schema.h"
#include "fielder.h"
#include "helper.h"
#include <iostream>


/*************************************************************************
 * Row::
 *
 * This class represents a single row of data constructed according to a
 * dataframe's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Internally a dataframe hold data in columns.
 * Rows have pointer equality.
 */
class Row : public Object {
 public:
  Object** elements;
  size_t num_elem;
  char* col_types;
  size_t index;

  /** Build a row following a schema. */
  Row(Schema& scm) {
    elements = new Object*[scm.width()];
    num_elem = scm.width();
    col_types = scm.col_types;
    index = 0;
  }

  /** Destructor for Row */
  ~Row() {delete elements; delete col_types;}
 
  /** Setters: set the given column with the given value. Setting a column with
    * a value of the wrong type is undefined. */
  void set(size_t col, int val) {
    if (col >= num_elem) {
      std::cout << "Illegal index size -- Row" << std::endl;
      return;
    }

    if (col_types[col] != 'I') {
      std::cout << "Illegal type -- Row" << std::endl;
      return;
    } 
    Int* i = new Int(val);
    elements[col] = i;

  }
  void set(size_t col, float val) {
    if (col >= num_elem) {
      std::cout << "Illegal index size -- Row" << std::endl;
      return;
    }

    if (col_types[col] != 'F') {
      std::cout << "Illegal type -- Row" << std::endl;
      return;
    } 
    Float* fl = new Float(val);
    elements[col] = fl;
  }
  void set(size_t col, bool val) {
    if (col >= num_elem) {
      std::cout << "Illegal index size -- Row" << std::endl;
      return;
    }

    if (col_types[col] != 'B') {
      std::cout << "Illegal type -- Row" << std::endl;
      return;
    } 
    Bool * b = new Bool(val);
    elements[col] = b;
  }
  /** Acquire ownership of the string. */
  void set(size_t col, String* val) {
    if (col >= num_elem) {
      std::cout << "Illegal index size -- Row" << std::endl;
      return;
    }
    //std::cout << col_types[col];
    if (col_types[col] != 'S') {
      std::cout << "Illegal type -- Row" << std::endl;
      return;
    } 
    String* s = new String(val->c_str());
    elements[col] = s;
  }
 
  /** Set/get the index of this row (ie. its position in the dataframe. This is
   *  only used for informational purposes, unused otherwise */
  void set_idx(size_t idx) {
    index = idx;
  }
  size_t get_idx() {
    return this-> index;
  }
 
  /** Getters: get the value at the given column. If the column is not
    * of the requested type, the result is undefined. */
  int get_int(size_t col) {
    if (col >= num_elem || col_types[col] != 'I') {
      return -1;
    } else {
      Int* i = dynamic_cast<Int*>(elements[col]);
      return i->get();
    }

  }
  bool get_bool(size_t col) {
    if (col >= num_elem || col_types[col] != 'B') {
      return false;
    } else {
      Bool* b = dynamic_cast<Bool*>(elements[col]);
      return b->get();
    }

  }
  float get_float(size_t col) {
    if (col >= num_elem || col_types[col] != 'F') {
      return -1.0;
    } else {
      Float* f = dynamic_cast<Float*>(elements[col]);
      return f->get();
    }

  }
  String* get_string(size_t col) {
    if (col >= num_elem || col_types[col] != 'S') {
      return NULL;
    } else {
      String* s = dynamic_cast<String*>(elements[col]);
      return s;
    }

  }
 
  /** Number of fields in the row. */
  size_t width() {
    return num_elem;
  }
 
   /** Type of the field at the given position. An idx >= width is  undefined. */
  char col_type(size_t idx) {
    if (idx >= num_elem) {
      return 'N';
    } else {
      return col_types[idx];
    }

  }
  /** Given a Fielder, visit every field of this row.
    * Calling this method before the row's fields have been set is undefined. */
  void visit(size_t idx, Fielder& f) {
    if (idx != this->index) {
      return ;
    }
    for (size_t i = 0; i < num_elem; i++) {
      char typ = col_types[i];
      Object* obj = elements[i];
      switch(typ) {
        case 'B':
        {
          Bool* b = dynamic_cast<Bool*>(obj);
          f.accept(b->get());
          break;
        }
        case 'I':
        {
          Int* i = dynamic_cast<Int*>(obj);
          f.accept(i->get());
          break;
        }
        case 'F':
        {
          Float* fl = dynamic_cast<Float*>(obj);
          f.accept(fl->get());
          break;
        }
        case 'S':
        {
          String* s = dynamic_cast<String*>(obj);
          f.accept(s);
          break;
        }
      }
    }
  }
 
};