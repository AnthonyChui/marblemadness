//
//  geodb.hpp
//  Project4
//
//  Created by Anthony Chui on 3/8/24.
//

#ifndef geodb_h
#define geodb_h
#include "base_classes.h"
#include "HashMap.h"
#include <iostream> // needed for any I/O
#include <fstream>  // needed in addition to <iostream> for file I/O
#include <sstream>  // needed in addition to <iostream> for string stream I/O
#include <string>
#include <list>
using namespace std;

class GeoDatabase: public GeoDatabaseBase
{
public:
    GeoDatabase();
    virtual ~GeoDatabase();
    virtual bool load(const std::string& map_data_file);
    virtual bool get_poi_location(const std::string& poi,
                                  GeoPoint& point) const;
    virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt)
    const;
    virtual std::string get_street_name(const GeoPoint& pt1,
                                        const GeoPoint& pt2) const;
private:
    struct POI {
        std::string name;
        double latitude;
        double longitude;
    };
    struct segment{
        GeoPoint* connected;
        std::string streetName;
        segment(GeoPoint* c, std::string s) : connected(c), streetName(s) {};
    };
    std::list<GeoPoint*> geopts; //all geopts
    HashMap<GeoPoint*> locationToGeopoint; //POI names to geopoint
    HashMap<std::list<segment>> geopointConnectors; //All geopoints connected to given geopoint
};
#endif /* geodb_h */


