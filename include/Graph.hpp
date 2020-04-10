#pragma once

#include <CoreType.hpp>

class Graph {
public:
	Graph() noexcept = default;
	Graph(const Graph&) = delete;
	virtual ~Graph() = default;

public:
	Graph& operator=(const Graph&) = delete;

public:
	Vector Solve(double x) const;
	virtual bool IsContinuous(Point from, Point to) const = 0;

protected:
	virtual void Solve(double x, Vector& y) const = 0;
};