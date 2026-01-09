#include "DosyaOkuma.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

static vector<string> id;
static vector<string> alt_baz;
static vector<string> ref_bazv;
static vector<int> posv;

string DosyaOkuma::fasta_oku(const string &dosya_adi)
{
    ifstream dosya(dosya_adi);
    if (!dosya.is_open()) { 
        cerr << "Böyle bir dosya yok: " << dosya_adi << endl; 
        return ""; 
    }

    string satir, baz_dizi;
    
    // ✅ Optimize: Dosya boyutunu ön tahmin et (reserve)
    dosya.seekg(0, ios::end);
    size_t dosya_boyutu = dosya.tellg();
    dosya.seekg(0, ios::beg);
    baz_dizi.reserve(dosya_boyutu);  // ✅ Yeniden tahsis yapılmayacak!
    
    while (getline(dosya, satir))
    {
        // > ile başlayan satırlar başlıktır, atla
        if (!satir.empty() && satir[0] == '>')
            continue;

        // ✅ += string concatenation artık O(1) çünkü reserve() yaptık
        baz_dizi += satir;
    }
    dosya.close();
    return baz_dizi;
}

void DosyaOkuma::vcf_oku(const string &dosya_adi,
                         vector<int> &posv,
                         vector<string> &id,
                         vector<string> &ref_bazv,
                         vector<string> &alt_baz)
{
    ifstream dosya(dosya_adi);
    if (!dosya.is_open()) { 
        cerr << "Böyle bir dosya yok: " << dosya_adi << endl; 
        return; 
    }

    // ✅ Optimize: Dosya boyutunu tahmin et
    dosya.seekg(0, ios::end);
    size_t dosya_boyutu = dosya.tellg();
    dosya.seekg(0, ios::beg);
    
    // Ortalama satır ~ 100 byte ise
    size_t tahmini_varyant = dosya_boyutu / 100;
    id.reserve(tahmini_varyant);
    ref_bazv.reserve(tahmini_varyant);
    alt_baz.reserve(tahmini_varyant);
    posv.reserve(tahmini_varyant);

    string satir;
    while (getline(dosya, satir))
    {
        // Meta satırları (# veya ## ile başlayan) atla
        if (satir.empty() || satir[0] == '#')
            continue;

        // Satırı VCF biçiminde parse et
        stringstream ss(satir);
        string chrom, posStr, ids, ref, alt, qual, filter, info;
        ss >> chrom >> posStr >> ids >> ref >> alt >> qual >> filter >> info;

        try
        {
            int pos = stoi(posStr);
            posv.push_back(pos);
            id.push_back(ids);  // ✅ Move semantiği (C++11) otomatik kullanılacak
            ref_bazv.push_back(ref);  // ✅ ref ekle (düzeltme: alt değil ref!)
            alt_baz.push_back(alt);  // ✅ alt ekle
        }
        catch (const invalid_argument &)
        {
            cerr << "Geçersiz POS değeri: " << posStr << " satir: " << satir << endl;
        }
    }

    dosya.close();
}

string DosyaOkuma::dosya_okuma(const string &chr_dosya1, const string &varyant_dosya1)
{
    string baz_dizi = fasta_oku(chr_dosya1);
    vcf_oku(varyant_dosya1, posv, id, ref_bazv, alt_baz);
    cout << "Okunan varyant sayisi: " << posv.size() << endl;
    return baz_dizi;
}

// ✅ DÜZELTME: const reference döndür (kopya YOK!)
const vector<int>& DosyaOkuma::get_pos(){
    return posv;
}

const vector<string>& DosyaOkuma::get_id(){
    return id;
}

const vector<string>& DosyaOkuma::get_ref_baz(){
    return ref_bazv;
}

const vector<string>& DosyaOkuma::get_alt_baz(){
    return alt_baz;
}