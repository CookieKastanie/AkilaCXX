#pragma once

#include <vector>

namespace Akila {
	template<typename T>
	class Collection {
	private:
		using vec_t = std::vector<T>;
		vec_t datas;

	public:
		void add(const T& data) { datas.push_back(data); }
		void remove(unsigned int index) {
			const int &&size = datas.size();
			if(size > 0 && index != (size - 1)) datas[index] = std::move(datas.back());
			datas.pop_back();
		}
		void clear() { datas.clear(); }

		using iterator = typename vec_t::iterator;
		using const_iterator = typename vec_t::const_iterator;

		iterator begin() { return datas.begin(); }
		iterator end() { return datas.end(); }
		const_iterator begin() const { return datas.begin(); }
		const_iterator end() const { return datas.end(); }
		const_iterator cbegin() const { return datas.cbegin(); }
		const_iterator cend() const { return datas.cend(); }
	};
}
