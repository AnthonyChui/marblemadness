#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "geodb.h"
#include "router.h"
#include "stops.h"
#include "tourcmd.h"
#include "tour_generator.h"

//#include <fstream>
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <iomanip>
//#include "geodb.h"
//using namespace std;
//
//int main() {
//    cout.setf(ios::fixed);
//        cout.precision(7);
//    GeoDatabase geodb;
//    geodb.load("/Users/anthonychui/Desktop/School/mapdata.txt");
//       // Load the map data
//    Router r(geodb);
//       
//
//       // Test getting the location of a POI
//       GeoPoint poiLocation;
//       std::string poiName = "Twentieth Century Fox Film Corporation";
//       if (geodb.get_poi_location(poiName, poiLocation)) {
//           std::cout << "Location of " << poiName << ": ("
//                     << poiLocation.latitude << ", " << poiLocation.longitude << ")" << std::endl;
//       } else {
//           std::cerr << "POI not found: " << poiName << std::endl;
//       }
//    
//    poiName = "Fox Plaza";
//    if (geodb.get_poi_location(poiName, poiLocation)) {
//        std::cout << "Location of " << poiName << ": ("
//                  << poiLocation.latitude << ", " << poiLocation.longitude << ")" << std::endl;
//    } else {
//        std::cerr << "POI not found: " << poiName << std::endl;
//    }
//
//       // Test getting connected points
//       GeoPoint testPoint("34.0476495", "-118.4910837"); // Use coordinates of a known point
//    
//       std::vector<GeoPoint> connectedPoints = geodb.get_connected_points(testPoint);
//       std::cout << "Connected points to (" << testPoint.latitude << ", " << testPoint.longitude << "):" << std::endl;
//       for (const auto& pt : connectedPoints) {
//           std::cout << "(" << pt.latitude << ", " << pt.longitude << ")" << std::endl;
//       }
//    
//    GeoPoint diddy("34.0630614", "-118.4468781");
//    GeoPoint fox("34.0614911", "-118.4464410");
//    
//    std::vector<GeoPoint> route = r.route(diddy, fox);
//    for(auto a: route){
//        std::cout << a.to_string() << endl;
//    }
//
//       return 0;
//}


void print_tour(vector<TourCommand>& tcs)
{
    double total_dist = 0;
    std::string direction;
    double street_distance = 0;

    cout << "Starting tour...\n";

    for (size_t i = 0; i < tcs.size(); ++i)
    {
        if (tcs[i].get_command_type() == TourCommand::commentary)
        {
            cout << "Welcome to " << tcs[i].get_poi() << "!\n";
            cout << tcs[i].get_commentary() << "\n";
        }
        else if (tcs[i].get_command_type() == TourCommand::turn)
        {
            cout << "Take a " << tcs[i].get_direction() << " turn on " << tcs[i].get_street() << endl;
        }
        else if (tcs[i].get_command_type() == TourCommand::proceed)
        {
            total_dist += tcs[i].get_distance();
            if (direction.empty())
                direction = tcs[i].get_direction();
            street_distance += tcs[i].get_distance();
            if (i+1 < tcs.size() && tcs[i+1].get_command_type() == TourCommand::proceed
                && tcs[i+1].get_street() == tcs[i].get_street() && tcs[i].get_street() != "a path")
            {
                continue;
            }

            cout << "Proceed " << std::fixed << std::setprecision(3) << street_distance << " miles " << direction << " on " << tcs[i].get_street() << endl;
            street_distance = 0;
            direction.clear();
        }
    }

    cout << "Your tour has finished!\n";
    cout << "Total tour distance: " << std::fixed << std::setprecision(3) << total_dist << " miles\n";
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "usage: BruinTour /Users/anthonychui/Desktop/School/mapdata.txt /Users/anthonychui/Desktop/School/stops.txt";
        return 1;
    }

    GeoDatabase geodb;
    if (!geodb.load(argv[1]))
    {
        cout << "Unable to load map data: " << argv[1] << endl;
        return 1;
    }

    Router router(geodb);
    TourGenerator tg(geodb, router);

    Stops stops;
    if (!stops.load(argv[2]))
    {
        cout << "Unable to load tour data: " << argv[2] << endl;
        return 1;
    }

    std::cout << "Routing...\n\n";

    vector<TourCommand> tcs = tg.generate_tour(stops);
    if (tcs.empty())
        cout << "Unable to generate tour!\n";
    else
        print_tour(tcs);
}
