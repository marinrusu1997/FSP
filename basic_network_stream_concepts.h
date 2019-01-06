#pragma once
#include <type_traits>
#include <utility>
#include <boost/system/error_code.hpp>

template<class Derived, typename = void>
struct implements_write : std::false_type {};

template<class Derived>
struct implements_write <Derived, std::void_t <
	decltype (
		&Derived::write_guard,
		&Derived::write_action,
		&Derived::write_error,
			(void)0)
	>> : std::true_type {};

template <class Derived, class = void>
struct implements_read : std::false_type {};

template<class Derived>
struct implements_read <Derived, std::void_t <
	decltype(
		&Derived::read_guard,
		&Derived::read_prepare,
		&Derived::read_action,
		&Derived::read_indeterminate,
		&Derived::read_done,
		&Derived::read_error,
			(void)0)
	>> : std::true_type{};