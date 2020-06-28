#include "Constants.h"



Constants::Constants()
{
}

Constants::~Constants()
{
}

ALG Constants::ALGORITHM = Dstream;
DATATYPE Constants::DATA = Social;
int Constants::NUM_THREAD = 70;
double Constants::DELTA = 1.0;
int Constants::K = 3;
int Constants::BUDGET = 10;
int Constants::M = 3;

// for social
double Constants::EPS = 0.5;
int Constants::NO_DENOISE_STEPS = 2;

// for sensor
//double Constants::EPS = 0.0;
//int Constants::NO_DENOISE_STEPS = 1;