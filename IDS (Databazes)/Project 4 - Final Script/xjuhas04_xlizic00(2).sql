--------------  autors  --------------
-------------  xjuhas04  -------------
-------------  xlizic00  -------------
--------------------------------------

----------   DROP TABLES    ----------
DROP TABLE zamestnanec CASCADE CONSTRAINTS;
DROP TABLE zakaznik CASCADE CONSTRAINTS;
DROP TABLE rezervacia CASCADE CONSTRAINTS;
DROP TABLE pobyt CASCADE CONSTRAINTS;
DROP TABLE izba CASCADE CONSTRAINTS;
DROP TABLE sluzby CASCADE CONSTRAINTS;
DROP TABLE zaplatil CASCADE CONSTRAINTS;
DROP TABLE firma CASCADE CONSTRAINTS;
DROP TABLE fyzicka_osoba CASCADE CONSTRAINTS;

--------------------------------------
--------       TABLES       ----------
CREATE TABLE izba(
    cislo_izby NUMBER PRIMARY KEY,
    pocet_posteli NUMBER NOT NULL,
    popis VARCHAR(1024) NOT NULL,
    cena NUMBER NOT NULL,
    CHECK (cena >= 0)
);

CREATE TABLE pobyt(
	id_pobytu NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    datum_zaciatku DATE NOT NULL,
    datum_koncu DATE NOT NULL,
    datum_prichodu DATE NOT NULL,
    datum_odchodu DATE NOT NULL,
    id_izby NUMBER NOT NULL,
    CONSTRAINT pokoj_pobyt_id_fk
        FOREIGN KEY (id_izby) REFERENCES izba (cislo_izby)
        ON DELETE CASCADE,
    CHECK (datum_zaciatku <= datum_koncu),
    CHECK (datum_prichodu <= datum_odchodu)
);

CREATE TABLE sluzby(
	id_sluzby NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    cena NUMBER NOT NULL,
    popis VARCHAR(1024) NOT NULL,
    id_pobytu NUMBER,
    CONSTRAINT pobyt_sluzby_id_fk
        FOREIGN KEY (id_pobytu) REFERENCES pobyt (id_pobytu)
        ON DELETE CASCADE,
    CHECK (cena >= 0)

);


CREATE TABLE zamestnanec(
	rodne_cislo NUMBER PRIMARY KEY,
	meno VARCHAR(50) NOT NULL,
    priezvisko VARCHAR(50) NOT NULL,
    pozicia VARCHAR(50) NOT NULL,
    id_pobytu NUMBER NOT NULL,
    id_sluzby NUMBER NOT NULL,
    CONSTRAINT pobyt_zamestnanec_id_fk
        FOREIGN KEY (id_pobytu) REFERENCES pobyt (id_pobytu)
        ON DELETE CASCADE,
    CONSTRAINT sluzby_zamestnanec_id_fk
        FOREIGN KEY (id_sluzby) REFERENCES sluzby (id_sluzby)
        ON DELETE CASCADE,
    CHECK (pozicia IN ('Recepcny', 'Manazer', 'Upratovacka', 'Kuchar', 'Vratnik', 'Kontrolor'))
);

CREATE TABLE rezervacia(
	id_rezervacie NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
	datum_rezervacie DATE NOT NULL,
    stav VARCHAR(255) NOT NULL,
    id_pobytu NUMBER NOT NULL,
    CONSTRAINT pobyt_rezervacia_id_fk
        FOREIGN KEY (id_pobytu) REFERENCES pobyt (id_pobytu)
        ON DELETE CASCADE,
    CHECK (stav IN ('Zrusena', 'Potvrdena', 'Nepotvrdena', 'Zaplatena', 'Nezaplatena'))
);

CREATE TABLE zakaznik(
    id_zakaznika NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    telefonne_cislo INTEGER NOT NULL,
    email VARCHAR (255) NOT NULL,
    id_rezervacie NUMBER,
    CONSTRAINT rezervacia_zakaznik_id_fk
        FOREIGN KEY (id_rezervacie) REFERENCES rezervacia (id_rezervacie)
        ON DELETE CASCADE,
    CHECK (telefonne_cislo > 0),
    CHECK (email LIKE '%@%')

);

CREATE TABLE firma(
    id_zakaznika NUMBER GENERATED BY DEFAULT AS IDENTITY NOT NULL,
	nazov VARCHAR(50) NOT NULL,
	dci INTEGER NOT NULL,
    ico INTEGER NOT NULL,
    CONSTRAINT FK_FIRMA FOREIGN KEY (id_zakaznika) REFERENCES zakaznik (id_zakaznika),
    CHECK (dci > 0),
    CHECK (ico > 0)
);

CREATE TABLE fyzicka_osoba(
    id_zakaznika NUMBER GENERATED BY DEFAULT AS IDENTITY NOT NULL,
	cislo_obcianskeho NUMBER,
	meno VARCHAR(50) NOT NULL,
    priezvisko VARCHAR(50) NOT NULL,
    CONSTRAINT FK_FYZ_OS FOREIGN KEY (id_zakaznika) REFERENCES zakaznik (id_zakaznika),
    CHECK (cislo_obcianskeho > 0)
);

CREATE TABLE zaplatil(
    id_platby NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    suma NUMBER NOT NULL,
    datum_platby DATE NOT NULL,
    id_pobytu NUMBER NOT NULL,
    id_zakaznika NUMBER NOT NULL,
    CONSTRAINT pobyt_zaplatil_id_fk
        FOREIGN KEY (id_pobytu) REFERENCES pobyt (id_pobytu)
        ON DELETE CASCADE,
    CONSTRAINT zakaznik_zaplatil_id_fk
        FOREIGN KEY (id_zakaznika) REFERENCES zakaznik (id_zakaznika)
        ON DELETE CASCADE,
    CHECK (suma >= 0)
);

--------------------------------------
-----  2 netriviálne trigery    ------
-- (vytvoření alespoň dvou netriviálních databázových triggerů vč. jejich předvedení)

-- Nasledujúci trigger skontroluje, či hodnoty počtu postelí a ceny izby sú väčšie
-- ako nula pri vytvorení novej izby v tabuľke izba. Ak áno, vytvorí sa nový záznam.
-- Ak nie, vyhodí sa chybové hlásenie.
CREATE OR REPLACE TRIGGER check_izba_values
BEFORE INSERT ON izba
FOR EACH ROW
DECLARE
    invalid_values EXCEPTION;
BEGIN
    IF :new.pocet_posteli <= 0 OR :new.cena <= 0 THEN
        RAISE invalid_values;
    END IF;
EXCEPTION
    WHEN invalid_values THEN
        RAISE_APPLICATION_ERROR(-20001, 'Pocet posteli a cena izby musia byt vacsie ako 0.');
END;
/

-- Trigger skontroluje, či číslo izby už existuje v tabuľke izba a ak áno, vyhodí chybu.
CREATE OR REPLACE TRIGGER check_cislo_izby
BEFORE INSERT OR UPDATE ON izba
FOR EACH ROW
DECLARE
    cnt NUMBER;
BEGIN
    SELECT COUNT(*) INTO cnt FROM izba WHERE cislo_izby = :new.cislo_izby;
    IF cnt > 0 THEN
        RAISE_APPLICATION_ERROR(-20001, 'Cislo izby uz existuje!');
    END IF;
END;
/

--------------------------------------
---------  Insert Values    ----------


------ Pokoj/ cislo_pokoje, cislo posteli, popis, cena
INSERT INTO izba VALUES('102', '2', 'Izba: dvojpostelova, Kupelna: ano, Toaletne potreby: ano, Wi-fi: ano, Satelitn? telev?zia: ano', '100');
INSERT INTO izba VALUES('103', '1', 'Izba: jednopostelova, Kupelna: ano, Toaletne potreby: ano, Wi-fi: nie, Satelitn? telev?zia: ano', '70');

------ Pobyt /id_pobytu, datum zaciatku, datum konca, datum prichodu, datum odchodu, id_pokoje
INSERT INTO pobyt VALUES('1', DATE '2022-10-10', DATE '2022-10-11', DATE '2022-10-11', DATE '2022-11-10', '103');
INSERT INTO pobyt VALUES('2', DATE '2023-11-10', DATE '2023-12-10',DATE'2023-11-11',DATE '2023-12-10','102');
INSERT INTO pobyt VALUES('3', DATE '2023-11-10', DATE '2023-12-10',DATE'2023-11-11',DATE '2023-12-10','102');
INSERT INTO pobyt VALUES('4', DATE '2023-11-10', DATE '2023-12-10',DATE'2023-11-11',DATE '2023-12-10','102');

------ Sluzby/ id_sluzby, cena, popis, id_pobytu
INSERT INTO sluzby VALUES('1', '25', 'vecera na izbu', '1');
INSERT INTO sluzby VALUES('2', '5', 'vycistenie bielizne', '2');
INSERT INTO sluzby VALUES('3', '5', 'Donaska alkoholu', '3');

------ Rezervacia/ id rezervacie, datum rezervacie, stav, id_pobytu
INSERT INTO rezervacia VALUES('1', DATE '2022-10-12', 'Nezaplatena', '1');
INSERT INTO rezervacia VALUES('2', DATE '2022-11-12', 'Zaplatena', '2');

------ Zamestnanec / rodne cislo, meno, priezvisko, pozicia, id_pobytu, id_sluzby
INSERT INTO zamestnanec VALUES('1362479712','Daniel', 'Kokos', 'Manazer', '1', '1');
INSERT INTO zamestnanec VALUES('4424093843','Erik', 'Molnar', 'Vratnik', '2', '2');
INSERT INTO zamestnanec VALUES('8762147612','Laura', 'Spackova', 'Upratovacka', '2' , '3');

------ Zakaznik / id_zakaznika,  telefonne cislo, email, id_rezervacie
INSERT INTO zakaznik VALUES('21','420488333888', 'robkoka@gmail.com', NULL);
INSERT INTO zakaznik VALUES('33','421464333098', 'dominikN@gmail.com', '1');
INSERT INTO zakaznik VALUES( '15','420488123764', 'ErikG@gmail.com', '2');

------ Zakaznik / id_zakaznika, telefonne cislo, email, id_rezervacie
INSERT INTO zakaznik VALUES('05','420488333888', 'Erma@gmail.com', NULL);
INSERT INTO zakaznik VALUES('09','422264333098', 'Create@gmail.com', NULL);
INSERT INTO zakaznik VALUES( '11','423488123764', 'Company@gmail.com', NULL);

------ Firma / id_zakaznika ,nazov, dci, ico, telefonne cislo, email, id_rezervacie
INSERT INTO firma VALUES('05','Erma s.r.o', '27082440', '27082440' );
INSERT INTO firma VALUES('09','Company corp', '27082440', '37042440');
INSERT INTO firma VALUES('11','CreativeNames inc', '27082440', '12082440');

------ Fyzicka osoba / cislo obcianskeho, meno, priezvisko, telefonne cislo, email, id_rezervacie
INSERT INTO fyzicka_osoba VALUES('21','324897129', 'Robert', 'Kalinak');
INSERT INTO fyzicka_osoba VALUES('33','984897129', 'Dominik', 'Nemcov');
INSERT INTO fyzicka_osoba VALUES('15','994897129', 'Erika', 'Gocikova');

------ Zaplatil/ id_platby, suma, datum platby, id_pobytu, rodne cislo zakaznika
INSERT INTO zaplatil VALUES ('1','300', DATE '2022-11-10','1', '05');
INSERT INTO zaplatil VALUES ('2','400', DATE '2023-11-10', '2' , '11');
INSERT INTO zaplatil VALUES ('3','400', DATE '2024-11-10', '3' , '15');


--------------------------------------
-----  2 netriviálne procedury  ------
-- (vytvoření alespoň dvou netriviálních uložených procedur vč. jejich
-- předvedení, ve kterých se musí (dohromady) vyskytovat alespoň jednou
-- kurzor, ošetření výjimek a použití proměnné s datovým typem odkazujícím
-- se na řádek či typ sloupce tabulky (table_name.column_name%TYPE nebo table_name%ROWTYPE)

-- Procedúra spočíta cenu pobytu za všetky nocí, ktoré má daná rezervácia.
-- Napr. v prípade troch nocí, kde jedna noc stojí 50eur, vráti 150eur.
CREATE OR REPLACE PROCEDURE spocitat_cenu_pobytu
    (p_id_pobytu IN pobyt.id_pobytu%TYPE,
    p_cena_pokoje IN izba.cena%TYPE,
    p_pocet_noci IN NUMBER,
    p_celkova_cena OUT NUMBER)
    IS
        l_pobyt pobyt%ROWTYPE;
    BEGIN
        SELECT * INTO l_pobyt FROM pobyt WHERE id_pobytu = p_id_pobytu;
        p_celkova_cena := p_cena_pokoje * p_pocet_noci;
        IF l_pobyt.datum_prichodu <= SYSDATE AND l_pobyt.datum_odchodu >= SYSDATE THEN
            p_celkova_cena := p_celkova_cena + p_cena_pokoje;
        END IF;
    EXCEPTION
        WHEN NO_DATA_FOUND THEN
            RAISE_APPLICATION_ERROR(-20001, 'Pobyt s daným ID neexistuje.');
END;
/


-- Procedúra vypíše zoznam všetkých pobyty so zákazníkmi a zoznam
-- všetkých služieb poskytnutých počas pobytu.
CREATE OR REPLACE PROCEDURE vypis_cenu_pobytu
IS
   CURSOR pobyt_cursor IS
      SELECT id_pobytu, id_izby, cena
      FROM pobyt JOIN izba ON pobyt.id_izby = izba.cislo_izby;

   pobyt_rec pobyt_cursor%ROWTYPE;
   cena_pobytu NUMBER(10,2);
BEGIN
   FOR pobyt_rec IN pobyt_cursor LOOP
        cena_pobytu := pobyt_rec.cena;
        DBMS_OUTPUT.PUT_LINE('Cena pobytu ' || pobyt_rec.id_pobytu ||
                           ' v izbe ' || pobyt_rec.id_izby ||
                           ' je ' || cena_pobytu);
   END LOOP;
END;
/

DECLARE
    l_cena NUMBER;
BEGIN
    spocitat_cenu_pobytu(1, 100, 3, l_cena);
    DBMS_OUTPUT.PUT_LINE('Celková cena pobytu: ' || l_cena);
END;
/
--EXEC vypis_cenu_pobytu; //TODO error



----- Vytvorenie indexu
-- explicitní vytvoření alespoň jednoho indexu tak, aby pomohl optimalizovat
-- zpracování dotazů, přičemž musí být uveden také příslušný dotaz, na který
-- má index vliv, a v dokumentaci popsán způsob využití indexu v tomto dotazy
-- (toto lze zkombinovat s EXPLAIN PLAN, vizte dále)

-- Optimalizovanie dotazov, ktoré vyhladávajú zákazníci na základe 'id_pobytu',
-- použitie nájdeného indexu na vyhľadávanie záznamov v tabulke zákazník, ktoré
-- odpovedajú hladanej hodnote v stĺpci id_pobytu.
CREATE INDEX idx_zakaznik_id_pobytu ON zakaznik(1);



----- EXPLAIN PLAN
-- alespoň jedno použití EXPLAIN PLAN pro výpis plánu provedení databazového dotazu se spojením
-- alespoň dvou tabulek, agregační funkcí a klauzulí GROUP BY, přičemž v dokumentaci musí být
-- srozumitelně popsáno, jak proběhne dle toho výpisu plánu provedení dotazu, vč. objasnění
-- použitých prostředků pro jeho urychlení (např. použití indexu, druhu spojení, atp.), a dále
-- musí být navrnut způsob, jak konkrétně by bylo možné dotaz dále urychlit (např. zavedením
-- nového indexu), navržený způsob proveden (např. vytvořen index), zopakován EXPLAIN PLAN
-- a jeho výsledek porovnán s výsledkem před provedením navrženého způsobu urychlení,

-- Vypočítanie súčtu cien pre každú izbu
EXPLAIN PLAN FOR
SELECT pobyt.id_pobytu, SUM(izba.cena) as cena_celkem
FROM izba JOIN pobyt ON pobyt.id_pobytu = pobyt.id_izby
GROUP BY pobyt.id_pobytu;

-- Vytvorenie indexu na stĺpci id_izby v tabulke pobyt pre zníženie záznamov, ktoré
-- sa budú prehliadať
CREATE INDEX pobyt_id_izby_idx ON pobyt(id_izby);

-- Opätove EXPLAIN PLAN pre porovnanie
EXPLAIN PLAN FOR
SELECT pobyt.id_pobytu, SUM(izba.cena) as cena_celkem
FROM izba JOIN pobyt ON pobyt.id_pobytu = pobyt.id_izby
GROUP BY pobyt.id_pobytu;





----- Prístupové práva
-- (definici přístupových práv k databázovým objektům pro druhého člena týmu)
GRANT ALL ON firma TO xjuhas04;
GRANT ALL ON fyzicka_osoba TO xjuhas04;
GRANT ALL ON izba TO xjuhas04;
GRANT ALL ON pobyt TO xjuhas04;
GRANT ALL ON rezervacia TO xjuhas04;
GRANT ALL ON sluzby TO xjuhas04;
GRANT ALL ON zakaznik TO xjuhas04;
GRANT ALL ON zamestnanec TO xjuhas04;
GRANT ALL ON zaplatil TO xjuhas04;

GRANT ALL ON firma TO xlizic00;
GRANT ALL ON fyzicka_osoba TO xlizic00;
GRANT ALL ON izba TO xlizic00;
GRANT ALL ON pobyt TO xlizic00;
GRANT ALL ON rezervacia TO xlizic00;
GRANT ALL ON sluzby TO xlizic00;
GRANT ALL ON zakaznik TO xlizic00;
GRANT ALL ON zamestnanec TO xlizic00;
GRANT ALL ON zaplatil TO xlizic00;
---- Materializovaný pohľad
DROP MATERIALIZED VIEW zakaznici_pobyty;
-- (vytvoření alespoň jednoho materializovaného pohledu patřící druhému
-- členu týmu a používající tabulky definované prvním členem týmu
-- (nutno mít již definována přístupová práva), vč. SQL příkazů/dotazů
-- ukazujících, jak materializovaný pohled funguje)

-- Vytvorenie pohľadu, ktorý bude obsahovať informácie o všetkých
-- zákazníkoch a ich pobytoch
CREATE MATERIALIZED VIEW LOG ON XJUHAS04.ZAKAZNIK WITH PRIMARY KEY, ROWID;
CREATE MATERIALIZED VIEW LOG ON XJUHAS04.REZERVACIA WITH PRIMARY KEY, ROWID;
CREATE MATERIALIZED VIEW zakaznici_pobyty
    REFRESH FAST ON COMMIT
AS
    SELECT z.id_zakaznika , z.telefonne_cislo, z.email, r.id_rezervacie
    FROM zakaznik z
    JOIN rezervacia r ON z.id_rezervacie = r.id_rezervacie
/


-- použitie mateialized view
SELECT * FROM zakaznici_pobyty;

-- pre aktualizáciu materiálneho pohľadu je potreba použiť:
INSERT INTO izba VALUES('104', '2', 'Izba: dvojpostelova, Kupelna: ano, Toaletne potreby: ano, Wi-fi: ano, Satelitn? telev?zia: ano', '100');
INSERT INTO pobyt VALUES('19', DATE '2023-10-10', DATE '2023-10-11', DATE '2023-10-11', DATE '2023-11-10', '104');
INSERT INTO rezervacia VALUES('3', DATE '2023-11-12', 'Zaplatena', '19');
INSERT INTO zakaznik VALUES('59','420588333888', 'new@gmail.com', '3');

COMMIT;
SELECT * FROM zakaznici_pobyty;
----- Komplexni dotaz SELECT
-- (vytvoření jednoho komplexního dotazu SELECT využívajícího klauzuli
-- WITH a operátor CASE. V poznámce musí být uvedeno, jaká data dotaz
-- získává)
-- Vráti stav pobytu podľa jeho ID: buď rezervovaný, prebiehajúci alebo ukončený
WITH typ_pobytu AS (
  SELECT
    p.id_pobytu,
    CASE
      WHEN p.datum_zaciatku > SYSDATE THEN 'Rezervovaný'
      WHEN p.datum_zaciatku <= SYSDATE AND p.datum_koncu >= SYSDATE THEN 'Prebiehajúci'
      WHEN p.datum_koncu < SYSDATE THEN 'Ukončený'
      ELSE NULL
    END AS typ
  FROM pobyt p
  LEFT JOIN rezervacia r ON r.id_pobytu = p.id_pobytu
)
SELECT typ
FROM typ_pobytu
WHERE id_pobytu = 2;