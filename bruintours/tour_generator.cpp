//
//  tour_generator.cpp
//  Project4
//
//  Created by Anthony Chui on 3/8/24.
//
#include "geotools.h"
#include "tour_generator.h"
TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router){
    geo = &geodb;
    rout = &router;
}

TourGenerator::~TourGenerator(){
    
}

std::string TourGenerator::getDirection(double angle) const{
    if (angle >= 0 && angle < 22.5) {
        return "east";
    } else if (angle >= 22.5 && angle < 67.5) {
        return "northeast";
    } else if (angle >= 67.5 && angle < 112.5) {
        return "north";
    } else if (angle >= 112.5 && angle < 157.5) {
        return "northwest";
    } else if (angle >= 157.5 && angle < 202.5) {
        return "west";
    } else if (angle >= 202.5 && angle < 247.5) {
        return "southwest";
    } else if (angle >= 247.5 && angle < 292.5) {
        return "south";
    } else if (angle >= 292.5 && angle < 337.5) {
        return "southeast";
    } else {
        return "east";
    }
}

std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const {
    
    std::vector<TourCommand> commands;
    
    for (int i = 0; i < stops.size(); i++) {
        std::string poi, commentary;
        stops.get_poi_data(i, poi, commentary);
        
        // Add commentary command
        TourCommand commentaryCommand;
        commentaryCommand.init_commentary(poi, commentary);
        commands.push_back(commentaryCommand);
        // If not the last stop, find the route to the next stop
        if (i + 1 < stops.size()) {
            std::string nextPoi, nextCommentary;
            stops.get_poi_data(i + 1, nextPoi, nextCommentary);
            GeoPoint currentPoint, nextPoint;
            geo->get_poi_location(poi, currentPoint);
            geo->get_poi_location(nextPoi, nextPoint);
            std::vector<GeoPoint> path = rout->route(currentPoint, nextPoint);
            // Process the path to create proceed and turn commands
            for (size_t j = 0; j < path.size() - 1; j++) {
                std::string street_name = geo->get_street_name(path[j], path[j + 1]);
                double angle = angle_of_line(path[j], path[j + 1]);
                std::string direction = getDirection(angle); // Convert angle to direction
                double distance = distance_earth_miles(path[j], path[j + 1]);
                
                // Create and add the proceed command
                TourCommand proceedCommand;
                proceedCommand.init_proceed(direction, street_name, distance, path[j], path[j + 1]);
                commands.push_back(proceedCommand);
                
                // Check if a turn command is needed
                if (j < path.size() - 2) {
                    double turn_angle = angle_of_turn(path[j], path[j + 1], path[j + 2]);
                    std::string turn_direction;
                    if (turn_angle >= 1 && turn_angle < 180) {
                        turn_direction = "left";
                    } else if (turn_angle >= 180 && turn_angle <= 359) {
                        turn_direction = "right";
                    }
                    std::string next_street_name = geo->get_street_name(path[j + 1], path[j + 2]);
                    if(street_name!=next_street_name){
                        if (!turn_direction.empty()) {
                            TourCommand turnCommand;
                            turnCommand.init_turn(turn_direction, next_street_name);
                            commands.push_back(turnCommand);
                        }
                    }
                }
            }
        }
    }
    return commands;
}
