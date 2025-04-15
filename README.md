# Rover na daljinsko upravljanje #
# Opis projekta #
Ovaj projekt je projejktni zadatak kolegija Razvoj uggradbenih sustava 
na Tehničkom veleučilištu u Zagrebu. 
Ideja je napraviti robota na daljinsko upravljanje sa prijenosom slike 
kao hobby maketu a sa vremenom trazviti u robotaza istraživanje 
nepristupačnih prostora.

# Funkcijski zahtjevi #
1. Kretanje u prostoru na daljinsko upravljanje.
2. Baterijsko napajanje.
3. Bežični prijenos slike putem WiFi-a na pametni telefon ili računalo.
4. Artikulacijski nosač kamere kako bi se kamera neovisno o platformi mogla kretati.
5. Rasvijetna tijela za kretanje u mračnim prostorima.
6. Mjerenje napona baterije.
7. Standardizirati komunikacijski protokol kako bi se moglo koristiti više različitih tipova kontrolera (kontroler specifično dizajniran za ovaj rover, PlayStation kontroler, tipkovnica za računalo...).
8. Domet pedesetak metara u otvorenom prostoru.

# Komponente #

| Komponenta        | Količina |
| ----------------- | -------- |
| Arduino Nano      | 1        |
| ESP32-CAM         | 1        |
| OV2640 (120° FOV) | 1        |
| Platforma sa gusjenicama | 1        |
| nrf24l01+pa+lna modul        | 2        |
| Artikulacijski nosač za kameru  | 1        |
