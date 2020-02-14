#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "deterministic.h"
#include "non_deterministic.h"
#include <string.h>
#include <ctype.h>
int displayMenu()
{
    printf("\n\n\n1 => Saisir votre propre automate (vous pourrez tester des mots sur cet automate)\n");
    printf("2 => Utiliser un exemple existant (executer des automates existants)\n");
    printf("3 => Creer et afficher un automate reconnaissant le langage vide\n");
    printf("4 => Creer et afficher un automate reconnaissant le langage constitué du seul mot vide\n");
    printf("5 => Creer et afficher un automate reconnaissant un caractère passé en paramètre\n");
    printf("6 => Quitter le programme\n");
    int choix = 0;

    do
    {
        printf("Entrez votre choix compris entre 1 - 5 :");
        scanf(" %d", &choix);
    } while (choix < 1 || choix > 6);
    return choix;
}
void computeChoice(int choix)
{
    AFND *afnd = NULL;
    char *c;
    switch (choix)
    {
    case 1:
        afnd = constructUserDefinedAFND();
        if (afnd != NULL)
            tasksOnUserDefinedAFND(afnd);
        break;
    case 2:
        exucuteBuildInSamples();
        break;

    case 3:
        Display(automate_vide());
        break;
    case 4:

        Display(automate_Seul_Mot_vide());
        break;
    case 5:
        
        printf("Entrez le caractère à reconaitre\n");
        scanf(" %1s", &c);
        Display(automate_Seul_Mot_vide(c[0]));
        break;

    default:
        printf("Merci et aurevoir \n");
        printf("Exiting... \n");
        exit(0);
        break;
    }
}
AFND *constructUserDefinedAFND()
{
    printf("\n\nCONSTRUCTION D'UN AUTOMATE : Veuillez entrer les transitions SVP:\n\n");
    printf("SYNTAXE: [chiffre]-[caractere]-[chiffre]\n");
    printf("Exemple 1 :  5-a-1  les etats 5 et 1 seront creés ainsi qu'une transition de 5 vers 1 par 'a'\n");
    printf("Exemple 2 :  15-r-15  l'état 15 sera creé ainsi qu'une transition vers lui meme par 'r' \n\n");

    char *input = (char *)malloc(sizeof(char) * 20);
    AFND *afnd = (AFND *)malloc(sizeof(AFND));
    afnd->nbre_etats = 0;
    afnd->Q = (etat *)malloc(sizeof(etat) * 0);
    afnd->Sigma = (char *)malloc(sizeof(char) * 0);
    afnd->taille_alphabet = 0;
    char *type = (char *)malloc(sizeof(char) * 1);
    int initialSetted = -1;
    while (1)
    {
        printf("Saisissez la prochaine transition (ou tapez q pour quitter) :");
        //fgets(input, 10, stdin);
        //scanf(" %s", input);
        fgets(input, 20, stdin);
        input[strcspn(input, "\n")] = 0;
        const char s[2] = "-";
        char caract;
        int edges[3];
        char *token;

        int validInput = 1;
        // fgets is \n terminated
        if (strlen(input) == 1 && input[0] == 'q')
            break;
        if (strlen(input) < 4 || strlen(input) > 20)
        {
            printf("Longuer de chaine non acceptée %d\n", strlen(input));
            continue;
            validInput = 0;
        }
        else
        {
            int countToks = 0;
            /* get the first token */
            token = strtok(input, s);

            /* walk through other tokens */
            while (token != NULL)
            {
                countToks++;
                printf(" %d %s\n", countToks, token);
                switch (countToks)
                {
                case 1:
                    edges[0] = atoi(token);
                    break;
                case 2:
                    caract = token[0];
                    break;

                case 3:
                    edges[1] = atoi(token);
                    break;
                default:
                    validInput = 0;
                    break;
                }

                token = strtok(NULL, s);
            }
            if (countToks != 3)
                validInput = 0; // 15-24-0-r -6 not allowed
            if (edges[0] < 0 || edges[1] < 0)
                validInput = 0; //No state should be nagative
        }
        if (validInput)
        {

            int existeS = stateAlreadyExists(*afnd, edges[0]);

            int existeSNew = existeS;

            etat *nouveauS = NULL;
            etat *nouveauD = NULL;
            if (existeS == -1)
            {
                nouveauS = (etat *)malloc(sizeof(etat));
                nouveauS->num = edges[0];
                nouveauS->transitions = (transition *)malloc(sizeof(transition) * 0);
                nouveauS->nbre_transitions = 0;
                nouveauS->type = NON_ACCEPTEUR;
                afnd->Q = (etat *)realloc(afnd->Q, sizeof(etat) * (afnd->nbre_etats + 1));
                afnd->Q[afnd->nbre_etats] = *nouveauS;
                existeSNew = afnd->nbre_etats;

                printf("S num is %d\n", nouveauS->num);
                printf("Quel est le type de %d (taper <<N>> ou <<n>> pour non accepteur ou autre chose pour accepteur)\n", edges[0]);
                scanf(" %1s", type);
                if (strlen(type) == 1 && toupper(type[0]) != 'N')
                {
                    nouveauS->type = ACCEPTEUR;
                    printf("aaaaacpeur1 %d\n", nouveauS->num);
                }
                if (initialSetted == -1)
                {
                    printf("-------------------------------------------------\n");
                    char isInit[20];
                    printf("Létat %d est t-il votre etat initial ? <<N>> ou <<n>> pour non et autre chose pour oui\n", nouveauS->num);
                    scanf(" %s", &isInit);
                    if (strlen(isInit) > 1 || (isInit[0] != 'N' && isInit[0] != 'n'))
                    {
                        initialSetted = afnd->nbre_etats;
                    }
                }
                afnd->nbre_etats++;
            }
            else
            {
                nouveauS = &(afnd->Q[existeS]);
                existeSNew = existeS;
            }
            int existeD = stateAlreadyExists(*afnd, edges[1]);
            int existeDNew = existeD;
            if (existeD == -1)
            {
                nouveauD = (etat *)malloc(sizeof(etat));
                nouveauD->num = edges[1];
                printf("D num is %d\n", nouveauD->num);
                nouveauD->transitions = (transition *)malloc(sizeof(transition) * 0);
                nouveauD->nbre_transitions = 0;
                nouveauD->type = NON_ACCEPTEUR;

                printf("Quel est le type de %d (taper <<N>> ou <<n>> pour non accepteur ou autre chose pour accepteur)\n", edges[1]);

                scanf(" %1s", type);
                if (strlen(type) == 1 && toupper(type[0]) != 'N')
                {
                    nouveauD->type = ACCEPTEUR;
                    printf("aaaaacpeur %d %c\n", nouveauD->num, type[0]);
                }
                afnd->Q = (etat *)realloc(afnd->Q, sizeof(etat) * (afnd->nbre_etats + 1));
                afnd->Q[afnd->nbre_etats] = *nouveauD;
                existeDNew = afnd->nbre_etats;
                if (initialSetted == -1)
                {
                    char isInit[20];
                    printf("Létat %d est t-il votre etat initial ? <<N>> ou <<n>> pour non et autre chose pour oui\n", nouveauD->num);
                    scanf(" %s", &isInit);
                    if (strlen(isInit) > 1 || (isInit[0] != 'N' && isInit[0] != 'n'))
                    {
                        initialSetted = afnd->nbre_etats;
                    }
                    printf("--------------------------------------2-----------\n");
                }
                afnd->nbre_etats++;
            }
            else
            {
                nouveauD = &(afnd->Q[existeD]);
                existeDNew = existeD;
            }

            transition trans;
            trans.c = caract;
            trans.suiv = &(afnd->Q[existeDNew]);
            printf("CHPT1 %d %d\n", existeSNew, afnd->Q[existeSNew].nbre_transitions);
            afnd->Q[existeSNew].transitions = (transition *)realloc(afnd->Q[existeSNew].transitions, sizeof(transition) * (afnd->Q[existeSNew].nbre_transitions + 1));
            afnd->Q[existeSNew].transitions[afnd->Q[existeSNew].nbre_transitions] = trans;

            printf("Transiton1 %d %c %d\n", afnd->Q[existeSNew].num, afnd->Q[existeSNew].transitions[afnd->Q[existeSNew].nbre_transitions].c, afnd->Q[existeSNew].transitions[afnd->Q[existeSNew].nbre_transitions].suiv->num);
            afnd->Q[existeSNew].nbre_transitions++;

            printf("CHPT2\n");
            if (characterAlreadyExists(*afnd, caract) == -1)
            {
                printf("%c existe pas encore\n", caract);
                afnd->Sigma = (char *)realloc(afnd->Sigma, sizeof(char) * (afnd->taille_alphabet + 1));
                afnd->Sigma[afnd->taille_alphabet] = caract;
                afnd->taille_alphabet++;
            }
            printf("Succès: (%d) -> [%c] -> (%d) crée.\n", nouveauS->num, caract, nouveauD->num);
        }
        else
        {
            printf("/!\\ Erreur format non reconnu! recommencez !\n");
            getchar();
        }

        printf("tapez <<q>> lorsque vous avez terminé, vous ne pouvez pas quitter sans avoir saisie au moins une transition valide:\n");
        printf("%d etats deja\n", afnd->nbre_etats);
        for (int i = 0; i < afnd->nbre_etats; i++)
        {
            printf("(%d)\n", afnd->Q[i].num);
            if (initialSetted != -1)
            {
                printf("init is %d\n", initialSetted);
            }
            if (afnd->Q[i].type == ACCEPTEUR)
                printf(" > TYPE: ACCEPTEUR\n ");
            else
            {
                if (afnd->Q[i].type == NON_ACCEPTEUR)
                    printf(" > TYPE: NON ACCEPTEUR\n ");
                else
                    printf(" > TYPE: ETAT MORT\n ");
            }
            for (int j = 0; j < afnd->Q[i].nbre_transitions; j++)
            {
                printf("trans from %d by %c to %d\n", afnd->Q[i].num, afnd->Q[i].transitions[j].c, afnd->Q[i].transitions[j].suiv->num);
            }
        }
        for (int i = 0; i < afnd->taille_alphabet; i++)
        {
            printf("################ %c|", afnd->Sigma[i]);
        }
    }
    if (afnd->nbre_etats > 0)
    {
        if (initialSetted == -1)
        {
            afnd->s = &(afnd->Q[0]);
            printf("Vous n'avez pas defini d'état initial, par défaut votre initial est <<%d>>", afnd->s->num);
            printf("AUTOMATE CREE AVEC SUCCES\n");
        }
        else
        {
            afnd->s = &(afnd->Q[initialSetted]);
        }
    }
    else
    {
        printf("AUCUN AUTOMATE N'A ETE CREE!\n");
        afnd = NULL;
    }

    free(type);

    printf("Taper une touche pour continuer...\n");
    getchar();

    free(input);
    system("clear");
    return afnd;
}
/**
 * Search in AFD if a state number already exists
 * @param afd       : the AFD in wich we search
 * @param stateId   : the id to search
 * @retun           : -1 if state does not exisit or the state index int afd->Q list if the state exists
 */
int stateAlreadyExists(AFND afnd, int stateId)
{
    int v = -1;
    for (int i = 0; i < afnd.nbre_etats; i++)
    {
        if (afnd.Q[i].num == stateId)
        {

            v = i;
            break;
        }
    }
    return v;
}
/**
 * Search in AFND if a caracter already exists
 * @param afd       : the AFND in wich we search
 * @param stateId   : the caracter to search
 * @retun           : -1 if char does not exisit or the char index int afd->Sigma list if the character exists
 */
int characterAlreadyExists(AFND afnd, char caracter)
{
    for (int i = 0; i < afnd.taille_alphabet; i++)
    {
        if (afnd.Sigma[i] == caracter)
            return i;
    }
    return -1;
}
void tasksOnUserDefinedAFND(AFND *afnd)
{
    int stayHere = 1;
    int doneSubMenu3 = 0;
    int doneSubMenu4 = 0;
    concatenated_etat **newStatesMAtrix2 = NULL;
    AFD *afdTestFrom_AFND2 = NULL;
    AFD *minimised = NULL;
    int *resulting_AFD_nbreEtats2 = NULL;
    AFND *afnd2 = NULL;
    while (stayHere)
    {
        //system("clear");

        printf("VOTRE PROPRE AUTOMATE A ETE CREE, QUE VOULEZ VOUS FAIRE ?\n\n");
        printf("/!\\ NOTE: Les modifications apportées dans ce menu seront persistantes sur l'automate de départ!! (déterminisation, concatenation...)\n\n");
        printf("1 => Afficher votre atomate\n");
        printf("2 => Creer un deuxieme automate pour faire l'union, la concatenation ou la fermeture transitive\n");
        printf("3 => Déterminiser cet automate et l'afficher\n");
        printf("4 => Minimiser cet automate et l'afficher\n");
        printf("5 => Exécuter sur un mot\n");
        printf("6 => revinir au menu principal\n");
        int choix = 0;
        do
        {
            printf("Entrez votre choix:");
            scanf(" %d", &choix);
        } while (choix < 1 || choix > 6);
        switch (choix)
        {
        case 1:
            Display(afnd);
            break;
        case 2:
            if (doneSubMenu3 != 1)
            {
                afnd2 = constructUserDefinedAFND();
                printf("LE deuxieme AFND a été crée, que voulez vous faire ?\n");
                printf("1 => Réunion et affichage\n");
                printf("2 => Concatenation et affichage\n");
                printf("3 => Fermeture transitive et affichage\n");
                char *choix2;
                do
                {
                    printf("Entrez votre choix:");
                    scanf(" %1s", &choix2);
                } while (atoi(choix2) < 0 || atoi(choix2) > 3);
                switch (atoi(choix2))
                {
                case 1:
                    //AFND *reunion = reunion_automate(afnd, afnd2);
                    //Display(reunion);
                    break;
                case 2:

                    //AFND *reunion = concatenation_automate(afnd, afnd2);
                    //Display(reunion);
                    break;
                default:

                    //AFND *reunion = farmeture_automate(afnd, afnd2);
                    //Display(reunion);
                    break;
                }
            }else
            {
                printf("Vous devez revenir au menu principal est saisir une nouvel AFND, car vous avez déja transformé le premier automate en AFD, \nvous ne pouvez donc plus faire ces 3 actions\n");
            }
            
            break;
        case 3:
            doneSubMenu3 = 1;
            resulting_AFD_nbreEtats2 = (int *)malloc(sizeof(int));
            if (alreadyDeterministic(*afnd) == 1)
            {
                printf("Already deterministic\n");
                afdTestFrom_AFND2 = AFND_to_AFD_recopie(*afnd);
            }
            else
            {
                printf("Not yet deterministic\n");
                newStatesMAtrix2 = createNewStatesMAtrix((*afnd2), resulting_AFD_nbreEtats2);
                afdTestFrom_AFND2 = create_AFD_fromStateMatrix(newStatesMAtrix2, (*afnd2), *resulting_AFD_nbreEtats2);
            }
            printf("SUCCES: L'automate a  été déterminisé.\n appuyer sur une touche pour l'afficher\n");
            getchar();
            displayDeterministic_AFD(*afdTestFrom_AFND2);
            printf("Retour au menu, NOTE: Cette modification de l'automate est persistante sur l'automate initial\n");
            break;
        case 4:

            if (doneSubMenu3 == 1)
            {
                doneSubMenu4 = 1;
                printf("Vérification...\n");
                printf("BRAVO: votre automate était déja déterministe !...\n");
                printf("Appuyer sur une touche pour afficher l'automate minimisé.\n");
                getchar();
                addDeathState(afdTestFrom_AFND2);
                minimised = constructBilan_AFD(*afdTestFrom_AFND2);
                removeDeathState(minimised);
                displayDeterministic_AFD(*minimised);
                printf("Minimisation et affichage terminée, \n");
            }
            else
            {
                printf("NOTE: tapez d'abord le (3) pour déterminiser avant de minimiser\n");
                printf("Appuyer sur une touche pour revenir au sous menu...\n");
                getchar();
            }

            break;
        case 5:
            if (doneSubMenu4 == 1)
            {
                char *mot = (char *)malloc(sizeof(char) * 100);
                do
                {

                    printf("Taper un mot exécuter sur l'automate\n");
                    scanf(" %s", mot);
                    executeDeterministicOnWord(mot, minimised);
                    printf("Taper <<q>> pour revenir au sous menu\n");
                } while (mot[0] != 'q');
                free(mot);
            }
            else
            {
                printf("NOTE: tapez d'abord le (3) pour déterminiser et (4) pour minimiser avant d'exécuter sur un mot\n");
                printf("Appuyer sur une touche pour revenir au sous menu...\n");
                getchar();
            }

            break;
        default:
            printf("NOTE: Si vous revenez au menu principal, toutes vos actions dans ce menu seront perdues ainsi que les automates que vous avez saisies!!\n");
            printf("Sortir quand meme? taper <<1>> pour sortir et <<0>>  pour rester dans le sous menu actuel\n");
            char *choice = (char *)malloc(sizeof(char) * 100);
            scanf(" %1s", choice);
            if (atoi(choice) == 1){
                stayHere = 0;
                system("clear");
            }
            free(choice);
            break;
        }
    }
    destroyAFD(afdTestFrom_AFND2);
    destroyAFD(minimised);
}
void exucuteBuildInSamples()
{
    /*
	AFND *automate = automate_Seul_Mot_vide();
	Display(automate);

	AFND *automate2 = automate_vide();
	Display(automate2);

	AFND *automateTest = automateTestPlein();
	Display(automateTest);
	*/
    /*
	piked ** matrix = afndToMatrix(*automateTest);
	int *msize = malloc(sizeof(int));
	char **a = computeNewEdgesFromMatrix(*automateTest, matrix, msize)
	*/
    /*
	int *resulting_AFD_nbreEtats = (int *)malloc(sizeof(int));
	concatenated_etat **newStatesMAtrix = createNewStatesMAtrix((*automateTest), resulting_AFD_nbreEtats);
	AFD * afdTestFrom_AFND = create_AFD_fromStateMatrix(newStatesMAtrix, (*automateTest), *resulting_AFD_nbreEtats);
	displayDeterministic_AFD(*afdTestFrom_AFND);
	int **res = constructBilan_AFD(*afdTestFrom_AFND);
	*/
    AFND *afnd2 = automateTestPlein2();
    Display(afnd2);

    AFD *afdTestFrom_AFND2 = NULL;
    int *resulting_AFD_nbreEtats2 = (int *)malloc(sizeof(int));
    if (alreadyDeterministic(*afnd2) == 1)
    {
        //printf("Already deterministic\n");
        afdTestFrom_AFND2 = AFND_to_AFD_recopie(*afnd2);
    }
    else
    {
        //printf("Not yet deterministic\n");
        concatenated_etat **newStatesMAtrix2 = createNewStatesMAtrix((*afnd2), resulting_AFD_nbreEtats2);
        afdTestFrom_AFND2 = create_AFD_fromStateMatrix(newStatesMAtrix2, (*afnd2), *resulting_AFD_nbreEtats2);
    }
    displayDeterministic_AFD(*afdTestFrom_AFND2);

    char *word = (char *)malloc(sizeof(char) * 6);
    word[0] = 'a';
    word[1] = 'b';
    word[2] = 'a';
    word[3] = 'a';
    word[4] = 'a';
    word[5] = 'a';
    executeDeterministicOnWord(word, afdTestFrom_AFND2);

    addDeathState(afdTestFrom_AFND2);
    displayDeterministic_AFD(*afdTestFrom_AFND2);
    AFD *minimised = constructBilan_AFD(*afdTestFrom_AFND2);
    printf("minimised\n");
    displayDeterministic_AFD(*minimised);
    removeDeathState(minimised);
    displayDeterministic_AFD(*minimised);

    char *word1 = (char *)malloc(sizeof(char) * 6);
    word1[0] = 'a';
    word1[1] = 'a';
    word1[2] = 'a';
    word1[3] = 'a';
    word1[4] = 'a';
    word1[5] = 'b';
    executeDeterministicOnWord(word1, minimised);
    destroyAFD(minimised);
    destroyAFD(afdTestFrom_AFND2);
}