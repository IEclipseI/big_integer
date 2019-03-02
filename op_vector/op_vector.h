//
// Created by roman on 09.05.18.
//

#ifndef VECTOR_OPTIMIZATIONS_OPTIMIZED_VECTOR_H
#define VECTOR_OPTIMIZATIONS_OPTIMIZED_VECTOR_H

#include <iostream>
#include <memory>
#include <vector>
#include <cstring>
using namespace std;

static const size_t _small_object_size = 2;


class op_vector {
 public:
  op_vector();
  op_vector(const op_vector &) noexcept;
  explicit op_vector(size_t size);
  op_vector &operator=(const op_vector &) noexcept;
  ~op_vector();
 private:
  bool _is_small() const;
  void ensure_capacity(size_t size);
 public:
  size_t capacity() const;
  size_t size() const;
  void push_back(uint32_t elem);
  uint32_t &operator[](uint32_t pos);
  uint32_t &operator[](uint32_t pos) const;
  void resize(uint32_t size);
  void make_unique();
  void pop_back();
  uint32_t back();
  void inverse_values();
  void swap(op_vector& rhs);
 private:
  uint32_t *data_ptr;
  union data {
    uint32_t small_object_data[_small_object_size];
    shared_ptr<vector<uint32_t>> _big_object_ptr;
    data(){}
    ~data(){}
  } u;
  size_t _size;
  size_t _capacity;
};

#endif //VECTOR_OPTIMIZATIONS_OPTIMIZED_VECTOR_H
