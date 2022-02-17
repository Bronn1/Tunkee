#include "pathfinding.h"

#include <ranges>
#include <queue>
#include <array>
#include <unordered_set>


namespace ranges = std::ranges;
namespace views = std::views;

int pathfinding::getLength(const GameTile& tile) {
    return int((abs(tile.m_q) + abs(tile.m_r) + abs(tile.m_s)) / 2);
}

int pathfinding::getDistance(const GameTile& a, const GameTile& b) {
    return getLength(a - b);
}

std::vector<GameTile> pathfinding::getNeighbors(const GameBoard& board, const GameTile& startingPos)
{
    auto [q, r, s, type, isAccessible] = startingPos; //c++20 magic
    std::array<GameTile, 6> possibleNeighbors = { GameTile(q, r - 1),GameTile(q + 1, r - 1), GameTile(q + 1, r),
                                             GameTile(q, r + 1), GameTile(q - 1, r + 1), GameTile(q - 1, r) };
    std::vector<GameTile> actualNeighbors{};
    for (auto&& i : possibleNeighbors) {
        if (board.contains(i))
            actualNeighbors.push_back(board.getTile(i));
    }
    return actualNeighbors;
}

std::vector<GameTile> pathfinding::getAvailableArea(const GameBoard& board, const GameTile& startingPos, const TileDistance movement)
{
    std::unordered_map<GameTile, int> pathCosts{};
    std::vector<GameTile> area{};
    std::queue<GameTile> travelQueue;
    //if (startingPos != nullptr)
    // TODO check for nullptr (operator !=)
    travelQueue.push(startingPos);
    pathCosts.insert({ startingPos , 0});
    while (!travelQueue.empty()) {
        GameTile current = travelQueue.front();
        travelQueue.pop();

        auto neighbors = getNeighbors(board, current);
        for (auto n : neighbors) {
            if (board.isAccessible(n)) {
                if (!pathCosts.contains(n)) { 
                    if (pathCosts.at(current) + n.travelCost() <= movement.distance) {
                        pathCosts.insert({ n, pathCosts.at(current) + n.travelCost() });
                        travelQueue.push(n);
                        area.push_back(n);
                    }
                }
                else { 
                    if (pathCosts.at(n) > pathCosts.at(current) + n.travelCost())
                        pathCosts.at(n) = pathCosts.at(current) + n.travelCost();

                }
            }
        }
    }

    return area;
}

std::vector<GameTile> pathfinding::getAvailableAreaWithRotation(const GameBoard& board, const GameTile& startingPos, const TileDistance movement, HexVertexNumber unitRotation)
{
    std::unordered_map<GameTile, unsigned> pathCosts{};
    std::queue<GameTile> travelQueue;
    // implement operators needed for set and switch to set, cuz of additional allocations
    std::unordered_set<GameTile> visited{};

    // finding all area in the front that doesnt require rotation
    auto tileInFront = pathfinding::getLineOfSightWithoutObstacles(board, startingPos, movement, unitRotation);
    auto  neighborsLeft = pathfinding::getLineOfSightWithoutObstacles(board, startingPos, movement, HexVertexNumber{ unitRotation.vertexNum + 5 });
    auto  neighborsRight = pathfinding::getLineOfSightWithoutObstacles(board, startingPos, movement, HexVertexNumber{ unitRotation.vertexNum + 1 });
    // TODO return unordered_sets or sets instead of additional allocation
    std::unordered_set < GameTile> tilesWithoutRotation(begin(tileInFront), end(tileInFront));
    tilesWithoutRotation.insert(begin(neighborsLeft), end(neighborsLeft));
    tilesWithoutRotation.insert(begin(neighborsRight), end(neighborsRight));

    travelQueue.push(startingPos);
    pathCosts.insert({ startingPos , 0 });
    while (!travelQueue.empty()) {
        GameTile current = travelQueue.front();
        travelQueue.pop();

        auto neighbors = getNeighbors(board, current);
        for (auto n : neighbors) {
            
            if (board.isAccessible(n) && ! visited.contains(n)) {

                if (tilesWithoutRotation.contains(n)) visited.insert(n);
                if (tilesWithoutRotation.contains(n) && pathCosts.at(current) + n.travelCost() > movement.distance)
                    tilesWithoutRotation.erase(n);

                if (!pathCosts.contains(n)) {
                    if (pathCosts.at(current) + n.travelCost() <= movement.distance) {
                        pathCosts.insert({ n, pathCosts.at(current) + n.travelCost() });
                        travelQueue.push(n);
                    }
                }
                else {

                    if (pathCosts.at(n) > pathCosts.at(current) + n.travelCost())
                        pathCosts.at(n) = pathCosts.at(current) + n.travelCost();
                }
            }
        }
    }

    auto checkIfTileNeedsRotation = [&tilesWithoutRotation, &movement](auto& tileCostPair) {
        if ( ! tilesWithoutRotation.contains(tileCostPair.first) && tileCostPair.second + 1 > movement.distance)
            return false;
        return true;
    };
    pathCosts.at( startingPos ) = 999 ;
    auto rotationFilterRange = pathCosts | views::filter(checkIfTileNeedsRotation) | views::transform([](const auto& tileCostPair) { return tileCostPair.first; });
    std::vector<GameTile> area(begin(rotationFilterRange), end(rotationFilterRange));

    return area;
}


//  A* Algorithm 
std::vector<GameTile> pathfinding::getShortestPath(const GameBoard& board, const GameTile& startingPos, const GameTile& destination)
{
    std::vector<GameTile> resultPath{};
    if(!board.contains(destination) && !board.isAccessible(destination))
        return resultPath;
    if (destination == startingPos)
        return resultPath;

    //lambda to compare GameTile by travel cost
    auto cmp = [](const std::pair<GameTile, int>& left, const  std::pair<GameTile, int>& right) { return (left.first.travelCost() +left.second) > (right.first.travelCost() + right.second); };
    std::priority_queue<std::pair<GameTile, int>, std::vector<std::pair<GameTile, int>>, decltype(cmp)> travelQueue(cmp);
    std::unordered_map<GameTile, int> pathCosts{};
    std::unordered_map<GameTile, GameTile> cameFrom{};

    // TODO check for nullptr (operator !=)
    travelQueue.push({ startingPos,  0 });
    pathCosts.insert({ startingPos , 0 });
    //cameFrom.insert({ startingPos , startingPos });

    while (!travelQueue.empty()) {
        auto [current, heuristic] = travelQueue.top();
        travelQueue.pop();
        //std::cout << current << "current\n";

        if(current == destination)
            return _createOrderedPath(cameFrom, startingPos, destination);

        auto neighbors = getNeighbors(board, current);
        for (auto& n : neighbors) {
            
            if (board.isAccessible(n)) {
                int newCost = pathCosts.at(current) + n.travelCost();
                //std::cout << n << "(" << newCost  <<");  ";
                if (!pathCosts.contains(n) || (newCost < pathCosts.at(n))) {
                    pathCosts[n] = newCost ;
                    cameFrom[n] = current;

                    travelQueue.push({ n, newCost + getDistance(n, destination)});

                }
            }
        }
    }

    return _createOrderedPath(cameFrom, startingPos, destination);
}

std::vector<GameTile> pathfinding::_createOrderedPath(const std::unordered_map<GameTile, GameTile>& cameFrom, const GameTile& startingPos, const GameTile& destination)
{
    std::vector<GameTile> resultPath{};
    GameTile current = destination;

    while (cameFrom.contains(current)) {
        resultPath.push_back(current);
        current = cameFrom.at(current);
    }
    ranges::reverse(resultPath);

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

std::vector<GameTile> pathfinding::getLineOfSightWithoutObstacles(const GameBoard& board, const GameTile& startingPos, const TileDistance layersAmount, HexVertexNumber point)
{
    std::vector<GameTile> lineOfSigh{};
    for (int curLayer : views::iota(1, (int)layersAmount.distance + 1))
    {
        auto [cur_q, cur_r, q_adder, r_adder] = _getLineOfSightLayerCoordinates(point, curLayer);
        for (int tilesAmount : views::iota(0, curLayer + 1))
        {
            if (board.getTile(GameTile(startingPos.m_q + cur_q, startingPos.m_r + cur_r)) == NullTile()) continue;

            lineOfSigh.push_back(GameTile(startingPos.m_q + cur_q, startingPos.m_r + cur_r));
            cur_q += q_adder;
            cur_r += r_adder;
        }
    }
    return lineOfSigh;
}

std::tuple<int, int, int, int> pathfinding::_getLineOfSightLayerCoordinates(HexVertexNumber point, int layer)
{
    using enum HexVertex;
    HexVertex vertex = static_cast<HexVertex>(point.vertexNum);
    switch (vertex)
    {
    case Vertex0:
        return { 0, -layer, +1, 0 }; 
    case Vertex1:
        return { layer, 0, 0, -1 }; 
    case Vertex2:
        return { 0, layer, +1, -1 };
    case Vertex3:
        return { 0, layer, -1, 0 };
    case Vertex4:
        return { -layer, 0, 0, +1 };
    case Vertex5:
        return { 0, -layer, -1, +1 };
    }
}


