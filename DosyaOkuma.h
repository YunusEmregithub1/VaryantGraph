#ifndef DOSYAOKUMA_H
#define DOSYAOKUMA_H

#include <string>
#include <vector>

class DosyaOkuma
{
public:
    static std::string dosya_okuma(const std::string &chr_dosya_adi,
                                   const std::string &varyant_dosya_adi);
    // ✅ DÜZELTME: const reference döndür (kopya yok!)
    static const std::vector<int>& get_pos();
    static const std::vector<std::string>& get_id();
    static const std::vector<std::string>& get_ref_baz();
    static const std::vector<std::string>& get_alt_baz();

private:
    static std::string fasta_oku(const std::string &dosya_adi);
    static void vcf_oku(const std::string &dosya_adi,
                        std::vector<int> &posv,
                        std::vector<std::string> &id_m,
                        std::vector<std::string> &ref_bazv,
                        std::vector<std::string> &alt_bazv);
};

#endif