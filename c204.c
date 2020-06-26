
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
int solved;

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {
    // Inicializujem premennú na uloženie hodnoty najvyššieho znaku na zasobníku
    char top_char;

    // zásobnik s nie je prázdny tak si uložím najvyššiu hodnotu na zásobniku a popnem ju
    while (!stackEmpty(s)){
        stackTop(s, &top_char);
        stackPop(s);

        // Ak sa najvyššia hodnota na zásobniku rovná '(' tak končím s vypúštaním znakov zo zásobniku
        if(top_char == '('){
            break;
        }

        //Jednotlivé operátory zo zásobnika zapisujem do postfixového tvaru
        postExpr[(*postLen)++] = top_char;
    }

}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {

    // Inicializujem si premennú pre hodnotu na vrchole zásobnika
    char topchar;

    // Ak je zásobnik prázdny tak hodnotu ktorú mam na vstupe vložím do zásobniku a skončím
    if(stackEmpty(s)) {
        stackPush(s, c);
        return;
    }

    // Vráti mi hodnotu na vrchole zásobnika
    stackTop(s, &topchar);

    // Ak sa nachádza na vrchole zásobnika '('
    if(topchar == '('){
        stackPush(s, c); //vloz operator na zasobnik
        return;
    }

    // Ak sa na vrchole zasobnika nachadza operator s nizsou prioritou
    if((topchar == '+' || topchar == '-') && (c == '*' || c == '/')){
            stackPush(s, c); //vlozenie operatoru na zasobnik
            return;
        }

    // Ulozenie znaku z vrcholu zasobniku na koniec vystupneho retazca
    postExpr[(*postLen)++] = topchar;
    // Vymazanie znaku z vrcholu zasobnika
    stackPop(s);
    // rekurzivne volanie funkcie dokym sa nepodari vlozit operator na zasobnik
    doOperation(s, c, postExpr, postLen);
}

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {
    // Alokacia pre vystupni retazec
    char* vystupny_retazec = (char*)malloc(MAX_LEN* sizeof(char));

    // Alokacia pamati pre zasobnik
    tStack* stack = (tStack*)malloc(sizeof(tStack));

    // Zlyhanie alokacie
    if(!vystupny_retazec){
        return NULL;
    }

    // Zlyhanie alokacie
    if(!stack){
        free(vystupny_retazec);
        return NULL;
    }

    // Inicializovanie zasobniku
    stackInit(stack);
    char c = *infExpr;
    // Inicializacia premennej pre dlzku vystupneho retazca
    unsigned int count = 0;

    //Prechadzanie vstupnym retazcom znak po znaku z lava do prava
    while(c != '\0'){
        if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
            vystupny_retazec[count++] = c; // Vlozenie znaku na koniec retazca
        }
        else if(c == '('){
            stackPush(stack, c); // Vlozenie znaku na zasobnik
        }
        else if(c == '+' || c == '-'|| c == '*' || c == '/'){
            doOperation(stack, c, vystupny_retazec, &count); // Spracovanie operatoru (urcenie priority)
        }
        else if(c == ')'){
            untilLeftPar(stack, vystupny_retazec, &count); // Vyprazdnenie zasobniku po lavu zatvorku
        }
        else if(c == '='){
            // Ak sa nachadza este nieco na zasobniku tak to odstran a pridaj na koniec vystupneho retazca
            while(!stackEmpty(stack)){
                stackTop(stack, &(vystupny_retazec[count++]));
                stackPop(stack);
            }
            // Ak je na vstupe = tak ukonci prechadzanie
            vystupny_retazec[count++] = '=';
                break;
        }
        // Pridanie '=' na koniec vystupneho retazca
        c = *(++infExpr);
    }

    // Pridanie znaku konca stringu
    vystupny_retazec[count++] = '\0';
    // Uvolnenie pamati pre zasobnik
    free(stack);
    // Vracia vystupny string
    return vystupny_retazec;
}

/* Konec c204.c */
