# Projekt: Ušteda energije u ugradbenim platformama

## Sadržaj

- Opis projekta
- Komponente
- Dijagram spoja
- Funkcionalnosti
- Pokretanje

## Opis projekta

Projekt služi kao jednostavan primjer kako prilagoditi ugradbene platforme (npr. Arduino) da koriste što manje energije.
Cilj projekta je demonstrirati kako koristiti stanja uštede energije koja dolaze sa pojedinom platformom.

Korišten je LowPower.h library

## Komponente

U projektu su korištene sljedeće komponente:

| Komponenta        | Količina |
| ----------------- | -------- |
| Arduino Nano      | 1        |
| LED diode         | 1        |
| Otpornici (1 kOhm)| 1        |
| PIR senzor        | 1        |

## Dijagram spoja

![Prikaz spoja](https://github.com/user-attachments/assets/f626978f-eb6f-4332-9c08-521ead62555f)


## Funkcionalnosti

- **Reagiranje na pokret** - šalje poruku da je pokret zamijećen.
- **Ulazak u sleep mode** - nakon slanja poruke ulazi u sleep mode

## Pokretanje

Kod se može isprobati u Wokwi simulatoru ili na fizičkom Arduino Mega 2560.

Za pokretanje je potrebno:

1. Učitati `Arduino.ino` datoteku u Arduino IDE
2. Spojiti komponente prema slici (gore).
3. Učitati kod na uređaj i otvoriti Serial Monitor za praćenje ispisa

### Link na Wokwi projekt
https://wokwi.com/projects/427698273234965505
