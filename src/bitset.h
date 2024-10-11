#ifndef _BITSET_H_
#define _BITSET_H_

#include <iostream>
#include <vector>

using namespace std;



// 位图
template <size_t N>
class bitset{
    public:
		bitset(){  // 全部位置0
			_bits.resize(N/8 + 1, 0);
		}
		
		void set(size_t x){  // 将某位置1
			size_t i = x / 8;
			size_t j = x % 8;
			_bits[i] |= (1 << j);
		}

		void reset(size_t x){  // 将某位置0
			size_t i = x / 8;
			size_t j = x % 8;
			_bits[i] &= ~(1 << j);
		}

		bool isExists(size_t x){  // 判断是否存在
			size_t i = x / 8;
			size_t j = x % 8;
			return _bits[i] & (1 << j);
		}
    private:
        vector<char> _bits;  // char占8位
};


#endif