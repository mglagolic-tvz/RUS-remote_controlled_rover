# Projekt: Rad s prekidima na ugradbenim platformama

## Sadržaj

- Opis projekta
- Komponente
- Dijagram spoja
- Funkcionalnosti
- Dijagrami odaziva na prekid
- Pokretanje

## Opis projekta

Projekt služi kao jednostavan primjer kako koristiti prekide na ugradbenim platformama (npr. Arduino). Korištene su tipke, PIR senzor pokreta, LED-ice i zvučnik za simulaciju reakcija sustava na različite ulazne događaje.

Glavni cilj je demonstrirati kako se koriste različite vrste prekida i na koji način oni utječu na izvođenje glavne petlje programa.

## Komponente

U projektu su korištene sljedeće komponente:

| Komponenta        | Količina |
| ----------------- | -------- |
| Arduino Mega 2560 | 1        |
| Tipkalo           | 3        |
| LED diode         | 4        |
| Otpornici (1 kOhm)| 4        |
| PIR senzor        | 1        |
| Zvučnik (buzzer)  | 1        |

## Dijagram spoja

![Prikaz spoja](https://github.com/user-attachments/assets/0e297833-a839-4f63-8892-9b0d1ccddf2d)

## Funkcionalnosti

- **Tipka 1** (INT0, pin 21): Mijenja stanje LED-ice.
- **Tipka 2** (INT1, pin 20): Pokreće blokirajući ISR koji trepće LED-icom kako bi signalizirao da rutina traje. Koristi se za prikaz kako dugotrajni prekid može blokirati ostatak sustava.
- **Tipka 3** (INT2, pin 19): Postavlja zastavicu koja uzrokuje da LED trepne 5 puta u glavnoj petlji.
- **PIR senzor pokreta** (INT4, pin 2): Aktivira zvučni alarm kad se detektira kretanje.
- **Timer1**: Pokreće prekid svakih 30 ms koji upravlja PWM-om za efekt disanja na LED-ici.
- **Serijski monitor**: Koristi se za ispis trajanja blokirajućeg ISR-a.

### Tablica funkcionalnosti

| Oznaka | Funkcija                                                  |
| ------ | --------------------------------------------------------- |
| FR-1   | Detekcija pritiska na tipkala i upravljanje LED-icama     |
| FR-2   | Softverski debounce                                       |
| FR-3   | Blokirajući ISR za tipku 2                                |
| FR-4   | Timer prekid i PWM upravljanje LED-om                     |
| FR-5   | Detekcija pokreta i aktivacija alarma                     |
| FR-6   | Ispis statusa na serijski monitor                         |

## Dijagrami odaziva na prekid

Dijagrami u nastavku prikazuju odaziv svake ledice na ulazni signal.

![INT0](https://github.com/user-attachments/assets/1156fa31-ca2b-44c7-96f3-3612c177e922)
![INT1](https://github.com/user-attachments/assets/dbd0bac0-b8bc-4521-8fba-0eb4311658af)
![INT2](https://github.com/user-attachments/assets/8f93099b-690a-4205-92c2-211339e3acea)

Iz sljedećeg dijagrama vidljivo je kako blokirajuća prekidna rutina onemogućuje 
izvršavanje drugih prekidnih rutina i ostatka koda. Kao što je vidljivo, 
kada bkolirajuća prekidna rutina završi, prethodno pokrenuti zadatci 
nastavljaju se dalje (blinkanje LED 3) a prekid koji se dogodio za vrijeme 
izvršavanja blokirajuće prekidne rutine naknadno će se izvršiti po završetku blokirajuće rutine.

![ZAJEDNO](https://github.com/user-attachments/assets/f07d1468-7d18-4fc6-a92a-c1d042704310)

## Pokretanje

Kod se može isprobati u Wokwi simulatoru ili na fizičkom Arduino Mega 2560.

Za pokretanje je potrebno:

1. Učitati `Arduino.ino` datoteku u Arduino IDE
2. Spojiti komponente prema slici (gore).
3. Učitati kod na uređaj i otvoriti Serial Monitor za praćenje ispisa



