#pragma once
#include "test.h"
#include <string>
#include <string_view>
#include <vector>

//#include "../oxygine-include.h"
//#include "CreateResourceContext.h"
//#include "ResAnim.h"
//#include "Resource.h"

class ResPiece : public Resource
{
    INHERITED(Resource);
public:
    ResPiece() = default;
    ~ResPiece() = default;

private:
    void _load(LoadResourcesContext*) override {}
    void _unload() override {}
};

class ResCollection : public Resource
{
    INHERITED(Resource);
public:
    static Resource* create(CreateResourceContext&);

    ResCollection() = default;
    ~ResCollection() = default;


    //std::string_view getName() const { return _name; }
    const std::vector<ResPiece*>& getPieces() const { return _pieces; }

private:
    void loadPieces(CreateResourceContext& context);

    void _load(LoadResourcesContext*) override {}
    void _unload() override {}

    //std::string _name;
    std::vector<ResPiece*> _pieces;
};