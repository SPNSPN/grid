#include <vector>
#include <numeric>
#include <tuple>
#include <cfloat>
#include <functional>
#include "util.h"

template<typename T>
class GridBase
{
	public:
		std::vector<size_t> shape;
		std::vector<std::vector<double>> coordinate;
		std::vector<T> data;

		GridBase (const std::vector<size_t> &shape_);
		GridBase (const std::vector<size_t> &shape_
				, const std::vector<double> &st
				, const std::vector<double> &ed);
		GridBase (const std::vector<size_t> &shape_
				, const std::vector<double> &st
				, const std::vector<double> &ed
				, const T &default_value);


		void map (const std::function<T(const std::vector<double>&)> &fn);

		std::vector<size_t> nearest_idx (const std::vector<double> &pt);
		bool inc_iter (std::vector<size_t> &iter);
};


template<typename T>
GridBase<T>::GridBase (const std::vector<size_t> &shape_)
	: shape(shape_)
	  , coordinate()
	  , data()
{
	for (auto s = this->shape.begin(); s != this->shape.end(); ++s) { assert(*s > 1UL); }

	size_t size = util::reduce<size_t, size_t>([](const size_t &acc, const size_t &s)
			{
				return acc * s;
			}, this->shape, 1UL);

	coordinate.resize(size);
	data.resize(size);
}

template<typename T>
GridBase<T>::GridBase (const std::vector<size_t> &shape_
				, const std::vector<double> &st
				, const std::vector<double> &ed)
	: GridBase(shape_)
{
	std::vector<size_t> iter(this->shape.size(), 0);
	for (size_t idx = 0; true; ++idx)
	{
		std::vector<double> pt = util::map<size_t, double>([=, *this](const size_t &i)
				{
					return (ed.at(i) - st.at(i))
							* static_cast<double>(iter.at(i))
							/ (this->shape.at(i) - 1UL) + st.at(i);
				}, util::range(0UL, iter.size()));
		this->coordinate.at(idx) = pt;
		if (! this->inc_iter(iter)) { break; }
	}
}

template<typename T>
GridBase<T>::GridBase (const std::vector<size_t> &shape_
				, const std::vector<double> &st
				, const std::vector<double> &ed
				, const T &default_value)
	: GridBase(shape_, st, ed)
{
	for (size_t idx = 0; idx < this->data.size(); ++idx)
	{
		this->data.at(idx) = default_value;
	}
}



template<typename T>
void GridBase<T>::map (const std::function<T(const std::vector<double>&)> &fn)
{
	for (size_t i = 0; i < this->data.size(); ++i)
	{
		this->data.at(i) = fn(this->coordinate.at(i));
	}
}


template<typename T>
std::vector<size_t> GridBase<T>::nearest_idx (const std::vector<double> &pt)
{
	std::vector<size_t> iter(this->shape.size(), 0UL);
	std::vector<size_t> min_iter(this->shape.size(), 0UL);
	double min = DBL_MAX;
	for (size_t i = 0; i < this->coordinate.size(); ++i)
	{
		double dist = util::distance(pt, this->coordinate.at(i));
		if (dist < min)
		{
			min = dist;
			min_iter = iter;
		}
		this->inc_iter(iter);
	}
	return min_iter;
}

template<typename T>
bool GridBase<T>::inc_iter (std::vector<size_t> &iter)
{
	for (size_t degree = 0; degree < this->shape.size(); ++degree)
	{
		if (iter.at(degree) < this->shape.at(degree) - 1)
		{
			iter.at(degree) += 1;
			return true;
		}
		iter.at(degree) = 0;
	}
	return false;
}


template<typename T>
class Grid : public GridBase<T>
{
	public:
		Grid (const std::vector<size_t> &shape_) : GridBase<T>(shape_) {};
		Grid (const std::vector<size_t> &shape_
			, const std::vector<double> &st
			, const std::vector<double> &ed) : GridBase<T>(shape_, st, ed) {};
		Grid (const std::vector<size_t> &shape_
			, const std::vector<double> &st
			, const std::vector<double> &ed
			, const T &default_value) : GridBase<T>(shape_, st, ed, default_value) {};

		std::pair<std::vector<double>&, T&> at (const std::vector<size_t> &idxs)
		{
			// TODO idxを直接計算するように修正
			std::vector<size_t> iter(this->shape.size(), 0UL);
			for (size_t i = 0; true; ++i)
			{
				if (iter == idxs)
				{
					return std::pair<std::vector<double>&, T&>(this->coordinate.at(i), this->data.at(i));
				}
				if (! this->inc_iter(iter)) { break; }
			}
			assert(false);
		}
};


template<>
class Grid<bool> : public GridBase<bool>
{
	public:
		Grid (const std::vector<size_t> &shape_) : GridBase<bool>(shape_) {};
		Grid (const std::vector<size_t> &shape_
			, const std::vector<double> &st
			, const std::vector<double> &ed) : GridBase<bool>(shape_, st, ed) {};
		Grid (const std::vector<size_t> &shape_
			, const std::vector<double> &st
			, const std::vector<double> &ed
			, const bool default_value) : GridBase<bool>(shape_, st, ed, default_value) {};

		std::pair<std::vector<double>&, std::_Bit_reference> at (
				const std::vector<size_t> &idxs)
		{
			// TODO idxを直接計算するように修正
			std::vector<size_t> iter(this->shape.size(), 0UL);
			for (size_t i = 0; true; ++i)
			{
				if (iter == idxs)
				{
					return std::pair<std::vector<double>&, std::_Bit_reference>(this->coordinate.at(i), this->data.at(i));
				}
				if (! this->inc_iter(iter)) { break; }
			}
			assert(false);
		}
};

