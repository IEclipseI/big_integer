//
// Created by roman on 09.05.18.
//

#include "op_vector.h"

using namespace std;

op_vector::op_vector()
    : _size(0), _capacity(_small_object_size) {
  data_ptr = u.small_object_data;
  for (size_t i = 0; i < _small_object_size; ++i) {
    data_ptr[i] = 0;
  }
}

op_vector::op_vector(const op_vector &rhs) noexcept :
    _size(rhs._size), _capacity(rhs._capacity) {
  if (rhs._is_small()) {
    for (size_t i = 0; i < _small_object_size; ++i) {
      u.small_object_data[i] = rhs.u.small_object_data[i];
    }
    data_ptr = u.small_object_data;
  } else {
    new(&u._big_object_ptr) shared_ptr<vector<uint32_t>>(rhs.u._big_object_ptr);
    data_ptr = rhs.data_ptr;
  }
}

op_vector::op_vector(size_t size) : _size(size) {
  if (size > _small_object_size) {
    new (&u._big_object_ptr) shared_ptr<vector<uint32_t>>(new vector<uint32_t>(size));
    data_ptr = u._big_object_ptr->data();
    _capacity = u._big_object_ptr->capacity();
  } else {
    data_ptr = u.small_object_data;
    _capacity = _small_object_size;
    for (uint32_t i = 0; i < size; ++i) {
      u.small_object_data[i] = 0;
    }
  }
}

op_vector &op_vector::operator=(const op_vector &rhs) noexcept {
  if (rhs._is_small()) {
    if (!_is_small()) u._big_object_ptr.~shared_ptr();
    for (size_t i = 0; i < _size; ++i) {
      u.small_object_data[i] = rhs.u.small_object_data[i];
    }
    data_ptr = u.small_object_data;
  } else {
    if (_is_small())
      new(&u._big_object_ptr) shared_ptr<vector<uint32_t>>(rhs.u._big_object_ptr);
    else
      u._big_object_ptr = rhs.u._big_object_ptr;
    data_ptr = rhs.data_ptr;
  }
  _size = rhs.size();
  _capacity = rhs.capacity();
  return *this;
}

void op_vector::ensure_capacity(size_t new_size) {
  if (_capacity < new_size) {
    if (_is_small()) {
      shared_ptr<vector<uint32_t>> ptr(new vector<uint32_t>(u.small_object_data,
                                                            u.small_object_data +
                                                                sizeof(u.small_object_data) / sizeof(uint32_t)));
      ptr->resize(_capacity * 2);
      new(&u._big_object_ptr) shared_ptr<vector<uint32_t>>(ptr);
      data_ptr = ptr->data();
      _capacity *= 2;
    } else {
      u._big_object_ptr->resize(_capacity * 2);
      data_ptr = u._big_object_ptr->data();
      _capacity *= 2;
    }
  }
}

op_vector::~op_vector() {

  if (!_is_small() ) {
    u._big_object_ptr.~shared_ptr();
  }
}

size_t op_vector::capacity() const {
  return _capacity;
}

size_t op_vector::size() const {
  return _size;
}

void op_vector::push_back(uint32_t elem) {
  ensure_capacity(_size + 1);
  data_ptr[_size++] = elem;
}

uint32_t &op_vector::operator[](uint32_t pos) {
  return data_ptr[pos];
}

uint32_t &op_vector::operator[](uint32_t pos) const {
  return data_ptr[pos];
}

void op_vector::resize(uint32_t size) {
  _size = size;
  ensure_capacity(_size);
}

void op_vector::make_unique() {
  if (_is_small() ) {
    for (size_t i = 0; i < _size; ++i) {
      u.small_object_data[i] = data_ptr[i];
    }
    data_ptr = u.small_object_data;
  } else {
    if (!u._big_object_ptr.unique()) {
      u._big_object_ptr.reset(new vector<uint32_t>(*u._big_object_ptr));
      data_ptr = u._big_object_ptr->data();
    }
  }
}

void op_vector::pop_back() {
  --_size;
}

uint32_t op_vector::back() {
  return data_ptr[_size - 1];
}

void op_vector::inverse_values() {
  for (uint32_t i = 0; i < _size; ++i) {
    data_ptr[i] = ~data_ptr[i];
  }
}
void op_vector::swap(op_vector &rhs) {
  std::swap(this->_size, rhs._size);
  std::swap(this->_capacity, rhs._capacity);
  if (_is_small() ) {
    if (rhs._is_small() ) {
      std::swap(u.small_object_data, rhs.u.small_object_data);
      this->data_ptr = this->u.small_object_data;
      rhs.data_ptr = rhs.u.small_object_data;
    } else {
      op_vector::data tmp;
      std::swap(tmp.small_object_data, u.small_object_data);
      std::swap(u._big_object_ptr, rhs.u._big_object_ptr);
      std::swap(tmp.small_object_data, rhs.u.small_object_data);
    }
  } else {
    if (rhs._is_small() ) {
      op_vector::data tmp;
      std::swap(tmp._big_object_ptr, u._big_object_ptr);
      std::swap(u.small_object_data, rhs.u.small_object_data);
      std::swap(tmp._big_object_ptr, rhs.u._big_object_ptr);
    } else {
      op_vector::data tmp;
      std::swap(tmp._big_object_ptr, u._big_object_ptr);
      std::swap(u._big_object_ptr, rhs.u._big_object_ptr);
      std::swap(tmp._big_object_ptr, rhs.u._big_object_ptr);
    }
  }
  this->data_ptr = _is_small()  ? this->u.small_object_data : this->u._big_object_ptr->data();
  rhs.data_ptr = rhs._is_small() ? rhs.u.small_object_data : rhs.u._big_object_ptr->data();
}
bool op_vector::_is_small() const {
  return _capacity <= _small_object_size;
}


