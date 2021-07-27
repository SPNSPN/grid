#include <gtest/gtest.h>
#include <cmath>
#include "grid.h"

class GridTest : public ::testing::Test {};

TEST_F (GridTest, bool_1d)
{
	Grid<bool> g(util::vec(5UL), util::vec(-10.0), util::vec(10.0), false);

	ASSERT_EQ(5UL, g.coordinate.size());
	ASSERT_EQ(5UL, g.data.size());
	
	EXPECT_EQ(util::vec(-10.0), g.at(util::vec(0UL)).first);
	EXPECT_EQ(util::vec(-5.0), g.at(util::vec(1UL)).first);
	EXPECT_EQ(util::vec(0.0), g.at(util::vec(2UL)).first);
	EXPECT_EQ(util::vec(5.0), g.at(util::vec(3UL)).first);
	EXPECT_EQ(util::vec(10.0), g.at(util::vec(4UL)).first);

	EXPECT_EQ(false, g.at(util::vec(0UL)).second);
	EXPECT_EQ(false, g.at(util::vec(1UL)).second);
	EXPECT_EQ(false, g.at(util::vec(2UL)).second);
	EXPECT_EQ(false, g.at(util::vec(3UL)).second);
	EXPECT_EQ(false, g.at(util::vec(4UL)).second);

	g.at(util::vec(1UL)).second = true;
	g.at(util::vec(4UL)).second = true;

	EXPECT_EQ(false, g.at(util::vec(0UL)).second);
	EXPECT_EQ(true, g.at(util::vec(1UL)).second);
	EXPECT_EQ(false, g.at(util::vec(2UL)).second);
	EXPECT_EQ(false, g.at(util::vec(3UL)).second);
	EXPECT_EQ(true, g.at(util::vec(4UL)).second);
}

TEST_F (GridTest, double_2d)
{
	Grid<double> g(util::vec(11UL, 21UL)
			, util::vec(-1.0, -1.0), util::vec(1.0, 1.0), 0.0);

	ASSERT_EQ(231UL, g.coordinate.size());
	ASSERT_EQ(231UL, g.data.size());

	g.map([](const std::vector<double> &p) -> double
			{
				return std::pow(p.at(0), 2.0) + std::pow(p.at(1), 2.0);
			});

	EXPECT_FLOAT_EQ(2.0, g.at(util::vec(0UL, 0UL)).second);
	EXPECT_FLOAT_EQ(1.0, g.at(util::vec(0UL, 10UL)).second);
	EXPECT_FLOAT_EQ(0.52, g.at(util::vec(8UL, 6UL)).second);

	EXPECT_EQ(util::vec(6UL, 2UL), g.nearest_idx(util::vec(0.15, -0.85)));
	EXPECT_EQ(util::vec(5UL, 0UL), g.nearest_idx(util::vec(0.022, -1.25)));
}

TEST_F (GridTest, bool_5d)
{
	Grid<double> g(util::vec(6UL, 5UL, 4UL, 3UL, 2UL)
			, util::vec(0.0, 0.0, 0.0, 0.0, 0.0)
			, util::vec(1.0, 1.0, 1.0, 1.0, 1.0), 0.0);

	ASSERT_EQ(720UL, g.coordinate.size());
	ASSERT_EQ(720UL, g.data.size());

	g.map([](const std::vector<double> &p) -> double
			{
				return std::accumulate(p.begin(), p.end(), 0.0);
			});

	EXPECT_FLOAT_EQ(5.0, g.at(util::vec(5UL, 4UL, 3UL, 2UL, 1UL)).second);
	EXPECT_FLOAT_EQ(163.0 / 60.0, g.at(util::vec(4UL, 3UL, 2UL, 1UL, 0UL)).second);
}

