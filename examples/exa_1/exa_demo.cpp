#include <ExMath.hpp>
#include <iostream>
#include <type_traits>

template <typename T, typename = std::enable_if_t<ExMath::Internal::is_r_matrix_v<T>>> void print_matrix(const char* name, T const& m)
{
  std::cout << "\n" << name << " = [\n";
  for (ExMath::index_t r = 0; r < T::matrix_size::rows; r++)
  {
    for (ExMath::index_t c = 0; c < T::matrix_size::columns; c++)
    {
      std::cout << m(r, c) << ",";
    }
    std::cout << "\n";
  }
  std::cout << "]\n";
}

using eye3 = ExMath::unit_matrix_t<float, 3>;
using m3x3 = ExMath::matrix_t<float, 3, 3>;
using m4x4 = ExMath::matrix_t<double, 4, 4>;

using index_t = ExMath::index_t;

static_assert(ExMath::Internal::is_r_matrix_v<eye3>);
static_assert(!ExMath::Internal::is_rw_matrix_v<eye3>);

static_assert(ExMath::Internal::is_r_matrix_v<m3x3>);
static_assert(ExMath::Internal::is_rw_matrix_v<m3x3>);

static_assert(!ExMath::Internal::is_scalar_v<ExMath::matrix_t<float, 1, 1>>);

static_assert(ExMath::Internal::is_same_size_matrix_v<m3x3, eye3>);


using dd = ExMath::Internal::type_promotion_t<m3x3, m3x3>

int main()
{
  std::cout << "This is a demo example!" << std::endl;

  ExMath::unit_matrix_t<float, 3> u3;
  print_matrix("m1", u3);

  {
    ExMath::matrix_t<float, 3, 3> m;
    print_matrix("m2", m);
  }
  {
    ExMath::matrix_t<float, 3, 3> m = u3;
    print_matrix("m3", m);
  }
  {
    ExMath::matrix_t<float, 3, 3> m;
    print_matrix("m4", m);
    m = u3;
    print_matrix("m5", m);
  }
  {
    ExMath::matrix_t<float, 3, 3> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    print_matrix("m6", m);
  }
  {
    ExMath::matrix_t<float, 3, 3> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
    auto                          m0 = m + m + ExMath::unit_matrix_t<float, 3>();
    print_matrix("m7", m0);
  }
  //{
  //  ExMath::matrix_t<float, 3, 3> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
  //  m += ExMath::unit_matrix_t<float, 3>();
  //  print_matrix("m8", m);
  //}
  //{
  //  ExMath::matrix_t<float, 3, 3> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
  //  auto                          m0 = m - m - ExMath::unit_matrix_t<float, 3>();
  //  print_matrix("m9", m0);
  //}
  //{
  //  ExMath::matrix_t<float, 3, 3> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
  //  m -= ExMath::unit_matrix_t<float, 3>();
  //  print_matrix("m10", m);
  //}
  //{
  //  ExMath::matrix_t<float, 3, 3> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
  //  auto                          m0 = m * m;
  //  print_matrix("m11", m0);
  //}
  //{
  //  ExMath::matrix_t<float, 3, 3> m1{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
  //  ExMath::matrix_t<float, 1, 1> m2{ 2.0f };
  //  auto                          m0 = m2 * m1 * m2;
  //  print_matrix("m12", m0);
  //}
  //{
  //  ExMath::matrix_t<float, 3, 3> m1{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
  //  auto                          m0 = 2.0f * m1 * 2.0f;
  //  print_matrix("m12", m0);
  //}

  return 0;
}
