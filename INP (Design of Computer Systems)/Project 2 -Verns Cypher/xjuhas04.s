; Vernamova sifra na architekture DLX
; Tomáš Juhász xjuhas04

        .data 0x04          ; zacatek data segmentu v pameti
login:  .asciiz "xjuhas04"  ; <-- nahradte vasim loginem
cipher: .space 9 ; sem ukladejte sifrovane znaky (za posledni nezapomente dat 0)

        .align 2            ; dale zarovnavej na ctverice (2^2) bajtu
laddr:  .word login         ; 4B adresa vstupniho textu (pro vypis)
caddr:  .word cipher        ; 4B adresa sifrovaneho retezce (pro vypis)

        .text 0x40          ; adresa zacatku programu v pameti
        .global main        ; 

main:   
	addi r12,r0,0 ;register pre znaky
	addi r20,r0,0 ;register na posun v loope
	addi r6, r0,0 ;register na kontrolu
	
While:
	
	lb r12, login(r20)	;nacita znak na indexe r20

	;ak ma znak hodnotu mensiu nez 97(a) ukonci sifrovanie

	sgei r6,r12,97		;if ((char) r12 >= 97) then r6 = 1; else r6 = 0;
	beqz r6,Finish		;if (r6 == 0) then Finish();
	nop
	addi r12,r12,10		;pricita k znaku hodnotu prveho kluca j == 10

	;Ak ma znak hodnotu vacsiu nez max h. v abecede opravy pretecenie
	
	slei r6,r12,122		;if ((char) r12 <= 122) then r6 = 1; else r6 = 0; 
	beqz r6,CorrectPlus	;if (r6 == 0) then CorrectPlus(); 		  	
	nop
ContinuePlus:			;navrat z CorrectPlus

 	sb cipher(r20),r12	;ulozi upraveny znak na index r20
 	addi r20,r20,1 		;posunie index
	lb r12, login(r20)	;nacita znak na indexe r20
	
	;ak ma znak hodnotu mensiu nez 97(a) ukonci sifrovanie

	sgei r6,r12,97		;if ((char) r12 >= 97) then r6 = 1; else r6 = 0;
	beqz r6,Finish		;if (r6 == 0) then Finish();
	nop
	subi r12,r12,21		;odcita od znaku hodnotu druheho kluca u == 21

	;Ak ma znak hodnotu mensiu nez min h. v abecede opravy pretecenie

	sgei r6,r12,97
	beqz r6,CorrectMinus
	nop
ContinueMinus:			;navrat z CorrectMinus
	sb cipher(r20),r12	;ulozi upraveny znak na index r20
	addi r20,r20,1  	;posunie index
	j While			;navrat na zaciatok while

CorrectPlus:			;pri preteceni cez max odcita 26
	subi r12,r12,26
	j ContinuePlus		;navrat do loopu
	nop
CorrectMinus:			;pri preteceni cez min pricita 26
	addi r12,r12,26
	j ContinueMinus		;navrat do loopu
	nop
Finish:
	sb cipher(r20),r0 	;pridanie '0' na koniec retazca

end:    addi r14, r0, caddr ; <-- pro vypis sifry nahradte laddr adresou caddr
        trap 5  ; vypis textoveho retezce (jeho adresa se ocekava v r14)
        trap 0  ; ukonceni simulace
