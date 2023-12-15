#ifndef RENKO_PRECISION_H
#define RENKO_PRECISION_H

/* decide whether to use single or double precision for this physics engine */
#include <math.h>

// define the precision of square root
#define real_sqrt sqrtf
#define real_pow powf

namespace renko {
    typedef float real;
}

#endif // RENKO_PRECISION_H
