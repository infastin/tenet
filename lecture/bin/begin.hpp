#include <concepts>
#include <ranges>

namespace ranges {

namespace detail {
	template<typename T>
	concept __member_begin = requires(T &t) {
		{ t.begin() } -> std::input_or_output_iterator;
	};

	template<typename T>
	concept __adl_begin = requires(T &t) {
		{ begin(t) } -> std::input_or_output_iterator;
	};

	struct _Begin {
		template<typename T>
		requires (std::is_array_v<std::remove_reference<T>> && std::is_lvalue_reference_v<T>) 
			|| __member_begin<T> || __adl_begin<T>
		auto
		operator()(T &&t) const noexcept
		{
			if constexpr (std::is_array_v<std::remove_reference<T>>) {
				return t + 0;
			} else if constexpr (__member_begin<T>) {
				return t.begin();
			} else {
				return begin(t);
			}
		}
	};
}

inline namespace cust {
	inline constexpr detail::_Begin begin{};
}

}
