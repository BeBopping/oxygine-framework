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

class BoardSprite : public Sprite
{
private:
    Vector2 _local;
    Simulation& _sim;
    Piece& _piece;

public:
    BoardSprite(Simulation& sim, Piece& piece) : _sim(sim), _piece(piece) {}

    //Draggable drag;
    void onEvent(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);
        if (te->type == TouchEvent::TOUCH_DOWN)
        {
            _local = te->localPosition;
            _stage->addEventListener(TouchEvent::MOVE, CLOSURE(this, &BoardSprite::onEvent));
            _stage->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &BoardSprite::onEvent));
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
        addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &BoardSprite::onEvent));
    }

    void onRemovedFromStage()
    {
        _stage->removeEventListeners(this);
    }
};

class BoardTest: public Test
{
public:
    Resources _resources;
    Simulation _sim;

    BoardTest()
    {
        Resources::registerResourceType(ResCollection::create, "collection");
        //Resources::registerResourceType(ResAtlas::create, "piece");
        _resources.loadXML("xmls/collections.xml");

        addButton("addboard", "Add Board");
        addButton("addpiece", "Add Piece");
        addButton("addchess", "Add Chess");

        _sim._root = new Piece;

        spSprite sprite = new BoardSprite(_sim, *_sim._root);
        ResAnim* res = _resources.getResAnim("background");
        Vector2 pos = Vector2(getWidth() / 2.0f, getHeight() / 2.0f);

        *_sim._root = Actions::createPiece(_sim, *sprite, *res, pos);
        sprite->setPriority(-32768);

        _content->addChild(sprite);
        _content->setScale(2.0f);
    }

    ~BoardTest()
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
            spSprite sprite = new BoardSprite(_sim, *piece);

            ResAnim* res = _resources.getResAnim("chess_board");
            Vector2 pos = Vector2(scalar::randFloat(0, (float)getWidth()), scalar::randFloat(0, (float)getHeight()));
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
            spSprite sprite = new BoardSprite(_sim, *piece);

            ResAnim* res = _resources.getResAnim("chess_white_pawn");
            Vector2 pos = Vector2(scalar::randFloat(0, (float)getWidth()), scalar::randFloat(0, (float)getHeight()));
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

            spSprite bgSprite = new BoardSprite(_sim, *s._root);
            ResAnim* bgRes = _resources.getResAnim("background");
            Vector2 pos = Vector2(getWidth() / 2.0f, getHeight() / 2.0f);

            *s._root = Actions::createPiece(s, *bgSprite, *bgRes, pos);
            bgSprite->setPriority(-32768);



            ResCollection* resCollection = _resources.getT<ResCollection>("COL_chess");

            assert(resCollection);
            assert(!resCollection->getPieces().empty());

            // First piece is the board.
            ResPiece* resBoard = resCollection->getPieces()[0];
            Piece* boardPiece = new Piece;
            spSprite sprite = new BoardSprite(_sim, *boardPiece);
            ResAnim* res = _resources.getResAnim(resBoard->getAttribute("name").as_string());
            Vector2 boardPos =
                Vector2(scalar::randFloat(0, (float)getWidth()),
                        scalar::randFloat(0, (float)getHeight()));
            *boardPiece = Actions::createPiece(s, *sprite, *res, boardPos);
            _content->addChild(sprite);

            Actions::drop(s, *boardPiece);

            Vector2 boardSize = Vector2(boardPiece->_surfaceCollision._a,
                                        boardPiece->_surfaceCollision._b);
            boardPos -= boardSize / 2.0f;

            for (ResPiece* resPiece : resCollection->getPieces()) {
                if (resPiece == resBoard) continue;

                Piece* piece = new Piece;
                spSprite sprite = new BoardSprite(_sim, *piece);
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
            safe_sprintf(str, "Add Collection");
            updateText(id, str);
        }
    }
};