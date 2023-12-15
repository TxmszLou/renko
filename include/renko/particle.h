#ifndef RENKO_PARTICLE_H
#define RENKO_PARTICLE_H

#include <renko/core.h>
#include <iostream>

namespace renko {
    class Particle {
        public:
            friend ostream& operator<<(ostream& os, const Particle& p) {
                return os << "Mass: " << p.getMass() << std::endl
                          << "Position: " << p.position
                          << ", Velocity: " << p.velocity
                          << ", Acceleration: " << p.acceleration << ')' << std::endl;
            }

            /* the position vector of this particle */
            Vector3 position;

            /* the velocity vector of this particle */
            Vector3 velocity;

            /* the acceleration vector of this particle */
            Vector3 acceleration;

            /* to simulate frictions experiences by the particle */
            // from 0 (most damped) to 1 (no damping)
            real damping;

            /* total force exerted on the particle */
            Vector3 totalForce;

            void setMass(real m) {
                inverseMass = ((real)1)/m;
            }

            real getMass() const {
                return ((real)1)/inverseMass;
            }

            void setInverseMass(real i) {
                inverseMass = i;
            }

            /* integrates the particule forward in time for a given amount */
            void integrate(real duration);


        protected:
            /* 1/mass of the object */
            // here we store 1/m instead of m because
            // (1) a = f/m and we can save one step in the computation when finding acceleration from force
            // (2) it's easy to simulate particle of infinite mass, simply set inverseMass = 0
            real inverseMass;
    };
}

#endif // RENKO_PARTICLE_H
