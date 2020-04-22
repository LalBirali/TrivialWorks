#pragma once
#include "object.h"
#include "string.h"
#include "helper.h"
#include <string>

/*************************************************************************
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object {
 public:
  size_t nrow;
  size_t ncol;
  char* col_types;
  String** rows;
  String** cols;

 
  /** Copying constructor */
  Schema(Schema& from) {
    this-> nrow = from.nrow;
    this-> ncol = from.ncol;
    this-> col_types = from.col_types;
    this-> rows = from.rows;
    this-> cols = from.cols;
  }
 
  /** Create an empty schema **/
  Schema() {
    this-> nrow = 0;
    this-> ncol = 0;
    this-> col_types = new char[4];
    this-> rows = new String*[4];
    this-> cols = new String*[4];

  }
 
  /** Create a schema from a string of types. A string that contains
    * characters other than those identifying the four type results in
    * undefined behavior. The argument is external, a nullptr argument is
    * undefined. **/
  Schema(const char* types) {
    col_types = new char[strlen(types)];
    this->ncol = strlen(types);
    for (size_t i = 0; types[i] != 0; i++) {
      col_types[i] = types[i];
    }
    this->nrow = 0;
    this-> rows = new String*[0];
    this-> cols = new String*[ncol];
    for (size_t ii = 0; ii < ncol; ii++) {
      String* s = new String(std::to_string(ii).c_str());
      cols[ii] = s;
    }
  }
  
  /** Destructor for schema */
  ~Schema() {
    delete [] col_types;
    delete [] rows;
    delete [] cols;
  }
 
  /** Add a column of the given type and name (can be nullptr), name
    * is external. */
  void add_column(char typ, String* name) {
    if (typ != 'B' && typ != 'I' && typ != 'F' && typ != 'S') {
      return ;
    }
    String* toAdd;
    if (name == nullptr) {
      toAdd = new String(std::to_string(ncol + 1).c_str());

    } else if (this->col_idx(name->cstr_) != -1) {
      toAdd = new String(std::to_string(ncol + 1).c_str());
      
    } else {
      toAdd = new String(name->c_str());
    }
    char* new_types = new char[ncol + 1];
    String** new_cols = new String*[ncol+1];
    for (size_t i = 0; i < ncol; i++) {
      new_types[i] = col_types[i];
      new_cols[i] = cols[i];
    }
    new_types[ncol] = typ;
    new_cols[ncol] = toAdd;
    this->col_types = new_types;
    this->cols = new_cols;
    this->ncol = this->ncol + 1;
  }
 
  /** Add a row with a name (possibly nullptr), name is external. */
  // if nullptr is passed in, an empty string will be stored as the name
  void add_row(String* name) {
    String* toAdd = name;
    if (name == nullptr || (this->row_idx(toAdd->cstr_) != -1)) {
      toAdd = new String(std::to_string(nrow + 1).c_str());
    }
    String** new_row = new String*[this->nrow+1];
    for (size_t i = 0; i < nrow; i++) {
      new_row[i] = rows[i];
    }
    new_row[nrow] = toAdd;
    this->rows = new_row;
    nrow++;

  }
 
  /** Return name of row at idx; nullptr indicates no name. An idx >= width
    * is undefined. */
  String* row_name(size_t idx) {
    if (idx >= this->nrow) {
      return NULL;
    } else {
      return rows[idx];
    }
  }
 
  /** Return name of column at idx; nullptr indicates no name given.
    *  An idx >= width is undefined.*/
  String* col_name(size_t idx) {
    if (idx >= this->ncol) {
      return NULL;
    } else {
      return cols[idx];
    }

  }
 
  /** Return type of column at idx. An idx >= width is undefined. */
  char col_type(size_t idx) {
    if (idx >= this->ncol) {
      return 'N';
    } else {
      return col_types[idx];
    }
  }
 
  /** Given a column name return its index, or -1. */
  int col_idx(const char* name) {

    String* to_find = new String(name);
    for (size_t i = 0; i < ncol; i++) {
      if (cols[i] == nullptr) {
        continue;
      }
      if (to_find->equals(cols[i])) {
        return (int)i;
      }
    }
    return -1;
  }
 
  /** Given a row name return its index, or -1. */
  int row_idx(const char* name) {
    String* to_find = new String(name);
    for (size_t i = 0; i < this->nrow; i++) {
      if (to_find->equals(rows[i])) {
        return i;
      }
    }
    return -1;
  }
 
  /** The number of columns */
  size_t width() {
    return this->ncol;

  }
 
  /** The number of rows */
  size_t length() {
    return this->nrow;
  }

  friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & boost::serialization::base_object<Object>(*this);
        ar & this->nrow;
        ar & this->ncol;
        for (int i = 0; i < ncol; i++) {
            ar & col_types[i];
        }
        for (int i = 0; i < nrow; i++) {
          ar & rows[i];
        }
        for (int i = 0; i < ncol; i++) {
          ar & cols[i];
        }

    }

};