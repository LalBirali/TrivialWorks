 #pragma once
 #include "object.h"
 #include "string.h"
 #include "helper.h"
 #include <cstdarg>
 #include <boost/serialization/array_wrapper.hpp>
 #include <boost/serialization/export.hpp>


static int bufferSize = 10000;

 /**************************************************************************
 * Column ::
 * Represents one column of a data frame which holds values of a single type.
 * This abstract class defines methods overriden in subclasses. There is
 * one subclass per element type. Columns are mutable, equality is pointer
 * equality. 
 * TODO: as per Jan's comments, still working on Refactoring
 * */

class IntColumn;
class BoolColumn;
class FloatColumn;
class StringColumn;

class Column : public Object {
 public:
  char type;
  /** Type converters: Return same column under its actual type, or
   *  nullptr if of the wrong type.  */
  virtual IntColumn* as_int() {return NULL;}
  virtual BoolColumn*  as_bool() {return NULL;}
  virtual FloatColumn* as_float() {return NULL;}
  virtual StringColumn* as_string() {return NULL;}
 
  /** Type appropriate push_back methods. Calling the wrong method is
    * undefined behavior. **/
  virtual void push_back(int val) {}
  virtual void push_back(bool val) {}
  virtual void push_back(float val) {}
  virtual void push_back(String* val) {}
 
 /** Returns the number of elements in the column. */
  virtual size_t size() {return 0;}
 
  /** Return the type of this column as a char: 'S', 'B', 'I' and 'F'. */
  char get_type() {
    return this->type;
  }

  friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & boost::serialization::base_object<Object>(*this);
        ar & type;
    }
};

/*************************************************************************
 * IntColumn::
 * Holds primitive int values, unwrapped.
 */
class IntColumn : public Column {
 public:
    int* list;
    size_t size_;
    size_t capacity;
    IntColumn * next;
    size_t total;
  /** Default constructor for IntColumn. Uses the global variable bufferSize to decide how much memory to allocate */
  IntColumn() {
    list = new int[bufferSize];
    capacity = bufferSize;
    size_ = 0;
    next = nullptr;
    total = 0;
    type = 'I';
  }

  /** Constructor for IntColumn that takes in n arguments in the ... of type int.
   * If n is less than the bufferSize, allocates bufferSize spaces for the array
   * if not, allocates n spaces in the array.
   */
  IntColumn(int n, ...) {
    if (n > bufferSize) {
      list = new int[n];
      capacity = n;
    } else {
      list = new int[bufferSize];
      capacity = bufferSize;
    }
    size_ = 0;
    type = 'I';
    std::va_list args;
    va_start(args, n);
    for (size_t i = 0; i < (size_t)n; ++i) {

        list[size_] = va_arg(args, int);
        size_++;
        total++;
        
    }
    va_end(args);
    this->next = nullptr;
  }

  /** Destructor for IntColumn */
  ~IntColumn() {
    delete [] list;
  }

  friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & boost::serialization::base_object<Column>(*this);
        ar & boost::serialization::make_array<int>(list, bufferSize);
        ar & capacity;
        ar & size_;
        ar & BOOST_SERIALIZATION_NVP(next);
        ar & total;

    }

  /**
  * Returns the integer at space idx in the IntColumn
  */
  int get(size_t idx) {
    if (idx >= total) {
        return 0;
    } else {
        if (idx < size_) {
          return list[idx];
        } else {
          return this->next->get(idx - capacity);
        }
    }
  }
  IntColumn* as_int()  {
    return this;
  }
  BoolColumn* as_bool()  {
    return nullptr;
  }
  FloatColumn* as_float()  {
    return nullptr;
  }
  StringColumn* as_string()  {
    return nullptr;
  }
  void set(size_t idx, int val) {
     if (idx >= total) {
        return;
    } else {
        if (idx < size_) {
          list[idx] = val;
          return;
        } else {
          this->next->set(idx - capacity, val);
        }
    }
  }
  void push_back(int val) {
      total++;
      if (size_ < capacity) {
        
        list[size_] = val;
        
        size_++;
      } else {
          if (this->next == nullptr) {
              this->next = new IntColumn();
          }
          this->next->push_back(val);
      } 
  }
  size_t size() {
      return total;
  }
  
};


/*************************************************************************
 * BoolColumn::
 * Holds primitive bool values, unwrapped.
 */
class BoolColumn : public Column {
 public:
    bool* list;
    size_t size_;
    size_t capacity;
    BoolColumn * next;
    size_t total;

  /** Default constructor for BoolColumn. Uses the global variable bufferSize to decide how much memory to allocate */
  BoolColumn() {
    list = new bool[bufferSize];
    capacity = bufferSize;
    size_ = 0;
    next = nullptr;
    total = 0;
    type = 'B';
  }

  /** Constructor for BoolColumn that takes in n arguments in the ... of type bool.
   * If n is less than the bufferSize, allocates bufferSize spaces for the array
   * if not, allocates n spaces in the array.
   */
  BoolColumn(int n, ...) {
    if (n > bufferSize) {
      list = new bool[n];
      capacity = n;
    } else {
      list = new bool[bufferSize];
      capacity = bufferSize;
    }
    size_ = 0;
    type = 'B';

    std::va_list args;
    va_start(args, n);
    for (size_t i = 0; i < (size_t)n; ++i) {

        list[size_] = va_arg(args, bool);
        size_++;
        total++;
        
    }
    va_end(args);
    this->next = nullptr;
  }

  /** Destructor for BoolColumn */
  ~BoolColumn() {
    delete [] list;
  }

  friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & boost::serialization::base_object<Column>(*this);
        ar & boost::serialization::make_array<bool>(list, bufferSize);
        ar & capacity;
        ar & size_;
        ar & BOOST_SERIALIZATION_NVP(next);
        ar & total;


    }

  /** Gets the boolean at index idx in the BoolColumn */
  bool get(size_t idx) {
      if (idx >= total) {
        return 0;
      } else {
        if (idx < size_) {
          return list[idx];
        } else {
          return this->next->get(idx - capacity);
        }
    }
  }
  IntColumn* as_int()  {
    return nullptr;
  }
  BoolColumn* as_bool()  {
    return this;
  }
  FloatColumn* as_float()  {
    return nullptr;
  }
  StringColumn* as_string()  {
    return nullptr;
  }
  void set(size_t idx, bool val) {
     if (idx >= total) {
        return;
    } else {
        if (idx < size_) {
          list[idx] = val;
          return;
        } else {
          this->next->set(idx - capacity, val);
        }
    }
    
  }
  void push_back(bool val) {

      total++;
      if (size_ < capacity) {
          list[size_] = val;
          size_++;
      } else {
          if (this->next == nullptr) {
              this->next = new BoolColumn();
          }
          this->next->push_back(val);
      } 
    
  }
  size_t size() {
      return total;
  }
  
};

/*************************************************************************
 * FloatColumn::
 * Holds primitive float values, unwrapped.
 */
class FloatColumn : public Column {
 public:
    float* list;
    size_t size_;
    size_t capacity;
    FloatColumn * next;
    size_t total;

  /** Default constructor for FloatColumn. Uses the global variable bufferSize to decide how much memory to allocate */
  FloatColumn() {
    list = new float[bufferSize];
    capacity = bufferSize;
    size_ = 0;
    next = nullptr;
    total = 0;
    type = 'F';
  }

  /** Constructor for FloatColumn that takes in n arguments in the ... of type float.
   * If n is less than the bufferSize, allocates bufferSize spaces for the array
   * if not, allocates n spaces in the array.
   */
  FloatColumn(int n, ...) {
    if (n > bufferSize) {
      list = new float[n];
      capacity = n;
    } else {
      list = new float[bufferSize];
      capacity = bufferSize;
    }
    size_ = 0;
    type = 'F';

    std::va_list args;
    va_start(args, n);
    for (size_t i = 0; i < (size_t)n; ++i) {

        list[size_] = va_arg(args, float);
        size_++;
        total++;
        
    }
    va_end(args);
    this->next = nullptr;
  }

  /** Destructor for FloatColumn */
  ~FloatColumn() {
    delete [] list;
  }

  friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & boost::serialization::base_object<Column>(*this);
        ar & boost::serialization::make_array<float>(list, bufferSize);
        ar & capacity;
        ar & size_;
        ar & BOOST_SERIALIZATION_NVP(next);
        ar & total;


    }

  /** Returns the float at index idx in the FloatColumn */
  float get(size_t idx) {
      if (idx >= total) {
        return 0;
      } else {
        if (idx < size_) {
          return list[idx];
        } else {
          return this->next->get(idx - capacity);
        }
    }
  }
  IntColumn* as_int()  {
    return nullptr;
  }
  BoolColumn* as_bool()  {
    return nullptr;
  }
  FloatColumn* as_float()  {
    return this;
  }
  StringColumn* as_string()  {
    return nullptr;
  }
  void set(size_t idx, float val) {
      if (idx >= total) {
        return;
    } else {
        if (idx < size_) {
          list[idx] = val;
          return;
        } else {
          this->next->set(idx - capacity, val);
        }
    }
    
  }
  void push_back(float val) {

      total++;
      if (size_ < capacity) {
          list[size_] = val;
          size_++;
      } else {
          if (this->next == nullptr) {
              this->next = new FloatColumn();
          }
          this->next->push_back(val);
      } 
    
  }
  size_t size() {
      return total;
  }
  
};

/*************************************************************************
 * StringColumn::
 * Holds string pointers. The strings are external.  Nullptr is a valid
 * value.
 */
class StringColumn : public Column {
 public:
 String** list;
 size_t size_;
 size_t capacity;
 StringColumn * next;
 size_t total;
  /** Default constructor for StringColumn. Uses the global variable bufferSize to decide how much memory to allocate */
  StringColumn() {
    list = new String*[bufferSize];
    capacity = bufferSize;
    size_ = 0;
    next = nullptr;
    total = 0;
    type = 'S';

  }

  /** Constructor for StringColumn that takes in n arguments in the ... of type String.
   * If n is less than the bufferSize, allocates bufferSize spaces for the array
   * if not, allocates n spaces in the array.
   */
  StringColumn(int n, ...) {
    if (n > bufferSize) {
      list = new String*[n];
      capacity = n;
    } else {
      list = new String*[bufferSize];
      capacity = bufferSize;
    }
    size_ = 0;
    type = 'S';

    std::va_list args;
    va_start(args, n);
    for (size_t i = 0; i < (size_t)n; ++i) {

        list[size_] = va_arg(args, String*);
        size_++;
        total++;
        
    }
    va_end(args);

  }

  friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & boost::serialization::base_object<Column>(*this);
        ar & boost::serialization::make_array<String*>(list, bufferSize);
        ar & capacity;
        ar & size_;
        ar & BOOST_SERIALIZATION_NVP(next);
        ar & total;


    }

  /** Destructor for StringColumn */
  ~StringColumn() {
    delete [] list;
  }
  IntColumn* as_int()  {
    return nullptr;
  }
  BoolColumn* as_bool()  {
    return nullptr;
  }
  FloatColumn* as_float()  {
    return nullptr;
  }
  StringColumn* as_string()  {
    return this;
  }
  /** Returns the string at idx; undefined on invalid idx.*/
  String* get(size_t idx) {
    if (idx >= total) {
        return 0;
    } else {
        if (idx < size_) {
          return list[idx];
        } else {
          return this->next->get(idx - capacity);
        }
    }
  }
  /** Acquire ownership for the string. */
  void set(size_t idx, String* val) {
    if (idx >= total) {
        return;
    } else {
        if (idx < size_) {
          list[idx] = val;
          return;
        } else {
          this->next->set(idx - capacity, val);
        }
    }
  }
  void push_back(String* val) {
    total++;
      if (size_ < capacity) {
          list[size_] = val;
          size_++;
      } else {
          if (this->next == nullptr) {
              this->next = new StringColumn();
          }
          this->next->push_back(val);
      } 
  }
  size_t size() {
    return total;
  }
  
};