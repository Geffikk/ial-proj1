
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    // Vsetky ukazatele v zozname nastavim na NULL
    L->Act= NULL;
    L->First= NULL;
    L->Last	= NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

	tDLElemPtr tDLElem = L->First;
	// Prechod zoznamom
	while(L->First != NULL){
	    // Odstranenie vsetkych prvkov
	    free(tDLElem);
	    L->First = L->First->rptr;
	    tDLElem = L->First;
	}

	// A znovu nastavenie na NULL
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    // Alokacia pamate strukturu prvku
    tDLElemPtr insert_item = (tDLElemPtr)malloc(sizeof(struct tDLElem));
    // Zlyhanie alokacie pamati
    if(!insert_item){
        DLError();
        return;
    }
    // Vlozenie hodnoty do listu
    insert_item->data = val;
    // Lavy ukazatel nastavim na NULL
    insert_item->lptr = NULL;
    // Pravy ukazatel na first
    insert_item->rptr = L->First;

    // Kontrola ci nie je zoznam prazdny
    if(L->First != NULL){
        //Lavy ukaz. prveho prvku na novy prvok
        L->First->lptr = insert_item;
    }
    else{
        // Priradenie prvku poslednemu prvku
        L->Last = insert_item;
    }
    // Priradenie prvku prvemu prvku
	L->First = insert_item;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    // Alokacia pamati pre novy prvok
    tDLElemPtr insert_item = (tDLElemPtr) malloc(sizeof(struct tDLElem));
    // Zlyhanie alokacie pamati
    if(!insert_item){
        DLError();
        return;
    }
    // Nastavenie dat
    insert_item->data = val;
    // Pravy ukazatel NULL
    insert_item->rptr = NULL;
    // Lavt ukazatel je posledny prvok
    insert_item->lptr = L->Last;
    // ak zoznam nie je prazdny
    if (L->Last != NULL) {
        // prirad na pravy ukazatel posledneho prvku novy prvok
        L->Last->rptr = insert_item;
    } else {
        // pridat na prvy prvok novy provk
        L->First = insert_item;
    }
    // na posledny prvok prirad novy prvok
    L->Last = insert_item;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    // nastavenie prvku ze prvy je aktivny
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    // Nastavenie prvku je posledny aktivny
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    // Ak je zoznam prazdny tak error
	if(L->First == NULL){
	    DLError();
	    return;
	}

	// Presun dat z prveho prvku na ukazatel (val)
	*val = L->First->data;

}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    // Ak je zoznam prazdny tak error
    if(L->First == NULL){
        DLError();
        return;
    }

    // Presun dat z prveho prvku na ukazatel (val)
    *val = L->Last->data;

}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    // Alokacia pamate
    tDLElemPtr del_item = (tDLElemPtr)malloc(sizeof(struct tDLElem));
    // Chyba pri alokacii
    if (!del_item){
        DLError();
        return;
    }

    // Ak zoznam nie je prazdny
    if (L->First != NULL){
        // Novemu prvku prirad prvy prvok
        del_item = L->First;
        // Ak je aktualny prvok prvy tak ho vynulujem
        if (L->Act == L->First){
            L->Act = NULL;
        }
        // Ak je prvy prvok zaroven posledny tak vynulujem obidva
        if (L->First == L->Last){
            L->Last = NULL;
            L->First = NULL;
        }
        else{
            // Prvy prvok je druhy prvok
            L->First = L->First->rptr;
            L->First->lptr = NULL;
        }
        // Uvolnim pamat
        free(del_item);
    }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    // Alokacia pamate
    tDLElemPtr del_item = (tDLElemPtr)malloc(sizeof(struct tDLElem));
    // Zlyhanie alokacie
    if (!del_item){
        DLError();
        return;
    }

    // Ak zoznam nie je prazdny
    if (L->Last != NULL){
        // Novy prvok je posledny prvok
        del_item = L->Last;
        // Ak sa aktualny prvok rovna poslednemu prvku tak ho vynuluj
        if (L->Act == L->Last){
            L->Act = NULL;
        }
        // Ak sa prvy prvok rovna poslednemu prvku tak ich obidva vynulujem (Mam iba jeden prvok v zozname)
        if (L->First == L->Last){
            L->Last = NULL;
            L->First = NULL;
        }
        else{
            // Novy posledny prvok je vlavo od minuleho posledneho prvku
            L->Last = L->Last->lptr;
            L->Last->rptr = NULL;
        }
        // Uvolnenie pamate
        free(del_item);
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
 *
**/
    // Alokacia pamate
    tDLElemPtr ActPostDel = (tDLElemPtr)malloc(sizeof(struct tDLElem));
    // Zlyhanie alokacie pamati
    if(!ActPostDel){
        DLError();
        return;
    }
    // V zozname je aktualny prvok
    if (L->Act != NULL) {
        // Pozriem ci mam co rusit
        if (L->Act->rptr != NULL) {
            // Priradim nasledujuci prvok za aktualnou hodnotou do noveho prvku
            ActPostDel = L->Act->rptr;
            // Do prvku za aktualnou hodnotou priradim nasledujuci prvok noveho prvku
            L->Act->rptr = ActPostDel->rptr;
            // Ak sa novy prvok rovna poslednemu prvku tak posledny prvok bude aktualny
            if (ActPostDel == L->Last) {
                L->Last = L->Act;
            } else { // Nie je posledny
                // Lavy ukazatel prvku za novym prvkom bude aktivny
                ActPostDel->rptr->lptr = L->Act;
                // Uvolnenie pamati
                free(ActPostDel);
            }
        }
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    // Alokacia pamate
    tDLElemPtr ActPreDel = (tDLElemPtr)malloc(sizeof(struct tDLElem));
    // Zlyhanie alokacie pamati
    if(!ActPreDel){
        DLError();
        return;
    }

    // V zozname je aktualny prvok
    if(L->Act != NULL){
        // Pozriem ci mam co rusit
        if(L->Act->lptr != NULL){
            // Priradim predchadzajuci prvok za aktualnou hodnotou do noveho prvku
            ActPreDel = L->Act->lptr;
            // Do prvku pred aktualnou hodnotou priradim predchadzajuci prvok noveho prvku
            L->Act->lptr = ActPreDel->lptr;
            // Ak sa novy prvok rovna prvemu prvku
            if(ActPreDel == L->First){
                // Tak prvy prvok je aktualny
                L->First = L->Act;
            }
            else{
                // Pravy ukazatel prvku za novym prvkom bude aktivny
                ActPreDel->lptr->rptr = L->Act;
                // Uvolnenie pamati
                free(ActPreDel);
            }
        }
    }

}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    // Ak mam aktualny prvok
    if (L->Act != NULL){
        // Alokujem pamat pre novy prvok
        tDLElemPtr ActPostIns = (tDLElemPtr)malloc(sizeof(struct tDLElem));
        if(!ActPostIns){
            DLError();
            return;
        }
        // Nastavenie dat
        ActPostIns->data = val;
        // Pravy ukazatel bude nasledujuci prvok za aktivny prvok
        ActPostIns->rptr = L->Act->rptr;
        // Lavy ukazatel bude aktivny prvok
        ActPostIns->lptr = L->Act;
        // Do praveho ukazatela za aktivnym prvkom priradim novy prvok
        L->Act->rptr = ActPostIns;
        // Ak sa aktualny prvok a posledny prvok rovnaju
        if (L->Act == L->Last){
            // Tak posledny prvok bude novy prvok
            L->Last = ActPostIns;
        }
        // Ak nemam aktualny prvok
        else{
            // Lavy ukazatel prvku napravo od aktivneho prvku bude novy prvok
            ActPostIns->rptr->lptr = ActPostIns;
        }
    }

}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    // Ak mam aktualny prvok
	if(L->Act != NULL){
	    // Alokujem pamat pre novy prvok
        tDLElemPtr ActPostIns = (tDLElemPtr)malloc(sizeof(struct tDLElem));
        // Zlyhanie alokacie pamati
        if(!ActPostIns){
            DLError();
            return;
        }
        // Nastavenie dat
        ActPostIns->data = val;
        // Do laveho ukazatela noveho prvku priradim lavy ukazatel aktivneho prvku
        ActPostIns->lptr = L->Act->lptr;
        // Pravy ukazatel noveho prvku bude aktualny prvok
        ActPostIns->rptr = L->Act;
        // Do laveho ukazatela aktivneho prvku priradim novy prvok
        L->Act->lptr = ActPostIns;
        // Ak sa aktivny prvok rovna prvemu prvku
        if (L->First == L->Act){
            // Tak novy prvok bude prvy prvok
            L->First = ActPostIns;
        }
        // Ak nemam aktualny prvok
        else{
            // Pravy ukazatel prvku nalavo od aktivneho prvku bude novy prvok
            ActPostIns->lptr->rptr = ActPostIns;
        }
	}

}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    // Ak nemam aktivny prvok tak error
	if(L->Act == NULL){
	    DLError();
	    return;
	}

	// Kopia dat z aktivneho prvku na val
	*val = L->Act->data;

}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	// Zoznam musi mat aktivny prvok
    if(L->Act != NULL){
        // Prepisanie dat do aktivneho prvku
        L->Act->data = val;
    }
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
    // Posunie aktivitu za aktualnym prvkom
    if(L->Act){
        L->Act = L->Act->rptr;
    }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	// Posunie aktivitu pred aktivny prvok
	if(L->Act){
	    L->Act = L->Act->lptr;
	}

}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    // Ak mam nejaky aktivny prvok tak mi vrat jeho hodnotu
    return L->Act != NULL;

}

/* Konec c206.c*/
