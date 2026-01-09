#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>

class Node {
private:
    std::string id;
    std::string baz_dizi;

public:
    Node();
    Node(const std::string &_id, const std::string &_baz_dizi);
    ~Node() = default;

    // ✅ DÜZELTME: const reference döndür (kopya yok!)
    const std::string& getId() const { return id; }
    const std::string& getBazDizi() const { return baz_dizi; }
};
#endif