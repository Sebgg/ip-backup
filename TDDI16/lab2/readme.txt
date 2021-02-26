/**********************************************************************
 *  Knäcka lösenord readme.txt
 **********************************************************************/

 Ungefärligt antal timmar spenderade på labben (valfritt): 6

/**********************************************************************
 *  Ge en högnivåbeskrivning av ditt program decrypt.c.
 **********************************************************************/

Vi börjar med att skapa en unordered_map där nyckeln är en Key (hashad version)
och värdet är en vector med Keys (ohashade). Sedan loopar vi igenom de låga
bitarna i lösenordet, fram tills att hälften av jämförelsenyckeln består av
femmor, t.ex. 'aaa555'. En hashfunktion som bland annat shiftar bitarna med 2 steg är implementerad för att placera nycklarna och värdena i hashtabellen.

Sedan går vi över till att bygga den övre halvan av lösenordet (vänstra delen) och jämför den hashade versionen av denna halva med de lösenord vi lagrat tidigare i vår unordered_map. Vi gör detta genom att subtrahera bitarna från det hashade lösenordet med vårt temporära lösenord, för att se om vi får en match i hashtabellen. Får vi en match skriver vi ut den.

/**********************************************************************
 *  Beskriv symboltabellen du använt för decrypt.c.
 **********************************************************************/
Vi har använt unordered_map. Då vi använder en egen klass "Key" som nyckel så krävs det en implementation av en hashfunktion, för att symboltabellen ska kunna hantera insättningen av värden.

/**********************************************************************
 *  Ge de dekrypterade versionerna av alla lösenord med 8 och 10
 *  bokstäver i uppgiften du lyckades knäca med DIN kod.
 **********************************************************************/


8 bokstäver         10 bokstäver
-----------         ------------
-congrats           -completely
-ibqfehan           -nbryjimvrd
-mqubzd45           -unbreakabl
-youfound           -cryptogram
-theright           -ormaybenot
-aeiqrsd4
-solution


/****************************************************************************
 *  Hur lång tid använder brute.c för att knäcka lösenord av en viss storlek?
 *  Ge en uppskattning markerad med en asterisk om det tar längre tid än vad
 *  du orkar vänta. Ge en kort motivering för dina uppskattningar.
 ***************************************************************************/


Char     Brute
--------------
 4          0s
 5          4s
 6        175s
 8      15400s*

Motivering: den tar orimligt lång tid på sig.

/******************************************************************************
 *  Hur lång tid använder decrypt.c för att knäcka lösenord av en viss storlek?
 *  Hur mycket minne använder programmet?
 *  Ge en uppskattning markerad med en asterisk om det tar längre tid än vad
 *  du orkar vänta. Ge en kort motivering för dina uppskattningar.
 ******************************************************************************/

Char    Tid (sekunder)    Minne (bytes)
----------------------------------------
6         0s
8         1s
10       50s
12     2500s*

/*************************************************************************
 * Hur många operationer använder brute.c för ett N-bitars lösenord?
 * Hur många operationer använder din decrypt.c för ett N-bitars lösenord?
 * Använd ordo-notation.
 *************************************************************************/
brute: O(2^N) där N=antal bitar i lösenordet
decrypt: O(2^(N/2)) där N=antal bitar i lösenordet

/*************************************************************************
 * Vilka egenskaper skulle ni leta efter i en hashfunktion för att
 * försäkra er om att den är "säker"?
 *************************************************************************/
Kunna skapa unika hash-värde för unika värden, för att minska antalet kollisioner.
Bitshifting verkar vara ett "säkert" sätt att uppnå detta.
Den ska även kunna replikera samma hash-värde för samma input varje gång.
