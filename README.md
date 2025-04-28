# Rover na daljinsko upravljanje

## Sadržaj
- [Opis projekta](#opis-projekta)
- [Funkcijski zahtjevi](#funkcijski-zahtjevi)
- [Komponente](#komponente)
- [Blok sheme](#blok-sheme)
- [Upute za korištenje](#upute-za-korištenje)
- [Programska podrška](#programska-podrška)
- [Poznate greške i ograničenja](#poznate-greške-i-ogrančenja)
- [Planovi za budući razvoj](#planovi-za-budući-razvoj)
- [Licenca](#licenca)

---

## Opis projekta
Ovaj projekt izrađen je kao projektni zadatak kolegija **Razvoj ugrađenih sustava** na **Tehničkom veleučilištu u Zagrebu**.  
Ideja je izraditi robota na daljinsko upravljanje s prijenosom slike kao hobi maketu, a s vremenom ga razviti u robota za istraživanje nepristupačnih prostora.

Za konstrukciju i pogon robota korištena je platforma s gusjenicama.

U trenutnoj fazi razvoja, robotom se upravlja PlayStation 3 kontrolerom, no protokol komunikacije nije ograničen isključivo na funkcije tog kontrolera. U ovom slučaju, PlayStation 3 kontroler šalje podatke predajniku koji ih prima, prevodi u protokol razumljiv robotu, te ih zatim odašilje robotu. Predajnik tako služi kao posrednik u komunikaciji i omogućuje bežični prijenos podataka.

Kao "mozak" robota koristi se **Arduino Nano**. On prima podatke od predajnika, upravlja motorima, očitava stanje baterije te komunicira s kamerom radi uključivanja/isključivanja svjetla i ulaska u "sleep" način rada.

Za funkciju kamere koristi se **ESP32-CAM** s **OV2640** senzorom. ESP32-CAM putem Wi-Fi mreže prenosi video zapis koji se može pregledavati u web pregledniku ili putem media playera koji podržava video streamove (npr. VLC ili mpv).

Korištena varijanta OV2640 senzora opremljena je lećama sa **120° FOV (Field of View)**, što osigurava širu percepciju prostora. Prednji dio robota lagano je vidljiv u kadru, čime se postiže bolji osjećaj dimenzija robota i njegovog odnosa s okolinom.

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
| Litijska baterija (2s)     | 1        |
| Modul za punjenje baterije | 1        |
| buck regulator napona sa izlaznim naponom 5V i enable pinom (npr. S09) | 1 |
| buck regulator napona sa izlaznim naponom 3.3VV i enable pinom (npr. S09) | 1 |
| Otpornik 1kOhm | 1 |
| Otpornik 6.7kOhm | 2 |
| Otpornik 10kOhm | 3 |
| Otpornik 30kOhm | 1 |
| LED dioda crvena | 1 |
| Arduino UNO R3             | 1        |
| USB host shield            | 1        |
| Generic bluetooth USB dongle | 1 |
| PlayStation 3 kontroler | 1 |
| Programator za ESP32-CAM | 1 |
| USB B kabel (za Arduino Uno) | 1 |
| USB mini B kabel (za Arduino Nano) | 1 |
| USB Micro B kabel (za ESP32-CAM programator) | 1 |

---

## Blok sheme

### Predajnik - blok shema
![blok_shema_predajnik drawio](https://github.com/user-attachments/assets/3c8b8bbf-0eb3-4c24-9e46-f98edcd10e87)

### Robot - blok shema
![blok_shema_kontroler drawio](https://github.com/user-attachments/assets/47ea0b02-8f2b-443d-82a4-00d7820ead77)

---

## Upute za korištenje

1. Spojiti predajnik i robota prema shemi u repozitoriju (trenutno nije dostupna, biti će uskoro).
2. Pomoću Arduino IDE snimiti kod iz repozitorija na razvojne pločice (Arduino UNO, Arduino Nano, ESP32-CAM)
3. Priključiti USB Bluetooth dongle i zabilježiti njegovu MAC adresu.
4. Povezati PlayStation 3 kontroler na računalo USB kablom te pomoću SixAxisPairingTool alata usnimiti mu ranije zabilježenu MAC adresu.
5. Na računalu ili mobilnom uređaju otvoriti web preglednik i pristupiti IP adresi ESP32-CAM-a za pregled slike (detaljnije o ovom koraku kasnije u tekstu).
6. Upravljati robotom pomoću PlayStation 3 kontrolera.

**Napomene:**  
- Potrebne knjižnice i firmware su opisani u sekciji [Programska podrška](#programska-podrška).
- Za uspješan upload koda na ESP32-CAM potrebno je instalirati "esp32" (by Espressif Systems) iz Board Managera u Arduino IDE.

### Povezivanje računala/mobitela sa ESP32-CAM modulom.
U kodu za kameru koji se nalazi u repozitoriju (kodovi/kamera/kamera.ino) potrebno je izmjeniti varijable SSID i Password tako da odgovaraju WiFi Access point-u sa kojime raspolažemo. To može biti WiFi kućna mreža ili HotSpot pametnog telefona. Nakon dodavanja SSID i Password parametara potrebno je kod usnimiti u ESP32-CAM. 
Nakon snimanja koda, dok je ESP32-CAM još povezan na računalo putem programatora, ESP32-CAM će u serijsku konzolu ispisati svoju IP adresu koju je dobio od DHCP servera. U praksi ta adresa se ne mijenja ćesto, dokle god je ESP na istoj mreži. No treba imati na umu da se ova adresa može promjeniti te ako ne možemo dobiti ping od ESP-a potrebno ga je isključiti iz robota, spojiti na programator i ponovo u serijskoj konzoli pročitati novu IP adresu. 
NAPOMENA: Da bi ESP vratio ispisao svoju IP adresu u serijsku konzolu, i da bi se uopće pokrenuo kako treba, korištena WiFi mreža MORA biti dostupna.
Video prijenosu može se pristupiti na dva načina. 
- Navigiranjem na <IP_adresa> bez dodatnih parametara otvara se WEB sučelje ESP32-cam modula u kojemu se mogu mijenjati parametri kamere te gledati prijenos.
- Navigiranjem na <IP_adresa>:81/stream prikazuje se samo prijenos slike bez dodatnih opcija.

U slučaju poknuća veze ili gašenja i paljenja robota, nije potrebno ponovo upisivati url kamere, dovoljno je samo osvježiti web preglednik (stranicu).

### Razina napunjenosti baterije
Kada robot registrira da je razina baterije pala ispod 20% ( < 7.4V ), signalizirati će to paljenjem LED-ice na predajniku.

### Ulazak u sleep mode
Ako se predajnik ili PlayStation kontroller isključe ili pak robot izgubi vezu sa predajnikom, robot nakon isteka vremena od pola sekunde ulazi u sleep mode.
Sleep mode je implementiran tako da robot gasi kameru (ESP32-cam ulazi u deep sleep) te isključuje napajanje Servo motorima i l293d motor driver-u.
Arduino i HC-12 moraju ostati budni radi osluškivanja za novim signalom te buđenje robota. Kako ESP32-CAM prilikom prijenosa slike i svaki servo motor samostalno troše više energije neho Arduino Nano i HC-12 zajedno, ovaj način uštede energije sasvim je dovoljan da robot može nekoliko sati ostati uključen bez da se osjeti pad razine baterije.
Dugoročni plan je dodati još jedan sleep mode u kojemu bi ugasilo napajanje ESP-u i HX-12, a Arduino Nano bi ušao u sleep gdje bi ga Timer probudio nakon određenog vremena. Isključivanje regulatora napona za ESP i HC-12 hardwer-ski je več omogućeno. Ovaj pristup teoretski bi omogučio robotu da danima spava no ima smisla jedino ako robot mora biti prisutan na nekoj lokaciji dugo vremena. Trenutno za izradu hobby modela ova funkcionanost nije potrebna.

---

## Programska podrška

- **Arduino Nano**:
  - Upravljanje motorima na osnovi zaprimljenih podataka.
  - Praćenje napona baterije.
  - Kontrola rasvjete i sleep moda robota.
- **ESP32-CAM**:
  - Streamanje slike putem HTTP servera.

**Potrebne knjižnice:**
- `PS3BT` - https://github.com/felis/USB_Host_Shield_2.0
- `Servo.h` - moguće preuzeti putem Library Managera u Arduino IDE
- Ostale standardne Arduino knjižnice.

Kod će biti dostupan u `/firmware` direktoriju repozitorija.

---

## Poznate greške i ograničenja

- Ponekad mogući gubitak komunikacije između HC-12 modula na većim udaljenostima (>50m bez izravne vidljivosti).
- ESP32-CAM ponekad gubi Wi-Fi vezu ako signal nije dovoljno jak te je moguće osjetiti zastoje u prijenosu slike pa i puknuće veze.

---

## Planovi za budući razvoj

- Razvoj vlastitog kontrolera za poboljšano upravljanje.
- Napuštnje WiFi veze za prijenos slike radi korištenja neke bolje tehnologije.
- Zamijeniti Arduino Nano kontrolerom sa više pinova i perifernih opcija (za izradu ovog robota iskorišteni su gotovo svi pinovi Arduino-a a napredne periferne funkcije kao SPI u koliziji su sa PWM izlazom Timer-a 2 koji je korišten za upravljanje brzinom motora.
- po potrebi zamjeniti HC-12 sa NRF24l01+pa+lna modula (kada se oslobodi SPI) ili pak koristiti oboje radi kombiniranja prednosti oba tipa veze ovisno o potrebama (koriste različite frekvencije koje ovisno o terenu mogu bolje ili lošije prodirati, imaju različite snage odašiljanja a samim time i potrošnju energije...).
- Upravljati PWM frekvencijom radi smanjenja šuma koji dolazi is DC motora - pogona. (Zato je za kontrolu brzine kretanja korišten Timer 2 no funkcionalnost promjene frekvencije još nije implementirana).
- ugradnja LCD ekrana na prdajnik radi ispisivanja dijagnostičkih podataka i očitanja senzora sa robota (precizno ispisivanje razine baterije).
- Sleep mode robota na način da se budi nakon isteka određenog vremena.

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

---

