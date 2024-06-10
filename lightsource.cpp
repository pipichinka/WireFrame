#include "lightsource.h"

LightSource::LightSource(const LightSourceConfig& conf):r(conf.r/ 255.0), b(conf.b / 255.0), g(conf.g / 255.0){
    original(0, 0) = conf.p(0, 0);
    original(1, 0) = conf.p(1, 0);
    original(2, 0) = conf.p(2, 0);
    original(3, 0) = 1.0;
    current = original;
}
