#pragma once

#include "oxygine/math/Vector2.h"
#include "oxygine/math/Rect.h"
#include <cmath>
#include <string_view>

class Surface {
public:
    unsigned int _type;
    float _a;
    float _b;
    float _c;
};

namespace SurfaceType {
    enum ENUM : unsigned int {
        NONE,
        POINT,
        CIRCLE,
        RECTANGLE,
        //CAPSULE,
        //OVAL,
    };

    inline SurfaceType::ENUM fromString(std::string_view str) {
        if (str == "NONE") { return SurfaceType::NONE; }
        if (str == "POINT") { return SurfaceType::POINT; }
        if (str == "CIRCLE") { return SurfaceType::CIRCLE; }
        if (str == "RECTANGLE") { return SurfaceType::RECTANGLE; }
        assert(false); return SurfaceType::NONE;
    }
};

namespace SurfaceHelpers {
    //Surface Create(SurfaceType::ENUM surfaceType, float a, float b, float c);
    template <SurfaceType::ENUM T> Surface Create();
    template <SurfaceType::ENUM T> Surface Create(float);
    template <SurfaceType::ENUM T> Surface Create(float, float);
    template <SurfaceType::ENUM T> Surface Create(float, float, float);
    template <> Surface Create<SurfaceType::NONE>();
    template <> Surface Create<SurfaceType::POINT>();
    template <> Surface Create<SurfaceType::CIRCLE>(float r);
    template <> Surface Create<SurfaceType::RECTANGLE>(float x, float y);

    float getWidth(Surface sur)
    {
        switch (sur._type) {
            case SurfaceType::NONE: return 0.0f;
            case SurfaceType::POINT: return 0.0f;
            case SurfaceType::CIRCLE: return sur._a * 2.0f;;
            case SurfaceType::RECTANGLE: return sur._a;
            default: assert(false); return 0.0f;
        }
    }

    float getHeight(Surface sur)
    {
        switch (sur._type) {
            case SurfaceType::NONE: return 0.0f;
            case SurfaceType::POINT: return 0.0f;
            case SurfaceType::CIRCLE: return sur._a * 2.0f;;
            case SurfaceType::RECTANGLE: return sur._b;
            default: assert(false); return 0.0f;
        }
    }

    float getLongestSection(Surface sur)
    {
        switch (sur._type) {
            case SurfaceType::NONE: return 0.0f;
            case SurfaceType::POINT: return 0.0f;
            case SurfaceType::CIRCLE: return sur._a * 2.0f;;
            case SurfaceType::RECTANGLE: return std::sqrt(sur._a*sur._a + sur._b*sur._b);
            default: assert(false); return 0.0f;
        }
    }

    float getShortestSection(Surface sur)
    {
        switch (sur._type) {
            case SurfaceType::NONE: return 0.0f;
            case SurfaceType::POINT: return 0.0f;
            case SurfaceType::CIRCLE: return sur._a * 2.0f;;
            case SurfaceType::RECTANGLE: return std::min(sur._a, sur._b);
            default: assert(false); return 0.0f;
        }
    }

    bool canContain(Surface surLhs, Surface surRhs);
    template <SurfaceType::ENUM LHS, SurfaceType::ENUM RHS> bool canContain(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::NONE, SurfaceType::NONE>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::NONE, SurfaceType::POINT>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::NONE, SurfaceType::CIRCLE>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::NONE, SurfaceType::RECTANGLE>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::POINT, SurfaceType::NONE>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::POINT, SurfaceType::POINT>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::POINT, SurfaceType::CIRCLE>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::POINT, SurfaceType::RECTANGLE>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::CIRCLE, SurfaceType::NONE>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::CIRCLE, SurfaceType::POINT>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::RECTANGLE, SurfaceType::NONE>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::RECTANGLE, SurfaceType::POINT>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(Surface surLhs, Surface surRhs);
    template <> bool canContain<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(Surface surLhs, Surface surRhs);

    bool collides(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <SurfaceType::ENUM LHS, SurfaceType::ENUM RHS> bool collides(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::NONE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::NONE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::NONE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::NONE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::POINT, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::POINT, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::POINT, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::POINT, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::CIRCLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::CIRCLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::RECTANGLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::RECTANGLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool collides<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);

    bool contains(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <SurfaceType::ENUM LHS, SurfaceType::ENUM RHS> bool contains(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::NONE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::NONE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::NONE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::NONE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::POINT, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::POINT, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::POINT, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::POINT, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::CIRCLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::CIRCLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::RECTANGLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::RECTANGLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> bool contains<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    
    Vector2 containSurface(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <SurfaceType::ENUM LHS, SurfaceType::ENUM RHS> Vector2 containSurface(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::NONE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::NONE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::NONE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::NONE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::POINT, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::POINT, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::POINT, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::POINT, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::CIRCLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::CIRCLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::RECTANGLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::RECTANGLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 containSurface<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);

    Vector2 expellSurface(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <SurfaceType::ENUM LHS, SurfaceType::ENUM RHS> Vector2 expellSurface(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::NONE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::NONE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::NONE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::NONE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::POINT, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::POINT, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::POINT, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::POINT, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::CIRCLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::CIRCLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::RECTANGLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::RECTANGLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);
    template <> Vector2 expellSurface<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs);

    RectF getBBox(Surface sur, Vector2 pos = Vector2());
    template <SurfaceType::ENUM T> RectF getBBox(Surface sur, Vector2 pos = Vector2());
    template <> RectF getBBox<SurfaceType::NONE>(Surface sur, Vector2 pos);
    template <> RectF getBBox<SurfaceType::POINT>(Surface sur, Vector2 pos);
    template <> RectF getBBox<SurfaceType::CIRCLE>(Surface sur, Vector2 pos);
    template <> RectF getBBox<SurfaceType::RECTANGLE>(Surface sur, Vector2 pos);
};

namespace SurfaceHelpers {
    template <> Surface Create<SurfaceType::NONE>()
    {
        return Surface{ SurfaceType::NONE, 0.0f, 0.0f, 0.0f};
    }

    template <> Surface Create<SurfaceType::POINT>()
    {
        return Surface{ SurfaceType::POINT, 0.0f, 0.0f, 0.0f };
    }

    template <> Surface Create<SurfaceType::CIRCLE>(float r)
    {
        return Surface{ SurfaceType::CIRCLE, r, 0.0f, 0.0f };
    }

    template <> Surface Create<SurfaceType::RECTANGLE>(float x, float y)
    {
        return Surface{ SurfaceType::RECTANGLE, x, y, 0.0f };
    }

    inline bool canContain(Surface surLhs, Surface surRhs)
    {
        switch (surLhs._type) {
            case SurfaceType::NONE: return false;
            case SurfaceType::POINT: return false;
            case SurfaceType::CIRCLE:
                switch (surRhs._type) {
                    case SurfaceType::NONE: return canContain<SurfaceType::CIRCLE, SurfaceType::NONE>(surLhs, surRhs);
                    case SurfaceType::POINT: return canContain<SurfaceType::CIRCLE, SurfaceType::POINT>(surLhs, surRhs);
                    case SurfaceType::CIRCLE: return canContain<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(surLhs, surRhs);
                    case SurfaceType::RECTANGLE: return canContain<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(surLhs, surRhs);
                    default: assert(false); return false;
                }
            case SurfaceType::RECTANGLE:
                switch (surRhs._type) {
                    case SurfaceType::NONE: return canContain<SurfaceType::RECTANGLE, SurfaceType::NONE>(surLhs, surRhs);
                    case SurfaceType::POINT: return canContain<SurfaceType::RECTANGLE, SurfaceType::POINT>(surLhs, surRhs);
                    case SurfaceType::CIRCLE: return canContain<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(surLhs, surRhs);
                    case SurfaceType::RECTANGLE: return canContain<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(surLhs, surRhs);
                    default: assert(false); return false;
                }
            default: assert(false); return false;
        }
    }

    template <> bool canContain<SurfaceType::NONE, SurfaceType::NONE>(Surface surLhs, Surface surRhs) { return false; }
    template <> bool canContain<SurfaceType::NONE, SurfaceType::POINT>(Surface surLhs, Surface surRhs) { return false; }
    template <> bool canContain<SurfaceType::NONE, SurfaceType::CIRCLE>(Surface surLhs, Surface surRhs) { return false; }
    template <> bool canContain<SurfaceType::NONE, SurfaceType::RECTANGLE>(Surface surLhs, Surface surRhs) { return false; }
    template <> bool canContain<SurfaceType::POINT, SurfaceType::NONE>(Surface surLhs, Surface surRhs) { return false; }
    template <> bool canContain<SurfaceType::POINT, SurfaceType::POINT>(Surface surLhs, Surface surRhs) { return false; }
    template <> bool canContain<SurfaceType::POINT, SurfaceType::CIRCLE>(Surface surLhs, Surface surRhs) { return false; }
    template <> bool canContain<SurfaceType::POINT, SurfaceType::RECTANGLE>(Surface surLhs, Surface surRhs) { return false; }
    template <> bool canContain<SurfaceType::CIRCLE, SurfaceType::NONE>(Surface surLhs, Surface surRhs) { return true; }
    template <> bool canContain<SurfaceType::CIRCLE, SurfaceType::POINT>(Surface surLhs, Surface surRhs) { return true; }
    template <> bool canContain<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(Surface surLhs, Surface surRhs)
    {
        return surLhs._a >= surRhs._a;
    }
    template <> bool canContain<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(Surface surLhs, Surface surRhs)
    {
        return surLhs._a >= Vector2(surRhs._a / 2.0f, surRhs._b / 2.0f).length();
    }
    template <> bool canContain<SurfaceType::RECTANGLE, SurfaceType::NONE>(Surface surLhs, Surface surRhs) { return true; }
    template <> bool canContain<SurfaceType::RECTANGLE, SurfaceType::POINT>(Surface surLhs, Surface surRhs) { return true; }
    template <> bool canContain<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(Surface surLhs, Surface surRhs)
    {
        return surLhs._a >= surRhs._a*2.0f &&
               surLhs._b >= surRhs._a*2.0f;
    }
    template <> bool canContain<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(Surface surLhs, Surface surRhs)
    {
        return surLhs._a >= surRhs._a &&
               surLhs._b >= surRhs._b;
    }

    inline bool collides(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::NONE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::NONE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::NONE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::NONE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::NONE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::NONE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::NONE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::NONE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::POINT, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::POINT, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::POINT, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::POINT, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::POINT, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::POINT, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::POINT, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::POINT, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::CIRCLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::CIRCLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::CIRCLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::CIRCLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::RECTANGLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::RECTANGLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::RECTANGLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::RECTANGLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }
    template <> bool collides<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return expellSurface<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs) != posRhs;
    }

    inline bool contains(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        switch (surLhs._type) {
            case SurfaceType::NONE: return false;
            case SurfaceType::POINT: return false;
            case SurfaceType::CIRCLE:
                switch (surRhs._type) {
                    case SurfaceType::NONE: return contains<SurfaceType::CIRCLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::POINT: return contains<SurfaceType::CIRCLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::CIRCLE: return contains<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::RECTANGLE: return contains<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs);
                    default: assert(false); return false;
                }
            case SurfaceType::RECTANGLE:
                switch (surRhs._type) {
                    case SurfaceType::NONE: return contains<SurfaceType::RECTANGLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::POINT: return contains<SurfaceType::RECTANGLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::CIRCLE: return contains<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::RECTANGLE: return contains<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs);
                    default: assert(false); return false;
                }
            default: assert(false); return false;
        }
    }

    template <> bool contains<SurfaceType::NONE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return false; }
    template <> bool contains<SurfaceType::NONE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return false; }
    template <> bool contains<SurfaceType::NONE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return false; }
    template <> bool contains<SurfaceType::NONE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return false; }
    template <> bool contains<SurfaceType::POINT, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return false; }
    template <> bool contains<SurfaceType::POINT, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return false; }
    template <> bool contains<SurfaceType::POINT, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return false; }
    template <> bool contains<SurfaceType::POINT, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return false; }
    template <> bool contains<SurfaceType::CIRCLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return posLhs.distance(posRhs) < surLhs._a;
    }
    template <> bool contains<SurfaceType::CIRCLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return posLhs.distance(posRhs) < surLhs._a;
    }
    template <> bool contains<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return surLhs._a - posLhs.distance(posRhs) >= surRhs._a;
    }
    template <> bool contains<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return surLhs._a - posLhs.distance(posRhs) >= Vector2(surRhs._a/2.0f, surRhs._b/2.0f).length();
    }

    template <> bool contains<SurfaceType::RECTANGLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return
            posLhs.x - surLhs._a / 2.0f < posRhs.x &&
            posLhs.x + surLhs._a / 2.0f > posRhs.x &&
            posLhs.y - surLhs._b / 2.0f < posRhs.y &&
            posLhs.y + surLhs._b / 2.0f > posRhs.y;
    }

    template <> bool contains<SurfaceType::RECTANGLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return
            posLhs.x - surLhs._a / 2.0f < posRhs.x &&
            posLhs.x + surLhs._a / 2.0f > posRhs.x &&
            posLhs.y - surLhs._b / 2.0f < posRhs.y &&
            posLhs.y + surLhs._b / 2.0f > posRhs.y;
    }

    template <> bool contains<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return
            posLhs.x - surLhs._a / 2.0f < posRhs.x - surRhs._a &&
            posLhs.x + surLhs._a / 2.0f > posRhs.x + surRhs._a &&
            posLhs.y - surLhs._b / 2.0f < posRhs.y - surRhs._a &&
            posLhs.y + surLhs._b / 2.0f > posRhs.y + surRhs._a;
    }

    template <> bool contains<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        return
            posLhs.x - surLhs._a / 2.0f < posRhs.x - surRhs._a / 2.0f &&
            posLhs.x + surLhs._a / 2.0f > posRhs.x + surRhs._a / 2.0f &&
            posLhs.y - surLhs._b / 2.0f < posRhs.y - surRhs._b / 2.0f &&
            posLhs.y + surLhs._b / 2.0f > posRhs.y + surRhs._b / 2.0f;
    }

    inline Vector2 containSurface(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        switch (surLhs._type) {
            case SurfaceType::NONE: return posLhs;
            case SurfaceType::POINT: return posLhs;
            case SurfaceType::CIRCLE:
                switch (surRhs._type) {
                    case SurfaceType::NONE: return containSurface<SurfaceType::CIRCLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::POINT: return containSurface<SurfaceType::CIRCLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::CIRCLE: return containSurface<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::RECTANGLE: return containSurface<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs);
                    default: assert(false); return posLhs;
                }
            case SurfaceType::RECTANGLE:
                switch (surRhs._type) {
                    case SurfaceType::NONE: return containSurface<SurfaceType::RECTANGLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::POINT: return containSurface<SurfaceType::RECTANGLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::CIRCLE: return containSurface<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::RECTANGLE: return containSurface<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs);
                    default: assert(false); return posLhs;
                }
            default: assert(false); return posLhs;
        }
    }
    template <> Vector2 containSurface<SurfaceType::NONE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posLhs; }
    template <> Vector2 containSurface<SurfaceType::NONE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posLhs; }
    template <> Vector2 containSurface<SurfaceType::NONE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posLhs; }
    template <> Vector2 containSurface<SurfaceType::NONE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posLhs; }
    template <> Vector2 containSurface<SurfaceType::POINT, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posLhs; }
    template <> Vector2 containSurface<SurfaceType::POINT, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posLhs; }
    template <> Vector2 containSurface<SurfaceType::POINT, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posLhs; }
    template <> Vector2 containSurface<SurfaceType::POINT, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posLhs; }
    template <> Vector2 containSurface<SurfaceType::CIRCLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (contains<SurfaceType::CIRCLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs)) {
            return posRhs;
        }
        else {
            return (posRhs - posLhs).normalizedTo(surLhs._a);
        }
    }
    template <> Vector2 containSurface<SurfaceType::CIRCLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (contains<SurfaceType::CIRCLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs)) {
            return posRhs;
        }
        else {
            return (posRhs - posLhs).normalizedTo(surLhs._a);
        }
    }
    template <> Vector2 containSurface<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (contains<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs)) {
            return posRhs;
        }
        else {
            return (posRhs - posLhs).normalizedTo(surLhs._a - surRhs._a);
        }
    }
    template <> Vector2 containSurface<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (contains<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs)) {
            return posRhs;
        }
        else {
            return (posRhs - posLhs).normalizedTo(surLhs._a - Vector2(surRhs._a / 2.0f, surRhs._b / 2.0f).length());
        }
    }
    template <> Vector2 containSurface<SurfaceType::RECTANGLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (contains<SurfaceType::RECTANGLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs)) {
            return posRhs;
        }
        else {
            Vector2 pos = posRhs;

            if (posLhs.x - surLhs._a / 2.0f > pos.x) {
                pos.x = posLhs.x - surLhs._a / 2.0f;
            }
            else if (posLhs.x + surLhs._a / 2.0f < pos.x) {
                pos.x = posLhs.x + surLhs._a / 2.0f;
            }

            if (posLhs.y - surLhs._b / 2.0f > pos.y) {
                pos.y = posLhs.y - surLhs._b / 2.0f;
            }
            else if (posLhs.y + surLhs._b / 2.0f < pos.y) {
                pos.y = posLhs.y + surLhs._b / 2.0f;
            }

            return pos;
        }
    }
    template <> Vector2 containSurface<SurfaceType::RECTANGLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (contains<SurfaceType::RECTANGLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs)) {
            return posRhs;
        }
        else {
            Vector2 pos = posRhs;

            if (posLhs.x - surLhs._a / 2.0f > pos.x) {
                pos.x = posLhs.x - surLhs._a / 2.0f;
            }
            else if (posLhs.x + surLhs._a / 2.0f < pos.x) {
                pos.x = posLhs.x + surLhs._a / 2.0f;
            }

            if (posLhs.y - surLhs._b / 2.0f > pos.y) {
                pos.y = posLhs.y - surLhs._b / 2.0f;
            }
            else if (posLhs.y + surLhs._b / 2.0f < pos.y) {
                pos.y = posLhs.y + surLhs._b / 2.0f;
            }

            return pos;
        }
    }
    template <> Vector2 containSurface<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (contains<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs)) {
            return posRhs;
        }
        else {
            Vector2 pos = posRhs;

            if (posLhs.x - surLhs._a / 2.0f > pos.x - surRhs._a) {
                pos.x = posLhs.x - surLhs._a / 2.0f + surRhs._a;
            }
            else if (posLhs.x + surLhs._a / 2.0f < pos.x + surRhs._a) {
                pos.x = posLhs.x + surLhs._a / 2.0f - surRhs._a;
            }

            if (posLhs.y - surLhs._b / 2.0f > pos.y - surRhs._a) {
                pos.y = posLhs.y - surLhs._b / 2.0f + surRhs._a;
            }
            else if (posLhs.y + surLhs._b / 2.0f < pos.y + surRhs._a) {
                pos.y = posLhs.y + surLhs._b / 2.0f - surRhs._a;
            }

            return pos;
        }
    }
    template <> Vector2 containSurface<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (contains<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs)) {
            return posRhs;
        }
        else {
            Vector2 pos = posRhs;

            if (posLhs.x - surLhs._a / 2.0f > pos.x - surRhs._a / 2.0f) {
                pos.x = posLhs.x - (surLhs._a - surRhs._a) / 2.0f;
            }
            else if (posLhs.x + surLhs._a / 2.0f < pos.x + surRhs._a / 2.0f) {
                pos.x = posLhs.x + (surLhs._a - surRhs._a) / 2.0f;
            }

            if (posLhs.y - surLhs._b / 2.0f > pos.y - surRhs._a / 2.0f) {
                pos.y = posLhs.y - (surLhs._b - surRhs._b) / 2.0f;
            }
            else if (posLhs.y + surLhs._b / 2.0f < pos.y + surRhs._a / 2.0f) {
                pos.y = posLhs.y + (surLhs._b - surRhs._b) / 2.0f;
            }

            return pos;
        }
    }

    inline Vector2 expellSurface(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        switch (surLhs._type) {
            case SurfaceType::NONE:
                switch (surRhs._type) {
                    case SurfaceType::NONE: return expellSurface<SurfaceType::CIRCLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::POINT: return expellSurface<SurfaceType::CIRCLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::CIRCLE: return expellSurface<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::RECTANGLE: return expellSurface<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs);
                    default: assert(false); return posLhs;
                }
            case SurfaceType::POINT:
                switch (surRhs._type) {
                    case SurfaceType::NONE: return expellSurface<SurfaceType::CIRCLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::POINT: return expellSurface<SurfaceType::CIRCLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::CIRCLE: return expellSurface<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::RECTANGLE: return expellSurface<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs);
                    default: assert(false); return posLhs;
                }
            case SurfaceType::CIRCLE:
                switch (surRhs._type) {
                    case SurfaceType::NONE: return expellSurface<SurfaceType::CIRCLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::POINT: return expellSurface<SurfaceType::CIRCLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::CIRCLE: return expellSurface<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::RECTANGLE: return expellSurface<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs);
                    default: assert(false); return posLhs;
                }
            case SurfaceType::RECTANGLE:
                switch (surRhs._type) {
                    case SurfaceType::NONE: return expellSurface<SurfaceType::RECTANGLE, SurfaceType::NONE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::POINT: return expellSurface<SurfaceType::RECTANGLE, SurfaceType::POINT>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::CIRCLE: return expellSurface<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(posLhs, surLhs, posRhs, surRhs);
                    case SurfaceType::RECTANGLE: return expellSurface<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(posLhs, surLhs, posRhs, surRhs);
                    default: assert(false); return posLhs;
                }
            default: assert(false); return posLhs;
        }
    }
    template <> Vector2 expellSurface<SurfaceType::NONE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posRhs; }
    template <> Vector2 expellSurface<SurfaceType::NONE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posRhs; }
    template <> Vector2 expellSurface<SurfaceType::NONE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (!contains<SurfaceType::CIRCLE, SurfaceType::NONE>(posRhs, surRhs, posLhs, surLhs)) {
            return posRhs;
        }
        else {
            Vector2 dir = posRhs - posLhs;
            if (dir.length() == 0.0f) {
                return posLhs + Vector2(surRhs._a, 0.0f);
            }
            else {
                return posLhs + dir.normalizedTo(surRhs._a);
            }
        }
    }
    template <> Vector2 expellSurface<SurfaceType::NONE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (!contains<SurfaceType::RECTANGLE, SurfaceType::NONE>(posRhs, surRhs, posLhs, surLhs)) {
            return posRhs;
        }
        else {
            Vector2 dir = posRhs - posLhs;
            Vector2 movement = Vector2(surRhs._a / 2.0f - std::fabs(dir.x), surRhs._b / 2.0f - std::fabs(dir.y));

            if (movement.x <= movement.y) {
                if (dir.x >= 0) {
                    return Vector2(posLhs.x + surRhs._a / 2.0f, posRhs.y);
                }
                else {
                    return Vector2(posLhs.x - surRhs._a / 2.0f, posRhs.y);
                }
            }
            else {
                if (dir.y >= 0) {
                    return Vector2(posRhs.x, posLhs.y + surRhs._b / 2.0f);
                }
                else {
                    return Vector2(posRhs.x, posLhs.y - surRhs._b / 2.0f);
                }
            }
        }
    }
    template <> Vector2 expellSurface<SurfaceType::POINT, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posRhs; }
    template <> Vector2 expellSurface<SurfaceType::POINT, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs) { return posRhs; }
    template <> Vector2 expellSurface<SurfaceType::POINT, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (!contains<SurfaceType::CIRCLE, SurfaceType::POINT>(posRhs, surRhs, posLhs, surLhs)) {
            return posRhs;
        }
        else {
            Vector2 dir = posRhs - posLhs;
            if (dir.length() == 0.0f) {
                return posLhs + Vector2(surRhs._a, 0.0f);
            }
            else {
                return posLhs + dir.normalizedTo(surRhs._a);
            }
        }
    }
    template <> Vector2 expellSurface<SurfaceType::POINT, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (!contains<SurfaceType::RECTANGLE, SurfaceType::POINT>(posRhs, surRhs, posLhs, surLhs)) {
            return posRhs;
        }
        else {
            Vector2 dir = posRhs - posLhs;
            Vector2 movement = Vector2(surRhs._a / 2.0f - std::fabs(dir.x), surRhs._b / 2.0f - std::fabs(dir.y));

            if (movement.x <= movement.y) {
                if (dir.x >= 0) {
                    return Vector2(posLhs.x + surRhs._a / 2.0f, posRhs.y);
                }
                else {
                    return Vector2(posLhs.x - surRhs._a / 2.0f, posRhs.y);
                }
            }
            else {
                if (dir.y >= 0) {
                    return Vector2(posRhs.x, posLhs.y + surRhs._b / 2.0f);
                }
                else {
                    return Vector2(posRhs.x, posLhs.y - surRhs._b / 2.0f);
                }
            }
        }
    }
    template <> Vector2 expellSurface<SurfaceType::CIRCLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (!contains<SurfaceType::NONE, SurfaceType::CIRCLE>(posRhs, surRhs, posLhs, surLhs)) {
            return posRhs;
        }
        else {
            Vector2 dir = posRhs - posLhs;
            if (dir.length() == 0.0f) {
                return posLhs + Vector2(surLhs._a, 0.0f);
            }
            else {
                return posLhs + dir.normalizedTo(surLhs._a);
            }
        }
    }
    template <> Vector2 expellSurface<SurfaceType::CIRCLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (!contains<SurfaceType::POINT, SurfaceType::CIRCLE>(posRhs, surRhs, posLhs, surLhs)) {
            return posRhs;
        }
        else {
            Vector2 dir = posRhs - posLhs;
            if (dir.length() == 0.0f) {
                return posLhs + Vector2(surLhs._a, 0.0f);
            }
            else {
                return posLhs + dir.normalizedTo(surLhs._a);
            }
        }
    }
    template <> Vector2 expellSurface<SurfaceType::CIRCLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if ((posRhs - posLhs).length() >= surLhs._a + surRhs._a) {
            return posRhs;
        }
        else {
            Vector2 dir = posRhs - posLhs;
            if (dir.length() == 0.0f) {
                return posLhs + Vector2(surLhs._a + surRhs._a, 0.0f);
            }
            else {
                return posLhs + dir.normalizedTo(surLhs._a + surRhs._a);
            }
        }
    }
    template <> Vector2 expellSurface<SurfaceType::CIRCLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (posLhs.x - surLhs._a >= posRhs.x + surRhs._a / 2.0f ||
            posLhs.x + surLhs._a <= posRhs.x - surRhs._a / 2.0f ||
            posLhs.y - surLhs._a >= posRhs.y + surRhs._b / 2.0f ||
            posLhs.y + surLhs._a <= posRhs.y - surRhs._b / 2.0f)
        {
            return posRhs;
        }
        else {
            Vector2 dir = posRhs - posLhs;
            Vector2 movement = Vector2(surLhs._a + surRhs._a / 2.0f - std::fabs(dir.x),
                surLhs._a + surRhs._b / 2.0f - std::fabs(dir.y));

            if (movement.x <= movement.y) {
                if (dir.x >= 0) {
                    return Vector2(posLhs.x + surLhs._a + surRhs._a / 2.0f, posRhs.y);
                }
                else {
                    return Vector2(posLhs.x - surLhs._a - surRhs._a / 2.0f, posRhs.y);
                }
            }
            else {
                if (dir.y >= 0) {
                    return Vector2(posRhs.x, posLhs.y + surLhs._a + surRhs._b / 2.0f);
                }
                else {
                    return Vector2(posRhs.x, posLhs.y - surLhs._a - surRhs._b / 2.0f);
                }
            }
        }
    }
    template <> Vector2 expellSurface<SurfaceType::RECTANGLE, SurfaceType::NONE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        // TODO:
        assert(false);
        return posRhs;
    }
    template <> Vector2 expellSurface<SurfaceType::RECTANGLE, SurfaceType::POINT>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        // TODO:
        assert(false);
        return posRhs;
    }
    template <> Vector2 expellSurface<SurfaceType::RECTANGLE, SurfaceType::CIRCLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (posLhs.x - surLhs._a / 2.0f >= posRhs.x + surRhs._a ||
            posLhs.x + surLhs._a / 2.0f <= posRhs.x - surRhs._a ||
            posLhs.y - surLhs._b / 2.0f >= posRhs.y + surRhs._a ||
            posLhs.y + surLhs._b / 2.0f <= posRhs.y - surRhs._a)
        {
            return posRhs;
        }
        else {
            Vector2 dir = posRhs - posLhs;
            Vector2 movement = Vector2(surLhs._a / 2.0f + surRhs._a - std::fabs(dir.x),
                surLhs._b / 2.0f + surRhs._a - std::fabs(dir.y));

            if (movement.x <= movement.y) {
                if (dir.x >= 0) {
                    return Vector2(posLhs.x + surLhs._a / 2.0f + surRhs._a, posRhs.y);
                }
                else {
                    return Vector2(posLhs.x - surLhs._a / 2.0f - surRhs._a, posRhs.y);
                }
            }
            else {
                if (dir.y >= 0) {
                    return Vector2(posRhs.x, posLhs.y + surLhs._b / 2.0f + surRhs._a);
                }
                else {
                    return Vector2(posRhs.x, posLhs.y - surLhs._b / 2.0f - surRhs._a);
                }
            }
        }
    }
    template <> Vector2 expellSurface<SurfaceType::RECTANGLE, SurfaceType::RECTANGLE>(Vector2 posLhs, Surface surLhs, Vector2 posRhs, Surface surRhs)
    {
        if (posLhs.x - surLhs._a / 2.0f >= posRhs.x + surRhs._a / 2.0f ||
            posLhs.x + surLhs._a / 2.0f <= posRhs.x - surRhs._a / 2.0f ||
            posLhs.y - surLhs._b / 2.0f >= posRhs.y + surRhs._b / 2.0f ||
            posLhs.y + surLhs._b / 2.0f <= posRhs.y - surRhs._b / 2.0f)
        {
            return posRhs;
        }
        else {
            Vector2 dir = posRhs - posLhs;
            Vector2 movement = Vector2(surLhs._a / 2.0f + surRhs._a / 2.0f - std::fabs(dir.x),
                                       surLhs._b / 2.0f + surRhs._b / 2.0f - std::fabs(dir.y));

            if (movement.x <= movement.y) {
                if (dir.x >= 0) {
                    return Vector2(posLhs.x + surLhs._a / 2.0f + surRhs._a / 2.0f, posRhs.y);
                }
                else {
                    return Vector2(posLhs.x - surLhs._a / 2.0f - surRhs._a / 2.0f, posRhs.y);
                }
            }
            else {
                if (dir.y >= 0) {
                    return Vector2(posRhs.x, posLhs.y + surLhs._b / 2.0f + surRhs._b / 2.0f);
                }
                else {
                    return Vector2(posRhs.x, posLhs.y - surLhs._b / 2.0f - surRhs._b / 2.0f);
                }
            }
        }
    }

    inline RectF getBBox(Surface sur, Vector2 pos /*= Vector2()*/)
    {
        switch (sur._type) {
            case SurfaceType::NONE: return getBBox< SurfaceType::NONE>(sur, pos);
            case SurfaceType::POINT:  return getBBox< SurfaceType::POINT>(sur, pos);
            case SurfaceType::CIRCLE:  return getBBox< SurfaceType::CIRCLE>(sur, pos);
            case SurfaceType::RECTANGLE:  return getBBox< SurfaceType::RECTANGLE>(sur, pos);
            default: assert(false); return getBBox< SurfaceType::NONE>(sur, pos);
        }
    }
    template <> RectF getBBox<SurfaceType::NONE>(Surface sur, Vector2 pos)
    {
        return RectF(pos, Vector2());
    }
    template <> RectF getBBox<SurfaceType::POINT>(Surface sur, Vector2 pos)
    {
        return RectF(pos, Vector2());
    }
    template <> RectF getBBox<SurfaceType::CIRCLE>(Surface sur, Vector2 pos)
    {
        return RectF(pos - Vector2(sur._a, sur._a), Vector2(sur._a * 2.0f, sur._a * 2.0f));
    }
    template <> RectF getBBox<SurfaceType::RECTANGLE>(Surface sur, Vector2 pos)
    {
        return RectF(pos - Vector2(sur._a / 2.0f, sur._b / 2.0f), Vector2(sur._a, sur._b));
    }
};