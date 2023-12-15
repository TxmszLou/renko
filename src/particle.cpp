#include <renko/core.h>
#include <renko/particle.h>

using namespace renko;

void Particle::integrate(real duration) {
    /* assert(duration > 0.0); */

    // update position
    // p' = p + v*t
    // we use this instead of the correct p' = p + v*t + a*t^2/2 because the latter summand is often insignificant
    position.addScaledVector(velocity, duration);

    // work out the acceleration from force imposed
    Vector3 resultAcc = acceleration;
    resultAcc.addScaledVector(totalForce, inverseMass);

    // update velocity
    velocity.addScaledVector(resultAcc, duration);

    // impose drag/frictions
    velocity *= real_pow(damping, duration);
}

