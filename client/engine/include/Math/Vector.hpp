#pragma once

#include <array>
#include <concepts>
#include <type_traits>

namespace Math {

template <unsigned int D>
concept IsLargerThanOne = D > 1;

template <unsigned int D, typename T>
  requires IsLargerThanOne<D> && std::is_arithmetic_v<T> && std::convertible_to<T, double>
class Vector {
 public:
  Vector<D, T>() : data(){};

  Vector<D, T>(std::array<T, D> data) : data(data) {}

  Vector<D, T>(const Vector<D, T>& v) : data(v.data) {}

  ~Vector<D, T>() = default;

  Vector<D, T>& operator=(const Vector<D, T>& v) {
    if (&v != this) {
      data = v.data;
    }
    return *this;
  };

  Vector<D, T>& operator=(const Vector<D, T>&& v) {
    if (&v != this) {
      data = std::move(v.data);
    }
    return *this;
  };

  Vector<D, T> operator+(const Vector<D, T>& v) const {
    Vector<D, T> a(*this);
    a += v;
    return a;
  };

  Vector<D, T> operator-(const Vector<D, T>& v) const {
    Vector<D, T> a(*this);
    a -= v;
    return a;
  };

  Vector<D, T> operator*(const Vector<D, T>& v) const {
    Vector<D, T> a(*this);
    a *= v;
    return a;
  };

  Vector<D, T> operator/(const Vector<D, T>& v) const {
    Vector<D, T> a(*this);
    a /= v;
    return a;
  };

  template <typename U>
    requires std::is_arithmetic_v<U>
  Vector<D, T> operator*(U b) const {
    Vector<D, T> v(*this);
    v *= b;
    return v;
  }

  template <typename U>
    requires std::is_arithmetic_v<U>
  Vector<D, T> operator/(U b) const {
    Vector<D, T> v(*this);
    v /= b;
    return v;
  }

  void operator+=(const Vector<D, T>& v) {
    for (size_t i = 0; i < D; ++i) {
      data[i] += v.data[i];
    }
  };

  void operator-=(const Vector<D, T>& v) {
    for (size_t i = 0; i < D; ++i) {
      data[i] -= v.data[i];
    }
  };

  void operator*=(const Vector<D, T>& v) {
    for (size_t i = 0; i < D; ++i) {
      data[i] *= v.data[i];
    }
  };

  void operator/=(const Vector<D, T>& v) {
    for (size_t i = 0; i < D; ++i) {
      data[i] /= v.data[i];
    }
  };

  template <typename U>
    requires std::is_arithmetic_v<U>
  void operator*=(U b) {
    for (size_t i = 0; i < D; ++i) {
      data[i] *= b;
    }
  }

  template <typename U>
    requires std::is_arithmetic_v<U>
  void operator/=(U b) {
    for (size_t i = 0; i < D; ++i) {
      data[i] /= b;
    }
  }

  T& operator[](size_t index) {
    return data[index];
  }

  const T& operator[](size_t index) const {
    return data[index];
  }

  bool operator==(const Vector<D, T>& v) const {
    return data == v.data;
  }

  bool ApproximatelyEqual(const Vector<D, T>& v, T maxDifference) const {
    bool equal = false;
    for (size_t i = 0; i < D; ++i) {
      equal = equal && data[i] >= v.data[i] - maxDifference && data[i] <= v.data[i] + maxDifference;
    }
    return equal;
  };

  void Clamp(T min, T max) {
    for (size_t i = 0; i < D; ++i) {
      data[i] = data[i] < min ? min : (data[i] > max ? max : data[i]);
    }
  }

 private:
  std::array<T, D> data;
};

}  // namespace Math
