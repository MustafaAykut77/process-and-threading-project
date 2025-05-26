# Süreç ve Thread Tabanlı İnşaat Simülasyonu

**Grup 16 – Mustafa Aykut & Ahmet Yusuf Birdir**  
**Ders:** İşletim Sistemleri  
**Teslim Tarihi:** 26/05/2025

## Özet

Bu projede, C dili kullanılarak süreç (process) ve iş parçacığı (thread) mimarisiyle çok katlı bir apartman inşaatı simüle edilmiştir. Her kat bağımsız bir süreç olarak; her daire ise thread olarak modellenmiştir. İşçilerin ve vinçlerin senkronize kullanılmasıyla gerçek dünya senaryosuna yakın bir modelleme yapılmıştır.

## Sistem Tasarımı

- **Bina:** 10 katlı, her katta 4 daire olmak üzere toplam 40 daire.
- **Kaynaklar:**
  - 40 işçi
  - 2 vinç

### İnşaat Aşamaları:

1. Temel atma ve kat iskeletlerinin inşası (süreçler ile sırayla).
2. Her katın 4 dairesinin eş zamanlı kaba inşası (thread'ler ile paralel).
3. Ortak alan inşaatı (merdiven boşluğu, mermer, korkuluk).
4. Çatı işlemleri (kiremit, oluk, sıva).
5. Dış cephe işlemleri (iskele, sıva, boya).
6. Daire iç mekan işlemleri (kapı, parke, mutfak vs.).

## Süreç ve İş Parçacığı Modeli

- Her **kat**, `fork()` ile oluşturulmuş bir **process** olarak çalışır.
- Her kat içinde yer alan **4 daire**, `pthread_create()` ile başlatılmış **thread**'lerdir.
- Üst kat, ancak alt kat tamamlandıktan sonra başlatılır (`wait()` ile).
- Aynı anda maksimum 2 vinç kullanılabilir (`sem_t` ile kontrol).
- İşçi sayısı `mutex` ile korunur ve gerektiğinde beklenir.

## Senkronizasyon Mekanizmaları

### İşçi Yönetimi:
- Tüm işlemler başlamadan önce gerekli sayıda işçi talep edilir.
- İşlem sonunda işçiler iade edilir.
- `pthread_mutex_t` ile yarış durumu engellenir.

### Vinç Yönetimi:
- Vinçler `sem_t` ile sınırlandırılır (en fazla 2 kullanım).
- `sem_wait()` ile alınır, `sem_post()` ile bırakılır.

## Kod Yapısı

| Fonksiyon             | Açıklama                                                                 |
|-----------------------|--------------------------------------------------------------------------|
| `isciAl()`            | İşlem için işçi talep eder. Yeterli yoksa bekler.                        |
| `isciBirak()`         | Belirtilen kadar işçiyi serbest bırakır.                                |
| `daireIslemleri()`    | Tuğla, tesisat, pencere, sıva gibi kaba işler burada yapılır.           |
| `ortakAlanIslemleri()`| Merdiven, korkuluk ve mermer işlemleri burada gerçekleştirilir.         |
| `icMekanIslemleri()`  | Kapı, dolap, parke, fayans gibi iç dizayn işlemleri burada yapılır.     |
| `catiIslemleri()`     | Kiremit, oluk ve çatı sıvası işlemlerini simüle eder.                   |
| `disCepheIslemleri()` | İskele, dış sıva ve cephe boyama işlemlerini içerir.                    |
| `main()`              | Bütün işlemleri sırayla organize eder ve simülasyonu başlatır.          |

## Kurulum ve Çalıştırma

> **Not:** Program sadece Linux ortamında çalıştırılmalıdır.

### Derleme:
```bash
gcc main.c -o insaat -lpthread
