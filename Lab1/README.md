# Projekt: Rad s prekidima na ugradbenim platformama

## Sadržaj

- Opis projekta
- Komponente
- Funkcionalnosti
- Kod
- Pokretanje

## Opis projekta

Projekt služi kao jednostavan primjer kako koristiti prekide na ugradbenim platformama (npr. Arduino). Korištene su tipke, PIR senzor pokreta, LED-ice i zvučnik za simulaciju reakcija sustava na različite ulazne događaje.

Glavni cilj je demonstrirati kako se koriste različite vrste prekida i na koji način oni utječu na izvođenje glavne petlje programa.

## Komponente

U projektu su korištene sljedeće komponente:

| Komponenta        | Količina | Pinovi        |
| ----------------- | -------- | ------------- |
| Arduino Mega 2560 | 1        | -             |
| Tipkalo           | 3        | 21, 20, 19    |
| LED diode         | 4        | 11, 12, 13, 8 |
| PIR senzor        | 1        | 2             |
| Zvučnik (buzzer)  | 1        | 22            |

## Funkcionalnosti

- **Tipka 1** (INT0, pin 21): Mijenja stanje LED-ice.
- **Tipka 2** (INT1, pin 20): Pokreće blokirajući ISR koji trepće LED-icom kako bi signalizirao da rutina traje. Koristi se za prikaz kako dugotrajni prekid može blokirati ostatak sustava.
- **Tipka 3** (INT2, pin 19): Postavlja zastavicu koja uzrokuje da LED trepne 5 puta u glavnoj petlji.
- **PIR senzor pokreta** (INT4, pin 2): Aktivira zvučni alarm kad se detektira kretanje.
- **Timer1**: Pokreće prekid svakih 30 ms koji upravlja PWM-om za efekt disanja na LED-ici.
- **Serijski monitor**: Koristi se za ispis trajanja blokirajućeg ISR-a.

###

### Tablica funkcionalnosti

| Oznaka | Funkcija                                                  |
| ------ | --------------------------------------------------------- |
| FR-1   | Detekcija pritiska na tipkala i upravljanje LED-icama     |
| FR-2   | Softverski debounce                                       |
| FR-3   | Blokirajući ISR za tipku 2                                |
| FR-4   | Timer prekid i PWM upravljanje LED-om                     |
| FR-5   | Detekcija pokreta i aktivacija alarma                     |
| FR-6   | Ispis statusa na serijski monitor                         |


## Pokretanje

Kod se može isprobati u Wokwi simulatoru ili na fizičkom Arduino Mega 2560.

Za pokretanje je potrebno:

1. Učitat `Arduino.ino` datoteku u Arduino IDE
2. Spojiti komponente prema pinovima iz tablice (gore).
3. Učitati kod na uređaj i otvoriti Serial Monitor za praćenje ispisa



