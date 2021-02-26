/**********************************************************************
 *  M�nsterigenk�nning readme.txt
 **********************************************************************/

 Ungef�rligt antal timmar spenderade p� labben (valfritt): 3h

/**********************************************************************
 *  Empirisk    Fyll i tabellen nedan med riktiga k�rtider i sekunder
 *  analys      n�r det k�nns vettigt att v�nta p� hela ber�kningen.
 *              Ge uppskattningar av k�rtiden i �vriga fall.
 *
 **********************************************************************/

      N       brute       sortering
 ----------------------------------
    150       10          4         ms
    200       20          9         ms
    300       93          18        ms
    400       167         26        ms
    800       997         57        ms
   1600       7446        209       ms
   3200       59267       783       ms
   6400       8*(m)       3449      ms
  12800       64*(m)      15308     ms


/**********************************************************************
 *  Teoretisk   Ge ordo-uttryck f�r v�rstafallstiden f�r programmen som
 *  analys      en funktion av N. Ge en kort motivering.
 *
 **********************************************************************/

Brute: O(n^3 * log(n))
Motivering: Den har 3 for-loopar som kör för varje punkt, och en som kör ibland.

Sortering: O(n^2)
Motivering: Den yttre for-loopen har tidskomplexitet O(n). Den första inre for-loopen har en tidskomplexitet på O(n), eftersom den kommer gå igenom n-1+1 element
jämfört med den yttre. Den andra inre for-loopen har en genomsnittlig ordo-notation som är mindre än O(n), som borde bara O(n/l) där l är antal element i genomsnitt
per hink i vår bucketsort-skapade lista. Detta ger en total tidskomplexitet på O(n) * max(O(n), O(n/l)) = O(n)*O(n) = O(n^2)