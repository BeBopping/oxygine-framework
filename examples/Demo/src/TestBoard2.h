#pragma once
#include "test.h"

#include "Piece.h"
#include "PieceHelpers.h"
#include "ResCollection.h"
#include "Simulation.h"
#include "Surface.h"

// TODO:
// Adding collection should not use drop()???
// Better Support/Collision
//      Mass matters for movement.
//          Pieces displace other piece but not a board.
//          Board displace pieces and maybe other boards?
//      Center of mass for support? (togglable)
//      Expolsion in the opposite direction as containment. (More satisfying)
//      Better resolution with single and multiple objects.
//
// Data driven pieces.
//      Rethink file structure. Images, templates, data, collections, etc.
//      <Collection name="chess" space="chess_board">
//          <Piece name="chess_board" pos_x="0.5" pos_y=0.5/>
//          <Piece name="chess_black_pawn" pos_x="0.125" pos_y="0.125"/>
//      </Collection>
//      Create collection from button press or menu selection.
//      Create piece from button press or menu selection.
//
// Undo/Redo?
//
// Debugging
//  - Show Support surface and collision surface.
//  - Edit piece templates from game.
//  - Edit collection templates from game.

// TODO Visuals.
// Camera Zoom and Pan?
// Offset other piece position when lifting. Calculate data while traversing tree?
// Animate positions resolutions.

class PhysicalSprite : public Sprite
{
private:
    Vector2 _local = {};
    pointer_index _pointerIndex = {};
    Simulation& _sim;
    Piece& _piece;

public:
    PhysicalSprite(Simulation& sim, Piece& piece) : _sim(sim), _piece(piece) {}

    //Draggable drag;
    void onEvent(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);

        // Ignore secondary touches.
        if (te->index != _pointerIndex && _pointerIndex != 0) {
            return;
        }

        _pointerIndex = te->index;
        if (te->type == TouchEvent::TOUCH_DOWN)
        {
            _local = te->localPosition;
            _stage->addEventListener(TouchEvent::MOVE, CLOSURE(this, &PhysicalSprite::onEvent));
            _stage->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &PhysicalSprite::onEvent));
        }

        if (te->type == TouchEvent::MOVE)
        {
            move(te->localPosition);
        }

        if (te->type == TouchEvent::TOUCH_UP)
        {
            move(te->localPosition);
            Actions::drop(_sim, _piece);

            _stage->removeEventListeners(this);
            _pointerIndex = 0;
        }
    }

    void move(const Vector2& pos)
    {
        Vector2 localPos = stage2local(pos);
        Vector2 offset = localPos - _local;

        Transform tr = getTransform();
        tr.x = 0;
        tr.y = 0;
        Vector2 p = tr.transform(offset);
        setPosition(getPosition() + p);

        Actions::lift(_sim, _piece, getPosition());
    }

    void doUpdate(const UpdateState& us)
    {
        pointer_index ind = getPressed();
        if (!ind)
            return;
        PointerState* st = Input::instance.getTouchByIndex(ind);
        move(_stage->parent2local(st->getPosition()));
    }

    void onAdded2Stage()
    {
        addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &PhysicalSprite::onEvent));
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
};

class BoardTest2: public Test
{
public:
    Resources _resources;
    Simulation _sim;

    BoardTest2()
    {
        Resources::registerResourceType(ResCollection::create, "collection");
        //Resources::registerResourceType(ResAtlas::create, "piece");
        _resources.loadXML("xmls/collections.xml");

        addButton("addchess", "Add Chess");
        addButton("addboard", "Add Board");
        addButton("addpiece", "Add Piece");

        _sim._root = new Piece;

        spSprite sprite = new PhysicalSprite(_sim, *_sim._root);
        ResAnim* res = _resources.getResAnim("background");
        Vector2 pos = Vector2(getScaledWidth() / 2.0f, getScaledHeight() / 2.0f);

        *_sim._root = Actions::createPiece(_sim, *sprite, *res, pos);
        sprite->setPriority(-32768);

        _content->addChild(sprite);
        //setScale(0.2f);
    }

    ~BoardTest2()
    {
        Helpers::traversePostOrder(_sim, *_sim._root, [](Piece& p)
        {
            delete &p;
        });

        _resources.free();
        Resources::unregisterResourceType("collection");
        //Resources::unregisterResourceType("piece");
    }

    void clicked(string id)
    {
        if (id == "addboard")
        {
            Piece* piece = new Piece;
            spSprite sprite = new PhysicalSprite(_sim, *piece);

            ResAnim* res = _resources.getResAnim("chess_board");
            Vector2 pos = Vector2(scalar::randFloat(0, (float)getScaledWidth()), scalar::randFloat(0, (float)getScaledHeight()));
            *piece = Actions::createPiece(_sim, *sprite, *res, pos);

            _content->addChild(sprite);

            Actions::drop(_sim, *piece);

            char str[255];
            safe_sprintf(str, "Add Board");
            updateText(id, str);
        }
        else if (id == "addpiece")
        {
            Piece* piece = new Piece;
            spSprite sprite = new PhysicalSprite(_sim, *piece);

            ResAnim* res = _resources.getResAnim("chess_white_pawn");
            Vector2 pos = Vector2(scalar::randFloat(0, (float)getScaledWidth()), scalar::randFloat(0, (float)getScaledHeight()));
            *piece = Actions::createPiece(_sim, *sprite, *res, pos);
            _content->addChild(sprite);

            Actions::drop(_sim, *piece);

            char str[255];
            safe_sprintf(str, "Add Piece");
            updateText(id, str);
        }
        else if (id == "addchess")
        {
            Simulation s;
            s._root = new Piece;

            spSprite bgSprite = new PhysicalSprite(_sim, *s._root);
            ResAnim* bgRes = _resources.getResAnim("background");
            Vector2 pos = Vector2(getScaledWidth() / 2.0f, getScaledHeight() / 2.0f);

            *s._root = Actions::createPiece(s, *bgSprite, *bgRes, pos);
            bgSprite->setPriority(-32768);



            ResCollection* resCollection = _resources.getT<ResCollection>("COL_chess");

            assert(resCollection);
            assert(!resCollection->getPieces().empty());

            // First piece is the board.
            ResPiece* resBoard = resCollection->getPieces()[0];
            Piece* boardPiece = new Piece;
            spSprite sprite = new PhysicalSprite(_sim, *boardPiece);
            ResAnim* res = _resources.getResAnim(resBoard->getAttribute("name").as_string());
            Vector2 boardPos =
                Vector2(scalar::randFloat(0, (float)getScaledWidth()),
                        scalar::randFloat(0, (float)getScaledHeight()));
            *boardPiece = Actions::createPiece(s, *sprite, *res, boardPos);
            _content->addChild(sprite);

            Actions::drop(s, *boardPiece);

            Vector2 boardSize = Vector2(boardPiece->_surfaceCollision._a,
                                        boardPiece->_surfaceCollision._b);
            boardPos -= boardSize / 2.0f;

            for (ResPiece* resPiece : resCollection->getPieces()) {
                if (resPiece == resBoard) continue;

                Piece* piece = new Piece;
                spSprite sprite = new PhysicalSprite(_sim, *piece);
                ResAnim* res = _resources.getResAnim(resPiece->getAttribute("name").as_string());
                Vector2 offset =
                    Vector2(resPiece->getAttribute("pos_x").as_float(),
                            resPiece->getAttribute("pos_y").as_float());
                Vector2 pos = boardPos + Vector2(boardSize.x * offset.x,
                                                 boardSize.y * offset.y);
                *piece = Actions::createPiece(s, *sprite, *res, pos);
                _content->addChild(sprite);

                Actions::drop(s, *piece);
            }

            for (Piece* p = s._root->_childFirst; p; p = s._root->_childFirst) {
                Actions::detach(s, *p);
                Actions::drop(_sim, *p);
            }

            char str[255];
            safe_sprintf(str, "Add Chess");
            updateText(id, str);
        }
    }
};