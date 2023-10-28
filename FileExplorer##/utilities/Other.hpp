#pragma once
#include<vector>
template<class T>
inline bool par_recommended(const std::vector<T>& vec)
{
	if (sizeof(T) * vec.size() > (static_cast<intmax_t>(100'000) * 32))
		return true;
	return false;
}