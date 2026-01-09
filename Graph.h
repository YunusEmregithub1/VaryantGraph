#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Node.h"

class Graph {
private:
    std::unordered_map<std::string, Node> dugumler;
    std::unordered_map<std::string, std::vector<std::string>> komsuluk;
    
public:
    Graph();
    ~Graph();
    
    void dugumEkle(const std::vector<int> &pos,
                   const std::vector<std::string> &node_id,
                   const std::string &baz_dizi,
                   const std::vector<std::string> &referans_baz,
                   const std::vector<std::string> &alternatif_baz);
    
    void kenarEkle(const std::string &from, const std::string &to);
    bool kenarVarMi(const std::string &from, const std::string &to) const;
    bool dugumVarMi(const std::string &id) const;
    std::vector<std::string> getKomsulari(const std::string &node_id) const;
    
    void grafiGFAyaYazdir(const std::string &dosya_adi) const;
};

#endif