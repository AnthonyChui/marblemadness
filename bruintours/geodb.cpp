#include <iomanip>
#include "geodb.h"
#include "geotools.h"
using namespace std;

GeoDatabase::GeoDatabase(){
}

GeoDatabase::~GeoDatabase() {
    // Delete all GeoPoint objects in the geopts list
    for (GeoPoint* point : geopts) {
        delete point;
    }
}

bool GeoDatabase::load(const string& map_data_file){
    cout.setf(ios::fixed);
    cout.precision(7);
    ifstream file(map_data_file);
    if (!file.is_open()) {
        cerr << "Couldn't open file" << endl;
        return false;
    }
    string streetName;
    while (getline(file, streetName)) {
        string coordsLine;
        getline(file, coordsLine);
        istringstream iss(coordsLine);
        double lat1, lon1, lat2, lon2;
        if (!(iss >> lat1 >> lon1 >> lat2 >> lon2)) {
            cerr << "Error: Malformed coordinates line." << endl;
            return false;
        }
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(7) << lat1; // Set precision to 10 decimal places
        std::string lat1Str = oss.str(); // Convert to string with higher precision

        // Do the same for other variables
        oss.str(""); // Clear the stringstream
        oss << std::fixed << std::setprecision(7) << lon1;
        std::string lon1Str = oss.str();
        
        oss.str(""); // Clear the stringstream
        oss << std::fixed << std::setprecision(7) << lon2;
        std::string lon2Str = oss.str();
        
        oss.str(""); // Clear the stringstream
        oss << std::fixed << std::setprecision(7) << lat2;
        std::string lat2Str = oss.str();
        string numPOIsLine;
        getline(file, numPOIsLine);
        int numPOIs = stoi(numPOIsLine);
        vector<POI> pois;
        
        GeoPoint* g = new GeoPoint(lat1Str,lon1Str);
        GeoPoint* g2 = new GeoPoint(lat2Str,lon2Str);
        geopts.push_back(g);
        segment toAdd(g2, streetName);
        if(geopointConnectors.find(g->to_string())!=nullptr){
            list<segment> *segments = geopointConnectors.find(g->to_string());
            segments->push_back(toAdd);
        }
        else{
            list<segment> segments;
            segments.push_back(toAdd);
            geopointConnectors.insert(g->to_string(), segments);
        }
        segment toAdd2(g, streetName);
        if(geopointConnectors.find(g2->to_string())!=nullptr){
            list<segment> *segments = geopointConnectors.find(g2->to_string());
            segments->push_back(toAdd2);
        }
        else{
            list<segment> segments;
            segments.push_back(toAdd2);
            geopointConnectors.insert(g2->to_string(), segments);
        }
        
        for (int i = 0; i < numPOIs; ++i) {
            GeoPoint *mid = new GeoPoint(midpoint(*g, *g2));
            
            //g and mid
            segment toAdd3(mid, streetName);
            if(geopointConnectors.find(g->to_string())!=nullptr){
                list<segment> *segments = geopointConnectors.find(g->to_string());
                segments->push_back(toAdd3);
            }
            else{
                list<segment> segments;
                segments.push_back(toAdd3);
                geopointConnectors.insert(g->to_string(), segments);
            }
            segment toAdd4(g, streetName);
            if(geopointConnectors.find(mid->to_string())!=nullptr){
                list<segment> *segments = geopointConnectors.find(mid->to_string());
                segments->push_back(toAdd4);
            }
            else{
                list<segment> segments;
                segments.push_back(toAdd4);
                geopointConnectors.insert(mid->to_string(), segments);
            }
            
            //g2 and mid
            segment toAdd5(mid, streetName);
            if (geopointConnectors.find(g2->to_string()) != nullptr) {
                list<segment> *segments = geopointConnectors.find(g2->to_string());
                segments->push_back(toAdd5);
            } else {
                list<segment> segments;
                segments.push_back(toAdd5);
                geopointConnectors.insert(g2->to_string(), segments);
            }
            segment toAdd6(g2, streetName);
            if (geopointConnectors.find(mid->to_string()) != nullptr) {
                list<segment> *segments = geopointConnectors.find(mid->to_string());
                segments->push_back(toAdd6);
            } else {
                list<segment> segments;
                segments.push_back(toAdd6);
                geopointConnectors.insert(mid->to_string(), segments);
            }
            string poiLine;
            getline(file, poiLine);
            size_t delimiterPos = poiLine.find('|');
            string poiName = poiLine.substr(0, delimiterPos);
            istringstream poiIss(poiLine.substr(delimiterPos + 1));
            double poiLat, poiLon;
            if (!(poiIss >> poiLat >> poiLon)) {
                cerr << "Error: Malformed POI coordinates." << endl;
                continue;
            }
            oss.str(""); // Clear the stringstream
            oss << std::fixed << std::setprecision(7) << poiLat;
            std::string poilatStr = oss.str();
            
            oss.str(""); // Clear the stringstream
            oss << std::fixed << std::setprecision(7) << poiLon;
            std::string poilonStr = oss.str();
            GeoPoint* poiPoint = new GeoPoint(poilatStr, poilonStr);
            locationToGeopoint.insert(poiName, poiPoint);
            
            //mid to poipoint
            segment toAdd7(mid, "a path");
            if (geopointConnectors.find(poiPoint->to_string()) != nullptr) {
                list<segment> *segments = geopointConnectors.find(poiPoint->to_string());
                segments->push_back(toAdd7);
            } else {
                list<segment> segments;
                segments.push_back(toAdd7);
                geopointConnectors.insert(poiPoint->to_string(), segments);
            }
            segment toAdd8(poiPoint, "a path");
            if (geopointConnectors.find(mid->to_string()) != nullptr) {
                list<segment> *segments = geopointConnectors.find(mid->to_string());
                segments->push_back(toAdd8);
            } else {
                list<segment> segments;
                segments.push_back(toAdd8);
                geopointConnectors.insert(mid->to_string(), segments);
            }
        }
    }
    file.close();
    return true;
}


bool GeoDatabase::get_poi_location(const string& poi, GeoPoint& point) const {
    GeoPoint *const *poiLocation = locationToGeopoint.find(poi);
    if (poiLocation != nullptr) {
        point = **poiLocation;
        return true;
    }
    return false;
}

vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
    vector<GeoPoint> c;
    const list<segment>* segments = geopointConnectors.find(pt.to_string());
    if (segments != nullptr) {
        for (const auto& seg : *segments) {
            if (seg.connected != nullptr) {
                c.push_back(*(seg.connected));
            }
        }
    }
    return c;
}

string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const{
    const list<segment>* segments = geopointConnectors.find(pt1.to_string());
    if(segments!=nullptr){
        for(const auto& seg: *segments){
            if(seg.connected!=nullptr){
                if(seg.connected->to_string()==pt2.to_string()){
                    return seg.streetName;
                }
            }
        }
    }
    return "";
}
