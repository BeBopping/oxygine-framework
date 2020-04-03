#include "ResCollection.h"

Resource* ResCollection::create(CreateResourceContext& context)
{
    ResCollection* rs = new ResCollection();
    pugi::xml_node node = context.walker.getNode();

    std::string name = node.attribute("name").value();

    rs->setName(std::string("COL_") + name);
    rs->loadPieces(context);
    setNode(rs, node);

    context.resources->add(rs);

    return rs;
}

void ResCollection::loadPieces(CreateResourceContext& context)
{
    int count = 0;
    while (true) {
        XmlWalker walker = context.walker.next();
        if (walker.empty())
            break;

        pugi::xml_node child_node = walker.getNode();

        const char* node_name = child_node.name();
        if (strcmp(node_name, "piece"))
            continue;

        ResPiece* rs = new ResPiece();
        _pieces.push_back(rs);

        std::string name = child_node.attribute("name").value();

        rs->setName(getName() + '_' + std::to_string(count) + '_' + name);
        setNode(rs, child_node);
        rs->setParent(this);
        context.resources->add(rs);

        ++count;
    }
}