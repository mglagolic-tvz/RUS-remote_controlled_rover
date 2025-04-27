# Rover na daljinsko upravljanje #
# Opis projekta #
Ovaj projekt je projektni zadatak kolegija Razvoj uggradbenih sustava 
na Tehničkom veleučilištu u Zagrebu. 
Ideja je napraviti robota na daljinsko upravljanje sa prijenosom slike 
kao hobby maketu a sa vremenom razviti u robota za istraživanje 
nepristupačnih prostora.

Za konstrukciju i pogon robota korištena je platforma s gusjenicama.

U trenutnoj fazi razvoja, robotom se upravlja PlayStation 3 kontrolerom, no protokol komunikacije nije ograničen isključivo na funkcije tog kontrolera. U ovom slučaju, PlayStation 3 kontroler šalje podatke predajniku koji ih prima, prevodi u protokol razumljiv robotu, te ih zatim odašilje robotu. Predajnik tako služi kao posrednik u komunikaciji i ujedno omogućuje bežični prijenos podataka.

Kao "mozak" robota koristi se Arduino Nano. On prima podatke od predajnika, upravlja motorima, očitava stanje baterije te komunicira s kamerom radi uključivanja/isključivanja svjetla i ulaska u "sleep" način rada.

Za funkciju kamere koristi se ESP32-CAM s OV2640 senzorom. ESP32-CAM putem Wi-Fi mreže prenosi video zapis koji se može pregledavati u web pregledniku ili putem media playera koji podržava video streamove (npr. VLC ili mpv).

Korištena varijanta OV2640 senzora opremljena je lećama sa 120° FOV (Field of View), što osigurava da vozač ima osjećaj šire percepcije prostora. Prednji dio robota lagano je vidljiv u kadru, što dodatno doprinosi boljem osjećaju dimenzija robota i njegovog odnosa s okolinom.

# Funkcijski zahtjevi #
1. Kretanje u prostoru na daljinsko upravljanje.
2. Baterijsko napajanje.
3. Bežični prijenos slike putem WiFi-a na pametni telefon ili računalo.
4. Artikulacijski nosač kamere kako bi se kamera neovisno o platformi mogla kretati.
5. Rasvijetno tijelo za kretanje u mračnim prostorima.
6. Mjerenje napona baterije.
7. Standardizirati komunikacijski protokol kako bi se moglo koristiti više različitih tipova kontrolera (kontroler specifično dizajniran za ovaj rover, PlayStation kontroler, tipkovnica za računalo...).

# Komponente #

| Komponenta        | Količina |
| ----------------- | -------- |
| Arduino Nano      | 1        |
| ESP32-CAM         | 1        |
| OV2640 (120° FOV) | 1        |
| Platforma sa gusjenicama | 1        |
| HC-12 modul        | 2        |
| Artikulacijski nosač za kameru  | 1        |

# Shema #
## predajnik ##
