#ifndef VECTOR2_HH
#define VECTOR2_HH

typedef struct
{
    float x;
    float y;
} Vector2;

Vector2 add(const Vector2 &v1, const Vector2 &v2)
{
    Vector2 v3;
    v3.x = v1.x + v2.x;
    v3.y = v1.x + v2.x;
    return v3;
}

Vector2 subtract(const Vector2 &v1, const Vector2 &v2)
{
    Vector2 v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.x - v2.x;
    return v3;
}

#endif // VECTOR2_HH
