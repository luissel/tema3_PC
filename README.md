# tema3_PC
Chirita Maria-Luissa 322CA
Tema 3 Protocoale de Comunicatie

Pentru implementarea temei am pornit de la rezolvarea laboratorului de HTTP.
Am pastrat fisierele helpers.h si helpers.c. Functiile de formare a mesajului
trimis la server se afla in fisierul requests.c.

Pentru metoda GET am format mesajul astfel:\
    - pe prima linie numelei metodei, urmat de url, de parametrii acestuia\
     (daca are) si de tipul protocolului\
    - pe a doua linie ip-ul hostului\
    - pe urmatoarele linii am adaugat token si cookie urile daca existau

Pentru metoda POST am format mesajul astfel:\
    - pe prima linie numelei metodei, urmat de url si de tipul protocolului
    - pe a doua linie ip-ul hostului\
    - pe urmatoarele 2 linii Content-Type pe care l-am extras din json si
      Content-Length\
    - pe urmatoarele linii am adaugat token si cookie urile daca existau
    - la final am adaugat datele

In fisierul client.c am implementat 3 functii ajutatoare:\
    - get_hostip: primeste ca parametru un string reprezentand hostname-ul
                si un string in care voi retine ip-ul corespunzator (am luat
                functia din laboratorul de DNS)\
    - get_cookies: primeste ca parametru un string si intoarce un string format
                in urma concatenarii cookie urilor gasite. Folosind functia
                strtok cu delimiatorii "\n" si ";", am cautat substringul care
                incepe cu "Set-Cookie". Am luat informatia incepand de l "Cookie"
                si am adaugat-o intr-un string, urmata de newline.\
    - parse_json: functie care primeste continutul jsonului ca string si toate
                campurile posibil existente in acesta. Cu ajutorul functiile din
                biblioteca parson, am extras toate informatiile necesare.

Pentru primul task am facut o cere de tip GET pe url-ul din enunt. In urma
acesteia am memorat raspunsul de la server in response. Cu ajutorul functiei
get_cookies am retinut cookie-urile primite. Am cautat pozitia primei "{" care
marcheaza inceputul jsonului. Cu ajutorul functiei parse_json am memorat
informatiile necesare.

Am facut o cere de tip POST pentru taskul 2 careia i-am dat ca parametru
stringul cu care sa se logheze format din username-ul si parola preluate din
json.

Pentru taskul 3 am parsat raspunsul primit de la taskul 2 ca pana acum si am
facut o cerere de tip GET in care am trimis ca parametrii pentru url cele 2
raspunsuri la intrebari si id-ul extras din json conform formatului precizat si
token-ul extras din json.

La taskul 4 am procedat ca la celelalte taskuri. Am facut o cerere de tip GET
simpla si nu am uitat de cookies, ca pana acum.

Pentru ultimul task am deschis mai intai o conexiune pe ip-ul obtinut cu functia
get_hostip dupa hostname-ul extras din url-ul precizat in json. Am facut o cere
GET pe noul ip cu parametrii precizati (q si appid) pentru a lua datele despre
vreme de la server. Nu am uitat sa ma autorizez cu token-ul si nici sa trimit
cookie-urile. Din raspunsul primit am extras continutul jsonului cu datele
necesare si l-am trimis nemodificat cu o cere POST catre serverul initial.

Astfel am reusit sa obtin comoara, nemancand prajituricile pe drum.
