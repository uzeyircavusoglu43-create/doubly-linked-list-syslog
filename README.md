# Çift Yönlü Bağlı Liste ile Linux Syslog Okuyucu

Bu proje, Sistem Programlama ve Veri Yapıları kapsamında geliştirilmiş bir uygulamadır. Temel amacı, Linux işletim sistemindeki sistem günlüklerini (`syslog`) C programlama dili kullanarak dinamik bir şekilde okumak, hafızada tutmak ve analiz etmektir.

## 📌 Proje Amacı ve Kapsamı
İşletim sistemleri, arka planda çalışan servislerin ve çekirdek (kernel) işlemlerinin durumlarını, hatalarını ve uyarılarını sürekli olarak log dosyalarına kaydeder. Bu proje, söz konusu log satırlarını dosyadan okuyarak **Çift Yönlü Bağlı Liste (Doubly Linked List)** veri yapısı üzerinde tutar ve sistem yöneticileri için anlamlı bir şekilde listeler.

## ⚙️ Kullanılan Veri Yapısı ve Tercih Sebebi

Bu projede dinamik bellek yönetimi (malloc/free) kullanılarak her bir log satırı bellekte bağımsız bir "Düğüm (Node)" olarak tutulmuştur.

**Neden Çift Yönlü Bağlı Liste?**
1. **Dinamik Boyut:** Syslog kayıtlarının boyutu önceden bilinemeyeceği için statik diziler (array) yerine dinamik bir liste kullanılmıştır.
2. **Hızlı Ekleme (O(1)):** Loglar doğası gereği kronolojik olarak art arda üretilir. Yeni gelen her kayıt, `Tail` (kuyruk) işaretçisi yardımıyla listenin en sonuna anında eklenir.
3. **Sondan Başa Okuma Avantajı:** Sistem yöneticileri logları incelerken genellikle en eski kayıtlardan ziyade **en son gerçekleşen hataları** (en güncel logları) görmek ister. Çift yönlü liste içerisindeki `prev` (önceki) işaretçisi sayesinde, hiçbir ekstra sıralama algoritmasına ihtiyaç duymadan loglar sondan başa doğru (yeniden eskiye) listelenebilmektedir. Bu durum Linux'taki `tail -f /var/log/syslog` komutunun çalışma mantığıyla birebir örtüşmektedir.

## 🚀 Kurulum ve Çalıştırma

Projeyi yerel makinenizde çalıştırmak için sisteminizde bir C derleyicisi (örn. GCC) kurulu olmalıdır.

1. Projeyi bilgisayarınıza indirin:
   ```bash
  git clone https://github.com/uzeyircavusoglu43-create/doubly-linked-list-syslog.git

2.Kodu derleyin:

Bash
gcc main.c -o syslog_reader

3.Aynı dizinde syslog.txt adında örnek bir log dosyası bulunduğundan emin olun ve programı çalıştırın:

Bash
./syslog_reader
