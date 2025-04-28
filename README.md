# Rover na daljinsko upravljanje

## Sadržaj
- [Opis projekta](#opis-projekta)
- [Funkcijski zahtjevi](#funkcijski-zahtjevi)
- [Komponente](#komponente)
- [Blok sheme](#blok-sheme)
- [Upute za korištenje](#upute-za-korištenje)
- [Programska podrška](#programska-podrška)
- [Poznate greške i ograničenja](#poznate-greške-i-ograničenja)
- [Planovi za budući razvoj](#planovi-za-budući-razvoj)
- [Licenca](#licenca)

---

## Opis projekta
Ovaj projekt izrađen je kao projektni zadatak kolegija **Razvoj ugrađenih sustava** na **Tehničkom veleučilištu u Zagrebu**.  
Ideja je izraditi robota na daljinsko upravljanje s prijenosom slike kao hobi maketu, a s vremenom ga razviti u robota za istraživanje nepristupačnih prostora.

Za konstrukciju i pogon robota korištena je platforma s gusjenicama.

U trenutnoj fazi razvoja, robotom se upravlja PlayStation 3 kontrolerom, no protokol komunikacije nije ograničen isključivo na funkcije tog kontrolera. U ovom slučaju, PlayStation 3 kontroler šalje podatke predajniku koji ih prima, prevodi u protokol razumljiv robotu te ih zatim odašilje robotu. Predajnik tako služi kao posrednik u komunikaciji i omogućuje bežični prijenos podataka.

Kao "mozak" robota koristi se **Arduino Nano**. On prima podatke od predajnika, upravlja motorima, očitava stanje baterije te komunicira s kamerom radi uključivanja/isključivanja svjetla i ulaska u "sleep" način rada.

Za funkciju kamere koristi se **ESP32-CAM** s **OV2640** senzorom. ESP32-CAM putem Wi-Fi mreže prenosi video zapis koji se može pregledavati u web pregledniku ili putem media playera koji podržava video streamove (npr. VLC ili mpv).

Korištena varijanta OV2640 senzora opremljena je lećama sa **120° FOV (Field of View)**, što osigurava širu percepciju prostora. Prednji dio robota lagano je vidljiv u kadru, čime se postiže bolji osjećaj dimenzija robota i njegovog odnosa s okolinom.

---

## Fotografija robota
![Robot-slika](https://github.com/user-attachments/assets/5e5f7a7a-c142-4b9a-95c3-88bc530a2e6c)




---

## Funkcijski zahtjevi
- Kretanje u prostoru na daljinsko upravljanje.
- Baterijsko napajanje.
- Bežični prijenos slike putem WiFi-a na pametni telefon ili računalo.
- Artikulacijski nosač kamere za neovisno usmjeravanje kamere.
- Rasvjetno tijelo za kretanje u mračnim prostorima.
- Mjerenje napona baterije.
- Standardizirani komunikacijski protokol za podršku različitim tipovima kontrolera (npr. specifični kontroler za rover, PlayStation kontroler, tipkovnica za računalo).

---

## Komponente

| Komponenta               | Količina |
| ------------------------- | :------: |
| Arduino Nano              | 1        |
| ESP32-CAM                 | 1        |
| OV2640 kamera (120° FOV)   | 1        |
| Platforma s gusjenicama    | 1        |
| HC-12 bežični modul        | 2        |
| Artikulacijski nosač kamere| 1        |
| Litijska baterija (2S)     | 1        |
| Modul za punjenje baterije | 1        |
| Buck regulator napona sa izlaznim naponom 5V i enable pinom (npr. S09) | 1 |
| Buck regulator napona sa izlaznim naponom 3.3V i enable pinom (npr. S09) | 1 |
| Logic level shifter (barem 4 kanala) | 1 |
| Otpornik 1 kΩ | 1 |
| Otpornik 6.7 kΩ | 2 |
| Otpornik 10 kΩ | 3 |
| Otpornik 30 kΩ | 1 |
| LED dioda crvena | 1 |
| Arduino UNO R3             | 1        |
| USB Host Shield            | 1        |
| Generic Bluetooth USB dongle | 1 |
| PlayStation 3 kontroler | 1 |
| Programator za ESP32-CAM | 1 |
| USB B kabel (za Arduino Uno) | 1 |
| USB Mini B kabel (za Arduino Nano) | 1 |
| USB Micro B kabel (za ESP32-CAM programator) | 1 |

---

## Blok sheme

### Predajnik - blok shema
![blok_shema_predajnik drawio](https://github.com/user-attachments/assets/3c8b8bbf-0eb3-4c24-9e46-f98edcd10e87)

### Robot - blok shema
![blok_shema_kontroler drawio](https://github.com/user-attachments/assets/47ea0b02-8f2b-443d-82a4-00d7820ead77)

---
## Opis komunikacijskog protokola

Za komunikaciju između predajnika i robota koristi se jednostavan binarni protokol temeljen na slanju podataka u byte formatu.  
Protokol je dizajniran tako da bude lagan za obradu i otporan na pogreške te omogućava jednostavno proširivanje.

### Struktura paketa
Paket podataka koji se šalje od predajnika prema robotu sastoji se od sljedećih elemenata:

- **Start byte** (`0xAA`) – označava početak paketa.
- **Podaci** – niz parova `ID` i `vrijednost`:
  - `ID` definira tip podatka (npr. brzina motora 1, rotacija kamere, upravljanje svjetlom itd.),
  - `vrijednost` predstavlja konkretan podatak (npr. brzina motora, kut serva, uključeno/isključeno svjetlo) koji je 8-bitna vrijednost.
- **Stop byte** (`0x55`) – označava kraj paketa.

### Primjer paketa

| Byte pozicija | Vrijednost | Opis              |
| ------------- | ---------- | ----------------- |
| 0             | `0xAA`      | Start byte        |
| 1             | `0x01`      | ID - Brzina motora 1 |
| 2             | `0x7F`      | Vrijednost (127 = pola brzine) |
| 3             | `0x02`      | ID - brzina motora 2 |
| 4             | `0x00`      | Vrijednost (0 = ugašen) |
| 5             | `0x03`      | ID - smjer motora 1 |
| 6             | `0x01`      | Vrijednost (1 = naprijed) |
niz parova ID-vrijednost se nastavlja
| N             | `0x55`      | Stop byte         |

---
## Upute za korištenje

1. Spojiti predajnik i robota prema shemi u repozitoriju (trenutno nije dostupna, bit će uskoro).
2. Pomoću Arduino IDE-a snimiti kod iz repozitorija na razvojne pločice (Arduino UNO, Arduino Nano, ESP32-CAM).
3. Priključiti USB Bluetooth dongle na računalo i zabilježiti njegovu MAC adresu.
4. Povezati PlayStation 3 kontroler na računalo USB kablom te pomoću SixAxisPairingTool alata usnimiti mu ranije zabilježenu MAC adresu.
5. Na računalu ili mobilnom uređaju otvoriti web preglednik i pristupiti IP adresi ESP32-CAM-a za pregled slike (detaljnije o ovom koraku kasnije u tekstu).
6. Upravljati robotom pomoću PlayStation 3 kontrolera.

**Napomene:**  
- Potrebne knjižnice i firmware opisani su u sekciji [Programska podrška](#programska-podrška).
- Za uspješan upload koda na ESP32-CAM potrebno je instalirati "esp32" (by Espressif Systems) iz Board Managera u Arduino IDE-u.

### Povezivanje računala/mobitela s ESP32-CAM modulom
U kodu za kameru koji se nalazi u repozitoriju (`kodovi/kamera/kamera.ino`) potrebno je izmijeniti varijable SSID i Password tako da odgovaraju Wi-Fi Access Point-u kojim raspolažemo. To može biti Wi-Fi kućna mreža ili HotSpot pametnog telefona. Nakon dodavanja SSID i Password parametara, potrebno je kod usnimiti u ESP32-CAM. 
Nakon snimanja koda, dok je ESP32-CAM još povezan na računalo putem programatora, ESP32-CAM će u serijsku konzolu ispisati svoju IP adresu koju je dobio od DHCP servera. U praksi ta adresa se rijetko mijenja dokle god je ESP na istoj mreži, no treba imati na umu da se može promijeniti. Ako ne možemo dobiti ping od ESP-a, potrebno ga je isključiti iz robota, spojiti na programator i ponovno u serijskoj konzoli pročitati novu IP adresu. 

**Napomena:** Da bi ESP ispisao svoju IP adresu i ispravno se pokrenuo, korištena Wi-Fi mreža **mora** biti dostupna.

Video prijenosu može se pristupiti na dva načina: 
- Navigiranjem na `<IP_adresa>` bez dodatnih parametara otvara se web sučelje ESP32-CAM modula u kojem se mogu mijenjati parametri kamere te gledati prijenos.
- Navigiranjem na `<IP_adresa>:81/stream` prikazuje se samo prijenos slike bez dodatnih opcija.

U slučaju prekida veze ili gašenja i paljenja robota, nije potrebno ponovo upisivati URL kamere; dovoljno je samo osvježiti web preglednik (stranicu).

### Razina napunjenosti baterije
Kada robot registrira da je razina baterije pala ispod 20% (<7.4V), signalizirat će to paljenjem LED-ice na predajniku.

### Ulazak u sleep mode
Ako se predajnik ili PlayStation kontroler isključe ili ako robot izgubi vezu s predajnikom, robot nakon isteka vremena od pola sekunde ulazi u sleep mode.  
Sleep mode je implementiran tako da robot gasi kameru (ESP32-CAM ulazi u deep sleep) te isključuje napajanje servo motorima i L293D motor driveru.  
Arduino i HC-12 ostaju budni radi osluškivanja novog signala i buđenja robota.  
Kako ESP32-CAM tijekom prijenosa slike i svaki servo motor samostalno troše više energije nego Arduino Nano i HC-12 zajedno, ovakav način štednje energije dovoljan je da robot ostane uključen nekoliko sati bez značajnog pada razine baterije.

Dugoročni plan je dodati još jedan sleep mode u kojem bi se ugasilo napajanje ESP-u i HC-12, a Arduino Nano bi ušao u sleep s Timer buđenjem nakon određenog vremena.  
Isključivanje regulatora napona za ESP i HC-12 hardverski je već omogućeno. Ovaj pristup teoretski bi omogućio robotu da "spava" danima, no smislen je samo ako robot mora dugo boraviti na nekoj lokaciji. Trenutno, za hobi model, ova funkcionalnost nije potrebna.

---

## Programska podrška

- **Arduino Nano**:
  - Upravljanje motorima na temelju zaprimljenih podataka.
  - Praćenje napona baterije.
  - Kontrola rasvjete i sleep moda robota.
- **ESP32-CAM**:
  - Streamanje slike putem HTTP servera.

**Potrebne knjižnice:**
- [`PS3BT`](https://github.com/felis/USB_Host_Shield_2.0)
- `Servo.h` — moguće preuzeti putem Library Managera u Arduino IDE-u
- Ostale standardne Arduino knjižnice.

Kod će biti dostupan u `/firmware` direktoriju repozitorija.

---

## Poznate greške i ograničenja

- Povremeni gubitak komunikacije između HC-12 modula na većim udaljenostima (>50m bez izravne vidljivosti).
- ESP32-CAM može izgubiti Wi-Fi vezu ako signal nije dovoljno jak, što može uzrokovati zastajkivanja u prijenosu slike ili prekid veze.

---

## Planovi za budući razvoj

- Razvoj vlastitog kontrolera za poboljšano upravljanje.
- Napuštanje Wi-Fi veze za prijenos slike radi korištenja kvalitetnije tehnologije.
- Korištenje boljih antena.
- Zamjena Arduino Nano s mikrokontrolerom s više pinova i perifernih opcija (trenutno su gotovo svi pinovi iskorišteni, a napredne periferne funkcije poput SPI-ja kolidiraju s PWM izlazom Timer-a 2 korištenim za upravljanje brzinom motora).
- Po potrebi zamijeniti HC-12 s NRF24L01+PA+LNA modulom (kad se oslobodi SPI) ili koristiti oba modula kombinirano.
- Upravljanje PWM frekvencijom radi smanjenja šuma iz DC motora.
- Ugradnja LCD ekrana na predajnik za ispis dijagnostičkih podataka i očitanje razine baterije.
- Implementacija sleep moda robota s periodičkim buđenjem.

---

## Licenca

Ovaj projekt je objavljen pod GNU General Public License v3.0.

```
Copyright (C) 2025

Ovaj program je slobodan softver: možete ga redistribuirati i/ili modificirati pod uvjetima
GNU General Public License kao što je objavila Free Software Foundation, bilo verzije 3 licence,
ili (po vašem izboru) bilo koje kasnije verzije.

Ovaj program se distribuira u nadi da će biti koristan, ali BEZ IKAKVOG JAMSTVA;
čak ni bez impliciranog jamstva za PRODAJNOST ili PRIKLADNOST ZA ODREĐENU NAMJENU.
Za više detalja pogledajte GNU General Public License.

Trebali ste primiti kopiju GNU General Public License uz ovaj program.
Ako niste, pogledajte <https://www.gnu.org/licenses/>.
```
