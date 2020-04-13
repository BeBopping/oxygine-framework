#pragma once

#include "Piece.h"
#include "Simulation.h"

#include "oxygine/res/ResAnim.h"
#include "oxygine/actor/Sprite.h"
#include "oxygine/math/Vector2.h"
#include "oxygine/math/Vector4.h"

namespace Actions {
    Piece createPiece(Simulation& sim, oxygine::Sprite& sprite, oxygine::ResAnim& res, oxygine::Vector2 pos);
    void detach(Simulation& sim, Piece& piece);
    void lift(Simulation& sim, Piece& piece, oxygine::Vector2 pos);
    void drop(Simulation& sim, Piece& piece);
    void rotate(Simulation& sim, Piece& piece, bool clockwise = true, int count = 1);
    void toggle(Simulation& sim, Piece& piece);
    void undo(Simulation& sim);
    void redo(Simulation& sim, int id);
};

namespace Helpers {
    //Simulation* __sim = nullptr;

    // TODO: Decide if the root should be included by using a templated bool value?
    template <typename F> F traversePreOrder(Simulation& sim, Piece& piece, F func);
    template <typename F> F traversePostOrder(Simulation& sim, Piece& piece, F func);
    template <typename F> F traverseDescendantsPreOrder(Simulation& sim, Piece& unusedRoot, F func);
    template <typename F> F traverseDescendantsPostOrder(Simulation& sim, Piece& unusedRoot, F func);

    bool collides(Piece& lhs, Piece& rhs);
    bool canSupport(Piece& lhs, Piece& rhs);
    Vector2 getSupportPos(Piece& lhs, Piece& rhs);
    Vector2 getExpellPos(Piece& lhs, Piece& rhs);

    void setPosition(Piece& piece, Vector2 pos);
}

// *****************************************************************************
// Impl
// *****************************************************************************

namespace Helpers {
    // TODO: Decide if the root should be included by using a templated bool value?
    template <typename F>
    F traversePreOrder(Simulation& sim, Piece& root, F func) {
        Piece* piece = &root;

        while (true) {
            func(*piece);

            if (piece->_childFirst) {
                piece = piece->_childFirst;
            }
            else {
                while (!piece->_siblingNext) {
                    if (piece == &root) {
                        return func;
                    }

                    piece = piece->_parent;
                }

                if (piece == &root) {
                    return func;
                }

                piece = piece->_siblingNext;
            }
        }
    }

    template <typename F>
    F traversePostOrder(Simulation& sim, Piece& root, F func) {
        Piece* piece = &root;

        while (true) {
            if (piece->_childFirst) {
                piece = piece->_childFirst;
            }
            else {
                while (!piece->_siblingNext) {
                    Piece* p = piece;
                    piece = piece->_parent;

                    func(*p);

                    if (p == &root) {
                        return func;
                    }
                }

                Piece* p = piece;
                piece = piece->_siblingNext;

                func(*p);

                if (p == &root) {
                    return func;
                }
            }
        }
    }

    template <typename F>
    F traverseDescendantsPreOrder(Simulation& sim, Piece& unusedRoot, F func) {
        Piece* piece = unusedRoot._childFirst;

        if (!piece) return func;

        while (true) {
            func(*piece);

            if (piece->_childFirst) {
                piece = piece->_childFirst;
            }
            else {
                while (!piece->_siblingNext) {
                    piece = piece->_parent;

                    if (piece == &unusedRoot) {
                        return func;
                    }
                }

                piece = piece->_siblingNext;
            }
        }
    }

    template <typename F>
    F traverseDescendantsPostOrder(Simulation& sim, Piece& unusedRoot, F func) {
        Piece* piece = unusedRoot._childFirst;

        if (!piece) return func;

        while (true) {
            if (piece->_childFirst) {
                piece = piece->_childFirst;
            }
            else {
                while (!piece->_siblingNext) {
                    Piece* p = piece;
                    piece = piece->_parent;

                    func(*p);

                    if (piece == &unusedRoot) {
                        return func;
                    }
                }

                Piece* p = piece;
                piece = piece->_siblingNext;

                func(*p);
            }
        }
    }

    inline bool collides(Piece& lhs, Piece& rhs)
    {
        return SurfaceHelpers::collides(lhs._position, lhs._surfaceCollision, rhs._position, rhs._surfaceCollision);
    }

    inline bool canSupport(Piece& lhs, Piece& rhs)
    {
        return SurfaceHelpers::canContain(lhs._surfaceSupport, rhs._surfaceCollision) &&
               SurfaceHelpers::contains(lhs._position,
                                        lhs._surfaceSupport,
                                        rhs._position,
                                        SurfaceHelpers::Create<SurfaceType::POINT>());
    }

    inline Vector2 getSupportPos(Piece& lhs, Piece& rhs)
    {
        return SurfaceHelpers::containSurface(lhs._position, lhs._surfaceSupport, rhs._position, rhs._surfaceCollision);
    }

    inline Vector2 getExpellPos(Piece& lhs, Piece& rhs)
    {
        return SurfaceHelpers::expellSurface(lhs._position, lhs._surfaceCollision, rhs._position, rhs._surfaceCollision);
    }

    inline void setPosition(Piece& piece, Vector2 pos)
    {
        piece._position = pos;

        if (piece._parent) {
            piece._positionOnParent = piece._position - piece._parent->_position;
        }

        RectF bBox = SurfaceHelpers::getBBox(piece._surfaceCollision, piece._position);
        short zIndex = (short)bBox.getTop();

        piece._sprite->setPosition(piece._position);
        piece._sprite->setPriority(zIndex); //0->32000
    }
}

namespace Actions {
    inline Piece createPiece(Simulation& sim, oxygine::Sprite& sprite, oxygine::ResAnim& res, oxygine::Vector2 pos)
    {
        //if (!__sim) __sim = sim;

        std::string surfaceTouchType = res.getAttribute("surface_touch_type").as_string();

        sprite.setResAnim(&res);
        sprite.setAnchor(res.getAttribute("center_x").as_float(),
                         res.getAttribute("center_y").as_float());
        sprite.setSize(res.getAttribute("size_x").as_float(),
                       res.getAttribute("size_y").as_float());
        sprite.setExtendedClickArea(10);
        if (surfaceTouchType == "NONE") {
            sprite.setTouchEnabled(false);
        }

        Piece piece;
        piece._sprite = &sprite;
        piece._parent = nullptr;
        piece._childFirst = nullptr;
        piece._siblingPrev = nullptr;
        piece._siblingNext = nullptr;
        //piece._position = pos;
        piece._positionOnParent = oxygine::Vector2();
        piece._positionOnParentOffset = oxygine::Vector2();
        piece._centerCollision = oxygine::Vector2();
        piece._centerImage = oxygine::Vector2();
        piece._orientation = 0;
        piece._orientationTotal = 1;
        piece._mass = 1.0f;
        piece._massOfChildren = 0.0f;
        piece._frictionToChildren = 1.0f;
        piece._frictionToParent = 1.0f;

        std::string surfaceBottomType = res.getAttribute("surface_bottom_type").as_string();
        std::string surfaceTopType = res.getAttribute("surface_top_type").as_string();

        piece._surfaceCollision =
            Surface{SurfaceType::fromString(surfaceBottomType),
                    res.getAttribute("surface_bottom_x").as_float(),
                    res.getAttribute("surface_bottom_y").as_float(),
                    res.getAttribute("surface_bottom_z").as_float()};

        piece._surfaceSupport =
            Surface{ SurfaceType::fromString(surfaceTopType),
                    res.getAttribute("surface_top_x").as_float(),
                    res.getAttribute("surface_top_y").as_float(),
                    res.getAttribute("surface_top_z").as_float()};

        //piece._surfaceImage;
        //piece._surfaceTouch;

        Helpers::setPosition(piece, pos);

        return piece;
    }

    inline void detach(Simulation& sim, Piece& piece)
    {
        // Only change things if we have to detach the piece from an existing
        // parent.
        if (piece._parent) {
            // Lower the parent's additional mass.
            piece._parent->_massOfChildren -= piece._mass + piece._massOfChildren;

            // Discontinue animations. Position is where it is.
            piece._positionOnParentOffset = oxygine::Vector2();
            piece._positionOnParent = oxygine::Vector2();

            // Update Parent's first child and prev sibling's next sibling.
            if (piece._parent->_childFirst == &piece) {
                assert(!piece._siblingPrev);
                piece._parent->_childFirst = piece._siblingNext;
            }
            else {
                assert(piece._siblingPrev);
                piece._siblingPrev->_siblingNext = piece._siblingNext;
            }

            // Update next sibling's prev sibling.
            if (piece._siblingNext) {
                piece._siblingNext->_siblingPrev = piece._siblingPrev;
            }

            // Remove Pieces reference to parent and siblings.
            piece._parent = nullptr;
            piece._siblingPrev = nullptr;
            piece._siblingNext = nullptr;
        }
    }
    
    inline void lift(Simulation& sim, Piece& piece, oxygine::Vector2 pos)
    {
        // Detach from parent.
        detach(sim, piece);

        // Set the current movement and the new offset.
        Helpers::setPosition(piece, pos);

        // Update all descendant positions.
        Helpers::traverseDescendantsPreOrder(sim, piece, [](Piece& p)
        {
            // TODO: Offests...
            p._positionOnParentOffset = oxygine::Vector2();
            Helpers::setPosition(p, p._parent->_position + p._positionOnParent);
        });
    }

    void drop(Simulation& sim, Piece& piece)
    {
        assert(sim._root);
        assert(!piece._parent);
        assert(!piece._siblingPrev);
        assert(!piece._siblingNext);

        // Find the farthest descendant that can support this piece.
        // TODO: Early out instead of doing a full tree traversal.
        Piece* target = sim._root;
        Helpers::traversePreOrder(sim, *sim._root, [&target, &piece](Piece& p)
        {
            if (&piece != &p && Helpers::canSupport(p, piece)) {
                target = &p;
            }
        });

        int count = 0;
        Vector2 newPos = Vector2(0, 0);
        std::vector<Piece*> movingPieces;

        // Settle Piece within target.
        if (!SurfaceHelpers::contains(target->_position, target->_surfaceSupport, piece._position, piece._surfaceCollision)) {
            newPos += Helpers::getSupportPos(*target, piece);
            ++count;

            // Temporarly move piece.
            lift(sim, piece, newPos);
        }

        // Handle Collision with other pieces.
        // TODO: Get all pieces that are collided with based on moving the piece after each collision.
        //       THEN average those positions.
        Helpers::traverseDescendantsPreOrder(sim, *target, [&sim, &piece, &newPos, &count, &movingPieces](Piece& p)
        {
            if (Helpers::collides(p, piece)) {
                if (SurfaceHelpers::canContain(p._surfaceSupport, piece._surfaceCollision)) {
                    newPos += Helpers::getExpellPos(p, piece);
                    ++count;
                }
                else {
                    movingPieces.push_back(&p);
                }
            }
        });

        // Move to the average new location.
        if (count) {
            newPos /= count;
            lift(sim, piece, newPos);
        }

        // Insert piece as the first child of target.
        piece._parent = target;
        piece._siblingNext = target->_childFirst;
        target->_childFirst = &piece;

        if (piece._siblingNext) {
            piece._siblingNext->_siblingPrev = &piece;
        }

        // Update target mass of children.
        target->_massOfChildren += piece._mass + piece._massOfChildren;

        // Update piece relative position.
        piece._positionOnParent = piece._position - target->_position;

        // Update secondary moving pieces
        for (Piece* p : movingPieces) {
            Helpers::setPosition(*p, Helpers::getExpellPos(piece, *p));
        }
    }

    void rotate(Simulation* sim, Piece* piece, bool clockwise = true, int count = 1) {}
    void toggle(Simulation* sim, Piece* piece) {}
    void undo(Simulation* sim) {}
    void redo(Simulation* sim, int id) {}
};

//Start with human level interactions
/*
    Board/Piece Has
        2D, top-down collision surface.
        2D, image surface. (Just use non-alpha pixels).
        2D, touch surface. (Larger than image and Changes intelligently based on occlusion).
        Collision Center.
        Image Center.
        Orientation n of N.
        Position.
        Parent.
        Pos on Parent (In parent reference frame - same even after parent rotation).
        Pos render offset on parent. (For during parent movement, and for animating colision resolution?)
        Mass
        Mass of children
        Friction Coeficient to children.
        Friction Coeficient to parent.

    Move board by touch/drag anywhere on screen.
        Animate the movement?
        Pieces move with it, but show some independant ease in and out based on momentum, etc.

    Rotate board by 90 degrees. (1 / 4)
        Animates the rotation.
        Pieces move with it, but show some independant ease in and out based on momentum, etc.

    Put away board by moving into cupboard/chest.
    Put away board (and pieces) by menu selection.
    Put away piece by moving into cupboard/chest.
    Put away piece by menu selection.

    Move piece by touch/drag anywhere on screen.
        Animate Movement?
        Collision only happens on let-go.
        Collision with another pieces dislocates the other piece. (In direction of minimal collision or in the direction of piece movement, based on collision amount)
        No cascading collision? Just let them intersect? Or queue the collisions and cycle x times a frame. No moves while cycling?
*/