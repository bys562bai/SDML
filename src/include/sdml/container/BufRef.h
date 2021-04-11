#pragma once

#include <stdint.h>
#include <compare>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <stdexcept>
#include <concepts>
#include <type_traits>
#include <fmt/format.h>


namespace sdml
{


    template <typename T>
    struct BufRef {
        using This = BufRef<T>;

        BufRef() {}

        BufRef(T* data, size_t size) {
            if (data == nullptr) {
                return;
            }
            _data = data;
            _size = size;
        }

        template<typename U> requires
            std::assignable_from<T, U>
        BufRef(const U& x) {
            return *this = x;
        }

        template<typename U> requires
            std::assignable_from<T*&, decltype(std::declval<U>().data())>
        BufRef<T>& operator = (U& x) {
            _data = x.data();
            _size = x.size();
            return *this;
        }


        T* data() const { return _data; }
        size_t size() const { return _size; }
        bool empty() const { return !_size; }

        T& operator[] (size_t i) {
            return _data[i];
        }

        //允许负数索引
        //会对边界进行检测
        T& at(int64_t i) {
            if (i < 0) {
                i = _size + i;
                if (i < 0)
                    throw std::out_of_range("out of left bound");
            }
            else if (i >= _size)
                throw std::out_of_range("out of right bound");
            return _data[i];
        }

        This& slice() const {
            return *this;
        }

        This slice(int64_t a) const {
            return slice(a, _size);
        }

        This slice(int64_t a, int64_t b) const {
            if (a < 0)
                return This();
            if (b < 0) {
                b = _size + b;
                if (b < 0)
                    return This();
            }
            b = std::min((size_t)b, _size);
            if (a >= b)
                return This();
            return This(_data + a, b - a);
        }

        void copy_from(This x) {
            x = x.slice(0, x.size());
            std::copy(x.begin(), x.end(), begin());
        }

        T* begin() {
            return _data;
        }

        T* end() {
            return _data + _size;
        }

        const T* cbegin() const {
            return _data;
        }

        const T* cend() const {
            return _data + _size;
        }


        std::reverse_iterator<T*> rbegin() {
            return std::reverse_iterator<T*>(end());
        }

        std::reverse_iterator<T*> rend() {
            return std::reverse_iterator<T*>(begin());
        }

        std::reverse_iterator<const T*> crbegin() const {
            return std::reverse_iterator<const T*>(cend());
        }

        std::reverse_iterator<const T*> crend() const {
            return std::reverse_iterator<const T*>(cbegin());
        }

        //operator basic_string_view<decay_t<T> >() const{
        //    return basic_string_view<decay_t<T> >(_data, _size);
        //}

        bool operator == (const This& x) const {
            return *this <=> x == 0;
        }

        bool operator != (const This& x) const {
            return *this <=> x != 0;
        }

        bool operator < (const This& x) const {
            return *this <=> x < 0;
        }

        bool operator <= (const This& x) const {
            return *this <=> x <= 0;
        }

        bool operator > (const This& x) const {
            return *this <=> x > 0;
        }

        bool operator >= (const This& x) const {
            return *this <=> x >= 0;
        }

        std::weak_ordering operator <=> (const This& x) const {
            return std::lexicographical_compare_three_way(begin(), end(), x.begin(), x.end());
        }

        size_t find(T& x) {
            return std::find(beign(), end(), x) - _data;
        }

        size_t find(This& x) {
            return std::search(x.begin(), x.end(), beign(), end()) - _data;
        }

    private:
        T* _data = nullptr;
        size_t _size = 0;
    };


	template<typename T>
	std::string to_string(const BufRef<T>& s) {
		if (s.empty()) {
			return "[]";
		}
		else {
			return fmt::format("[{}]", fmt::join(s.cbegin(), s.cend(), ", "));
		}
	}

	template<typename T>
	std::ostream& operator << (std::ostream& out, const BufRef<T>& s) {
		return out << to_string(s);
	}
}