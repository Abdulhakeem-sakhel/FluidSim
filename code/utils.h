#pragma once
#include <cmath>
#include <raylib.h>
namespace Vec2Ops {
    inline void normalize(Vector2 &v1) {
        float length = std::sqrt(v1.x*v1.x + v1.y*v1.y);
        v1.x /= length;
        v1.y /= length;
    }

    inline float length(const Vector2 &v1) {
        return std::sqrt(v1.x*v1.x + v1.y*v1.y);
    }

     inline float length2(const Vector2 &v1) {
        return v1.x*v1.x + v1.y*v1.y;
    }


    inline Vector2 getNormal(const Vector2 &v1) {
        return Vector2{v1.y, v1.x};
    } 

    inline float dot(const Vector2 &v1,const Vector2 &v2) {
        return v1.x*v2.x + v1.y*v2.y;
    }

    const Vector2 DOWN = {0,1};
    const Vector2 ZERO = {0,0};

    inline Vector2 add(const Vector2 &v1,const Vector2 &v2) {
        return Vector2{v1.x+v2.x, v1.y+v2.y};
    }

    inline Vector2 sub(const Vector2 &v1,const Vector2 &v2) {
        return Vector2{v1.x-v2.x, v1.y-v2.y};
    }

    inline Vector2 scale(const Vector2 &v1,float f1) {
        return Vector2{v1.x*f1, v1.y*f1};
    }
}

namespace myMaths {
    inline int generateRandomNumber(int min, int max) {
    if (min >= max) {
        //std::cout << ("Error: min must be less than max.\n");
        return -1;
    }
    return (rand() % (max - min + 1)) + min;
    }
    
    inline float randf() {return static_cast<float>(rand()) / static_cast <float> (RAND_MAX); }
}