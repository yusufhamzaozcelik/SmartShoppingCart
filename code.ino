
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// firebase ve wifi giriş
#define FIREBASE_HOST "iotproject-23aa4-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "akKpPfvXt1HN2rbQdDbmHqhirFtkPlapY10UQP2X"
#define WIFI_SSID "Yusuf"
#define WIFI_PASSWORD "123456789"
//spi haberleşme ve mfrc modülünün kütüphaneleri 
#include <SPI.h>
#include <MFRC522.h>
#define  RST_PIN D2   
#define  SS_PIN D4     // gerekli pinleri nodemcuya belirtiyoruz
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
FirebaseData veritabanim;
int Bakiye=0;
int toplam =0;
void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
   Firebase.reconnectWiFi(true);

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
}

int n = 0;
String tag;
int count_ayakkabi=0;
int count_cips=0;
int count_makarna=0;
int count_cikolata=0;
int count_kraker=0;
int count_popkek=0;
int count_anahtarlik=0;
//priceeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
int ayakkabiPrice=0;
int cipsPrice=0;
int makarnaPrice=0;
int cikolataPrice=0;
int krakerPrice=0;
int popkekPrice=0;
int anahtarlikPrice=0;
String sepetModu;// 0 ise ekleme modu 1 ise çıkartma modu 
String islemBitir;// 1 olursa bütün sistemi 0 layacağız. ardından islem bitir değerini 0 a çekicez.

void loop() {// bütün olay burada
   
    Firebase.getString(veritabanim,"islembitir",islemBitir);
  if(islemBitir=="0"){// uygulamadan işlem bitire basılmadıysa 
    
    tagOku();
    }
  if(islemBitir=="1"){//işlem bitire basıldıysa sistemi sıfırla 

    sifirla();
     //tagOku();  
    }
  

  delay(1000);
    

  
}

void tagOku(){

  String ayakkabi="2312614275";
  String cips="4136116106";
  String makarna="4121117106";
  String cikolata="4132116106";
  String kraker="4128116106";
  String popkek="4124116106";
  String anahtarlik="2022229128";
   Firebase.getString(veritabanim,"sepetmod",sepetModu);
   
  if (rfid.PICC_IsNewCardPresent()){
    
      if (rfid.PICC_ReadCardSerial()) {//hangi tag okunmuşsa analiz et 
        for (byte i = 0; i < 4; i++) {
          tag += rfid.uid.uidByte[i];
        }
        Serial.print(tag);

        if(tag==ayakkabi){
          
         if(sepetModu=="0"){//ekleme modu
               count_ayakkabi++;
              }
              else if (sepetModu=="1"){
                if(count_ayakkabi>=1)
                 count_ayakkabi--;
                }
          Serial.print(" ");
          Serial.print(count_ayakkabi);
          Serial.println(" adet ayakkabi okundu");

          Firebase.setInt(veritabanim,"ayakkabi",count_ayakkabi);
          }
          if(tag==cips){

            if(sepetModu=="0"){
               count_cips++;
              }
              else if (sepetModu=="1"){
                if(count_cips>=1)
                 count_cips--;
                }
           
            Serial.print(" ");
          Serial.print(count_cips);
          Serial.println(" adet cips okundu");

         Firebase.setInt(veritabanim,"cips",count_cips);
      
         delay(50);
          }
          if(tag==makarna){
            if(sepetModu=="0"){
               count_makarna++;
              }
              else if (sepetModu=="1"){
                if(count_makarna>=1)
                 count_makarna--;
                }
            Serial.print(" ");
          Serial.print(count_makarna);
          Serial.println(" adet makarna okundu");

          Firebase.setInt(veritabanim,"makarna",count_makarna);
          }
          if(tag==cikolata){
            if(sepetModu=="0"){
               
               count_cikolata++;
              }
              else if (sepetModu=="1"){
                if(count_cikolata>=1)
                 count_cikolata--;
                }
            Serial.print(" ");
          Serial.print(count_cikolata);
          Serial.println(" adet cikolata okundu");

          Firebase.setInt(veritabanim,"cikolata",count_cikolata);
          }
          if(tag==kraker){
            if(sepetModu=="0"){
            
               count_kraker++;
              }
              else if (sepetModu=="1"){
                   if(count_kraker>=1)
                 count_kraker--;
                }
            Serial.print(" ");
          Serial.print(count_kraker);
          Serial.println(" adet kraker okundu");

          Firebase.setInt(veritabanim,"kraker",count_kraker);
          }
          if(tag==popkek){
            if(sepetModu=="0"){
               count_popkek++;
              }
              else if (sepetModu=="1"){
                   if(count_popkek>=1)
                 count_popkek--;
                }
            Serial.print(" ");
          Serial.print(count_popkek);
          Serial.println(" adet popkek okundu");

          Firebase.setInt(veritabanim,"popkek",count_popkek);
          }
          if(tag==anahtarlik){
           if(sepetModu=="0"){
               count_anahtarlik++;
              }
              else if (sepetModu=="1"){
                if(count_anahtarlik>=1)
                 count_anahtarlik--;
                }
            Serial.print(" ");
          Serial.print(count_anahtarlik);
          Serial.println(" adet anahtarlik okundu");

          Firebase.setInt(veritabanim,"anahtarlik",count_anahtarlik);
          }
        tag = "";
        rfid.PICC_HaltA();
        fiyatHesapla();//okunan ürünlerin fiyatını hesapla ve veritabanına yaz
        Firebase.setInt(veritabanim,"toplam",toplam);
      
      }
  }
}

void sifirla(){
  
  
 count_ayakkabi=0;
 count_cips=0;
 count_makarna=0;
 count_cikolata=0;
 count_kraker=0;
 count_popkek=0;
 count_anahtarlik=0;
 islemBitir="0";
 toplam=0;
  Firebase.setString(veritabanim,"islembitir",islemBitir);
  
  Firebase.setInt(veritabanim,"ayakkabi",count_ayakkabi);
  Firebase.setInt(veritabanim,"cips",count_cips);
  Firebase.setInt(veritabanim,"makarna",count_makarna);
  Firebase.setInt(veritabanim,"kraker",count_kraker);
  Firebase.setInt(veritabanim,"popkek",count_popkek);
   Firebase.setInt(veritabanim,"cikolata",count_cikolata);
  
  Firebase.setInt(veritabanim,"anahtarlik",count_anahtarlik);
  Firebase.setInt(veritabanim,"toplam",toplam);
  }

  void fiyatHesapla(){
    Firebase.getInt(veritabanim,"ayakkabiPrice",ayakkabiPrice);
    Firebase.getInt(veritabanim,"cipsPrice",cipsPrice);
    Firebase.getInt(veritabanim,"makarnaPrice",makarnaPrice);
    Firebase.getInt(veritabanim,"cikolataPrice",cikolataPrice);
    Firebase.getInt(veritabanim,"krakerPrice",krakerPrice);
    Firebase.getInt(veritabanim,"popkekPrice",popkekPrice);
    Firebase.getInt(veritabanim,"anahtarlikPrice",anahtarlikPrice);
      toplam=(count_ayakkabi*ayakkabiPrice)+(count_cips*cipsPrice)+(count_makarna*makarnaPrice)+(count_cikolata*cikolataPrice)+(count_kraker*krakerPrice)+(count_popkek*popkekPrice)+(count_anahtarlik*anahtarlikPrice);
    }
