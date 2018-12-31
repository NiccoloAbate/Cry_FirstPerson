#pragma once

template<typename T>
struct Is_Constructed_Template_Class
{
	/* Has destructor :) */
	template <typename A>
	static std::true_type test(decltype(std::declval<A>())) {
		return std::true_type();
	}

	/* Has no destructor :( */
	template<typename A>
	static std::false_type test(...) {
		return std::false_type();
	}

	/* This will be either `std::true_type` or `std::false_type` */
	typedef decltype(test<T>(0)) type;

	static const bool value = type::value; /* Which is it? */
};


template <int K, typename T, int I, bool END>
struct _Static_TypeId_
{
	static constexpr int ID = _Static_TypeId_<K, T, I+1, true>::ID;
};

template <int K, typename T, int I>
struct _Static_TypeId_<K, T, I, true>
{
	static constexpr int ID = I;
};

template <int K, typename T>
struct Static_TypeId
{
	static constexpr int ID = _Static_TypeId_<K, T, 1, false>::ID;
};