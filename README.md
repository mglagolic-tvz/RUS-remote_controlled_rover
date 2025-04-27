# Rover na daljinsko upravljanje

## Sadržaj
- [Opis projekta](#opis-projekta)
- [Funkcijski zahtjevi](#funkcijski-zahtjevi)
- [Komponente](#komponente)
- [Shema](#shema)

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
| Arduino UNO R3 | 1 |
| USB host shield | 1 |

---

## Blok sheme

### Predajnik
![blok_shema_predajnik drawio](https://github.com/user-attachments/assets/ed0e8707-699d-41ea-8bc3-c1ebee23289e)

### Robot
![blok_shema_kontroler drawio](https://github.com/user-attachments/assets/6a426cbe-30f8-484f-84c3-ec68f17f3ff7)



