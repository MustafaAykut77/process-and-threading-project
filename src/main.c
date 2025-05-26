#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

// Isci, kat ve daire sayisinin belirlenmesi.
#define TOPLAM_ISCI 20
#define KAT_SAYISI 10
#define DAIRE_SAYISI 4

// Insaat boyunca kullanilacak kaynaklarin atanmasi.
int mevcut_isci = TOPLAM_ISCI; // toplam isci sayisi.
pthread_mutex_t isci_mutex; // isci islemleri sirasinda kilitlemek icin kullanilacak kilit.
sem_t vincler; // esyalari tasiyacak vinc.

// Isci talebi oldugunda calisacak fonksiyon.
void isciAl(int adet) {
    int yeterli_isci_flag = 0; // yeterli isci olup olmadigi bildirmek icin bayrak.
    while (1) {
        pthread_mutex_lock(&isci_mutex); // kritik nokta oldugundan ayni anda isci talebi olursa karmasiklik yasanmamasi icin mutex kilidi.
        if (mevcut_isci >= adet) { // yeterli isci varsa isci sayisini azalt ve kilidi kaldir.
            mevcut_isci -= adet;
            pthread_mutex_unlock(&isci_mutex);
            if (yeterli_isci_flag) { // yeterli isci yokken var durumuna gecdiyse bildir. 
                printf("│ %d işçi alındı, bekleme sona erdi\n", adet);
            } else { // yeterli isci halihazirda varsa sadece alindi yazdir.
                printf("│ %d işçi alındı\n", adet);
            }
            break;
        } else { // yeterli isci yoksa bayragi setle ve bekleme durumuna gec.
            if (!yeterli_isci_flag) {
                printf("│ %d işçi bekleniyor\n", adet);
                yeterli_isci_flag = 1;
            }
        }
        pthread_mutex_unlock(&isci_mutex);
        usleep(100000); // 100ms bekle
    }
}

// Isci serbest birakma emri geldiginde calisacak fonksiyon.
void isciBirak(int adet) {
    pthread_mutex_lock(&isci_mutex); // isci serbest birakma sirasinda karmasiklik
    mevcut_isci += adet;             // yasanmamasi icin mutex kilidi.
    pthread_mutex_unlock(&isci_mutex);
}

// Daireleri insa edecek fonksiyon.
void* daireIslemleri(void* arg) {
    int daire_no = *(int*)arg;
    int daire_index = daire_no % 4 + 1;

    printf("├─ Daire %d işlemleri başlıyor.\n", daire_index);

    // Vinc ile tuglalari tasi.
    sem_wait(&vincler);
    printf("├── [Daire %d] Vinç tuğlaları taşıyor...\n", daire_index);
    sleep(1);
    printf("├── [Daire %d] Tuğlalar teslim edildi.\n", daire_index);
    sem_post(&vincler);
    
    // 8 isci ile daire duvarlarini or.
    isciAl(8);
    printf("├── [Daire %d] İç duvar tuğlaları örülüyor...\n", daire_index);
    sleep(1);
    isciBirak(8);

    // Vinc ile tüm kablolari tasi.
    sem_wait(&vincler);
    printf("├── [Daire %d] Vinç tüm kabloları (elektrik, telefon, uydu) taşıyor...\n", daire_index);
    sleep(1);
    printf("├── [Daire %d] Kablolar teslim edildi.\n", daire_index);
    sem_post(&vincler);

    // 5 isci ile elektrik tesisatini dose.
    isciAl(5);
    printf("├── [Daire %d] Elektrik tesisatı döşeniyor...\n", daire_index);
    sleep(1);
    isciBirak(5);

    // 5 isci ile kablolari dose.
    isciAl(5);
    printf("├── [Daire %d] Telefon ve uydu kabloları döşeniyor...\n", daire_index);
    sleep(1);
    isciBirak(5);

    // Vinc ile dogalgaz borularini tasi.
    sem_wait(&vincler);
    printf("├── [Daire %d] Vinç doğalgaz borularını taşıyor...\n", daire_index);
    sleep(1);
    printf("├── [Daire %d] Doğalgaz boruları teslim edildi.\n", daire_index);
    sem_post(&vincler);
    
    // 5 isci ile dogalgaz tesisatini dose.
    isciAl(5);
    printf("├── [Daire %d] Doğalgaz tesisatı döşeniyor...\n", daire_index);
    sleep(1);
    isciBirak(5);

    // Vinc ile gider borusunu tasi.
    sem_wait(&vincler);
    printf("├── [Daire %d] Vinç gider borularını taşıyor...\n", daire_index);
    sleep(1);
    printf("├── [Daire %d] Gider boruları teslim edildi.\n", daire_index);
    sem_post(&vincler);
    
    // 5 isci ile gider borularini dose.
    isciAl(5);
    printf("├── [Daire %d] Gider boruları döşeniyor...\n", daire_index);
    sleep(1);
    isciBirak(5);

    // 5 isci ile su tesisatini dose.
    isciAl(5);
    printf("├── [Daire %d] Su tesisatı döşeniyor...\n", daire_index);
    sleep(1);
    isciBirak(5);

    // Vinc ile yalitim malzemelerini tasi.
    sem_wait(&vincler);
    printf("├── [Daire %d] Vinç yalıtım malzemelerini taşıyor...\n", daire_index);
    sleep(1);
    printf("├── [Daire %d] Yalıtım malzemeleri teslim edildi.\n", daire_index);
    sem_post(&vincler);
    
    // 8 isci ile yalitimi yap.
    isciAl(8);
    printf("├── [Daire %d] Ses ve ısı yalıtımı uygulanıyor...\n", daire_index);
    sleep(1);
    isciBirak(8);

    // 8 isci ile duvarlara siva at.
    isciAl(8);
    printf("├── [Daire %d] Duvarlara sıva atılıyor...\n", daire_index);
    sleep(1);
    isciBirak(8);

    // 5 isci ile daire pencere bosluklarini kes.
    isciAl(5);
    printf("├── [Daire %d] Pencere boşlukları kesiliyor...\n", daire_index);
    sleep(1);
    isciBirak(5);

    printf("├─ Daire %d işlemleri tamamlandı\n", daire_index);
    free(arg);
    return NULL;
}

// Ortak alani insa edecek fonksiyon.
void* ortakAlanIslemleri(void* arg) {
    // Ortak alan islemleri baslatiliyor.
    printf("┌ Ortak alan işlemleri başlıyor\n");

    // 8 isci ile merdiven boslugu sivasi yap.
    isciAl(8);
    printf("├─ Merdiven boşluğu sıvası yapılıyor...\n");
    sleep(1);
    isciBirak(8);

    // 6 isci ile merdiven korkuluklarini tak.
    isciAl(6);
    printf("├─ Merdiven korkulukları takılıyor...\n");
    sleep(1);
    isciBirak(6);

    // 10 isci ile merdiven mermerlerini dose.
    isciAl(10);
    printf("├─ Merdiven mermerleri döşeniyor...\n");
    sleep(1);
    isciBirak(10);

    // Ortak alan islemleri tamamlandi.
    printf("└ Ortak alan işlemleri tamamlandı\n\n");

    return NULL;
}

// Daire ic mekanlarini insa edecek fonksiyon.
void* icMekanIslemleri(void* arg) {
    int daire_no = *((int*)arg);
    int daire_index = daire_no % 4 + 1;

    printf("├─ Daire %d iç mekan işlemleri başlıyor... \n", daire_index);

    // Malzemelerin vincle tasinmasi
    printf("├── [Daire %d] Malzemeler vinçle taşınıyor...\n", daire_index);
    sem_wait(&vincler);
    sleep(2); // Malzeme tasima suresi
    sem_post(&vincler);
    printf("├── [Daire %d] Malzemeler vinçle taşındı\n", daire_index);

    // 4 isci ile kapi montajini yap.
    isciAl(4);
    printf("├── [Daire %d] Kapı montajı yapılıyor\n", daire_index);
    sleep(1);
    isciBirak(4);

    // 4 isci ile pencere montajini yap.
    isciAl(4);
    printf("├── [Daire %d] Pencere montajı yapılıyor\n", daire_index);
    sleep(2);
    isciBirak(4);

    // 3 isci ile banyo fayanslarini dose.
    isciAl(3);
    printf("├── [Daire %d] Banyo fayansları döşeniyor\n", daire_index);
    sleep(1);
    isciBirak(3);

    // 4 isci ile parkeleri dose.
    isciAl(4);
    printf("├── [Daire %d] Parkeler döşeniyor\n", daire_index);
    sleep(1);
    isciBirak(4);

    // 6 isci ile duvarlari boya.
    isciAl(6);
    printf("├── [Daire %d] Duvarlar boyanıyor\n", daire_index);
    sleep(1);
    isciBirak(6);

    // 4 isci ile supurgelikleri tak.
    isciAl(4);
    printf("├── [Daire %d] Süpürgelikler takılıyor\n", daire_index);
    sleep(1);
    isciBirak(4);

    // 6 isci ile mutfak dolaplarini tak.
    isciAl(6);
    printf("├── [Daire %d] Mutfak dolapları takılıyor\n", daire_index);
    sleep(1);
    isciBirak(6);

    // 2 isci ile mutfak tezgahini koy.
    isciAl(2);
    printf("├── [Daire %d] Mutfak tezgahı koyuluyor\n", daire_index);
    sleep(1);
    isciBirak(2);

    // 3 isci ile banyo dolabini tak.
    isciAl(3);
    printf("├── [Daire %d] Banyo dolapları takılıyor\n", daire_index);
    sleep(1);
    isciBirak(3);

    // 3 isci ile banyo dusakabinini tak.
    isciAl(3);
    printf("├── [Daire %d] Banyo duşakabini takılıyor\n", daire_index);
    sleep(1);
    isciBirak(3);

    // 2 isci ile klozeti koy.
    isciAl(2);
    printf("├── [Daire %d] Klozet koyuluyor\n", daire_index);
    sleep(1);
    isciBirak(2);

    printf("├─ Daire %d İç mekan işlemleri tamamlandı.\n", daire_index);

    return NULL;
}

	
// Binanin cati islemlerinin yapilacagi fonksiyon.
void* catiIslemleri(void* arg) {
    printf("┌ Çatı işlemleri başlıyor\n");

    // Vinc ile kiremit tasi.
    sem_wait(&vincler);
    printf("├─ [Çatı] Vinç kiremitleri taşıyor...\n");
    sleep(1);
    printf("├─ [Çatı] Kiremitler teslim edildi.\n");
    sem_post(&vincler);
    
    // 10 isci ile kiremitleri dose.
    isciAl(10);
    printf("├─ [Çatı] Kiremitler döşeniyor...\n");
    sleep(1);
    isciBirak(10);

    // Vinc ile yağmur oluklarini tasi.
    sem_wait(&vincler);
    printf("├─ [Çatı] Vinç yağmur oluklarını taşıyor...\n");
    sleep(1);
    printf("├─ [Çatı] Yağmur olukları teslim edildi.\n");
    sem_post(&vincler);
    
    // 6 isci ile yagmur oluklarini yerlestir.
    isciAl(6);
    printf("├─ [Çatı] Yağmur olukları yerleştiriliyor...\n");
    sleep(1);
    isciBirak(6);

    // 6 isci ile kiremitlerin sivasini yap.
    isciAl(6);
    printf("├─ [Çatı] Kiremit sıvası yapılıyor...\n");
    sleep(1);
    isciBirak(6);

    printf("└ Çatı işlemleri tamamlandı\n\n");
    return NULL;
}

// Binanin dis cephe islemlerinin yapilacagi fonksiyon.
void* disCepheIslemleri(void* arg) {
    printf("┌ Dış cephe işlemleri başlıyor\n");

    // Vinc ile dış cephe iskele malzemeleri taşı
    sem_wait(&vincler);
    printf("├─ [Dış Cephe] Vinç iskele malzemelerini taşıyor...\n");
    sleep(1);
    printf("├─ [Dış Cephe] İskele malzemeleri teslim edildi.\n");
    sem_post(&vincler);

    // 12 isci ile iskele kur
    isciAl(12);
    printf("├─ [Dış Cephe] İskele kuruluyor...\n");
    sleep(1);
    isciBirak(12);

    // 10 isci ile dış cephe sıvası yap
    isciAl(10);
    printf("├─ [Dış Cephe] Dış cephe sıvası yapılıyor...\n");
    sleep(2);
    isciBirak(10);

    // Vinc ile boya malzemeleri taşı
    sem_wait(&vincler);
    printf("├─ [Dış Cephe] Vinç boya malzemelerini taşıyor...\n");
    sleep(1);
    printf("├─ [Dış Cephe] Boya malzemeleri teslim edildi.\n");
    sem_post(&vincler);

    // 10 isci ile cephe boyası yap
    isciAl(10);
    printf("├─ [Dış Cephe] Cephe boyası yapılıyor...\n");
    sleep(2);
    isciBirak(10);

    // 12 isci ile iskeleyi sök
    isciAl(12);
    printf("├─ [Dış Cephe] İskele sökülüyor...\n");
    sleep(1);
    isciBirak(12);

    printf("└ Dış cephe işlemleri tamamlandı\n\n");
    return NULL;
}

// Main fonksiyonu.
int main() {
    // Binanin temelinin atilmasi.
    printf("Binanın temeli atılıyor.\n");
    sleep(1);
    printf("Binanın temeli tamamlandı.\n\n");

    // Senkronizasyon araclarinin ayarlanmasi.
    pthread_mutex_init(&isci_mutex, NULL); // isci sayisina gore atomik islemlerin yapilmasi icin mutex kilidi.
    sem_init(&vincler, 0, 2); // 2 adet vinc atamasi.

    // Katlarin sirayla insa edilmesi.
    for (int kat = 1; kat <= KAT_SAYISI; kat++) {
        pid_t pid = fork();

        if (pid == 0) {
            // Alt katlar bitmeden bu kat baslamaz (ebeveyn bekletiyor)

            printf("┌ Kat %d inşaatı başlıyor.\n", kat);

            // Vinc ile demirleri tasi.
            sem_wait(&vincler);
            printf("├─ [Kat %d] Vinç demirleri taşıyor...\n", kat);
            sleep(1);
            printf("├─ [Kat %d] Demirler teslim edildi.\n", kat);
            sem_post(&vincler);

            // 20 isci ile alinan demirleri dose.
            isciAl(20);
            printf("├─ [Kat %d] Zemin ve kolon demirleri döşeniyor...\n", kat);
            sleep(1);
            isciBirak(20);

            // 20 isci ile demirlere beton dok.
            isciAl(20);
            printf("├─[Kat %d] Zemin ve kolon betonları dökülüyor...\n", kat);
            sleep(1);
            isciBirak(20);

            printf("└ Kat %d inşaatı tamamlandı.\n", kat);	
            
            // Merdiven insasi.
            printf("Kat %d Apartman boşluğuna merdiven yapıldı.\n", kat);
            sleep(1);

            // Son kata ulasildiysa cati islemleri basliyor.
            if (kat == 10) {
                // 5 isci ile cati islemlerini yap.
                isciAl(5);
                printf("\nÇatı iskeleti döşeniyor...\n");
                sleep(2);
                isciBirak(5);
                printf("Çatı tamamlandı.\n\n");
            }

            exit(0); // alt process bitince cik.
        } else {
            wait(NULL); // bir kat bitmeden ust katin baslamamasi icin bekleme ifadesi.
        }
    }
    // Bina iskeleti tamamlandiktan sonra bildiri.
    printf(">>> Bina iskeleti tamamlandı. Daire iskeletine geçiliyor...\n\n");

    // Katlardaki dairelerin insa edildigi kisim.
    for (int kat = 1; kat <= KAT_SAYISI; kat++) {
        pid_t pid = fork();

        if (pid == 0) {
            printf("┌ [Kat %d] Daire işlemleri başlıyor.\n", kat);

            pthread_t threadler[DAIRE_SAYISI]; // daire sayisi kadar thread tutacak liste olustur.
            for (int i = 0; i < DAIRE_SAYISI; i++) { // daire sayisi boyunca calistir.
                int* daire_no = malloc(sizeof(int)); // integer boyutunda yer ayir.
                *daire_no = (kat - 1) * DAIRE_SAYISI + i + 1; // daire numarasinin duzgun ilerlemesi icin hesaplama.
                pthread_create(&threadler[i], NULL, daireIslemleri, daire_no); // daire sayisi kadar thread olustur listede sakla.
            }

            for (int i = 0; i < DAIRE_SAYISI; i++) { // daire sayisi kadar calistir.
                pthread_join(threadler[i], NULL); // olusturulan threadleri birlestir.
            }

            printf("└ [Kat %d] Daire işlemleri tamamlandı. Sonraki kata geçiliyor...\n\n", kat);
            exit(0); // alt process bitinci cik.
        } else {
            wait(NULL); // bir kattaki dairelerin hepsi bitene kadar bekle.
        }    
    }
	
    // Daire islemlerinin bittiginin bildirisi.
    printf(">>> Dairelerin hepsi tamamlandı...\n\n");
    
    // Cati, dis cephe ve ortak alan islemlerinin thread'inin olusturulmasi.
    pthread_t cati_thread, dis_cephe_thread, ortakAlan_thread;
    
    // Ortak alan thread'inin baslatilmasi.
    pthread_create(&ortakAlan_thread, NULL, ortakAlanIslemleri, NULL);
    pthread_join(ortakAlan_thread, NULL);
    
    // Cati islemlerinin thread'inin baslatilmasi.
    pthread_create(&cati_thread, NULL, catiIslemleri, NULL);
    pthread_join(cati_thread, NULL);
    
    // Dis cephe islemlerinin thread'inin baslatilmasi.
    pthread_create(&dis_cephe_thread, NULL, disCepheIslemleri, NULL);
    pthread_join(dis_cephe_thread, NULL);

    // Ic mekan düznelemesinin yapildigi kisim.
    for (int kat = 1; kat <= KAT_SAYISI; kat++) {
    pid_t pid = fork();

    	if (pid == 0) {
       	   printf("┌ [Kat %d] İç mekan işlemleri başlıyor.\n", kat);
           pthread_t ic_mekan_threadler[DAIRE_SAYISI];// daire sayisi kadar thread tutacak liste olustur.
           for (int i = 0; i < DAIRE_SAYISI; i++) { // daire sayisi boyunca calistir.
                int* daire_no = malloc(sizeof(int)); // integer boyutunda yer ayir.
		    *daire_no = (kat - 1) * DAIRE_SAYISI + i + 1; // daire numarasinin duzgun ilerlemesi icin hesaplama.
		    pthread_create(&ic_mekan_threadler[i], NULL, icMekanIslemleri, daire_no); // daire sayisi kadar thread olustur listede sakla.
		}

	   for (int i = 0; i < DAIRE_SAYISI; i++) { // daire sayisi kadar calistir.
	       pthread_join(ic_mekan_threadler[i], NULL); // olusturulan threadleri birlestir.
	   }

           printf("└ [Kat %d] İç mekan işlemleri tamamlandı.\n\n", kat);

	   exit(0);// alt process bitinci cik.
	} else {
	   wait(NULL); // bir kattaki dairelerin hepsi bitene kadar bekle.
	}
    }
    printf(">>> Bina İnşaatı Tamamlandı...\n");

    pthread_mutex_destroy(&isci_mutex); // isci kilidinin silinmesi.
    sem_destroy(&vincler); // semafor'un silinmesi.

    return 0; // programi bitir.
}
