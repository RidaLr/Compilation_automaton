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
    printf("2 => [TEST AFnD] Utiliser un exemple existant (executer des automates prêts pour les tests)\n");
    printf("3 => [TEST AFD] Utiliser un exemple existant (executer des automates prêts pour les tests)\n");
    printf("4 => Creer et afficher un automate reconnaissant le langage vide\n");
    printf("5 => Creer et afficher un automate reconnaissant le langage constitué du seul mot vide\n");
    printf("6 => Creer et afficher un automate reconnaissant un caractère passé en paramètre\n");
    printf("7 => Quitter le programme\n");
    int choix = 0;

    do
    {
        printf("Entrez votre choix compris entre 1 - 5 :");
        scanf(" %d", &choix);
    } while (choix < 1 || choix > 7);
    return choix;
}
void computeChoice(int choix)
{
    AFND *afnd = NULL;
    char c;
    switch (choix)
    {
    case 1:
        afnd = constructUserDefinedAFND();
        if (afnd != NULL)
            tasksOnUserDefinedAFND(afnd);
        break;
    case 2:
        Test_AFND();
        break;
    case 3:
        exucuteBuildInSamples();
        break;

    case 4:
        Display(automate_vide());
        break;
    case 5:

        Display(automate_Seul_Mot_vide());
        break;
    case 6:

        printf("Entrez le caractère à reconaitre\n");
        scanf(" %c", &c);
        printf("vous avez tapé %c\n", c);
        Display(automate_standard(c));
        break;

    default:
        printf("Merci et aurevoir \n");
        printf("Exiting... \n");
        exit(0);
        break;
    }
    printf("Terminé, appuyer sur une touche pour afficher le sous menu\n");
    //getchar();
}
AFND *constructUserDefinedAFND()
{
    int nbreEtat;
    int nbreEtatEffectif = 0;
    printf("Quel est le nombre d'états de votre automate ?\n");
    scanf(" %d", &nbreEtat);
    printf("\n\nCONSTRUCTION D'UN AUTOMATE : Veuillez entrer les transitions SVP:%d\n\n", nbreEtat);
    printf("SYNTAXE: [chiffre]-[caractere]-[chiffre]\n");
    printf("Exemple 1 :  5-a-1  les etats 5 et 1 seront creés ainsi qu'une transition de 5 vers 1 par 'a'\n");
    printf("Exemple 2 :  15-r-15  l'état 15 sera creé ainsi qu'une transition vers lui meme par 'r' \n\n");
    printf("NOTE: L'état mort sera rajouté automatiquement par le programme\n");

    AFND *afnd = (AFND *)malloc(sizeof(AFND));
    afnd->nbre_etats = 0;
    afnd->Q = (etat *)malloc(sizeof(etat) * nbreEtat);
    afnd->Sigma = (char *)malloc(sizeof(char) * 0);
    afnd->taille_alphabet = 0;
    char *type = (char *)malloc(sizeof(char) * 1);
    int initialSetted = -1;
    int endSaisieEtats = 0;
    transitionTemps *temp = (transitionTemps *)malloc(sizeof(transitionTemps) * 0);
    int taille = 0;
    while (1)
    {
        if (nbreEtatEffectif == nbreEtat)
            endSaisieEtats = 1;
        char *input = (char *)malloc(sizeof(char) * 20);
        printf("[ %d / %d ]", nbreEtatEffectif, nbreEtat);
        printf("Saisissez la prochaine transition (ou tapez q pour quitter) :");
        //fgets(input, 10, stdin);
        scanf(" %s", input);
        //fgets(input, 20, stdin);
        input[strcspn(input, "\n")] = 0;
        const char s[2] = "-";
        char caract;
        int edges[3] = {};
        char *token;

        int existeS = stateAlreadyExists(*afnd, edges[0]);

        int existeSNew = existeS;

        etat *nouveauS = NULL;
        etat *nouveauD = NULL;
        int validInput = 1;
        // fgets is \n terminated
        if (strlen(input) == 1 && input[0] == 'q')
        {
            if (nbreEtatEffectif == nbreEtat)
                break;
            else
            {
                printf("Vous ne pouvez pas sortir sans creer tous les etats [ %d / %d]", nbreEtatEffectif, nbreEtat);
            }
        }
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
                // printf(" %d %s\n", countToks, token);
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
        if (endSaisieEtats)
        {
            if (stateAlreadyExists(*afnd, edges[0]) == -1 || stateAlreadyExists(*afnd, edges[1]) == -1)
            {
                printf("\n\nNombre d'états atteints (%d/%d)\n", nbreEtat, nbreEtatEffectif);
                printf("Vous ne pouvez plus creer de nouveaux etats, vous pouvez rajouter des transitions sur les etats existant\n");
                validInput = 0;
            }
        }
        if (validInput)
        {
            //Transition existe déja ?
            int indexS = stateAlreadyExists(*afnd, edges[0]);
            int indexD = stateAlreadyExists(*afnd, edges[1]);
            int go = 1;
            if (indexS != -1 && indexD != -1)
            {
                for (int l = 0; l < afnd->taille_alphabet; l++)
                {
                    for (int t = 0; t < temp[indexS].taille; t++)
                    {
                        if (temp[indexS].Tabtransition[t].c == caract && temp[existeS].Tabtransition[t].suiv->num == temp[indexD].etatId)
                        {
                            go = 0;
                            printf("\n\n[Ignoré]: Cette transition existe déja \n");
                            break;
                        }
                    }
                }
            }
            if (go)
            {

                if (existeS == -1)
                {
                    //nouveauS = (etat *)malloc(sizeof(etat));
                    //nouveauS->num = edges[0];
                    //nouveauS->transitions = (transition *)malloc(sizeof(transition) * 0);
                    //nouveauS->nbre_transitions = 0;
                    //nouveauS->type = NON_ACCEPTEUR;
                    //afnd->Q = (etat *)realloc(afnd->Q, sizeof(etat) * (afnd->nbre_etats + 1));
                    temp = (transitionTemps *)realloc(temp, sizeof(transitionTemps) * (taille + 1));
                    taille++;
                    nbreEtatEffectif++;
                    //afnd->Q[afnd->nbre_etats] = *nouveauS;
                    afnd->Q[afnd->nbre_etats].num = edges[0];
                    afnd->Q[afnd->nbre_etats].nbre_transitions = 0;
                    afnd->Q[afnd->nbre_etats].type = NON_ACCEPTEUR;
                    //afnd->Q[afnd->nbre_etats].transitions = (transition *)malloc(sizeof(transition) * 0);
                    existeSNew = afnd->nbre_etats;
                    transitionTemps trans;
                    trans.etatId = afnd->Q[afnd->nbre_etats].num;
                    trans.taille = 0;
                    temp[existeSNew] = trans;
                    temp[existeSNew].Tabtransition = (transition *)malloc(sizeof(transition) * 0);
                    //printf("S num is %d\n", temp[existeSNew].etatId);
                    printf("Quel est le type de %d (taper <<N>> ou <<n>> pour non accepteur ou autre chose pour accepteur)\n", edges[0]);
                    scanf(" %1s", type);
                    if (strlen(type) == 1 && toupper(type[0]) != 'N')
                    {
                        afnd->Q[afnd->nbre_etats].type = ACCEPTEUR;
                        //printf("aaaaacpeur1 %d\n", afnd->Q[afnd->nbre_etats].num);
                    }

                    if (initialSetted == -1)
                    {
                        printf("-------------------------------------------------\n");
                        char isInit[20];
                        printf("Létat %d est t-il votre etat initial ? <<N>> ou <<n>> pour non et autre chose pour oui\n", temp[existeSNew].etatId);
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
                    existeSNew = stateAlreadyExists(*afnd, edges[0]);
                }
                int existeD = stateAlreadyExists(*afnd, edges[1]);
                int existeDNew = existeD;
                if (existeD == -1)
                {
                    //nouveauD = (etat *)malloc(sizeof(etat));
                    //nouveauD->num = edges[1];
                    // printf("D num is %d\n", nouveauD->num);
                    //nouveauD->transitions = (transition *)malloc(sizeof(transition) * 0);
                    //nouveauD->nbre_transitions = 0;
                    //nouveauD->type = NON_ACCEPTEUR;
                    afnd->Q[afnd->nbre_etats].type = NON_ACCEPTEUR;
                    printf("Quel est le type de %d (taper <<N>> ou <<n>> pour non accepteur ou autre chose pour accepteur)\n", edges[1]);

                    scanf(" %1s", type);
                    if (strlen(type) == 1 && toupper(type[0]) != 'N')
                    {
                        afnd->Q[afnd->nbre_etats].type = ACCEPTEUR;
                        //printf("aaaaacpeur %d %c\n", afnd->Q[afnd->nbre_etats].num, type[0]);
                    }
                    //afnd->Q = (etat *)realloc(afnd->Q, sizeof(etat) * (afnd->nbre_etats + 1));
                    nbreEtatEffectif++;
                    //afnd->Q[afnd->nbre_etats] = *nouveauD;
                    int s;
                    //afnd->Q[afnd->nbre_etats].transitions = (transition *)malloc(sizeof(transition) * 0);
                    temp = (transitionTemps *)realloc(temp, sizeof(transitionTemps) * (taille + 1));
                    taille++;
                    afnd->Q[afnd->nbre_etats].num = edges[1];
                    afnd->Q[afnd->nbre_etats].nbre_transitions = 0;

                    existeDNew = afnd->nbre_etats;
                    transitionTemps trans;
                    trans.etatId = afnd->Q[afnd->nbre_etats].num;
                    trans.taille = 0;
                    temp[existeDNew] = trans;
                    temp[existeDNew].Tabtransition = (transition *)malloc(sizeof(transition) * 0);
                    if (initialSetted == -1)
                    {
                        char isInit[20];
                        printf("Létat %d est t-il votre etat initial ? <<N>> ou <<n>> pour non et autre chose pour oui\n", temp[existeDNew].etatId);
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
                    existeDNew = stateAlreadyExists(*afnd, edges[1]);
                }
                // printf("SOURCE index zzzzzzzzzzzzzzzzzzzz = %d\n", temp[existeSNew].etatId);
                transition trans;
                trans.c = caract;
                //trans.suiv = &(afnd->Q[existeDNew]);
                //printf("CHPT1111 %d %d\n", existeSNew, afnd->Q[existeSNew].nbre_transitions);

                temp[existeSNew].Tabtransition = (transition *)realloc(temp[existeSNew].Tabtransition, sizeof(transition) * (temp[existeSNew].taille + 1));
                temp[existeSNew].Tabtransition[temp[existeSNew].taille] = trans;
                temp[existeSNew].Tabtransition[temp[existeSNew].taille].suiv = &(afnd->Q[existeDNew]);

                //printf("Transiton1 %d %c %d\n", temp[existeSNew].etatId, temp[existeSNew].Tabtransition[temp[existeSNew].taille].c, temp[existeSNew].Tabtransition[temp[existeSNew].taille].suiv->num);
                //afnd->Q[existeSNew].nbre_transitions++;
                temp[existeSNew].taille++;
                //printf("CHPT2\n");
                if (characterAlreadyExists(*afnd, caract) == -1)
                {
                    //   printf("%c existe pas encore\n", caract);
                    afnd->Sigma = (char *)realloc(afnd->Sigma, sizeof(char) * (afnd->taille_alphabet + 1));
                    afnd->Sigma[afnd->taille_alphabet] = caract;
                    afnd->taille_alphabet++;
                }
                printf("Succès: (%d) -> [%c] -> (%d) crée.\n", temp[existeSNew].etatId, caract, temp[existeDNew].etatId);
            }
        }
        else
        {
            printf("\n\n/!\\ Erreur format non reconnu ou nombre d'états atteint! Appuyez sur une touche et recommencez !\n");
            getchar();
        }

        printf("tapez <<q>> lorsque vous avez terminé:\n");
        //printf("%d etats deja\n", afnd->nbre_etats);
        /*
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
                printf("trans from %d (%p) by %c to %d p = %p s = %p\n", afnd->Q[i].num, &(afnd->Q[i]), afnd->Q[i].transitions[j].c, afnd->Q[i].transitions[j].suiv->num, afnd->Q[i].transitions[j].suiv, afnd->Q[i].transitions[j].c, afnd->Q[i].transitions[j].suiv->num, afnd->Q);
            }
        }*/
        /*
        for (int i = 0; i < afnd->taille_alphabet; i++)
        {
            printf("\t####\t####\t####\t#### %c|", afnd->Sigma[i]);
        }
        printf("nbre etats = %d\n", afnd->nbre_etats);

        printf("EBN\n");*/
    }
    if (afnd->nbre_etats > 0)
    {
        if (initialSetted == -1)
        {
            afnd->s = &(afnd->Q[0]);
            printf("\n\nVous n'avez pas defini d'état initial, par défaut votre initial est <<%d>>", afnd->s->num);
            printf("AUTOMATE CREE AVEC SUCCES\n");
        }
        else
        {
            afnd->s = &(afnd->Q[initialSetted]);
        }
        for (int i = 0; i < taille; i++)
        {
            transitionTemps currentTempTrans = temp[i];

            afnd->Q[currentTempTrans.etatId].transitions = (transition *)malloc(sizeof(transition) * currentTempTrans.taille);
            for (int j = 0; j < currentTempTrans.taille; j++)
            {
                afnd->Q[currentTempTrans.etatId].transitions[j].c = currentTempTrans.Tabtransition[j].c;
                afnd->Q[currentTempTrans.etatId].transitions[j].suiv = currentTempTrans.Tabtransition[j].suiv;
                afnd->Q[currentTempTrans.etatId].nbre_transitions++;
            }
        }
        int alreadyDeterministic = 1;
        for (int i = 0; i < afnd->nbre_etats; i++)
        {
            //printf("CHPT 1\n");

            int transitionExist = 0;
            for (int j = 0; j < afnd->Q[i].nbre_transitions; j++)
            {
                //printf("CHPT 2\n");
                char currentCaracter = afnd->Q[i].transitions[j].c;
                for (int k = j + 1; k < afnd->Q[i].nbre_transitions; k++)
                {
                    //printf("CHPT 3\n");
                    if (currentCaracter == afnd->Q[i].transitions[k].c)
                    {
                        //printf("CHPT 4\n");
                        //We found two different transitions on the same caracter
                        //printf("SAME AT %d %d for ID %d For caract %c", j, k, afnd->Q[i].num, currentCaracter);
                        transitionExist = 1;
                        j = afnd->Q[i].nbre_transitions;
                        //i = afnd.nbre_etats;
                        alreadyDeterministic = 0;
                        //printf("CHPT 5\n");
                        break;
                    }
                }
            }
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
    AFND *reunion = NULL;
    AFND *concatenation = NULL;
    AFND *fermeture = NULL;
    char *mot = NULL;
    char choixS[50];
    while (stayHere)
    {
        //system("clear");

        printf("VOTRE PROPRE AUTOMATE A ETE CREE, QUE VOULEZ VOUS FAIRE ?\n\n");

        printf("1 => Afficher votre atomate\n");
        printf("2 => Creer un deuxieme automate pour faire l'union, la concatenation ou la fermeture transitive\n");

        printf("3 => Fermeture transitive et affichage (Modification persistante)\n");
        printf("4 => revinir au menu principal\n");
        int choix = 0;
        do
        {
            printf("Entrez votre choix:");
            scanf(" %d", &choix);
        } while (choix < 1 || choix > 7);
        switch (choix)
        {
        case 1:
            Display(afnd);
            break;
        case 2:
            afnd2 = constructUserDefinedAFND();
            if (afnd2 != NULL)
            {
                do
                {
                    printf("LE deuxieme AFND a été crée, que voulez vous faire ?\n\n");
                    printf("1 => Réunion et affichage \n");
                    printf("2 => Concatenation et affichage\n");

                    printf("/!\\ NOTE: Les modifications apportées dans ce menu seront persistantes sur l'automate 1 de départ!! (déterminisation, concatenation...)\n\n");
                    char *choix2 = (char *)malloc(sizeof(char));
                    do
                    {
                        printf("Entrez votre choix:");
                        scanf(" %1s", choix2);
                    } while (atoi(choix2) < 0 || atoi(choix2) > 2);
                    switch (atoi(choix2))
                    {
                    case 1:

                        reunion = reunion_automate(afnd, afnd2);
                        Display(reunion);
                        break;
                    default:

                        concatenation = concatenation_automate(afnd, afnd2);
                        Display(concatenation);
                        break;
                    }

                    break;
                } while (choixS[0] != 'q');
            }
            else
            {
                printf("Le deuxième automate n'a pas encore été crée.");
            }
            break;
        case 3:

            fermeture = farmeture_automate(afnd);
            Display(fermeture);
            printf("NB: (Cette modification est persistante sur l'automate de départ)\n");
            break;
        default:
            printf("NOTE: Si vous revenez au menu principal, toutes vos actions dans ce menu seront perdues ainsi que les automates que vous avez saisies!!\n");
            printf("Sortir quand meme? taper <<1>> pour sortir et <<0>>  pour rester dans le sous menu actuel\n");
            char *choice = (char *)malloc(sizeof(char) * 100);
            scanf(" %1s", choice);
            if (atoi(choice) == 1)
            {
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
    char buff[50];
    printf("\t####Appuyer sur une touche et valider pour afficher un automate reconnaissant le langage vide\n");
    scanf(" %s", buff);
    AFND *automate2 = automate_vide();
    Display(automate2);

    printf("\t####Appuyer sur une touche et valider pour afficher un automate reconnaisant le mot vide\n");
    scanf(" %s", buff);
    AFND *automate = automate_Seul_Mot_vide();
    Display(automate);

    printf("\t####Appuyer sur une touche et valider pour générer un AUTOMATE NON DETERMINISTE\n");
    scanf(" %s", buff);
    system("clear");
    AFND *automateTest = automateTestPlein();
    Display(automateTest);
    printf("\n\n Nous allons utiliser l'automate (AFND) ci dessus pour le test  (FIG.1 du TD N°3)");
    printf("\t####Appuyer sur une touche pour déterminiser cet automate\n");
    scanf(" %s", buff);

    int *resulting_AFD_nbreEtats = (int *)malloc(sizeof(int));
    concatenated_etat **newStatesMAtrix = createNewStatesMAtrix((*automateTest), resulting_AFD_nbreEtats);
    AFD *afdTestFrom_AFND = create_AFD_fromStateMatrix(newStatesMAtrix, (*automateTest), *resulting_AFD_nbreEtats);
    displayDeterministic_AFD(*afdTestFrom_AFND);
    printf("L'automate AFD résulat est ci dessus\n");

    printf("\n\n\n\t####Appuyer sur une touche pour Minimiser cet automate\n");
    scanf(" %s", buff);

    addDeathState(afdTestFrom_AFND);
    AFD *minimised = constructBilan_AFD(*afdTestFrom_AFND);

    //displayDeterministic_AFD(*afdTestFrom_AFND);
    //AFD *minimised = constructBilan_AFD(*afdTestFrom_AFND);
    printf("minimised\n");
    //displayDeterministic_AFD(*minimised);
    removeDeathState(minimised);
    displayDeterministic_AFD(*minimised);
    printf("L'automate AFD minimisé résulat est ci dessus\n");

    printf("\n\n\n\t####Appuyer sur une touche pour Executer des mots de test sur cet automate minimisé\n");
    scanf(" %s", buff);

    char word[50];
    do
    {
        printf("Entrer un mot à exécuter: (<<q>> pour quitter)\n");
        scanf(" %s", word);
        executeDeterministicOnWord(word, minimised);

    } while (word[0] != 'q');
    printf("Merci d'avoir Testé ce programme, validez une touche pour revenir au menu\n");
    scanf(" %s", buff);
    destroyAFD(minimised);
    destroyAFD(afdTestFrom_AFND);
    /*
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
*/
}