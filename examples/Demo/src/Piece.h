#pragma once

#include "Surface.h"

#include "oxygine/math/Vector2.h"

namespace oxygine { class Sprite; }

//class Point {};
using Mass = float;
using Friction = float;
class Camera_ {};

class Piece {
public:
    oxygine::Sprite* _sprite;
    Piece* _parent;             // Null if root or moving.
    Piece* _childFirst;         // Null if no children.
    Piece* _siblingPrev;        // Null if first sibling.
    Piece* _siblingNext;        // Null if last sibling.
    oxygine::Vector2 _position;                 // Raw position on screen. Affected by animation offsets.
    oxygine::Vector2 _positionOnParent;         // Fixed in parent reference frame. Not affected by animation offsets.
    oxygine::Vector2 _positionOnParentOffset;   // For subtle animation during parent movement, animating after collision, and while being moved.
    oxygine::Vector2 _centerCollision;
    oxygine::Vector2 _centerImage;
    int _orientation;
    int _orientationTotal;
    Mass _mass;
    Mass _massOfChildren;
    Friction _frictionToChildren;
    Friction _frictionToParent;
    Surface _surfaceCollision;  // 2D, top-down collision surface.
    Surface _surfaceSupport;    // 2D, top-down support surface.
    //Surface _surfaceImage;      // 2D, image surface. Just use non-alpha pixels.
    //Surface _surfaceTouch;      // 2D, touch surface. Larger than image and Changes intelligently based on occlusion.
};