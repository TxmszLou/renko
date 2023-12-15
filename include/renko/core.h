#ifndef RENKO_CORE_H
#define RENKO_CORE_H

#include <renko/precision.h>
#include <iostream>

using namespace std;

namespace renko {

    /* 3-dimensional vector */
    class Vector3 {
        public:
            real x;
            real y;
            real z;

        private:
            // add padding to ensure data are aligned in 4-words
            // purely for performance
            real pad;

        public:
            /* zero vector constructor */
            Vector3() : x(0), y(0), z(0) {}
            /* constructor for a vector */
            Vector3(const real x, const real y, const real z) : x(x), y(y), z(z) {}

            /* print the vector */
            friend ostream& operator<<(ostream& os, const Vector3& v) {
                return os << '(' << v.x << ',' << v.y << ',' << v.z << ')';
            }

            /* invert a vector: v -> -v */
            void invert() {
                x = -x;
                y = -y;
                z = -z;
            }

            /* return the magnitute of this vector */
            // const after function name indicates to the compiler that this method will not change this object
            real magnitute() const {
                return real_sqrt(x*x + y*y + z*z);
            }

            real squareMagnitute() const {
                return x*x + y*y + z*z;
            }

            /* scale itself */
            void operator*=(const real value) {
                x *= value;
                y *= value;
                z *= value;
            }

            /* return a *copy* of scaled vector */
            Vector3 operator*(const real value) const {
                return Vector3(x*value, y*value, z*value);
            }

            Vector3 operator/(const real value) const {
                return Vector3(x/value, y/value, z/value);
            }

            /* add a vector to this */
            void operator+=(const Vector3& v) {
                x += v.x;
                y += v.y;
                z += v.z;
            }

            /* return a copy of sum of vectors */
            Vector3 operator+(const Vector3& v) const {
                return Vector3(x+v.x, y+v.y, z+v.z);
            }

            /* subtract a vector from this */
            void operator-=(const Vector3& v) {
                x -= v.x;
                y -= v.y;
                z -= v.z;
            }

            /* return a copy of difference of vectors */
            Vector3 operator-(const Vector3& v) const {
                return Vector3(x-v.x, y-v.y, z-v.z);
            }

            /* add a scaled vector to this */
            void addScaledVector(const Vector3& v, real scale) {
                x += v.x * scale;
                y += v.y * scale;
                z += v.z * scale;
            }

            /* dot product of this with a given vector */
            real dotProduct(const Vector3& v) const {
                return x*v.x + y*v.y + z*v.z;
            }

            real operator*(const Vector3& v) const {
                return x*v.x + y*v.y + z*v.z;
            }

            /* return the component-wise product with another vector */
            Vector3 componentProduct(const Vector3& v) const {
                return Vector3(x*v.x, y*v.y, z*v.z);
            }

            /* set this vector to be the component-wise product with another vector */
            void componentProductUpdate(const Vector3& v) {
                x *= v.x;
                y *= v.y;
                z *= v.z;
            }

            /* cross product */
            Vector3 crossProduct(const Vector3& v) const {
                return Vector3(y*v.z - v.y*z, v.x*z - x*v.z, x*v.y - v.x-y);
            }

            Vector3 operator%(const Vector3& v) const {
                return crossProduct(v);
            }

            /* set this vector to be this x v */
            void operator%=(const Vector3& v) {
                *this = crossProduct(v);
            }

            void normalize() {
                real l = magnitute();
                if (l > 1) {
                    (*this) *= ((real)1)/l;
                }
            }
    };
}

#endif // RENKO_CORE_H
