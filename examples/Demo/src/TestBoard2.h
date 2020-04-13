#pragma once
#include "test.h"

#include "Piece.h"
#include "PieceHelpers.h"
#include "ResCollection.h"
#include "Simulation.h"
#include "Surface.h"

DECLARE_SMART(PhysicalSprite, spPhysicalSprite);
class PhysicalSprite : public Sprite
{
private:
    Vector2 _local = {};
    pointer_index _pointerIndex = {};
    int _orientation = {};
    int _orientationTotal = 1;
    float _mass = 1.0f;
    float _massOfChildren = {};
    float _frictionToChildren = 1.0f;
    float _frictionToParent = 1.0f;
    Surface _surfaceCollision = {}; // 2D, top-down collision surface.
    Surface _surfaceSupport = {};   // 2D, top-down support surface.

public:
    PhysicalSprite(ResAnim& res, Vector2 pos)
    {
        static int n = 0;
        ++n;
        std::string surfaceTouchType = res.getAttribute("surface_touch_type").as_string();

        setResAnim(&res);
        setAnchor(res.getAttribute("center_x").as_float(),
                  res.getAttribute("center_y").as_float());
        setSize(res.getAttribute("size_x").as_float(),
                res.getAttribute("size_y").as_float());
        setExtendedClickArea(10);
        if (surfaceTouchType == "NONE") {
            setTouchEnabled(false);
        }
        setName(res.getName() + std::to_string(n));

        _orientation = 0;
        _orientationTotal = 1;
        _mass = 1.0f;
        _massOfChildren = 0.0f;

        _frictionToChildren = 1.0f;
        _frictionToParent = 1.0f;

        std::string surfaceBottomType = res.getAttribute("surface_bottom_type").as_string();
        std::string surfaceTopType = res.getAttribute("surface_top_type").as_string();

        _surfaceCollision =
            Surface{SurfaceType::fromString(surfaceBottomType),
                    res.getAttribute("surface_bottom_x").as_float(),
                    res.getAttribute("surface_bottom_y").as_float(),
                    res.getAttribute("surface_bottom_z").as_float()};

        _surfaceSupport =
            Surface{ SurfaceType::fromString(surfaceTopType),
                    res.getAttribute("surface_top_x").as_float(),
                    res.getAttribute("surface_top_y").as_float(),
                    res.getAttribute("surface_top_z").as_float()};

        //piece._surfaceImage;
        //piece._surfaceTouch;

        setAnchorOnStage(pos);
    }

    void setAnchorOnStage(const Vector2& pos)
    {
        Sprite::setPosition(getPosition() + getStageToAnchor(pos));

        RectF bBox = SurfaceHelpers::getBBox(_surfaceCollision, getAnchorToStage());
        short zIndex = (short)bBox.getTop();
        setPriority(zIndex); //0->32000
    }

    void onEventTouchDown(Event* ev)
    {
        if (ev->phase != Event::Phase::phase_target) {
            return;
        }

        TouchEvent* te = safeCast<TouchEvent*>(ev);

        // Ignore secondary touches.
        if (te->index != _pointerIndex && _pointerIndex != 0) {
            return;
        }

        _pointerIndex = te->index;
        _local = te->localPosition;
        _stage->addEventListener(TouchEvent::MOVE, CLOSURE(this, &PhysicalSprite::onEventMove));
        _stage->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &PhysicalSprite::onEventTouchUp));

        ev->stopImmediatePropagation();
    }

    void onEventMove(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);

        move(te->localPosition);

        ev->stopImmediatePropagation();
    }

    void onEventTouchUp(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);

        move(te->localPosition);
        drop();

        _stage->removeEventListeners(this);
        _pointerIndex = 0;

        ev->stopImmediatePropagation();
    }

    void move(const Vector2& pos)
    {
        Vector2 localPos = stage2local(pos);
        Vector2 offset = localPos - _local;

        Transform tr = getTransform();
        tr.x = 0;
        tr.y = 0;
        Vector2 p = tr.transform(offset);
        //Sprite::setPosition(getPosition() + p);

        lift(getAnchorToStage(p));
    }

    void doUpdate(const UpdateState& us)
    {
        //pointer_index ind = getPressed();
        //if (!ind)
        //    return;
        //PointerState* st = Input::instance.getTouchByIndex(ind);
        //move(_stage->parent2local(st->getPosition()));
    }

    void onAdded2Stage()
    {
        if (getTouchEnabled()) {
            addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &PhysicalSprite::onEventTouchDown));
        }
    }

    void onRemovedFromStage()
    {
        _stage->removeEventListeners(this);
    }

    bool isOn(const Vector2& localPosition, float localScale) override
    {
        //RectF r = getDestRect();
        //r.expand(Vector2(_extendedIsOn, _extendedIsOn), Vector2(_extendedIsOn, _extendedIsOn));

        //auto pos = (localPosition - r.getCenter()) / 1.5 + r.getCenter();

        //return __super::isOn(pos, localScale);
        return __super::isOn(localPosition, localScale);
        //return result;
        /*localScale = 1.2f;

        RectF r = getDestRect();
        r.expand(Vector2(_extendedIsOn, _extendedIsOn), Vector2(_extendedIsOn, _extendedIsOn));

        auto pos = (localPosition - r.getCenter()) / localScale + r.getCenter();

        if (!r.pointIn(pos))
        {
            return false;
        }

        if (_extendedIsOn)
            return true;

        const HitTestData& ad = _frame.getHitTestData();
        if (!ad.data)
            return true;

        const int BITS = (sizeof(int32_t) * 8);

        const unsigned char* buff = ad.data;

        pos = pos * _frame.getResAnim()->getAppliedScale();
        pos = pos.div(_localScale);
        Point lp = pos.cast<Point>() / 4;
        Rect rs(0, 0, ad.w, ad.h);
        if (rs.pointIn(lp))
        {
            const int32_t* ints = reinterpret_cast<const int32_t*>(buff + lp.y * ad.pitch);

            int n = lp.x / BITS;
            int b = lp.x % BITS;

            return (ints[n] >> b) & 1;
        }
        return false;*/
    }

    void detach()
    {
        // Only change things if we have to detach the piece from an existing
        // parent.
        if (!hasNullParent()) {
            spPhysicalSprite sp = this;
            Vector2 stagePos = getAnchorToStage();
            getPieces()->addChild(this);
            setAnchorOnStage(stagePos);

            //Actor* oldParent = Sprite::detach();
            //getPieces()->addChild(this);
        }
    }

    void lift(Vector2 stagePos)
    {
        // Detach from parent.
        //detach();

        // Set the current movement and the new offset.
        setAnchorOnStage(stagePos);
        //setAnchorOnStage(local2parent(stage2local(stagePos - getAnchor().mult(getSize()))));

        // Update all descendant positions.
        // traverseDescendantsPreOrder([](Actor* a)
        // {
        //     //PhysicalSprite* piece = dynamic_cast<PhysicalSprite*>(a);
        //     // TODO: Offests...
        //     a->setPosition(p, p._parent->_position + p._positionOnParent);
        // });
    }

    void drop()
    {
        // Find the farthest descendant that can support this piece.
        // TODO: Early out instead of doing a full tree traversal.
        PhysicalSprite* target = getPieces();
        target->traversePreOrder([&target, this](Actor* a)
        {
            PhysicalSprite* piece = dynamic_cast<PhysicalSprite*>(a);

            if (piece == this) {
                return false;
            }

            if (piece->canSupport(this)) {
                target = piece;
            }

            return true;
        });

        int count = 0;
        Vector2 newPos = Vector2(0, 0);
        std::vector<PhysicalSprite*> movingPieces;

        // Settle Piece within target.
        if (!SurfaceHelpers::contains(target->getAnchorToStage(),
                                      target->_surfaceSupport,
                                      getAnchorToStage(),
                                      _surfaceCollision))
        {
            //newPos += target->getSupportPos(this);
            //++count;

            // Temporarily move piece.
            lift(target->getSupportPos(this));
        }

        // Handle Collision with other pieces.
        // TODO: Get all pieces that are collided with based on moving the piece after each collision.
        //       THEN average those positions.
        target->traverseDescendantsPreOrder([this, &newPos, &count, &movingPieces](Actor* a)
        {
            PhysicalSprite* piece = dynamic_cast<PhysicalSprite*>(a);

            if (piece == this) {
                return false; // Stop processing children.
            }

            if (piece->collides(this)) {
                if (SurfaceHelpers::canContain(piece->_surfaceSupport, this->_surfaceCollision)) {
                    // Other piece is "too big" to be displaced. Displace the piece that we are dropping.
                    newPos += piece->getExpellPos(this);
                    ++count;
                }
                else {
                    // Other piece is small enough to be pushed arround.
                    movingPieces.push_back(piece);
                }
            }

            return true;
        });

        // Move to the average new location.
        if (count) {
            newPos /= count;
            lift(newPos);
        }

        // Insert piece as the first child of target.
        {
            spPhysicalSprite sp = this;
            Vector2 stagePos = getAnchorToStage();
            target->addChild(this);
            setAnchorOnStage(stagePos);
        }

        // Update target mass of children.
        target->_massOfChildren += _mass + _massOfChildren;

        // Update secondary moving pieces
        for (PhysicalSprite* p : movingPieces) {
            p->setAnchorOnStage(getExpellPos(p));
        }
    }

    PhysicalSprite* getPieces()
    {
        return dynamic_cast<PhysicalSprite*>(Test::activeInstance->getContent()->getFirstChild().get());
    }

    Actor* getUI()
    {
        return Test::activeInstance->getUI().get();
    }

    bool hasNullParent()
    {
        const Actor* parent = getParent();
        return parent == nullptr || parent == Test::activeInstance->getContent();
    }

    inline bool collides(PhysicalSprite* piece)
    {
        return SurfaceHelpers::collides(getAnchorToStage(),
                                        _surfaceCollision,
                                        piece->getAnchorToStage(),
                                        piece->_surfaceCollision);
    }

    inline bool canSupport(PhysicalSprite* piece)
    {
        return SurfaceHelpers::canContain(_surfaceSupport, piece->_surfaceCollision) &&
               SurfaceHelpers::contains(getAnchorToStage(),
                                        _surfaceSupport,
                                        piece->getAnchorToStage(),
                                        SurfaceHelpers::Create<SurfaceType::POINT>());
    }

    inline Vector2 getSupportPos(PhysicalSprite* piece)
    {
        return SurfaceHelpers::containSurface(getAnchorToStage(),
                                              _surfaceSupport,
                                              piece->getAnchorToStage(),
                                              piece->_surfaceCollision);
    }

    inline Vector2 getExpellPos(PhysicalSprite* piece)
    {
        return SurfaceHelpers::expellSurface(getAnchorToStage(),
                                             _surfaceCollision,
                                             piece->getAnchorToStage(),
                                             piece->_surfaceCollision);
    }
};

DECLARE_SMART(Test, spTest);
class BoardTest2: public Test
{
public:
    Resources _resources;
    spTextField txtMove;

    BoardTest2()
    {
        Resources::registerResourceType(ResCollection::create, "collection");
        //Resources::registerResourceType(ResAtlas::create, "piece");
        _resources.loadXML("xmls/collections.xml");

        addButton("addchess", "Add Chess");
        addButton("addboard", "Add Board");
        addButton("addpiece", "Add Piece");

        ResAnim* res = _resources.getResAnim("background");
        Vector2 pos = Vector2(getScaledWidth() / 2.0f, getScaledHeight() / 2.0f);
        spPhysicalSprite sprite = new PhysicalSprite(*res, pos);

        sprite->setPriority(-32768);
        _content->addChild(sprite);

        //auto x = getPieces();
        //auto y = Test::instance->getContent();
        //(x); (y);

        //auto a = Test::instance->getContent();
        //auto b = a->getFirstChild();
        //auto c = b.get();
        //auto d = dynamic_cast<PhysicalSprite*>(c);
        //(d);
        //setScale(0.2f);

        addEventListener(TouchEvent::MOVE, CLOSURE(this, &BoardTest2::event));

        spTextField tf = new TextField;
        tf->attachTo(_ui);
        tf->setWidth(400);
        tf->setColor(Color::Yellow);
        tf->setX(0);
        tf->setY(80);
        tf->setMultiline(true);
        tf->setFont(Test::_resources.getResFont("big"));
        txtMove = tf;
    }

    ~BoardTest2()
    {
        //Helpers::traversePostOrder(_sim, *_sim._root, [](Piece& p)
        //{
        //    delete &p;
        //});

        _resources.free();
        _content->detach();
        Resources::unregisterResourceType("collection");
        //Resources::unregisterResourceType("piece");
    }

    void event(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);
        Vector2 touchGlobal;        // Good
        Vector2 touchLocal;         //
        Vector2 touchLocalAnchor;   //
        Vector2 actorGlobal;        // Good
        Vector2 actorGlobalAnchor;  //
        Vector2 actorParent;        //
        Vector2 actorParentAnchor;  //

        Actor* a = dynamic_cast<Actor*>(te->target.get());
        if (a) {
            touchGlobal = convert_local2stage(a, te->position);
            touchLocal = te->position;
            touchLocalAnchor = te->position - a->getAnchor().mult(a->getSize());
            actorGlobal = convert_local2stage(a, Vector2());
            actorGlobalAnchor = convert_local2stage(a, Vector2()) + a->getAnchor().mult(a->getSize());
            actorParent = a->getPosition();
            actorParentAnchor = a->getPosition() + a->getAnchor().mult(a->getSize());
        }

        static int n = 0;
        ++n;
        char str[512];
        safe_sprintf(str,
            "%d)Touch event: %s\n"
            "Target: %s\n"
            "Touch global: %d %d\n"
            "Touch local: %d %d\n"
            "Touch local anchor: %d %d\n"
            "Actor global: %d %d\n"
            "Actor global anchor: %d %d\n"
            "Actor parent: %d %d\n"
            "Actor parent anchor: %d %d\n"
            "button: %d\n"
            ,
            n,
            "Move",
            te->target->getName().c_str(),
            (int)touchGlobal.x, (int)touchGlobal.y,
            (int)touchLocal.x, (int)touchLocal.y,
            (int)touchLocalAnchor.x, (int)touchLocalAnchor.y,
            (int)actorGlobal.x, (int)actorGlobal.y,
            (int)actorGlobalAnchor.x, (int)actorGlobalAnchor.y,
            (int)actorParent.x, (int)actorParent.y,
            (int)actorParentAnchor.x, (int)actorParentAnchor.y,
            (int)te->mouseButton
        );

        txtMove->setText(str);
    }

    void clicked(string id)
    {
        if (id == "addboard")
        {
            ResAnim* res = _resources.getResAnim("chess_board");
            Vector2 pos = Vector2(scalar::randFloat(0, (float)getScaledWidth()), scalar::randFloat(0, (float)getScaledHeight()));
            spPhysicalSprite sprite = new PhysicalSprite(*res, pos);

            sprite->drop();

            char str[255];
            safe_sprintf(str, "Add Board");
            updateText(id, str);
        }
        else if (id == "addpiece")
        {
            ResAnim* res = _resources.getResAnim("chess_white_pawn");
            Vector2 pos = Vector2(scalar::randFloat(0, (float)getScaledWidth()), scalar::randFloat(0, (float)getScaledHeight()));
            spPhysicalSprite sprite = new PhysicalSprite(*res, pos);

            sprite->drop();

            char str[255];
            safe_sprintf(str, "Add Piece");
            updateText(id, str);
        }
        else if (id == "addchess")
        {
            std::vector<PhysicalSprite*> sprites;

            ResCollection* resCollection = _resources.getT<ResCollection>("COL_chess");

            assert(resCollection);
            assert(!resCollection->getPieces().empty());

            Vector2 hugeOffset = Vector2(10000, 10000);
            Vector2 boardSize;
            Vector2 boardPos;

            // First piece is the board.
            {
                ResPiece* resPiece = resCollection->getPieces()[0];
                ResAnim* resBoard = _resources.getResAnim(resPiece->getAttribute("name").as_string());
                boardSize = Vector2(resBoard->getAttribute("surface_bottom_x").as_float(),
                                    resBoard->getAttribute("surface_bottom_y").as_float());
                boardPos = hugeOffset +
                           Vector2(scalar::randFloat(boardSize.x/2.0f, (float)getScaledWidth() - boardSize.x/2.0f),
                                   scalar::randFloat(boardSize.y/2.0f, (float)getScaledHeight() - boardSize.y/2.0f));
                spPhysicalSprite boardSprite = new PhysicalSprite(*resBoard, boardPos);

                boardSprite->drop();
                sprites.push_back(boardSprite.get());

            }

            boardPos -= boardSize / 2.0f;

            for (int i = 1; i < resCollection->getPieces().size(); ++i) {
                ResPiece* resPiece = resCollection->getPieces()[i];
                ResAnim* res = _resources.getResAnim(resPiece->getAttribute("name").as_string());
                Vector2 offset =
                    Vector2(resPiece->getAttribute("pos_x").as_float(),
                            resPiece->getAttribute("pos_y").as_float());
                Vector2 pos = boardPos + Vector2(boardSize.x * offset.x,
                                                 boardSize.y * offset.y);
                spPhysicalSprite sprite = new PhysicalSprite(*res, pos);

                sprite->drop();
                sprites.push_back(sprite.get());
            }

            for (PhysicalSprite* sprite : sprites) {
                if (sprite->getParent() == sprite->getPieces()) {
                    sprite->lift(sprite->getAnchorToStage() - hugeOffset);
                    sprite->drop();
                }
            }

            char str[255];
            safe_sprintf(str, "Add Chess");
            updateText(id, str);
        }
    }
};