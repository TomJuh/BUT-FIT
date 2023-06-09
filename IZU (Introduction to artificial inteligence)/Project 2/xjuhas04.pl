%autor :xjuhas04
% Zad?n? ?. 25:
% Napi?te program ?e??c? ?kol dan? predik?tem u25(VIN), kde prom?nn? VIN  
% obsahuje p?irozen? ??slo. Predik?t bude pravdiv?, pokud vyp??e toto  
% ??slo v bin?rn? soustav?. 

% Testovac? predik?ty:                         			        
u25_1:- u25(113).					% 1110001
u25_2:- u25(1024).                                      % 10000000000
u25_3:- u25(1).                                         % 1
u25_r:- write('Zadej VIN: '),read(VIN),u25(VIN).


u25_1:- u25(113).					% 1110001
u25_2:- u25(1024).                                      % 10000000000
u25_3:- u25(1).                                         % 1
u25_r:- write('Zadej VIN: '),read(VIN),u25(VIN).

u25(VIN) :- toBinary(VIN, VOUT), write(VOUT).
toBinary(0,'0'). toBinary(1, '1'). %prepis hodnot 
toBinary(A, B) :-
    C is A mod 2, %ziskanie prveho bitu
    D is A//2, %ziskanie polovice (iba cely integer)
    toBinary(D, E), %rekurzivne volanie 
    string_concat(E, C, B), %spojenie casti stringov
 	A>0. %podmienka rekurzie
