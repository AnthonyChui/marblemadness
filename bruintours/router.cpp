#include <queue>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include "router.h"

Router::Router(const GeoDatabaseBase& geo_db){
    geo = &geo_db;
}

Router::~Router(){
    
}

double Router::findDistance(const GeoPoint& pt1, const GeoPoint& pt2) const {
    // Calculate differences in coordinates in degrees
    double dlat = pt2.latitude - pt1.latitude;
    double dlon = pt2.longitude - pt1.longitude;
    // Approximate the distance using the Pythagorean theorem
    double distance = sqrt(dlat * dlat + dlon * dlon);
    return distance;
}

std::vector<GeoPoint> Router::route(const GeoPoint& start, const GeoPoint& goal) const {
    // Define the priority queue
    auto cmp = [](const std::pair<GeoPoint, double>& a, const std::pair<GeoPoint, double>& b) {
        return a.second > b.second;
    };
    std::priority_queue<std::pair<GeoPoint, double>, std::vector<std::pair<GeoPoint, double>>, decltype(cmp)> frontier(cmp);

    // Define the cost map and predecessor map
    std::unordered_map<std::string, double> cost_so_far;
    std::unordered_map<std::string, GeoPoint> came_from;

    // Initialize the frontier with the start point
    frontier.push(std::make_pair(start, 0.0));
    cost_so_far[start.to_string()] = 0.0;

    while (!frontier.empty()) {
        GeoPoint current = frontier.top().first;
        frontier.pop();

        // Check if we have reached the goal
        if (current.to_string() == goal.to_string()) {
            break;
        }

        // Explore connected points
        for (const GeoPoint& next : geo->get_connected_points(current)) {
            double new_cost = cost_so_far[current.to_string()] + findDistance(current, next);
            if (cost_so_far.find(next.to_string()) == cost_so_far.end() || new_cost < cost_so_far[next.to_string()]) {
                cost_so_far[next.to_string()] = new_cost;
                double priority = new_cost + findDistance(next, goal);
                frontier.push(std::make_pair(next, priority));
                came_from[next.to_string()] = current;
            }
        }
    }

    // Reconstruct the path
    std::vector<GeoPoint> path;
    if (cost_so_far.find(goal.to_string()) != cost_so_far.end()) {
        for (GeoPoint at = goal; at.to_string() != start.to_string(); at = came_from[at.to_string()]) {
            path.push_back(at);
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
    }
    return path;
}
