#include <iostream>
#include <renko/core.h>
#include <renko/particle.h>

using namespace renko;

int main () {
    /* Vector3 myVector(1,1,1); */

    /* std::cout << myVector << std::endl; */
    /* std::cout << "magnitute: " << myVector.magnitute() << std::endl; */
    /* std::cout << "1/magnitute: " << (real)1/myVector.magnitute() << std::endl; */

    /* Vector3 myVectorNormalized = myVector / myVector.magnitute(); */

    /* myVector.normalize(); */
    /* std::cout << myVector << std::endl; */
    /* std::cout << myVectorNormalized << std::endl; */
    /* std::cout << "magnitute: " << myVectorNormalized.magnitute() << std::endl; */

    /* Vector3 i(1,0,0); */
    /* Vector3 j(0,1,0); */
    /* Vector3 k = i.crossProduct(j); */
    /* std::cout << k << std::endl; */
    /* std::cout << i << " x " << j << " = " << i.crossProduct(j) << std::endl; */

    Particle p {};
    p.totalForce = Vector3(0,-10,0);
    p.setMass(10.0);
    p.damping = 1;
    std::cout << p << std::endl;

    for (int t = 0; t < 50; t++) {
        std::cout << p << std::endl;
        std::cout << p.totalForce << std::endl;
        p.integrate((float)1);
    }

    return 0;
}
