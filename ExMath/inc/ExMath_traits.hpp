#pragma once
#ifndef EXMATH_TRAITS_HPP
#define EXMATH_TRAITS_HPP

#include <cmath>
#include <type_traits>

namespace ExMath
{
  using index_t = uint32_t;

  template <index_t rows, index_t columns> struct matrix_size_t
  {
    static constexpr index_t rows     = rows;
    static constexpr index_t columns  = columns;
    static constexpr index_t elements = rows * columns;

    static constexpr index_t calc_idx(index_t const& row, index_t const& column) noexcept { return row * columns + column; }
  };

  namespace Internal
  {
    template <typename T> struct matrix_traits
    {
    private:
      template <typename U> static std::false_type has_value_type_check(...) { return std::false_type(); }
      template <typename U> static std::true_type  has_value_type_check(typename U::value_type) { return std::true_type(); }

      template <typename U> static decltype(has_value_type_check<U>(std::declval<typename U::value_type>())) has_value_type_check(nullptr_t)
      {
        using ret_type = decltype(has_value_type_check<U>(std::declval<typename U::value_type>()));
        return ret_type;
      };

      using has_value_type_type = decltype(has_value_type_check<T>(nullptr));

      template <typename U> static std::false_type has_matrix_size_check(...) { return std::false_type(); }
      template <typename U> static std::true_type  has_matrix_size_check(index_t, index_t) { return std::true_type(); }

      template <typename U> static decltype(has_matrix_size_check<U>(U::matrix_size::rows, U::matrix_size::columns)) has_matrix_size_check(nullptr_t)
      {
        using ret_type = decltype(has_matrix_size_check<U>(U::matrix_size::rows, U::matrix_size::columns));
        return ret_type;
      };

      using has_matrix_size_type = decltype(has_matrix_size_check<T>(nullptr));

      template <typename U> static std::false_type has_getter_check(...) { return std::false_type(); }

      template <typename U,
                typename Ret = decltype(std::declval<U const&>()(std::declval<index_t const&>(), std::declval<index_t const&>())),
                typename     = std::enable_if_t<std::is_same_v<Ret, U::value_type const&> || std::is_same_v<Ret, U::value_type>>>
      static std::true_type has_getter_check(std::nullptr_t)
      {
        return std::true_type();
      }

      using has_getter_type = decltype(has_getter_check<T>(nullptr));

      template <typename U> static std::false_type has_setter_check(...) { return std::false_type(); }

      template <typename U,
                typename Ret = decltype(std::declval<U&>()(std::declval<index_t const&>(), std::declval<index_t const&>())),
                typename     = std::enable_if_t<std::is_same_v<Ret, U::value_type&>>>
      static std::true_type has_setter_check(std::nullptr_t)
      {
        return std::true_type();
      }

      using has_setter_type = decltype(has_setter_check<T>(nullptr));

    public:
      static constexpr bool has_value_type  = has_value_type_type::value;
      static constexpr bool has_matrix_size = has_matrix_size_type::value;
      static constexpr bool has_getter      = has_getter_type::value;
      static constexpr bool has_setter      = has_setter_type::value;

      static constexpr bool is_r_matrix  = has_value_type && has_matrix_size && has_getter;
      static constexpr bool is_rw_matrix = is_r_matrix && has_setter;

      static constexpr bool is_matrix_scalar = is_r_matrix && false;
      //&&T::matrix_size::rows == 1 && T::matrix_size::columns == 1;
    };

    template <typename T> constexpr bool is_r_matrix_v  = Internal::matrix_traits<T>::is_r_matrix;
    template <typename T> constexpr bool is_rw_matrix_v = Internal::matrix_traits<T>::is_rw_matrix;
    template <typename T> constexpr bool is_matrix_scalar_v = Internal::matrix_traits<T>::is_matrix_scalar;
    template <typename T> constexpr bool is_scalar_v    = std::is_integral_v<T> || std::is_floating_point_v<T>;

    template <typename T1, typename T2> constexpr bool is_same_size_matrix_v = is_r_matrix_v<T1> && is_r_matrix_v<T2> && T1::matrix_size::rows == T2::matrix_size::rows&& T1::matrix_size::columns == T2::matrix_size::columns;

    template <typename T1, typename T2> struct type_promotion_traits
    {
    private:
      template <typename U1, typename U2>
      static std::enable_if_t<matrix_traits<U1>::has_value_type && matrix_traits<U2>::has_value_type,
                              decltype(std::declval<U1::value_type>() + std::declval<U2::value_type>())>
      promotion_check(U1 const& u1, U2 const& u2)
      {
        return u1 + u2;
      }

      template <typename U1, typename U2>
      static std::enable_if_t<!matrix_traits<U1>::has_value_type && matrix_traits<U2>::has_value_type &&
                                  (std::is_floating_point_v<U1> || std::is_integral_v<U1>),
                              decltype(std::declval<U1>() + std::declval<U2::value_type>())>
      promotion_check(U1 const& u1, U2 const& u2)
      {
        return u1 + u2;
      }

      template <typename U1, typename U2>
      static std::enable_if_t<matrix_traits<U1>::has_value_type && !matrix_traits<U2>::has_value_type &&
                                  (std::is_floating_point_v<U2> || std::is_integral_v<U2>),
                              decltype(std::declval<U1::value_type>() + std::declval<U2>())>
      promotion_check(U1 const& u1, U2 const& u2)
      {
        return u1 + u2;
      }

      template <typename U1, typename U2>
      static std::enable_if_t<!matrix_traits<U1>::has_value_type && !matrix_traits<U2>::has_value_type &&
                                  (std::is_floating_point_v<U1> || std::is_integral_v<U1>)&&(std::is_floating_point_v<U2> || std::is_integral_v<U2>),
                              decltype(std::declval<U1>() + std::declval<U2>())>
      promotion_check(U1 const& u1, U2 const& u2)
      {
        return u1 + u2;
      }

    public:
      using value_type = decltype(promotion_check(std::declval<T1 const&>(), std::declval<T2 const&>()));
    };

    template <typename T1, typename T2> using type_promotion_t = typename Internal::type_promotion_traits<T1, T2>::value_type;
  }    // namespace Internal

  namespace Internal
  {
    template <typename LHS, typename RHS> void assign(LHS& lhs, RHS const& rhs)
    {
      for (index_t r = 0; r < LHS::matrix_size::rows; r++)
      {
        for (index_t c = 0; c < LHS::matrix_size::columns; c++)
        {
          lhs(r, c) = rhs(r, c);
        }
      }
    }
  }    // namespace Internal

  template <typename T, index_t rows, index_t columns = rows> class unit_matrix_t
  {
  public:
    using value_type  = std::remove_cv_t<T>;
    using matrix_size = matrix_size_t<rows, columns>;

    constexpr value_type operator()(index_t const& row, index_t const& column) const& noexcept
    {
      if (row == column)
        return static_cast<value_type>(1);
      return static_cast<value_type>(0);
    }
  };

  template <typename T, index_t rows, index_t columns = rows> class matrix_t
  {
  public:
    using value_type  = std::remove_cv_t<T>;
    using matrix_size = matrix_size_t<rows, columns>;

    constexpr matrix_t() = default;

    template <typename U, typename = std::enable_if_t<Internal::is_r_matrix_v<U>>> constexpr matrix_t(U const& val) { Internal::assign(*this, val); }

    template <typename... Us>
    constexpr matrix_t(Us... args)
        : m_elements{ args... }
    {
    }

    constexpr value_type const& operator()(index_t const& row, index_t const& column) const& noexcept
    {
      return this->m_elements[matrix_size::calc_idx(row, column)];
    }

    constexpr value_type& operator()(index_t const& row, index_t const& column) & noexcept { return this->m_elements[matrix_size::calc_idx(row, column)]; }

  private:
    value_type m_elements[matrix_size::elements] = {};
  };

}    // namespace ExMath

template <typename LHS, typename RHS>
constexpr std::enable_if_t<ExMath::Internal::is_same_size_matrix_v<LHS, RHS>,
                           ExMath::matrix_t<ExMath::Internal::type_promotion_t<LHS, RHS>, LHS::matrix_size::rows, LHS::matrix_size::columns>>
operator+(LHS const&, RHS const&);

#endif
