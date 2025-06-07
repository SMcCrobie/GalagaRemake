#pragma once

#define RANDOM_FLOAT_WITHIN_RANGE(lowLimit , highLimit) ((lowLimit) + static_cast <float> (rand()) / (RAND_MAX / ((highLimit) - (lowLimit))))