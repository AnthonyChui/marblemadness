//
//  tour_generator.hpp
//  Project4
//
//  Created by Anthony Chui on 3/8/24.
//

#ifndef tour_generator_h
#define tour_generator_h
#include "base_classes.h"

class TourGenerator: public TourGeneratorBase
{
public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
    virtual ~TourGenerator();
    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
    std::string getDirection(double angle) const;
    const GeoDatabaseBase *geo;
    const RouterBase *rout;
};

#endif /* tour_generator_h */
