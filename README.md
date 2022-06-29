Projekat iz Autoelektronike
Tema: Kontrola sigurnosnih pojaseva u automobilu.
Zadatak projekta:

Napisati softver za simulaciju pracenja sigurnosnih pojaseva u automobilu. Napraviti minimum 4
taska, jedan task za prijem podataka senzora koji detektuje da li je pojas prikačen, drugi task za
slanje i primanje podataka od PC-ja (moze i dva, jedan za slanje a jedan za prijem), treci za
obradu podataka, i cetvrti za prikaz na displeju. Dodati jos taskova po potrebi. Sihronizaciju
izmedju tajmera i taskova, kao i izmedju taskova ako je potrebno, realizovati pomocu semafora
(ili task notifikacija) ili mutexa, zavisno od potrebe. Podatke izmedju taskova slati preko redova
( queue ). Taskovi koji sluze za primanje i slanje podataka napraviti to jednostavnijim, samo da
pokupe podatak i proslede ga taskovima koji obrađuju podatke.
Trenutno stanje senzora simulirati pomoću UniCom, simulatora serijske komunikacije. Računati
da se informacije o trenutnom stanju senzora za vozača i suvozača dobijaju preko UniCom
softvera svakih 100 ms na kanalu 0. Komunikaciju sa PC-ijem ostvariti isto preko simulatora
serijske veze, ali na kanalu 1. Za simulaciju displeja koristiti Seg7Mux, a za simulaciju logičih
ulaza i izlaza koristiti LED_bar.
1. Pratiti stanje senzora. Posmatrati vrednosti koje se dobijaju iz UniCom softvera. Za
vozačevo mesto postoji senzor koji daje logičku jedinicu ako je pojas vezan, ili logičku nulu ako
pojas nije vezan. Kod suvozača pored tog senzora za detekciju da li je pojas prikačen postoji i
senzor koji detektuje da li neko sedi na seditu. Taj senzor daje analognu vrednost senzora pritiska
na sedište između 0 i 1023.
2. Realizovati komunikaciju sa simuliranim sistemom. Slati naredbe preko simulirane serijske
komunikacije. Naredbe i poruke koje se salju preko serijske veze treba da sadrze samo ascii
slova i brojeve, i trebaju se zavrsavati sa carriagereturn (CR), tj brojem 13 (decimalno), čime se
detektuje kraj poruke. Naredbe su:
a. Ukljucivanje i iskljucivanje sistema preko serijske komunikacije:
- START sistem kontinualno radi
- STOP zaustaviti rad sistema
- PRAG koji govori da li se određeni pritisak na suvozačevom sedištu smatra kao da na njemu
sedi osoba ili ne
b. Pored komandi sa serijske, omogućiti uključivanje sistema preko prekidaca, u tu svrhu pratiti
stanje najniže LEDovke na LED baru (podesiti 1 stubac kao ulazni, a drugi kao izlazni) koja će
simulirati prekidač, tj kad je pritisnut prekidač (dioda uključena) potrebno je proveravati stanje
pojaseva.
Kada je sistem uključen potrebno je jednu izlaznu diodu na LED baru koristiti kao
LED_sistem_aktivan znak.
Ako je sistem uključen, a nije vezan pojas, potrebno je da preostale izlazne LED diode blink-aju
svakih 2 sekunde. A ako pojas nije vezan duže od 20 sekundi, sve diode treba da blinkaju svakih
200ms. (Simuliramo pištanje nekog Buzzer-a npr).
3. Na LCD displeju prikazati trenutno stanje senzora pojasa. Ako je potrebno da se veže pojas
prikazati što je bolje moguće (Left/Right ili Both), istu tu informaciju slati i preko Serijskekomunikacije na 100ms, dok se ne veže pojas.


Realizacija i simulacija.
Prvi način za pokretanje sistema.
Sistem pokrećemo slanjem naredbe startCR na kanal1 seriske komunikacije.
Pri pokretanju sistema pali se dioda u donjem desnom uglu i oba putnika su odvezana što se i vidi na
displeju.
Vrednost praga pri pokretanju sistema je 400 a to možemo menjati slanjem naredbe (npr. prag 500CR)na
kanali 1 seriske komunikacije
Na kanalu 0 serijske komunikacije, koriscenjem opcije auto i slanjem trigger signala 'T', na svakih 100ms
primamo informaciju o vrednosti senzora koje dobijamo iz
stringa u obliku npr 1 1 700CR
Komandom (1 1 700CR) sistem prepoznaje da vozač vezan , da je suvozač vezan dok treći broj označava
vrednost analognog senzora.
Ako je vrednos treće cifre veća od praga suvozač je prisutan a ako nije onda je vozač sam u autu.
Ukoliko je bar jedan od putnika odvezan, izlazni LED stubac treba da treperi na svake dve sekunde u
trajanju od 20s.
Ukoliko se nakon 20 sekundi ne vežu oba putnika(ili samo vozač, ukoliko je samo on prisutan),
frekvencija će se povecati, tj treperenje će biti na 200ms.
Sve vreme na display-u i kanalu 1 serijske komunikacije dobijamo poruke:
Both - oba putnika odvezana
Left - odvezan vozac
Right - odvezan suvozac
Ili je prazan display, i bez slanja na serijsku ukoliko su oba zavezana, ili samo vozac(ukoliko nema
suvozaca u kolima)
Sistem zaustavljamo slanjem poruke stopCR ili gasenjem diode u donjem levom uglu.Drugi način za pokretanje sistema:
Takođe sistem možemo pokrenuti pritiskom na dolju levu diodu.
Ako želimo da sistem zaustavi jednostavno isključimo donju levu diodu.
U prilogu se nalaze slike koje pokazuju rad sistema
Slika1 Pokrećemo sistem pomoću komande (startCR)(niko nije vezan)
Slika2 Ovde simuliramo prisustvo vozača koji je vezan i suvozača koji je vezan kao i vrednost analognog
senzora koji je iznad praga(1 1 600CR)
Slika3 Ovde simuliramo prisustvo vozača koji je vezan kao i vrednost analognog senzora koji je ispod
praga(1 1 300CR)(nema suvozača)
Slika4 Ovde simuliramo prisustvo vozača koji je vezan i suvozača koji nije vezan
Slika5 Ovde simuliramo prisustvo vozača koji nije vezan i suvozača koji jeste vezan
Slika6 Slanjem komande pragCR na kanalu 1 seriske komunikacije može se videti da li su putnici vezani i
koliki je prag prisustva putnika u vozilu
*** Napomena (diode su podešene da blinkanja kao što je traženo u zadatku)***
SerialReceive0_Task
Ovaj task ima za zadatak da obradi podatke koji stižu sa kanala 0 serijske komunikacije.
Podatak koji stiže je u vidu poruke npr. 1 1 600CR. To je podatak o koji govori da li su putnici vezani i
trenurno stanje analognog senzora
Karakteri se smeštaju u niz koji se smješta u red, kako bi ostali taskovi taj podatak imali na raspolaganju.
Ovaj task "čeka" semafor koji će da pošalje interrupt svaki put kada neki karakter stigne na kanal 0.
OnLED_ChangeInterrupt
Ovaj task nam sluzi iskljucivo za proveru da li je pritisnut skroz donji taster prvog stubca.
Ukoliko jeste palimo skroz donju diodu drugog stubca kao indikaciju da je sistem upaljen,
postavljano promeljivu start na 1 i saljemo to u queue u koji se skladiste start/stop komande.
Ukoliko nije pritisnut, ili ga je korisnik iskljucio(prebacio u drugo stanje),
gasimo prethodno upaljenu diodu, start postavljamo na 0 i to saljemo u isti queue.start(void* pvParameters)
Ispituje pristigli string da li sadrži kod za aktivaviju ili gašenje sistema.
SerialReceive1_Task
Ovaj task ima za zadatak da obradi podatke koji stižu sa kanala 1 serijske komunikacije.
Naredbe koje stižu su formata startCR, stopCR, pragCR.
Task takođe kao i prethodni čeka odgovarajući semafor da bi se odblokirao i izvršio.
Taj semafor daje interrupt svaki put kada pristigne karakter na kanal 1.
obrada_senzora(void* pvParameters)
U ovom tasku se prvo detektuje o kom prijemu se radi.
Nakon toga obradjuje podatke koji se dobijaju sa SerialReceive0_Taska, SerialReceive1_Taska ili
LED_bar_Taska.
SerialSend_Task(void* pvParameters)
Ovaj task ima za zadatak da šalje trenutnu vrednost stanja pojaseva u autu na kanal 1 serijske
komunikacije svakih 1s.
Task šalje samo pod uslovom da postoji putnik koji nije vezan ,
Uveden je brojač, kako bi se osiguralo da svaki naredni put šalje naredni karakter iz stringa str.
