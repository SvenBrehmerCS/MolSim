/**
 * @file Vec.hpp
 *
 * @brief This file defines the main vector functionality.
 *
 * This file defines the main vector functionality. This comprises basic arithmetic operations such as +, -, *, but also more advanced operations.
 */

#pragma once

#include <cmath>
#include <initializer_list>
#include <iostream>
#include <string>
#include <sstream>

/**
 * @brief Enable vector alignment.
 *
 * If this macro is defined, the vectors will be aligned. This gives the compiler the option to use simd and other optimizations more efficiently.
 */
#define VEC_ALIGN

// Define a local set of macros used for optimization
#if defined(__GNUG__)
/**
 * @brief This tag forces inlining.
 *
 * Every function with this tag will always be inlined if the compiler supports gnu extensions.
 */
#define ALWAYS_INLINE [[gnu::always_inline]]

/**
 * @brief This tag forces flattening.
 *
 * Every function with this tag will always be flattened if the compiler supports gnu extensions.
 */
#define FLATTEN [[gnu::flatten]]

/**
 * @brief This tag tells the compiler to handle this function as a hot function.
 *
 * This tag tells the compiler to handle this function as a hot function  if the compiler supports gnu extensions.
 */
#define HOT [[gnu::hot]]

/**
 * @brief This tag tells the compiler to handle this function as a cold function.
 *
 * This tag tells the compiler to handle this function as a cold function if the compiler supports gnu extensions.
 */
#define COLD [[gnu::cold]]

#ifdef VEC_ALIGN
/**
 * @brief This tag tells the compiler to align a variable.
 *
 * This tag tells the compiler to align the variable if the compiler supports gnu extensions.
 *
 * @param __ALIGN The specified alignment.
 */
#define ALIGN(__ALIGN) [[gnu::aligned(__ALIGN)]]
#else
/**
 * @brief This tag tells the compiler to align a variable.
 *
 * This tag tells the compiler to align the variable if the compiler supports gnu extensions.
 *
 * @param __ALIGN The specified alignment.
 */
#define ALIGN(__ALIGN)
#endif

#else
/**
 * @brief This tag forces inlining.
 *
 * Every function with this tag will always be inlined if the compiler supports gnu extensions.
 */
#define ALWAYS_INLINE

/**
 * @brief This tag forces flattening.
 *
 * Every function with this tag will always be flattened if the compiler supports gnu extensions.
 */
#define FLATTEN

/**
 * @brief This tag tells the compiler to handle this function as a hot function.
 *
 * This tag tells the compiler to handle this function as a hot function  if the compiler supports gnu extensions.
 */
#define HOT

/**
 * @brief This tag tells the compiler to handle this function as a cold function.
 *
 * This tag tells the compiler to handle this function as a cold function  if the compiler supports gnu extensions.
 */
#define COLD

/**
 * @brief This tag tells the compiler to align a variable.
 *
 * This tag tells the compiler to align the variable  if the compiler supports gnu extensions.
 *
 * @param __ALIGN The specified alignment.
 */
#define ALIGN(__ALIGN)
#endif

/**
 * @class Vec
 *
 * @brief This class defines a 3D vector.
 *
 * This vector can be used to simplify calculations, by providing multiple well tested utility functions.
 *
 * @tparam number_t The template number type. It must either be float, double or long double.
 */
template <typename number_t = double> class Vec final {
private:
    /**
     * @brief Create a zero value.
     *
     * This is a compile time constant, created with a high precision.
     */
    static constexpr number_t zero = static_cast<number_t>(0.0L);

    /**
     * @brief Create a one value.
     *
     * This is a compile time constant, created with a high precision.
     */
    static constexpr number_t one = static_cast<number_t>(1.0L);

    /**
     * @typedef vec_t
     *
     * @brief Define the vector type with the given numerical precision.
     *
     * Define the current vector type. THis type is a vector with the numerical precision defined by number_t.
     */
    typedef Vec<number_t> vec_t;

public:
    /**
     * @brief Define the x component of the vector.
     *
     * The x component of the vector.
     */
    ALIGN(sizeof(number_t) * 4) number_t x;

    /**
     * @brief Define the y component of the vector.
     *
     * The y component of the vector.
     */
    number_t y;

    /**
     * @brief Define the z component of the vector.
     *
     * The z component of the vector.
     */
    number_t z;

    /**
     * @brief Create a vector without any arguments.
     *
     * Create a vector without any arguments. The vector will be a unit vector.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr Vec() noexcept(true)
        : x(zero)
        , y(zero)
        , z(zero) { }

    /**
     * @brief Create a vector with a x component.
     *
     * Initialize the vector with the given x component. The y and z components will be initialized to zero.
     *
     * @param __x The x component of the vector.
     */
    HOT FLATTEN ALWAYS_INLINE inline explicit constexpr Vec(const number_t __x) noexcept(true)
        : x(__x)
        , y(zero)
        , z(zero) { }
    /**
     * @brief Create a vector with a x and y component.
     *
     * Initialize the vector with the given x and y component. The z component will be initialized to zero.
     *
     * @param __x The x component of the vector.
     * @param __y The y component of the vector.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr Vec(const number_t __x, const number_t __y) noexcept(true)
        : x(__x)
        , y(__y)
        , z(zero) { }

    /**
     * @brief Create a vector with a x and y component.
     *
     * Initialize the vector with the given x, y and z component.
     *
     * @param __x The x component of the vector.
     * @param __y The y component of the vector.
     * @param __z The z component of the vector.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr Vec(const number_t __x, const number_t __y, const number_t __z) noexcept(true)
        : x(__x)
        , y(__y)
        , z(__z) { }

    /**
     * @brief Create a vector from an initializer list.
     *
     * Create a vector from an initializer list. If a dimension is not provided it will be set to zero.
     *
     * @param __init The initializer list that should be used for initialization.
     *
     * @throws std::invalid_argument If the initializer list is too long.
     */
    HOT FLATTEN inline constexpr Vec(std::initializer_list<number_t> __init) noexcept(false) {
        x = zero;
        y = zero;
        z = zero;

        if (__init.size() > 3) [[unlikely]] {
            throw std::invalid_argument("The size of the initializer list was too long.");
        }

        if (__init.size() > 0) {
            x = *(__init.begin());
        }

        if (__init.size() > 1) {
            y = *(__init.begin() + 1);
        }

        if (__init.size() > 2) {
            z = *(__init.begin() + 2);
        }
    }

    /**
     * @brief Define a copy constructor for a vector.
     *
     * This constructor copys a vector into another one.
     *
     * @param __v The vector that should be copied.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr Vec(const vec_t& __restrict__ __v) noexcept(true) = default;

    /**
     * @brief Define a default constructor for a vector.
     *
     * This destructor destroys a vector element. It is a default constructor and has no site effects.
     */
    HOT FLATTEN ALWAYS_INLINE inline ~Vec() noexcept(true) = default;

    /**
     * @brief Get the x component of the vector.
     *
     * This method returns the x component of the vector.
     *
     * @return The x component of the vector.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr number_t get_x() const noexcept(true) { return x; }

    /**
     * @brief Get the y component of the vector.
     *
     * This method returns the y component of the vector.
     *
     * @return The y component of the vector.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr number_t get_y() const noexcept(true) { return y; }

    /**
     * @brief Get the z component of the vector.
     *
     * This method returns the z component of the vector.
     *
     * @return The z component of the vector.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr number_t get_z() const noexcept(true) { return z; }

    /**
     * @brief Set the x component of a vector.
     *
     * This method updates the x component of a vector.
     *
     * @param __x The new x component.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr void set_x(const number_t __x) noexcept(true) { x = __x; }

    /**
     * @brief Set the y component of a vector.
     *
     * This method updates the y component of a vector.
     *
     * @param __y The new y component.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr void set_y(const number_t __y) noexcept(true) { y = __y; }

    /**
     * @brief Set the z component of a vector.
     *
     * This method updates the z component of a vector.
     *
     * @param __z The new z component.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr void set_z(const number_t __z) noexcept(true) { z = __z; }

    /**
     * @brief Get the element of a vector at a given component.
     *
     * This method returns the indexed component as a reference.
     *
     * @param __idx The index of the component.
     *
     * @return A reference to the indexed element.
     *
     * @throws std::invalid_argument If the index is larger than 2.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline number_t& operator[](const size_t __idx) noexcept(false) {
        if (__idx == 0) {
            return x;
        } else if (__idx == 1) {
            return y;
        } else if (__idx == 2) {
            return z;
        } else [[unlikely]] {
            throw std::invalid_argument("Tried to access a vector at an illegal index.");
        }
    }

    /**
     * @brief Get the element of a vector at a given component.
     *
     * This method returns the indexed component as a reference.
     *
     * @param __idx The index of the component.
     *
     * @return A reference to the indexed element.
     *
     * @throws std::invalid_argument If the index is larger than 2.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr const number_t& operator[](const size_t __idx) const noexcept(false) {
        if (__idx == 0) {
            return x;
        } else if (__idx == 1) {
            return y;
        } else if (__idx == 2) {
            return z;
        } else [[unlikely]] {
            throw std::invalid_argument("Tried to access a vector at an illegal index.");
        }
    }

    /**
     * @brief Test if two vectors are equal.
     *
     * This method tests if this vector equals another.
     *
     * @param __v The vector that should be tested.
     *
     * @return A boolean indicating if the two vectors are equal.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr bool equal(const vec_t& __restrict__ __v) const noexcept(true) {
        return __v.x == x && __v.y == y && __v.z == z;
    }

    /**
     * @brief Test if two vectors are different.
     *
     * This method tests if this vector differs from another.
     *
     * @param __v The vector that should be tested.
     *
     * @return A boolean indicating if the two vectors differ.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr bool not_equal(const vec_t& __restrict__ __v) const noexcept(true) {
        return __v.x != x || __v.y != y || __v.z != z;
    }

    /**
     * @brief Get the string representation of the vector.
     *
     * Get the string representation of the vector. The representation of the vector is (x, y, z).
     *
     * @return The string representation of the vector.
     */
    COLD [[nodiscard]] std::string to_string() const noexcept(true) {
        std::ostringstream str_stream;

        str_stream << "(" << x << ", " << y << ", " << z << ")";

        return str_stream.str();
    }

    /**
     * @brief Add another vector to one self.
     *
     * This method adds another vector to one self.
     *
     * @param __v The other vector.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr void add(const vec_t& __restrict__ __v) noexcept(true) {
        x += __v.x;
        y += __v.y;
        z += __v.z;
    }

    /**
     * @brief Add another vector to one self.
     *
     * This method adds another vector to one self.
     *
     * @param __v The other vector.
     *
     * @return A reference to one self.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr vec_t& operator+=(const vec_t& __restrict__ __v) noexcept(true) {
        x += __v.x;
        y += __v.y;
        z += __v.z;

        return this;
    }

    /**
     * @brief Subtract another vector from one self.
     *
     * This method subtracts another vector from one self.
     *
     * @param __v The other vector.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr void sub(const vec_t& __restrict__ __v) noexcept(true) {
        x -= __v.x;
        y -= __v.y;
        z -= __v.z;
    }

    /**
     * @brief Subtract another vector from one self.
     *
     * This method subtracts another vector from one self.
     *
     * @param __v The other vector.
     *
     * @return A reference to one self.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr vec_t& operator-=(const vec_t& __restrict__ __v) noexcept(true) {
        x -= __v.x;
        y -= __v.y;
        z -= __v.z;

        return this;
    }

    /**
     * @brief Scale one self by alpha.
     *
     * This method scales one self by alpha.
     *
     * @param __a The alpha value.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr void scale(const number_t __a) noexcept(true) {
        x *= __a;
        y *= __a;
        z *= __a;
    }

    /**
     * @brief Scale one self by alpha.
     *
     * This method scales one self by alpha.
     *
     * @param __a The alpha value.
     *
     * @return A reference to one self.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr vec_t& operator*=(const number_t __a) noexcept(true) {
        x *= __a;
        y *= __a;
        z *= __a;

        return this;
    }

    /**
     * @brief Compute the dot product of one self with anothere vector.
     *
     * This method computes the dot product of one self with another vector.
     *
     * @param __v The other vector.
     *
     * @return The dot product.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr number_t dot(const vec_t& __restrict__ __v) const noexcept(true) {
        return x * __v.x + y * __v.y + z * __v.z;
    }

    /**
     * @brief Compute the cross product of one self with another vector.
     *
     * This method computes the cross product of one self with another vector.
     *
     * @param __v The other vector.
     *
     * @return The resulting vector.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr vec_t cross(const vec_t& __restrict__ __v) const noexcept(true) {
        return Vec<number_t>(y * __v.z - z * __v.y, z * __v.x - x * __v.z, x * __v.y - y * __v.x);
    }

    /**
     * @brief Compute the euclidean length of a vector.
     *
     * This method returns the euclidean length (L2 norm) of a vector.
     *
     * @return The euclidean length of the vector.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr number_t len() const noexcept(true) { return std::sqrt(x * x + y * y + z * z); }

    /**
     * @brief Compute the squared euclidean length of a vector.
     *
     * This method returns the squared euclidean length of a vector.
     *
     * @return The squared euclidean length of the vector.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr number_t len_squ() const noexcept(true) { return std::fma(x, x, std::fma(y, y, z * z)); }

    /**
     * @brief Compute the L1 norm of a vector.
     *
     * This method returns the L1 norm of a vector.
     *
     * @return The L1 norm of the vector.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr number_t l_1_norm() const noexcept(true) {
        return std::abs(x) + std::abs(y) + std::abs(z);
    }

    /**
     * @brief Compute the L2 norm of a vector.
     *
     * This method returns the L2 norm of a vector.
     *
     * @return The L2 norm of the vector.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr number_t l_2_norm() const noexcept(true) { return std::sqrt(x * x + y * y + z * z); }

    /**
     * @brief Compute the L inf norm of a vector.
     *
     * This method returns the L inf norm of a vector.
     *
     * @return The L inf norm of the vector.
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr number_t l_inf_norm() const noexcept(true) {
        return std::max(std::max(std::abs(x), std::abs(y)), std::abs(z));
    }

    /**
     * @brief Norm a vector.
     *
     * This method norms the vector so that it has the euclidean length of one.
     */
    HOT FLATTEN ALWAYS_INLINE inline constexpr void norm() noexcept(false) {
        const number_t length = len();

        if (length <= zero) [[unlikely]] {
            throw std::invalid_argument("Tried to call norm on a zero vector.");
        }

        const number_t len_inv = one / length;

        x *= len_inv;
        y *= len_inv;
        z *= len_inv;
    }

    /**
     * @brief Compute the distance to another vector.
     *
     * This method computs the euclidean distance to another vector. This is equivalent to calculating l_2_norm(this - __v).
     *
     * @param __v The other vector.
     *
     * @return The distance
     */
    HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr number_t dist_to(const vec_t __restrict__& __v) const noexcept(true) {
        return l_2_norm(this - __v);
    }
};

/**
 * @brief Test if two vectors are equal.
 *
 * This method tests if two vectors are equal.
 *
 * @param __lhs The vector on the left site of the comparison.
 * @param __rhs The vector on the right site of the comparison.
 *
 * @tparam number_t The numerical precision of the vector.
 *
 * @return A boolean indicating if both vectors are equal.
 */
template <typename number_t = double>
HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr bool operator==(
    const Vec<number_t>& __restrict__ __lhs, const Vec<number_t>& __restrict__ __rhs) noexcept(true) {
    return __lhs.x == __rhs.x && __lhs.y == __rhs.y && __lhs.z == __rhs.z;
}

/**
 * @brief Test if two vectors are different.
 *
 * This method tests if two vectors are different.
 *
 * @param __lhs The vector on the left site of the comparison.
 * @param __rhs The vector on the right site of the comparison.
 *
 * @tparam number_t The numerical precision of the vector.
 *
 * @return A boolean indicating if both vectors are equal.
 */
template <typename number_t = double>
HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr bool operator!=(
    const Vec<number_t>& __restrict__ __lhs, const Vec<number_t>& __restrict__ __rhs) noexcept(true) {
    return __lhs.x != __rhs.x || __lhs.y != __rhs.y || __lhs.z != __rhs.z;
}

/**
 * @brief Add two vectors.
 *
 * This method adds two vectors to each other.
 *
 * @param __lhs The vector on the left site of the addition.
 * @param __rhs The vector on the right site of the addition.
 *
 * @tparam number_t The numerical precision of the vector.
 *
 * @return The sum of both vectors.
 */
template <typename number_t = double>
HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr Vec<number_t> operator+(
    const Vec<number_t>& __restrict__ __lhs, const Vec<number_t>& __restrict__ __rhs) noexcept(true) {
    return Vec<number_t>(__lhs.x + __rhs.x, __lhs.y + __rhs.y, __lhs.z + __rhs.z);
}

/**
 * @brief Compute the difference of two vectors.
 *
 * This method computes the difference of two vectors.
 *
 * @param __rhs The vector that should be inverted.
 *
 * @tparam number_t The numerical precision of the vector.
 *
 * @return The inverted vectors.
 */
template <typename number_t = double>
HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr Vec<number_t> operator-(
    const Vec<number_t>& __restrict__ __lhs, const Vec<number_t>& __restrict__ __rhs) noexcept(true) {
    return Vec<number_t>(__lhs.x - __rhs.x, __lhs.y - __rhs.y, __lhs.z - __rhs.z);
}

/**
 * @brief Compute the negative of a vector.
 *
 * This method computes the negative of a vector.
 *
 * @param __rhs The vector on the right site of the subtraction.
 *
 * @tparam number_t The numerical precision of the vector.
 *
 * @return The negative of the vector.
 */
template <typename number_t = double>
HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr Vec<number_t> operator-(const Vec<number_t>& __restrict__ __rhs) noexcept(true) {
    return Vec<number_t>(-__rhs.x, -__rhs.y, -__rhs.z);
}

/**
 * @brief Compute the cross products of two vectors.
 *
 * This method computes the cross product of two vectors.
 *
 * @param __lhs The vector on the left site of the cross product.
 * @param __rhs The vector on the right site of the cross product.
 *
 * @tparam number_t The numerical precision of the vector.
 *
 * @return The cross product of both vectors.
 */
template <typename number_t = double>
HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr Vec<number_t> operator*(
    const Vec<number_t>& __restrict__ __lhs, const Vec<number_t>& __restrict__ __rhs) noexcept(true) {
    return Vec<number_t>(__lhs.y * __rhs.z - __lhs.z * __rhs.y, __lhs.z * __rhs.x - __lhs.x * __rhs.z, __lhs.x * __rhs.y - __lhs.y * __rhs.x);
}

/**
 * @brief Scale a vector.
 *
 * This method scales a vectors.
 *
 * @param __lhs The vector that should be scales.
 * @param __rhs The factor by which the vector should be scaled.
 *
 * @tparam number_t The numerical precision of the vector.
 *
 * @return The cross product of both vectors.
 */
template <typename number_t = double>
HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr Vec<number_t> operator*(
    const Vec<number_t>& __restrict__ __lhs, const number_t __restrict__ __rhs) noexcept(true) {
    return Vec<number_t>(__lhs.x * __rhs, __lhs.y * __rhs, __lhs.z * __rhs);
}

/**
 * @brief Scale a vector.
 *
 * This method scales a vectors.
 *
 * @param __lhs The factor by which the vector should be scaled.
 * @param __rhs The vector that should be scales.
 *
 * @tparam number_t The numerical precision of the vector.
 *
 * @return The cross product of both vectors.
 */
template <typename number_t = double>
HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr Vec<number_t> operator*(
    const number_t& __restrict__ __lhs, const Vec<number_t>& __restrict__ __rhs) noexcept(true) {
    return Vec<number_t>(__lhs * __rhs.x, __lhs * __rhs.y, __lhs * __rhs.z);
}

/**
 * @brief Print a vector.
 *
 * This method prints a vector to the provided output stream.
 *
 * @param __lhs The output stream.
 * @param __rhs The vector that should be printed.
 *
 * @tparam number_t The numerical precision of the vector.
 *
 * @return The updated output stream.
 */
template <typename number_t = double>
COLD inline std::ostream& operator<<(std::ostream& __restrict__ __lhs, const Vec<number_t>& __restrict__ __rhs) noexcept(true) {
    return __lhs << "(" << __rhs.x << ", " << __rhs.y << ", " << __rhs.z << ")";
}

/**
 * @brief Compute the norm of a vector.
 *
 * This method computes the norm of a vector.
 *
 * @param __rhs The vector that should be normalized.
 *
 * @tparam number_t The numerical precision of the vector.
 *
 * @return The norm of the vector.
 */
template <typename number_t = double>
HOT FLATTEN ALWAYS_INLINE [[nodiscard]] inline constexpr Vec<number_t> operator~(const Vec<number_t>& __restrict__ __rhs) noexcept(false) {
    constexpr number_t zero = static_cast<number_t>(0.0L);
    constexpr number_t one = static_cast<number_t>(1.0L);

    const number_t length = __rhs.len();

    if (length <= zero) [[unlikely]] {
        throw std::invalid_argument("Tried to call norm on a zero vector.");
    }

    const number_t len_inv = one / length;

    return Vec<number_t>(__rhs.x * len_inv, __rhs.y * len_inv, __rhs.z * len_inv);
}
