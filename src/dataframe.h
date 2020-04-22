#pragma once
#include "rower.h"
#include "column.h"
#include "object.h"
#include "row.h"
#include "helper.h"
#include <iostream>
#include <chrono>

class DataFrame;

class KVStore {
    public:
    std::vector<Key*> keys;
    std::vector<DataFrame*> dfs;
    /**
    Construtor for KVStore
    */
    KVStore() {}

    /**
    Destructs a Dataframe
    */
    ~KVStore() {}

    /**
    This method returns the dataframe associated with the given key.
    */
    DataFrame * get(Key * k) {
        if (k == NULL) {
            return nullptr;
        }
        for (unsigned int i = 0; i < keys.size(); i++) {
            if (k->equals(keys[i])) {
                return dfs[i];
            }
        }
        return nullptr;
    }

    /**
    Returns the dataframe assciated with the given key.
    */
    DataFrame * waitAndGet(Key & k) {
        return get(&k);
    }
    
    /**
    Maps the inputted dataframe to the given key.
    If the key has been mapped to a different dataframe, that dataframe is returned.
    If not, a nullptr is returned.
    */
    DataFrame * put(Key * k, DataFrame * df) {
        if (k == NULL || df == NULL) {
            return nullptr;
        }
        if (get(k) == nullptr) {
            keys.push_back(k);
            dfs.push_back(df);
            return nullptr;
        } else {
            for (unsigned int i = 0; i < keys.size(); i++) {
                if (k->equals(keys[i])) {
                    DataFrame * temp = dfs[i];
                    dfs[i] = df;
                    return temp;
                }
            }
        }
        return nullptr;
    }
};

/****************************************************************************
 * DataFrame::
 *
 * A DataFrame is table composed of columns of equal length. Each column
 * holds values of the same type (I, S, B, F). A dataframe has a schema that
 * describes it.
 */
class DataFrame : public Object {
 public:
  Schema* schem_;
  Column** columns;
  size_t nrow;
  size_t ncol;
  
  DataFrame() {
    nrow = 0;
    ncol = 0;
    schem_ = new Schema();
    columns = new Column*[10];
    
  }


  /** Create a data frame with the same columns as the give df but no rows */
  DataFrame(DataFrame& df) {
    schem_ = df.schem_;
    columns = df.columns;
    nrow = df.nrow;
    ncol = df.ncol;
  }
 
  /** Create a data frame from a schema and columns. Results are undefined if
    * the columns do not match the schema. */
  DataFrame(Schema& schema) {
    schem_ = &schema;
    columns = new Column*[schem_->width()];
    for (size_t i = 0; i < schem_->width(); i++) {
      char typ = schem_->col_types[i];
      switch(typ) {
        case 'B':
          columns[i] = new BoolColumn();
        break;
        case 'I':
          columns[i] = new IntColumn();
        break;
        case 'F':
          columns[i] = new FloatColumn();
        break;
        case 'S':
          columns[i] = new StringColumn();
        break;
      }
    }
    nrow = schem_->length();
    ncol = schem_->width();
  }

  /** Destructor for Dataframe */
  ~DataFrame() {
      delete [] columns;
  }

  friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        //ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
        ar & nrow;
        ar & ncol;
        ar & BOOST_SERIALIZATION_NVP(schem_);
        for (size_t i = 0; i < ncol; i++) {
          switch(schem_->col_types[i]) {
            case 'I': {
              IntColumn * in = dynamic_cast<IntColumn*>(columns[i]);
              ar & in;
            }break;
            case 'F': {
              FloatColumn * in = dynamic_cast<FloatColumn*>(columns[i]);
              ar & in;
            }break;
            case 'S': {
              StringColumn * in = dynamic_cast<StringColumn*>(columns[i]);
              ar & in;
            }break;
            case 'B': {
              BoolColumn * in = dynamic_cast<BoolColumn*>(columns[i]);
              ar & in;
            }break;
          }
        }
        

    }
 
  /** Returns the dataframe's schema. Modifying the schema after a dataframe
    * has been created in undefined. */
  Schema& get_schema() {

    return *schem_;

  }


  static DataFrame* fromArray(Key* k, KVStore * kv, size_t size, double* vals) {
    char * cols = new char[1];
    cols[0] = 'F';
    Schema sc(cols);
    Row r(sc);
    DataFrame * ret = new DataFrame(sc);
    for (size_t i = 0; i < size; i++) {
      r.set(0,(float)vals[i]);
      ret->add_row(r);
    }
    kv->put(k, ret);
    return ret;
  }

  static DataFrame* fromArray(Key* k, KVStore * kv, size_t size, int* vals) {
    char * cols = new char[1];
    cols[0] = 'I';
    Schema sc(cols);
    Row r(sc);
    DataFrame * ret = new DataFrame(sc);
    for (size_t i = 0; i < size; i++) {
      r.set(0,vals[i]);
      ret->add_row(r);
    }
    kv->put(k, ret);
    return ret;
  }

  static DataFrame* fromArray(Key* k, KVStore * kv, size_t size, String* vals) {
    char * cols = new char[1];
    cols[0] = 'S';
    Schema sc(cols);
    Row r(sc);
    DataFrame * ret = new DataFrame(sc);
    for (size_t i = 0; i < size; i++) {
      r.set(0,&vals[i]);
      ret->add_row(r);
    }
    kv->put(k, ret);
    return ret;
  }

  static DataFrame* fromArray(Key* k, KVStore * kv, size_t size, bool* vals) {
    char * cols = new char[1];
    cols[0] = 'B';
    Schema sc(cols);
    Row r(sc);
    DataFrame * ret = new DataFrame(sc);
    for (size_t i = 0; i < size; i++) {
      r.set(0,vals[i]);
      ret->add_row(r);
    }
    kv->put(k, ret);
    return ret;
  }


  static DataFrame* fromScalar(Key * k, KVStore * kv, double val) {
    char * cols = new char[1];
    cols[0] = 'F';
    Schema sc(cols);
    Row r(sc);
    DataFrame * ret = new DataFrame(sc);
    r.set(0,(float)val);
    ret->add_row(r);
    kv->put(k, ret);
    return ret;

  }

  static DataFrame* fromScalar(Key * k, KVStore * kv, int val) {
    char * cols = new char[1];
    cols[0] = 'I';
    Schema sc(cols);
    Row r(sc);
    DataFrame * ret = new DataFrame(sc);
    r.set(0,val);
    ret->add_row(r);
    kv->put(k, ret);
    return ret;
  }

  static DataFrame* fromScalar(Key * k, KVStore * kv, String* val) {
    char * cols = new char[1];
    cols[0] = 'S';
    Schema sc(cols);
    Row r(sc);
    DataFrame * ret = new DataFrame(sc);
    r.set(0,val);
    ret->add_row(r);
    kv->put(k, ret);
    return ret;
  }

  static DataFrame* fromScalar(Key * k, KVStore * kv, bool val) {
    char * cols = new char[1];
    cols[0] = 'B';
    Schema sc(cols);
    Row r(sc);
    DataFrame * ret = new DataFrame(sc);
    r.set(0,val);
    ret->add_row(r);
    kv->put(k, ret);
    return ret;
  }


  static DataFrame* fromScalar(Key * k, KVStore * kv, size_t val) {
    char * cols = new char[1];
    cols[0] = 'I';
    Schema sc(cols);
    Row r(sc);
    DataFrame * ret = new DataFrame(sc);
    r.set(0,(int)val);
    ret->add_row(r);
    kv->put(k, ret);
    return ret;
  }

 
  /** Adds a column this dataframe, updates the schema, the new column
    * is external, and appears as the last column of the dataframe, the
    * name is optional and external. A nullptr colum is undefined. */
  void add_column(Column* col, String* name) {
      if (col == nullptr) {
          std::cout << "Added Column cannot be a nullptr" << std::endl;
          return;
      }
      char typ = col->get_type();
    if (col != nullptr && (typ == 'B' || typ == 'I' || typ == 'F' || typ == 'S')) {
      if (ncols() == 0) {
        columns = new Column*[1];
        columns[0] = col;
        schem_->add_column(typ, name);
        nrow = col->size();
        schem_->nrow = col->size();
      }
      else if (col->size() == columns[0]->size()) {
        Column** colCopy = new Column*[schem_->width() + 1];
        for (size_t i = 0; i < schem_->width(); i++) {
          colCopy[i] = columns[i];
        }
        columns = colCopy;
        columns[schem_->width()] = col;

        schem_->add_column(typ, name);
      }
      ncol++;
    }
  }
 
  /** Return the value at the given column and row. Accessing rows or
   *  columns out of bounds, or request the wrong type is undefined.*/
  int get_int(size_t col, size_t row) {
    if (col < schem_->width()) {
      if (columns[col]->get_type() == 'I') {
        IntColumn*i = dynamic_cast<IntColumn*>(columns[col]);
        return i->get(row);
      } 
    }

    return -1;

  }
  bool get_bool(size_t col, size_t row) {

    if (col < schem_->width()) {
      if (columns[col]->get_type() == 'B') {
        BoolColumn*b = dynamic_cast<BoolColumn*>(columns[col]);
        return b->get(row);
      } 
    }

    return 0;

  }
  float get_float(size_t col, size_t row) {

    if (col < schem_->width()) {
      if (columns[col]->get_type() == 'F') {
        FloatColumn*fl = dynamic_cast<FloatColumn*>(columns[col]);
        return fl->get(row);
      } 
    }

    return -1;

  }

  double get_double(size_t col, size_t row) {

    if (col < schem_->width()) {
      if (columns[col]->get_type() == 'F') {
        FloatColumn*fl = dynamic_cast<FloatColumn*>(columns[col]);
        return (double)fl->get(row);
      } 
    }

    return -1;

  }
  String*  get_string(size_t col, size_t row) {

    if (col < schem_->width()) {
      if (columns[col]->get_type() == 'S') {
        StringColumn*str = dynamic_cast<StringColumn*>(columns[col]);
        return str->get(row);
      } 
    }

    return nullptr;

  }
 
  /** Return the offset of the given column name or -1 if no such col. */
  int get_col(String& col) {

    for (size_t i = 0; i < schem_->width(); i++) {
      String* name = schem_->cols[i];
      if (name->equals(&col)) {
        return i;
      }
    }
    return -1;

  }
 
  /** Return the offset of the given row name or -1 if no such row. */
  int get_row(String& col) {

    for (size_t i = 0; i < schem_->width(); i++) {
      String* name = schem_->rows[i];
      if (name->equals(&col)) {
        return i;
      }
    }
    return -1;

  }
 
  /** Set the value at the given column and row to the given value.
    * If the column is not  of the right type or the indices are out of
    * bound, the result is undefined. */
  void set(size_t col, size_t row, int val) {

    if (col < schem_->width()) {
      if (columns[col]->get_type() == 'I') {
         IntColumn*i = dynamic_cast<IntColumn*>(columns[col]);
          i->set(row, val);
      } 
    }

  }
  void set(size_t col, size_t row, bool val) {

    if (col < schem_->width()) {
      if (columns[col]->get_type() == 'B') {
        BoolColumn*bo = dynamic_cast<BoolColumn*>(columns[col]);
          bo->set(row, val);
      } 
    }

  }
  void set(size_t col, size_t row, float val) {

    if (col < schem_->width()) {
      if (columns[col]->get_type() == 'F') {
        FloatColumn*fl = dynamic_cast<FloatColumn*>(columns[col]);
          fl->set(row, val);
      } 
    }

  }
  void set(size_t col, size_t row, String* val) {

    if (col < schem_->width()) {
      if (columns[col]->get_type() == 'S') {
        StringColumn*str = dynamic_cast<StringColumn*>(columns[col]);
          str->set(row, val);
      } 
    }

  }
 
  /** Set the fields of the given row object with values from the columns at
    * the given offset.  If the row is not form the same schema as the
    * dataframe, results are undefined.
    */
  void fill_row(size_t idx, Row& row) {
    if (row.width() == schem_->width()) {
      for (size_t i = 0; i < row.width(); i++) {
        if (row.col_types[i] != schem_->col_types[i]) {
          return;
        }
      }
      for (size_t i = 0; i < row.width(); i++) {
        char typ = columns[i]->get_type();
        switch(typ) {
          case 'B': {
              bool b = row.get_bool(i);
              
              ((BoolColumn*)columns[i])->set(idx,b);
              
              break;
          }
          case 'I':
            {
              int i = row.get_int(i);
              if (i != -1) {
                ((IntColumn*)columns[i])->set(idx,i);
              } else {
                ((IntColumn*)columns[i])->set(idx,0);
              }
              break;
            }
          case 'F':
            {
              float fl = row.get_float(i);
              if (fl != -1.0) {
                ((FloatColumn*)columns[i])->set(idx,fl);
              } else {
                ((FloatColumn*)columns[i])->set(idx,0.0);
              }
              break;
            }
          case 'S':
            {
              String* s = row.get_string(i);
              if (s != nullptr) {
                ((StringColumn*)columns[i])->set(idx,s);
              } else {
                char* c = new char[0];
                String* bruh = new String(true, c, 0);
                ((StringColumn*)columns[i])->set(idx,bruh);
              }
              break;
            }
        }
      }

    }

  }

 
  /** Add a row at the end of this dataframe. The row is expected to have
   *  the right schema and be filled with values, otherwise undefined.  */
  void add_row(Row& row) {
    if (row.width() == this->ncol) {
      for (size_t i = 0; i < row.width(); i++) {
        if (row.col_types[i] != schem_->col_types[i]) {
          return;
        }
      }
      for (size_t i = 0; i < row.width(); i++) {
        char typ = columns[i]->get_type();
        switch(typ) {
          case 'B': {
            bool b = row.get_bool(i);
            columns[i]->push_back(b);
            break;
          }
          case 'I':
            {
              int ii = row.get_int(i);
              if (ii != -1) {
                columns[i]->push_back(ii);
              } else {
                columns[i]->push_back(0);
              }
              break;
            }
          case 'F':
            {
              float fl = row.get_float(i);
              if (fl != -1.0) {
                columns[i]->push_back(fl);
              } else {
                float lel = 0.0;
                columns[i]->push_back(lel);
              }
              break;
            }
          case 'S':
            {
              String* s = row.get_string(i);
              if (s != nullptr) {
                columns[i]->push_back(s);
              } else {
                char* c = new char[0];
                String* bruh = new String(true, c, 0);
                columns[i]->push_back(bruh);
              }
              break;
            }
        }
      }
      nrow++;
      String * str = new String(std::to_string(nrow).c_str());
      schem_->add_row(str);
    } else {
        std::cout << "Size of the row is off -- add_row" << std::endl;
    } 
    
    

  }
 
  /** The number of rows in the dataframe. */
  size_t nrows() {

    return nrow;

  }
 
  /** The number of columns in the dataframe.*/
  size_t ncols() {
    return ncol;
  }

  /**
  * Constructs a row to be used by map.
  */
  Row* convert_to_row(size_t index) {
    if (index >= nrows()) {
      return nullptr;
    }
    Row* r = new Row(*schem_);
    for (size_t i = 0; i < this->ncols(); i++) {
      char typ = schem_->col_types[i];
        switch(typ) {
          case 'B':
            {
              bool b = ((BoolColumn*)columns[i])->get(index);
              r->set(i,b);
              break;
            }
          case 'I':
            {
              int inter = ((IntColumn*)columns[i])->get(index);

              r->set(i,inter);
              break;
            }
          case 'F':
            {
              float f = ((FloatColumn*)columns[i])->get(index);
              r->set(i,f);
              break;
            }
          case 'S':
            {
              String* s = ((StringColumn*)columns[i])->get(index);
              r->set(i,s);
              break;
            }
        }
      }
    return r;
  } 

  DataFrame * subset(size_t from_row, size_t end_row) {
    if (from_row >= end_row) {
      return NULL;
    }
    if (end_row > this->nrows()) {
      return NULL;
    }
    char* types = new char[ncol];
    for (size_t ii = 0; ii < ncol; ii++) {
      types[ii] = schem_->col_types[ii];
    }
    Schema* sc = new Schema(types);
    DataFrame* ret = new DataFrame(*sc);

    Row * r;
    for (size_t i = from_row; i < end_row; i++) {
      r = this->convert_to_row(i);
      ret->add_row(*r);
    }
    return ret;
  }
 
  /** Visit rows in order */
  void map(Rower& r) {
    std::chrono::high_resolution_clock::time_point h_start, h_end;
    h_start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < this->nrows(); i++) {
      Row* row = this->convert_to_row(i);
      r.accept(*row);
    }
    h_end = std::chrono::high_resolution_clock::now();
    std::cout << "highres = " << std::chrono::duration<double>(h_end - h_start).count() << " s \n";
  }
 
  /** Create a new dataframe, constructed from rows for which the given Rower
    * returned true from its accept method. */
  DataFrame* filter(Rower& r) {
    DataFrame* df;
    for (size_t i = 0; i < this->nrows(); i++) {
      Row* row = this->convert_to_row(i);
      if(r.accept(*row)) {
        df->add_row(*row);
      }
    }
    return df;
  }
 
  /** Print the dataframe in SoR format to standard output. */
  void print() {
    Sys syst;
    for (size_t i = 0; i < schem_->width(); i++) {
      for (size_t j = 0; j < schem_->length(); j++) {
          if (j > 0) {
              syst.p(" ");
          }
        char typ = columns[i]->get_type();
        switch(typ) {
          case 'B':
            {
              bool b = ((BoolColumn*)columns[i])->get(j);
              syst.p(b);
              break;
            }
          case 'I':
            {
              int inter = ((IntColumn*)columns[i])->get(j);
              syst.p(inter);
              break;
            }
          case 'F':
            {
              float f = ((FloatColumn*)columns[i])->get(j);
              syst.p(f);
              break;
            }
          case 'S':
            {
              String* s = ((StringColumn*)columns[i])->get(j);
              syst.p(s->c_str());
              break;
            }
        }
      }
      syst.pln();
    }

  }
};