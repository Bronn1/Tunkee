#include "pathfinding.h"

#include <iostream>
#include <ranges>
#include <queue>


int pathfinding::getLength(const GameTile& tile) {
	return int((abs(tile.m_q) + abs(tile.m_r) + abs(tile.m_s)) / 2);
}

int pathfinding::getDistance(const GameTile& a, const GameTile& b) {
	return getLength(a - b);
}

std::vector<GameTile> pathfinding::getNeighbors(const GameBoard& board, const GameTile& source)
{
	auto [q, r, s, type, isAccessible] = source; //c++20 magic
	std::vector<GameTile> possibleNeighbors = { GameTile(q, r - 1),GameTile(q + 1, r - 1), GameTile(q + 1, r),
											 GameTile(q, r + 1), GameTile(q - 1, r + 1), GameTile(q - 1, r) };
	std::vector<GameTile> actualNeighbors{};
	for (auto&& i : possibleNeighbors) {
		if (board.contains(i))
			actualNeighbors.push_back(board.getTile(i));
	}
	return actualNeighbors;
}

std::vector<GameTile> pathfinding::getAvailableArea(const GameBoard& board, const GameTile& source, const TileDistance movement)
{
	std::unordered_map<GameTile, int> pathCosts{};
	std::vector<GameTile> area{};
	std::queue<GameTile> travelQueue;
	//if (source != nullptr)
	// TODO check for nullptr (operator !=)
	travelQueue.push(source);
	pathCosts.insert({ source , 0});
	while (!travelQueue.empty()) {
		GameTile current = travelQueue.front();
		travelQueue.pop();

		auto neighbors = getNeighbors(board, current);
		for (auto n : neighbors) {
			if (board.isAccessible(n)) {
				if (!pathCosts.contains(n)) { 
					if (pathCosts.at(current) + n.staticCastTypeToInt() <= movement.distance) {
						pathCosts.insert({ n, pathCosts.at(current) + n.staticCastTypeToInt() });
						travelQueue.push(n);
						area.push_back(n);
					}
				}
				else { 
					if (pathCosts.at(n) > pathCosts.at(current) + n.staticCastTypeToInt())
						pathCosts.at(n) = pathCosts.at(current) + n.staticCastTypeToInt();

				}
			}
		}
	}

	return area;
}

//  A* Algorithm 
std::vector<GameTile> pathfinding::getShortestPath(const GameBoard& board, const GameTile& source, const GameTile& destination)
{
	std::vector<GameTile> resultPath{};
	if(!board.contains(destination) && !board.isAccessible(destination))
		return resultPath;
	if (destination == source)
		return resultPath;

	//lambda to compare GameTile by travel cost
	auto cmp = [](const std::pair<GameTile, int>& left, const  std::pair<GameTile, int>& right) { return (left.first.staticCastTypeToInt() +left.second) > (right.first.staticCastTypeToInt() + right.second); };
	std::priority_queue<std::pair<GameTile, int>, std::vector<std::pair<GameTile, int>>, decltype(cmp)> travelQueue(cmp);
	std::unordered_map<GameTile, int> pathCosts{};
	std::unordered_map<GameTile, GameTile> cameFrom{};

	// TODO check for nullptr (operator !=)
	travelQueue.push({ source,  0 });
	pathCosts.insert({ source , 0 });
	//cameFrom.insert({ source , source });

	while (!travelQueue.empty()) {
		auto [current, heuristic] = travelQueue.top();
		travelQueue.pop();
		//std::cout << current << "current\n";

		if(current == destination)
			return _createOrderedPath(cameFrom, source, destination);

		auto neighbors = getNeighbors(board, current);
		for (auto& n : neighbors) {
			
			if (board.isAccessible(n)) {
				int newCost = pathCosts.at(current) + n.staticCastTypeToInt();
				//std::cout << n << "(" << newCost  <<");  ";
				if (!pathCosts.contains(n) || (newCost < pathCosts.at(n))) {
					pathCosts[n] = newCost ;
					cameFrom[n] = current;

					travelQueue.push({ n, newCost + getDistance(n, destination)});

				}
			}
		}
	}

	return _createOrderedPath(cameFrom, source, destination);
}

std::vector<GameTile> pathfinding::_createOrderedPath(const std::unordered_map<GameTile, GameTile>& cameFrom, const GameTile& source, const GameTile& destination)
{
	std::vector<GameTile> resultPath{};
	GameTile current = destination;

	while (cameFrom.contains(current)) {
		resultPath.push_back(current);
		current = cameFrom.at(current);
	}
	std::ranges::reverse(resultPath);


	return resultPath;
}

GameTile pathfinding::_roundToTile(const float q, const float r, const float s) {

	int rQ = int(round(q));
	int rR = int(round(r));
	int rS = int(round(s));

	double q_diff = abs(rQ - q);
	double r_diff = abs(rR - r);
	double s_diff = abs(rS - s);

	if (q_diff > r_diff && q_diff > s_diff)
		rQ = -rR - rS;
	else if (r_diff > s_diff)
		rR = -rQ - rS;

	return GameTile(rQ, rR);
}

//linear interpolation for line drawing.
inline float pathfinding::_lerp(const float a, const float b, const  float t)
{
	return a + (b - a) * t;
}

std::tuple<float, float, float> pathfinding::_cubeLerp(const GameTile& a, const GameTile& b, const float t) {
	return { _lerp(a.m_q, b.m_q, t), _lerp(a.m_r, b.m_r, t), _lerp(a.m_s, b.m_s, t) };

}

std::vector<GameTile> pathfinding::drawLine(const GameTile& source, const GameTile& destination) {
	int distance = getDistance(source, destination);
	std::vector<GameTile> resultPath{};
	if (source == destination) {
		return resultPath;
	}

	for (int i : std::views::iota(1, distance))
	{
		auto [q, r, s] = _cubeLerp(source, destination, 1.0f / distance * i);
		resultPath.push_back(_roundToTile(q, r, s));
	}
	resultPath.push_back(destination);

	return resultPath;
}

std::ostream& core::operator<<(std::ostream& os, const GameTile& gt)
{
	os << "[" << gt.m_q << ", " << gt.m_r << ", " << gt.m_s  << "]";
	return os;
}