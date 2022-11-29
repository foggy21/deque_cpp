#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Deque.hpp"

using namespace fefu_laboratory_two;

TEST_CASE("Deque push_back") {
	Deque<int> c;
	c.push_back(5);
	REQUIRE(c.front() == 5);
	REQUIRE(c.back() == 5);
}

TEST_CASE("Deque push_front") {
	Deque<int> c;
	c.push_front(5);
	REQUIRE(c.front() == 5);
	REQUIRE(c.back() == 5);
}

TEST_CASE("Deque assign v1") {
	size_t count = 5;
	const int value = 10;
	size_t count_assign = 3;
	const int value_assign = 50;
	Deque<int> c(count, value);
	c.assign(count_assign, value_assign);
	REQUIRE(c.front() == 50);
	REQUIRE(c.back() == 10);
}

TEST_CASE("Deque assign v2") {
	Deque<int> c;
	c.assign(3, 50);
	REQUIRE(c.front() == 49);
	REQUIRE(c.back() == 3);
}

TEST_CASE("Deque []") {
	Deque<int> c;
	c.assign(3, 50);
	int aaa = c[2];
	REQUIRE(aaa == 47);
}

TEST_CASE("Deque size") {
	size_t count = 5;
	const int value = 10;
	Deque<int> c(count, value);
	REQUIRE(c.size() == count);
	REQUIRE(c.max_size() == ULLONG_MAX);
}

TEST_CASE("Deque front") {
	Deque<int> c;
	c.push_back(3);
	c.push_back(4);
	c.push_back(5);
	REQUIRE(c.front() == 3);
}

TEST_CASE("Deque back") {
	Deque<int> c;
	c.push_front(5);
	c.push_front(4);
	c.push_front(3);
	REQUIRE(c.back() == 5);
}

TEST_CASE("Deque pop_back") {
	Deque<int> c;
	c.push_back(3);
	c.pop_back();
	REQUIRE(c.empty());
}

TEST_CASE("Deque pop_back v2") {
	Deque<int> c;
	c.pop_back();
	REQUIRE(c.empty());
}

TEST_CASE("Deque pop_back v3") {
	Deque<int> c;
	c.push_back(3);
	c.push_back(4);
	c.pop_back();
	REQUIRE(!c.empty());
}

TEST_CASE("Deque constructor") {
	size_t count = 5;
	int val = 10;
	Deque<int> c(count, val);

	REQUIRE(c.back() == 10);
}

