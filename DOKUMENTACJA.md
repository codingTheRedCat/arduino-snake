# SNAKE

## Gra

SNAKE to gra, w której wąż porusza się po planszy, zjada jabłka, a przy tym rośnie. Celem gry jest zapełnienie wężem całego ekranu. Przegrywa się wtedy, gdy wąż uderzy w samego siebie lub w odpowiednim trybie gry, gdy uderzy w ścianę.

## Wybór Trybu

Na początku trzeba wybrać tryb (game mode). Są dostępne dwa:

### Przechodzenie przez ściany

Plansza jest nieskończona. Jeżeli wąż napotka ścianę na swojej drodze, to przejdzie przez nią. (Przerywana linia obramywująca ekran)

### Ściany zabijają

Jeżeli wąż napotka ścianę, gra się kończy. (Ciągła obramówka ekranu)

## Mechaniki pozycji i poruszania się

  Wąż składa się z jednej lub wielu członów, z których każdy ma swoją pozycję. Człony tworzą łańcuch. Poruszanie się węża odbywa się w klatkach. Podczas klatki obok czoła węża pojawia się nowy człon, który staje się czołem. Jeżeli wąż nie zmienia swojej długości, tylni człon zostaje usunięty. 
  Wąż nie przechodzi przez samego siebie. Jeżeli spróbujemy przejść przez samego siebie gra się zakończy. W trybie "Ściany zabijają" nie można przejść przez ścianę. Przechodzenie przez ścianę w tym trybie powoduje przegraną.

Prędkość węża określa kierunek i szybkość poruszania się. Wartość prędkości jest zawsze równa 1 (w jednej klatce wąż porusza się o jedno pole). Zmienia się kierunek i zwrot prędkości poprzez posunięcie drążka joysticka. Aby wąż poruszał się szybciej można też włączyć tryb SPRINT dociskając drążek do podstawki. Tryb SPRINT nie zmienia jednak prędkości tylko częstolitliwość klatek zmiany pozycji.

## Mechaniki zmiany długości

  Wąż może zmieniać swoją długość poprzez jedzenie jabłek. Jabłka są to migające pola na planszy. Gdy człon czołowy węża napotka jabłko, to tylni człon nie jest usuwany i pozostaje na swoim miejscu, co powoduje zwiększenie długości węża o +1.
  
## Zakończnenie gry

  Gra się kończy przegraną lub wygraną. Wygrana jest wtedy, gdy wąż ma taką długość, że zapełnia całą planszę. Wyświetla się wtedy napis "GG" (ang. Good Game). Przegrana następuje wtedy, gdy człon czołowy napodka inny człon, który ma inną prędkość (gdy prędkość jest taka sama, to wąż zapętla się). Drugi przypadek przegrany jest wtedy, gdy wąż uderzy w ścianę w trybie "Ściana zabija".
