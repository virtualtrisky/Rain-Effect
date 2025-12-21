#ifndef UTILS_HPP
#define UTILS_HPP

struct Vector2f {
    float x, y;
};

struct Raindrop {
    Vector2f position;
    Uint8 z_index;
    float size;
    Vector2f speed;
};

#endif
