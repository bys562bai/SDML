#pragma once
#include <regex>
#include <vector>
#include <algorithm>
#include <iostream>

#include "BufRef.h"

namespace sdml{

	using namespace std;
	template <typename T, typename Alloc = std::allocator<T> >
	struct Buffer : std::vector<T, Alloc> {
		using Base = std::vector<T, Alloc>;
		using Base::Base;
		using Base::operator =;
		using This = Buffer<T>;

		Buffer(const BufRef<T>& s) : Base(s.cbegin(), s.cend()) {}
		This& operator = (const BufRef<T>& s) {
			~();
			new (this) (s);
		}

		auto slice() const {
			return BufRef(data(), size());
		}

		auto slice(int a, int b) const {
			return slice().slice(a, b);
		}

		operator auto() const {
			return slice();
		}
	};

	template<typename T, typename Alloc>
	std::ostream& operator << (std::ostream& out, const Buffer<T, Alloc>& buf) {
		return out << buf.slice();
	}

}