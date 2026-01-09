# VaryantGraph
#Varyantlari Graf Veri Yapısına Ekleme

## Genel Bakış
Bu proje, varyantların geleneksel lineer veri yapıları (dizi, liste vb.)
yerine graf (graph) veri yapısı kullanılarak modellenmesini amaçlamaktadır.

Lineer yapılar, varyantlar arasında yalnızca sıralı ve tek boyutlu ilişkileri
temsil edebilirken; gerçek dünyadaki varyantlar çoğu zaman çoklu, yönlü ve
hiyerarşik ilişkilere sahiptir. Bu nedenle varyantlar graf düğümleri (node),
ilişkiler ise kenarlar (edge) olarak ele alınmıştır.

---

## Problem Tanımı
Lineer veri yapılarının temel kısıtları:
- Varyantlar arası ilişkileri açık şekilde temsil edememesi
- Zorunlu sıralama varsayımı
- İlişki sayısı arttıkça ölçeklenebilirliğin düşmesi

Bu proje, söz konusu kısıtları ortadan kaldırmak için
graf tabanlı bir yaklaşım önermektedir.

---

## Yaklaşım
- Her varyant bir düğüm (node) olarak tanımlanır
- Varyantlar arası ilişkiler kenar (edge) olarak modellenir

---

## Veri Yapısı Tasarımı

Variant (Node)
 ├─ pos
 ├─ id
 ├─ referans_baz
 ├─ alternatif_baz
 
Durumlar:
Normal durum : bir sonraki gelecek varyant konum olarak ilerde ise
Aradaki fark kucukse: Arada ki konum farkı referans_baz dan kucukse //839543 -839515 -> fark = 28  ref baz_  = 29 bu durumu kontrol edip gerekli işlemler alındı
Aynı pozisyonda varyant: //788757 den iki farklı dallanlama ref ve alt gerekir 788757	1:788757:T:TAATGG   788757	1:788758:T:TAATGGAATG
