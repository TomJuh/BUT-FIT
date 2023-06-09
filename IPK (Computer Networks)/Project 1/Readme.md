# IPK Projekt 1 : HTTP Server
*  ##### autor :  xjuhas04


## Vlastnosti
- Výpis záťaže cpu na serveru 
- Výpis hostname serveru 
- Výpis názvu cpu 

## Funkcie
Krátky popis jednotlivých funkcií. 
### Hlavne funkcie :

- **main**
- **construct_socket** - Priradí potrebne parametre k socketu a spusti funkcie bind a listen 
- **socket_comms** - Jedna sa o hlavný loop celej komunikácie, ktorý príma požiadavky klienta a ďalej spúšťa funkciu select_func. Po získaní jej návratovej hodnoty klientovi posiela respond message. 

- **load** - Vytvorí dve štruktúry pre sledovanie posunu (previous a current), prečíta dáta zo súboru /proc/stat pre obe štruktúry s časovým rozdielom, z ktorých sa následne vypočíta záťaž, zaokrúhli sa a pripojí sa na koniec odpovede klientovi. 
cpu_name: Prečíta dáta zo súboru /proc/cpuinfo, kde nájde riadok na ktorom sa nachádza názov cpu a pridá ho na koniec  odpovedi klientovi. 

- **hostname**  - Prečíta dáta zo súboru /etc/hostname a pridá ho na koniec odpovede klientovi. 

### Pomocné funkcie : 

- **select_func** - Stará sa o spracovanie klientovho požiadavku a následne vyvolá príslušnú funkciu. 

- **read_cpustat** - Prečíta dáta z /proc/stat a priradí ich do štruktúry. 

- **calculate_load** - Vypočíta celkovú záťaž cpu pomocou dát z oboch štruktúr. 

- **construct_message** - Alokuje dáta potrebné pre jednotlivé správy a spája ich časti (http hlavičku so samotným obsahom správy). 

- **free_memory** Kontroluje či je možne pamäť vrátiť. 

And of course Dillinger itself is open source with a [public repository][dill]
 on GitHub.

## Installation

Projekt vyžaduje [gcc](https://gcc.gnu.org/) a [make](https://www.gnu.org/software/make/) pre spustenie.

Následne stačí rozbaliť zip súbor(napr. pomocou programu [zip](https://www.tecmint.com/install-zip-and-unzip-in-linux/)) a skompilovať pomocou make. 

```sh
unzip xjuhas04.zip
make
```


## Použite 

Najprv je potrebné server spustiť :
```sh
./hinfosvc cislo_portu 
```

Následne je možne v druhom termináli alebo v priehladaci získať 3 typy dát :


### Záťaž cpu pomocou : 
```sh
curl http://hostname:cislo_portu/load 
```
alebo 
```sh
GET http://hostname:cislo_portu/load 
```
a taktiež v priehladaci na adrese: http://hostname:cislo_portu/load.  

### Názov cpu pomocou : 
```sh
curl http://hostname:cislo_portu/cpu-name
```
alebo 
```sh
GET http://hostname:cislo_portu/cpu-name 
```
a taktiež v priehladaci na adrese: http://hostname:cislo_portu/cpu-name.  
  

### Hostname serveru pomocou : 
```sh
curl http://hostname:cislo_portu/hostname
```
alebo 
```sh
GET http://hostname:cislo_portu/hostname 
```
a taktiež v priehladaci na adrese: http://hostname:cislo_portu/hostname.

 
