# TDP028

## Beskrivning

Paetrol är en community-driven app för att hitta, och rapportera in
rådande bensinpriser i närheten. Priser för den valda bensinstationen
visas tillsammans med när de uppdaterades senast, och prisläge i
förhållande till totalt medelvärde.

Baserat på din position kommer Paetrol hitta bensinstationer i närheten
och lista upp dom tydligt.

Det går själv att rapportera in priser, för att uppdatera appen.
Det går även att logga in för att visa vem det är som rapporterat
in priset.

I detaljvyn för en bensinstation går det att se mer information om 
både bensinstation och priser. Det går att därifrån få ruttvägledning via
Google Maps.

Paetrol har följande funktioner:
*  Sorterbar lista över bensinstationer i närheten.
*  Ruttvägledning till bensinstation med Google Maps.
*  Indikator som visar pris i jämförelse med snittpriser.
*  Tidsstämpel på användarrapporterade priser, för att se aktualitet.
*  Användarinloggning för att visa vem som rapporterat, samt favoritmarkera bensinstationer.
*  Få en notis ifall användaren är i närheten av en bensinstation, med pris och prisläge.
*  Visa en karta för att få en grafisk översikt över bensinstationer i närheten.
*  Stilren och modern UI för lätt navigering i appen.

## Screen-casts
>[App-screencast](https://youtu.be/hP1QTB9XnaQ)

>[Kod-screencast](https://youtu.be/XO8nion-0Cg)

## Betygsdel

##### Uppnådda tekniska krav:
*  Modulär kod med fragments och activities uppdelade.
*  Väl avvägd användning av fragments, då activities används för att innehålla relevanta fragment.
*  Hantering av användarinput på rätt nivå, fragments tar hand om all användar input, men kan skicka vidare till activity.
*  Kan hantera rotation, de fragment som behöver ändra layout vid rotation gör det på ett snyggt sätt.
*  Hanterar bakåtknappen, så att man inte kan komma från *main activity* till *auth activity* eller tvärtom.
*  Egenskriven adapter för *Station list* som hanterar info rätt.

***Totala tekniska poäng: 6***

##### Uppnådda API krav:
*  Använder firebase som databas, för att hämta data om användaren.
*  Multispråkstöd, då appen fungerar på både svenska och engelska, med fler språk lätt implementerade.
*  Använder Google maps på ett meningsfullt sätt i appen, genom att visa användare och närliggande POI's.
*  Enkel inloggning mot Firebase, både email/password och anonymt.
*  Koll på inloggningsstatus, ingen data sparas för anonyma användare.
*  Tredjepartsinloggning mot Google.
*  Performance monitoring med Firebase.

***Totala api poäng: 7***

### Betygsnivå: 4