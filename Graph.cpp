#include "Graph.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include<climits>
#include <fstream>
#include <cctype>    
#include <tuple>     

#include <string>
#include <sstream>
#include <vector>
using namespace std;

Graph::Graph() = default;
Graph::~Graph() = default;


inline void buildRefAltId(const std::string &base_id, 
                         std::string &ref_id, 
                         std::string &alt_id) {
    ref_id = base_id + "_ref";
    alt_id = base_id + "_alt";
}

void Graph::dugumEkle(const std::vector<int> &pos, //varyantların genom üzerindeki pozisyonları
                      const std::vector<std::string> &node_id, //her varyant için temel node ID
                      const std::string &baz_dizi,//tüm referans genom dizisi
                      const std::vector<std::string> &referans_baz,
                      const std::vector<std::string> &alternatif_baz) 
{
    
    int onceki_pos = 0;//bir önceki varyantın pos bilgisi tutmak icin
    
    for (size_t i = 0; i < node_id.size(); i++) {
        // kontrol veri seti için negatif pos olursa
        if (pos[i] < 0 || pos[i] > static_cast<int>(baz_dizi.size())) {
            cerr << "Geçersiz pozisyon: " << pos[i] << " (" << node_id[i] << ")" << endl;
            continue;
        }
        const::string onceki_ref_id = node_id[i - 1] + "_ref"; //önceki ref
        const::string onceki_alt_id = node_id[i - 1] + "_alt";
        int fark = pos[i] - onceki_pos;//iki varyant arasında ki genom mesafesi
        
        if (pos[i] > onceki_pos) {//yeni varyant önceki varyantdan konum olarak ilerdeyse NORMAL durum
            int len_ref = referans_baz[i-1].size();//önceki varyantın referans uzunluğu alıyoruz
            
            // Aradaki konum farkı referans bazdan küçükse

            if (fark < len_ref && i > 0) {  //yeni varyant önceki varyantın referansın içine düşme durumu
                //len_ref = referans_baz[i-1].size();
                //839543 -839515 = fark (28  ref baz_  = 29
                const std::string &onceki_referans_baz_dizi = referans_baz[i-1];
                //önceki ref_baz_dizi adldik
          
               // referans_baz[i - 1].erase(onceki_referans_baz_dizi.size() - fark);//öncekş referansı parçalayıp alalım
                if(dugumVarMi(onceki_ref_id)){
                    komsuluk.erase(onceki_ref_id);//onceki baglantiyi sil. Graf yapısı yeniden duzenlenecek.
                }
                std::string segment_alt; 
                std::string segment_alt_id = node_id[i] + "_alt";
                segment_alt.reserve(alternatif_baz[i].size());
                segment_alt += alternatif_baz[i];
                dugumler[segment_alt_id] = Node(segment_alt_id, segment_alt);
                if(dugumVarMi(onceki_ref_id)){
                    komsuluk[onceki_ref_id].push_back(segment_alt_id);
                    komsuluk[segment_alt_id].push_back(node_id[i+1]);//boyut aşımı
                }
                if(dugumVarMi(onceki_alt_id)){
                    komsuluk[onceki_alt_id].push_back(node_id[i+1]);
                }
                std::string segment_ref;
                std::string segment_ref_id = node_id[i] + "_ref";
                segment_ref.reserve(referans_baz[i].size());
                segment_ref += referans_baz[i];
                dugumler[segment_ref_id]  =Node(segment_ref_id, segment_ref);
                if(dugumVarMi(onceki_ref_id)){
                komsuluk[onceki_ref_id].push_back(segment_ref_id);
                komsuluk[segment_ref_id].push_back(node_id[i+1]);}
            }
            else {//normal durum overloap yok
                std::string segment;
                if (onceki_pos + len_ref < static_cast<int>(baz_dizi.size())) {
                    segment = baz_dizi.substr(onceki_pos + len_ref, fark);
                }
                
                dugumler[node_id[i]] = Node(node_id[i], segment);

                std::string id_ref, id_alt;
                buildRefAltId(node_id[i], id_ref, id_alt);
                dugumler[id_ref] = Node(id_ref, referans_baz[i]);
                dugumler[id_alt] = Node(id_alt, alternatif_baz[i]);
                komsuluk[node_id[i]].push_back(id_ref);
                komsuluk[node_id[i]].push_back(id_alt);

                if (i > 0) {//onceki varyantlara baglanma
                    std::string onceki_ref, onceki_alt;
                    buildRefAltId(node_id[i-1], onceki_ref, onceki_alt);
                    
                    if (dugumVarMi(onceki_ref)) {
                        komsuluk[onceki_ref].push_back(node_id[i]);

                    }
                    if (dugumVarMi(onceki_alt)) {
                        komsuluk[onceki_alt].push_back(node_id[i]);
                    }
                }
            }
            
            onceki_pos = pos[i];
        }
        else if (pos[i] == onceki_pos) {//aynı pozisyonda varyant
            // Aynı pozisyon - alternatif yol
            std::string id_alt = node_id[i] + "_alt";
            dugumler[id_alt] = Node(id_alt, alternatif_baz[i]);
            komsuluk[node_id[i - 1]].push_back(id_alt);
            if (i > 0 && dugumVarMi(id_alt)) {
                komsuluk[id_alt].push_back(node_id[i+1]);
                komsuluk[onceki_ref_id].push_back(node_id[i+1]);
                komsuluk[onceki_alt_id].push_back(node_id[i+1]);
            }
        }
    }
    
    if (onceki_pos < static_cast<int>(baz_dizi.size())) {
        std::string kalan = baz_dizi.substr(onceki_pos);  //son karaktere kadar al
        const std::string id_kalan = "SONNN";
        dugumler[id_kalan] = Node(id_kalan, kalan);
        
        if (!pos.empty()) {
            std::string son_ref, son_alt;
            buildRefAltId(node_id.back(), son_ref, son_alt);
            
            if (dugumVarMi(son_ref)) {
                komsuluk[son_ref].push_back(id_kalan);
            }
            if (dugumVarMi(son_alt)) {
                komsuluk[son_alt].push_back(id_kalan);
            }
        }
    }
    
    cout << "Toplam varyant sayisi: " << pos.size() << endl;
}

void Graph::kenarEkle(const std::string &from, const std::string &to) {
    if (!dugumVarMi(from) || !dugumVarMi(to)) {
        std::cerr << "Geçersiz kenar: " << from << " -> " << to << std::endl;
        return;
    }
    
    // ✅ Optimize: tek lookup ile kontrol ve ekle
    auto &komsu_listesi = komsuluk[from];
    if (std::find(komsu_listesi.begin(), komsu_listesi.end(), to) 
        == komsu_listesi.end()) {
        komsu_listesi.push_back(to);
    }
}

bool Graph::kenarVarMi(const std::string &from, const std::string &to) const {
    auto it = komsuluk.find(from);
    if (it == komsuluk.end()) return false;
    
    const auto &komsu_listesi = it->second;
    return std::find(komsu_listesi.begin(), komsu_listesi.end(), to) 
           != komsu_listesi.end();
}

bool Graph::dugumVarMi(const string &id) const {
    return dugumler.find(id) != dugumler.end();
}



struct VariantKey {
    int chromosome;
    long long position;
    std::string ref;
    std::string alt;
    std::string original_key;
    
    VariantKey(const std::string& key) : original_key(key) {
        parseKey(key);
    }
    
    void parseKey(const std::string& key) {
        // Format: "1:10000458:T:C" veya sadece anahtar
        std::vector<std::string> parts;
        std::stringstream ss(key);
        std::string part;
        
        while (std::getline(ss, part, ':')) {
            parts.push_back(part);
        }
        
        if (parts.size() >= 2) {
            try {
                chromosome = std::stoi(parts[0]);
                position = std::stoll(parts[1]);
                ref = (parts.size() > 2) ? parts[2] : "";
                alt = (parts.size() > 3) ? parts[3] : "";
            } catch (...) {
                chromosome = 0;
                position = 0;
            }
        } else {
            // Format desteği yoksa olduğu gibi tut
            chromosome = 0;
            position = std::numeric_limits<long long>::max();
        }
    }
    
    // Sorting: chromosome → position
    bool operator<(const VariantKey& other) const {
        if (chromosome != other.chromosome) {
            return chromosome < other.chromosome;
        }
        if (position != other.position) {
            return position < other.position;
        }
        return original_key < other.original_key;  // Tiebreaker
    }
};

void Graph::grafiGFAyaYazdir(const std::string &dosya_adi) const
{
    std::ofstream gfa_dosya(dosya_adi);
    if (!gfa_dosya.is_open()) {
        std::cerr << "GFA dosyasi açilamadi: " << dosya_adi << std::endl;
        return;
    }

    // SEGMENTS
    int segment_sayac = 0;
    std::vector<std::pair<VariantKey, std::string>> segments;
    segments.reserve(dugumler.size());
    
    for (const auto &pair : dugumler) {
        segments.emplace_back(VariantKey(pair.first), 
                            pair.second.getBazDizi());
    }
    
    // Sırala
    std::sort(segments.begin(), segments.end());
    
    for (const auto &pair : segments) {
        gfa_dosya << "S\t" << pair.first.original_key << "\t" 
                  << pair.second << std::endl;
        segment_sayac++;
    }

    // LINKS
    int link_sayac = 0;
    std::vector<std::pair<VariantKey, std::vector<std::string>>> links;
    links.reserve(komsuluk.size());
    
    for (const auto &pair : komsuluk) {
        links.emplace_back(VariantKey(pair.first), pair.second);
    }
    
    std::sort(links.begin(), links.end());
    
    for (const auto &pair : links) {
        const auto &komsular = pair.second;
        for (const auto &komsu_id : komsular) {
            gfa_dosya << "L\t" << pair.first.original_key << "\t+\t" 
                      << komsu_id << "\t+\t0M" << std::endl;
            link_sayac++;
        }
    }

    gfa_dosya.close();
    
    std::cout << "\n✅ GFA dosyası oluşturuldu: " << dosya_adi << std::endl;
    std::cout << "  📊 Segments: " << segment_sayac << std::endl;
    std::cout << "  🔗 Links: " << link_sayac << std::endl;
}
