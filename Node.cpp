#include "Node.h"

// Default Constructor
Node::Node() : id(""), baz_dizi("") {}

// Parametreli Constructor
Node::Node(const std::string &_id, const std::string &_baz_dizi)
    : id(_id), baz_dizi(_baz_dizi) {}
