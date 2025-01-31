//
//  router.hpp
//  Project4
//
//  Created by Anthony Chui on 3/8/24.
//

#ifndef router_h
#define router_h
#include "base_classes.h"

class Router: public RouterBase
{
public:
    Router(const GeoDatabaseBase& geo_db);
    virtual ~Router();
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1,const GeoPoint& pt2) const;
private:
    bool route(const GeoPoint& pt1,const GeoPoint& pt2, std::vector<GeoPoint> &geopoints) const;
private:
    double findDistance(const GeoPoint &pt1, const GeoPoint &pt2) const;
    const GeoDatabaseBase* geo;
};

#endif /* router_h */
