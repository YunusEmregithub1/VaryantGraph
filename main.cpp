#include "DosyaOkuma.h"
#include "Graph.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

int main()
{
    string chr_dosya_adi = "chr1.fa";
    string varyant_dosya_adi = "sirali.vcf";

    DosyaOkuma d;
    string baz_dizi = d.dosya_okuma(chr_dosya_adi, varyant_dosya_adi);
    cerr<<"baz_dizi verisini aşdi\n";
    const auto &pos = d.get_pos();
    const auto &id = d.get_id();
    const auto &referans_baz = d.get_ref_baz();
    const auto &alternatif_baz = d.get_alt_baz();
    cerr<<"veriler alindi id, pos, ref_baz, alt_baz\n";
    Graph g;
    g.dugumEkle(pos, id, baz_dizi, referans_baz, alternatif_baz);  
    cerr<<"dugum eklendi gfa ya geçiyor";
    g.grafiGFAyaYazdir("genom.gfa");

    return 0;
}