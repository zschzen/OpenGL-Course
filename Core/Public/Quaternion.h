#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#pragma once

#include <stdexcept>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>

namespace Vosgi
{
    /** \brief Quaternion class for representing rotations in 3D space */
    struct Quaternion
    {
        float x; /** X component of the quaternion */
        float y; /** Y component of the quaternion */
        float z; /** Z component of the quaternion */
        float w; /** W component of the quaternion */

        /** Default constructor to initialize the quaternion with no rotation (x=0, y=0, z=0, w=1) */
        Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

        /**
         * \brief Constructor to initialize the quaternion with x, y, z, and w components
         * \param x The X component
         * \param y The Y component
         * \param z The Z component
         * \param w The W component
         */
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        /**
         * \brief Constructor to initialize the quaternion with glm::quat
         * \param quat The glm::quat
         */
        Quaternion(const glm::quat &quat) : x(quat.x), y(quat.y), z(quat.z), w(quat.w) {}

        /**
         * \brief Constructor to initialize the quaternion with euler angles
         * \param euler The euler angles
         */
        Quaternion(const glm::vec3 &euler)
        {
            glm::vec3 c = glm::cos(euler * 0.5f);
            glm::vec3 s = glm::sin(euler * 0.5f);
            x = s.x * c.y * c.z - c.x * s.y * s.z;
            y = c.x * s.y * c.z + s.x * c.y * s.z;
            z = c.x * c.y * s.z - s.x * s.y * c.z;
            w = c.x * c.y * c.z + s.x * s.y * s.z;
        }

        /**
         * \brief Constructor to initialize the quaternion with glm::vec4
         * \param vec The glm::vec4
         */
        Quaternion(const glm::vec4 &vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}

        /**
         * \brief Overloaded [] operator for accessing the quaternion components
         * \param index The index of the component (0 for X, 1 for Y, 2 for Z, 3 for W)
         * \return A reference to the component value
         * \throw std::out_of_range If the index is invalid
         */
        float &operator[](int index)
        {
            switch (index)
            {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            case 3:
                return w;
            default:
                throw std::out_of_range("Invalid Quaternion index!");
            }
        }

        /**
         * \brief Overloaded [] operator for accessing the quaternion components (const version)
         * \param index The index of the component (0 for X, 1 for Y, 2 for Z, 3 for W)
         * \return A const reference to the component value
         * \throw std::out_of_range If the index is invalid
         */
        const float &operator[](int index) const
        {
            switch (index)
            {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            case 3:
                return w;
            default:
                throw std::out_of_range("Invalid Quaternion index!");
            }
        }

        /**
         * \brief Get the euler angles from the quaternion
         * \return The euler angles
         */
        glm::vec3 GetEulerAngles() const
        {
            return glm::eulerAngles(glm::quat(w, x, y, z));
        }

        /**
         * \brief Get the euler angles in degrees from the quaternion
         * \return The euler angles in degrees
         */
        glm::vec3 GetEulerAnglesDegrees() const
        {
            return glm::degrees(GetEulerAngles());
        }

        /**
         * \brief Set the euler angles of the quaternion
         * \param euler The euler angles
         */
        void SetEulerAngles(const glm::vec3 &euler)
        {
            glm::vec3 c = glm::cos(euler * 0.5f);
            glm::vec3 s = glm::sin(euler * 0.5f);
            x = s.x * c.y * c.z - c.x * s.y * s.z;
            y = c.x * s.y * c.z + s.x * c.y * s.z;
            z = c.x * c.y * s.z - s.x * s.y * c.z;
            w = c.x * c.y * c.z + s.x * s.y * s.z;
        }

        /**
         * \brief Multiply two quaternions and return the result
         * \param lhs The left-hand side quaternion
         * \param rhs The right-hand side quaternion
         * \return The resulting quaternion
         */
        static Quaternion Multiply(const Quaternion &lhs, const Quaternion &rhs)
        {
            glm::quat lhsQuat(lhs.w, lhs.x, lhs.y, lhs.z);
            glm::quat rhsQuat(rhs.w, rhs.x, rhs.y, rhs.z);
            glm::quat resultQuat = lhsQuat * rhsQuat;
            return Quaternion(resultQuat.x, resultQuat.y, resultQuat.z, resultQuat.w);
        }

        /**
         * \brief Multiply a quaternion and a vector and return the result
         * \param rotation The quaternion to multiply
         * \param point The vector to multiply
         * \return The resulting vector
         */
        static glm::vec3 Multiply(const Quaternion &rotation, const glm::vec3 &point)
        {
            glm::quat rotationQuat(rotation.w, rotation.x, rotation.y, rotation.z);
            glm::vec3 pointVec(point.x, point.y, point.z);
            glm::vec3 resultVec = rotationQuat * pointVec;
            return glm::vec3(resultVec.x, resultVec.y, resultVec.z);
        }

        /**
         * \brief Calculate the dot product between two quaternions
         * \param a The first quaternion
         * \param b The second quaternion
         * \return The dot product
         */
        static float Dot(const Quaternion &a, const Quaternion &b)
        {
            glm::quat aQuat(a.w, a.x, a.y, a.z);
            glm::quat bQuat(b.w, b.x, b.y, b.z);
            return glm::dot(aQuat, bQuat);
        }

    
        static Quaternion AngleAxis(float angle, const glm::vec3 &axis)
        {
            glm::quat quat = glm::angleAxis(angle, axis);
            return Quaternion(quat.x, quat.y, quat.z, quat.w);
        }

        /**
         * \brief Check if two quaternions are equal
         * \param rhs The quaternion to compare against
         * \return True if the quaternions are approximately equal, false otherwise
         */
        bool operator==(const Quaternion &rhs)
        {
            return Dot(*this, rhs) > 1.0f - glm::epsilon<float>();
        }

        /**
         * \brief Check if two quaternions are not equal
         * \param rhs The quaternion to compare against
         * \return True if the quaternions are not approximately equal, false otherwise
         */
        bool operator!=(const Quaternion &rhs)
        {
            return !(*this == rhs);
        }

        /**
         * \brief Multiply two quaternions and return the result
         * \param rhs The quaternion to multiply
         * \return The resulting quaternion
         */
        Quaternion operator*(const Quaternion &rhs) const
        {
            return Multiply(*this, rhs);
        }

        /**
         * \brief Multiply a quaternion and a vector and return the result
         * \param rhs The vector to multiply
         * \return The resulting vector
         */
        glm::vec3 operator*(const glm::vec3 &rhs) const
        {
            return Multiply(*this, rhs);
        }

        /**
         * \brief Multiply a quaternion by another quaternion and assign the result to the first quaternion
         * \param rhs The quaternion to multiply
         * \return The resulting quaternion
         */
        Quaternion &operator*=(const Quaternion &rhs)
        {
            *this = Multiply(*this, rhs);
            return *this;
        }

        Quaternion operator-() const
        {
            return Quaternion(-x, -y, -z, -w);
        }

        /**
         * \brief Convert the Quaternion to a glm::quat
         * \return The glm::quat
         */
        operator glm::quat() const
        {
            return glm::quat(w, x, y, z);
        }

        /**
         * \brief Convert the Quaternion to a glm::mat4
         * \return The glm::mat4
         */
        operator glm::mat4() const
        {
            return glm::mat4_cast(static_cast<glm::quat>(*this));
        }

        /**
         * \brief Convert the Quaternion to euler angles
         * \return The euler angles
         */
        operator glm::vec3() const
        {
            return glm::eulerAngles(static_cast<glm::quat>(*this));
        }

        /**
         * \brief Assignment operator to assign Euler angles to the Quaternion.
         * \param euler The Euler angles (in degrees) to assign to the Quaternion.
         * \return A reference to the Quaternion after assignment.
         */
        Quaternion &operator=(const glm::vec3 &euler)
        {
            *this = Quaternion(euler);
            return *this;
        }

        /**
         * \brief Return the normalized version of the quaternion
         * \return The normalized quaternion
         */
        Quaternion normalized() const
        {
            glm::quat rotationQuat(w, x, y, z);
            rotationQuat = glm::normalize(rotationQuat);
            return Quaternion(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w);
        }

        /**
         * \brief Normalize the quaternion
         */
        void Normalize()
        {
            *this = normalized();
        }

        /**
         * \brief Return the conjugate of the quaternion
         * \return The conjugate quaternion
         */
        Quaternion Conjugate() const
        {
            return Quaternion(-x, -y, -z, w);
        }

        /**
         * \brief Return the quaternion string representation
         * \return The quaternion string representation
         */
        std::string ToString() const
        {
            return std::string("(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")\n");
        }
    };
}

#endif // __QUATERNION_H__
