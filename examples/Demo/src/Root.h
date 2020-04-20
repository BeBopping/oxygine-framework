#pragma once
#include "test.h"
#include "oxygine-framework.h"
#include <map>

using namespace oxygine;

DECLARE_SMART(Root, spRoot);
class Root : public Actor
{
private:
    std::map<pointer_index, std::pair<Vector2, Vector2>> _stageTouchPoints;

public:
    Root()
    {
    }

    void onEventTouchDown(Event* ev)
    {
        if (ev->phase != Event::Phase::phase_target) {
            return;
        }

        TouchEvent* te = safeCast<TouchEvent*>(ev);

        if (_stageTouchPoints.empty()) {
        //if (!hasEventListeners(TouchEvent::MOVE, CLOSURE(this, &Root::onEventMove))) {
            addEventListener(TouchEvent::MOVE, CLOSURE(this, &Root::onEventMove));
            addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &Root::onEventTouchUp));
        }

        Vector2 stagePoint = local2stage(te->localPosition);
        _stageTouchPoints[te->index] = std::make_pair(stagePoint, stagePoint);
    }

    void onEventMove(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);

        auto it = _stageTouchPoints.find(te->index);

        // Only react to touches that touched down on us.
        if (it == _stageTouchPoints.end()) {
            return;
        }

        it->second.first = it->second.second;
        it->second.second = local2stage(te->localPosition);
    }

    void onEventTouchUp(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);

        auto it = _stageTouchPoints.find(te->index);

        // Only react to touches that touched down on us.
        if (it == _stageTouchPoints.end()) {
            return;
        }

        _stageTouchPoints.erase(it);

        if (_stageTouchPoints.empty()) {
            removeEventListener(TouchEvent::MOVE, CLOSURE(this, &Root::onEventMove));
            removeEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &Root::onEventTouchUp));
        }
    }

    void doUpdate(const UpdateState& us)
    {
        if (_stageTouchPoints.empty()) {
            return;
        }

        if (_stageTouchPoints.begin()->second.first == _stageTouchPoints.begin()->second.second) {
            int a = 1;
            (++a);
        }

        // Get width and height of screen, so that we can get movement percentages.
        // Position is the center of all the touch points.
        Vector2 prevCenter;
        Vector2 nextCenter;
        RectF prevBounds = RectF(_stageTouchPoints.begin()->second.first, Vector2());
        RectF nextBounds = RectF(_stageTouchPoints.begin()->second.second, Vector2());

        for (auto& it : _stageTouchPoints) {
            prevCenter += it.second.first;
            nextCenter += it.second.second;
            prevBounds.unite(it.second.first);
            nextBounds.unite(it.second.second);

            it.second.first = it.second.second;
        }

        // Move
        Transform tr = getTransform(); tr.x = 0; tr.y = 0;
        Vector2 p = tr.transform(nextCenter - prevCenter);
        setPosition(getPosition() + p);

        // Scale
        // delta / stage
        float prevDiagonal = prevBounds.getWidestLength();
        float nextDiagonal = nextBounds.getWidestLength();
        if (prevDiagonal > 0.0001) {
            //setAnchor(stage2local(nextCenter).div(getScaledSize()));
            setScale(getScale() * (nextDiagonal / prevDiagonal));
        }
    }

    void onAdded2Stage()
    {
        if (!hasEventListeners(TouchEvent::TOUCH_DOWN, CLOSURE(this, &Root::onEventTouchDown))) {
            addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &Root::onEventTouchDown));
        }
    }

    void onRemovedFromStage()
    {
        removeEventListeners(this);
    }
};
