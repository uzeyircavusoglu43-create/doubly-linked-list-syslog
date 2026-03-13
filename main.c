#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* * 1. VERİ YAPISI TANIMLAMASI
 * Syslog kayıtlarını tutacak olan Çift Yönlü Bağlı Liste düğüm (node) yapısı.
 * Her düğüm log metnini ve bir önceki/sonraki düğümlerin adreslerini tutar.
 */
typedef struct SyslogNode {
    char logMessage[512];       // Log satırını tutacak metin dizisi
    struct SyslogNode* prev;    // Bir önceki log kaydını gösteren işaretçi (pointer)
    struct SyslogNode* next;    // Bir sonraki log kaydını gösteren işaretçi (pointer)
} Node;

// Bağlı listenin başını ve sonunu takip etmek için global veya fonksiyona özel işaretçiler kullanılabilir.
// Bu örnekte main içinde tanımlayıp fonksiyonlara parametre olarak göndereceğiz.

/*
 * YENİ DÜĞÜM OLUŞTURMA FONKSİYONU
 * Parametre olarak gelen log metnini dinamik bellek (malloc) kullanarak yeni bir düğüme atar.
 */
Node* create_node(const char* message) {
    // Bellekte yeni bir düğüm için yer ayırıyoruz
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Hata: Bellek tahsisi basarisiz oldu!\n");
        exit(1);
    }
    
    // Gelen metni düğümün içindeki logMessage değişkenine kopyalıyoruz
    strcpy(newNode->logMessage, message);
    newNode->prev = NULL;
    newNode->next = NULL;
    
    return newNode;
}

/*
 * LİSTEYE LOG EKLEME FONKSİYONU
 * Syslog kayıtları kronolojik olduğu için yeni gelen her log, listenin sonuna (tail) eklenir.
 */
void insert_log(Node** head, Node** tail, const char* message) {
    Node* newNode = create_node(message);

    // Eğer liste boşsa, yeni düğüm hem head hem de tail olur
    if (*head == NULL) {
        *head = newNode;
        *tail = newNode;
    } else {
        // Liste boş değilse, mevcut tail'in sonuna yeni düğümü ekliyoruz
        (*tail)->next = newNode;  // Eski son düğümün next'i yeni düğümü göstersin
        newNode->prev = *tail;    // Yeni düğümün prev'i eski son düğümü göstersin
        *tail = newNode;          // Tail işaretçisini yeni düğüm olarak güncelleyelim
    }
}

/*
 * İLERİYE DOĞRU YAZDIRMA FONKSİYONU (Eskiden Yeniye)
 * Listenin başından (head) başlayarak sonuna kadar tüm logları ekrana basar.
 */
void print_logs_forward(Node* head) {
    printf("\n--- SYSLOG KAYITLARI (KRONOLOJIK SIRAYLA) ---\n");
    Node* current = head;
    while (current != NULL) {
        printf("%s", current->logMessage);
        current = current->next;
    }
    printf("---------------------------------------------\n");
}

/*
 * GERİYE DOĞRU YAZDIRMA FONKSİYONU (Yeniden Eskiye)
 * Çift yönlü bağlı listenin en büyük avantajı: Listenin sonundan (tail) başa doğru okuma.
 * Sistem yöneticileri genelde en son (en güncel) hataları görmek ister.
 */
void print_logs_reverse(Node* tail) {
    printf("\n--- EN GUNCEL LOGLAR (SONDAN BASA DOGRU) ---\n");
    Node* current = tail;
    while (current != NULL) {
        printf("%s", current->logMessage);
        current = current->prev; // Geriye doğru gidiyoruz
    }
    printf("---------------------------------------------\n");
}

/*
 * BELLEK TEMİZLEME FONKSİYONU (Memory Management)
 * Program kapanmadan önce malloc ile ayrılan bellek alanlarını sisteme geri iade eder.
 */
void free_list(Node* head) {
    Node* current = head;
    Node* nextNode;
    
    while (current != NULL) {
        nextNode = current->next; // Bir sonraki düğümün adresini yedekle
        free(current);            // Mevcut düğümü bellekten sil
        current = nextNode;       // Sonraki düğüme geç
    }
}

/*
 * ANA FONKSİYON
 */
int main() {
    Node* head = NULL; // Listenin başı
    Node* tail = NULL; // Listenin sonu
    
    char buffer[512];
    FILE* file;

    // syslog.txt dosyasını okuma modunda açıyoruz
    file = fopen("syslog.txt", "r");
    if (file == NULL) {
        printf("Hata: syslog.txt dosyasi bulunamadi. Lutfen ayni dizine bir ornek dosya ekleyin.\n");
        return 1;
    }

    // Dosyayı satır satır okuyup bağlı listeye ekliyoruz
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        insert_log(&head, &tail, buffer);
    }
    fclose(file); // Dosya ile işimiz bitti, kapatıyoruz

    // Fonksiyonları test edelim
    print_logs_forward(head);
    
    // Çift yönlü listenin tercih edilme sebebini gösteren fonksiyon
    print_logs_reverse(tail);

    // Belleği temizleyip programı sonlandırıyoruz
    free_list(head);
    
    return 0;
}