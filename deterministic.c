#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "deterministic.h"
#include <unistd.h>

void executeDeterministicOnWord(char *word, AFD *afd)
{
    if (afd->nbre_etats < 1)
    {
        printf("L'automate ne contient pas d'états..\n");
        printf("exiting...\n");
        exit(0);
    }
    int KnownWord = 0;
    int currentIndex = 0;
    int wordSize = strlen(word);

    // if empty word (epsilon)
    if (wordSize < 1 && afd->s->type == ACCEPTEUR)
    {
        KnownWord = 1;
    }
    else
    {
        concatenated_etat *currentEtat = afd->s;
        while (wordSize > 0)
        {
            int foundValidTransition = 0;
            for (int i = 0; i < currentEtat->nbre_transitions; i++)
            {
                if (currentEtat->transitions[i].c == word[currentIndex])
                {
                    currentIndex++;
                    wordSize--;
                    currentEtat = currentEtat->transitions[i].suiv;
                    foundValidTransition = 1;
                    break;
                }
            }
            //We reached a state where the word has not been read entirely and the is no valid transition
            if (foundValidTransition == 0)
            {
                printf("no by %c on ID %d val %d\n", word[currentIndex], currentEtat->num, currentEtat->concatenatedNums[0]);
                break;
            }

            //Word read entirely and state is a final state
            if (wordSize < 1 && currentEtat->type == ACCEPTEUR)
                KnownWord = 1;
        }
    }
    if (KnownWord)
    {
        printf("LE mot << %s >> est reconnu par l'automate !\n", word);
    }
    else
    {
        printf("LE mot << %s >> N'EST PAS reconnu par l'automate !\n", word);
    }
}
int *canGoTo(AFND afnd, int sourceId, int *resultSize, char caract)
{
    int *result = (int *)malloc(sizeof(etat) * 0);
    int rSize = 0;
    for (int i = 0; i < afnd.nbre_etats; i++)
    {
        etat currentEtat = afnd.Q[i];
        if (currentEtat.num == sourceId && currentEtat.nbre_transitions > 0)
        {
            for (int j = 0; j < currentEtat.nbre_transitions; j++)
            {
                if (currentEtat.transitions[j].c == caract)
                {
                    //printf("from %d to %d nbTRans = %d\n", sourceId, currentEtat.transitions[j].suiv->num, currentEtat.nbre_transitions);
                    result = (int *)realloc(result, sizeof(int) * (rSize + 1));
                    result[rSize] = currentEtat.transitions[j].suiv->num;
                    rSize++;
                }
            }
            break;
        }
    }
    *resultSize = rSize;
    return result;
}
concatenated_etat **createNewStatesMAtrix(AFND afnd, int *resulting_AFD_nbreEtats)
{
    if (afnd.nbre_etats < 0)
    {
        printf("Erreur, l'automate ne contient pas d'états..\n");
        printf("exiting...\n");
        exit(0);
    }
    int lastDataTakenIndex = 0;
    int newEdgesSize = 1;
    int nexNum = afnd.s->num;

    concatenated_etat *nouveau = (concatenated_etat *)malloc(sizeof(concatenated_etat));
    nouveau->num = nexNum;
    nexNum++;
    nouveau->nbre_transitions = 0;
    nouveau->concatenatedNumSize = 0;
    nouveau->transitions = (AFD_transition *)malloc(sizeof(AFD_transition) * 0);
    nouveau->concatenatedNums = (int *)malloc(sizeof(int));
    nouveau->concatenatedNums[0] = nouveau->num;
    nouveau->concatenatedNumSize++;
    nouveau->type = afnd.s->type;
    concatenated_etat **oldStateMAtrix = (concatenated_etat **)malloc(sizeof(concatenated_etat));
    oldStateMAtrix[lastDataTakenIndex] = nouveau;
    while (lastDataTakenIndex < newEdgesSize)
    {
        concatenated_etat *currentOldState = oldStateMAtrix[lastDataTakenIndex];

        for (int l = 0; l < afnd.taille_alphabet; l++)
        {
            int finalResultSize = 0;
            int *finalResult = (int *)malloc(sizeof(int) * 0);
            //printf("currentOldstate is %d and numSize is %d\n", currentOldState->num, currentOldState->concatenatedNumSize);

            char currentChar = afnd.Sigma[l];

            for (int i = 0; i < currentOldState->concatenatedNumSize; i++)
            {
                int *resultSize = (int *)malloc(sizeof(int));
                int *result = canGoTo(afnd, currentOldState->concatenatedNums[i], resultSize, currentChar);
                for (int j = 0; j < *resultSize; j++)
                {
                    finalResult = (int *)realloc(finalResult, sizeof(int) * (finalResultSize + 1));
                    finalResult[finalResultSize] = result[j];
                    finalResultSize++;
                }
            }
            //printf("currentChar is %c and cangotosize is %d\n", currentChar, finalResultSize);

            //printf("[CGT() rSize is %d]\n", finalResultSize);
            //printf("++++++de letat %d  par %c on arrive à:", currentOldState->num, currentChar);
            /*
            for (int p = 0; p < finalResultSize; p++)
            {
                printf("%d", finalResult[p]);
            }
            printf("\n");
            */
            //stateThroughtCaract[currentChar][currentOldState->num] = finalResult;

            //search if the returned new concatenated_etat does not exist
            int alreadyExist = 0;
            int existeIndex = -1; //if it exists what is the index
            for (int i = 0; i < newEdgesSize; i++)
            {
                concatenated_etat currentEtat = *(oldStateMAtrix[i]);
                if (currentEtat.concatenatedNumSize == finalResultSize)
                {
                    int nberOfIndexeMatched = 0;
                    for (int j = 0; j < finalResultSize; j++)
                    {
                        int currentTastedNum = currentEtat.concatenatedNums[j];
                        int found = 0;
                        for (int k = 0; k < finalResultSize; k++)
                        {
                            if (currentTastedNum == finalResult[k])
                            {
                                found = 1;
                                break;
                            }
                        }
                        if (found == 1)
                        {
                            nberOfIndexeMatched++;
                        }
                    }
                    if (nberOfIndexeMatched == finalResultSize)
                    {
                        alreadyExist = 1;
                        existeIndex = i;
                        break;
                    }
                }
            }
            if (alreadyExist == 1)
            {
                //printf("State num %d already exists at index %d\n", oldStateMAtrix[existeIndex]->num, existeIndex);

                AFD_transition trans;
                trans.c = currentChar;
                trans.suiv = oldStateMAtrix[existeIndex];

                currentOldState->transitions = (AFD_transition *)realloc(currentOldState->transitions, sizeof(AFD_transition) * (currentOldState->nbre_transitions + 1));
                currentOldState->transitions[currentOldState->nbre_transitions] = trans;
                currentOldState->nbre_transitions++;
            }
            else
            {
                //printf("state does not exists\n");
                concatenated_etat *nouveau = (concatenated_etat *)malloc(sizeof(concatenated_etat));
                nouveau->num = nexNum;
                nouveau->type = NON_ACCEPTEUR;
                nexNum++;
                nouveau->nbre_transitions = 0;
                nouveau->concatenatedNumSize = 0;
                nouveau->transitions = (AFD_transition *)malloc(sizeof(AFD_transition) * 0);
                nouveau->concatenatedNums = (int *)malloc(sizeof(int) * 0);

                for (int i = 0; i < finalResultSize; i++)
                {
                    nouveau->concatenatedNums = (int *)realloc(nouveau->concatenatedNums, sizeof(int) * (nouveau->concatenatedNumSize + 1));
                    nouveau->concatenatedNums[nouveau->concatenatedNumSize] = finalResult[i];
                    nouveau->concatenatedNumSize++;
                    if (afnd.Q[finalResult[i]].type == ACCEPTEUR)
                    {
                        nouveau->type = ACCEPTEUR;
                    }
                }
                //printf("Nouveau num is %d\n", nouveau->num);
                oldStateMAtrix = (concatenated_etat **)realloc(oldStateMAtrix, sizeof(concatenated_etat *) * (newEdgesSize + 1));
                oldStateMAtrix[newEdgesSize] = nouveau;

                AFD_transition trans;
                trans.c = currentChar;
                trans.suiv = oldStateMAtrix[newEdgesSize];

                currentOldState->transitions = (AFD_transition *)realloc(currentOldState->transitions, sizeof(AFD_transition) * (currentOldState->nbre_transitions + 1));
                currentOldState->transitions[currentOldState->nbre_transitions] = trans;
                currentOldState->nbre_transitions++;

                newEdgesSize++;
            }
        }
        lastDataTakenIndex++;
    }
    *resulting_AFD_nbreEtats = newEdgesSize;

    return oldStateMAtrix;
}
AFD *create_AFD_fromStateMatrix(concatenated_etat **newStatesMAtrix, AFND afndFrom, int nbreEtats)
{
    AFD *afd = (AFD *)malloc(sizeof(AFD));
    afd->nbre_etats = nbreEtats;
    afd->Q = (concatenated_etat *)malloc(sizeof(concatenated_etat) * nbreEtats);

    afd->taille_alphabet = afndFrom.taille_alphabet;
    afd->Sigma = (char *)malloc(sizeof(char) * afndFrom.taille_alphabet);
    for (int i = 0; i < afndFrom.taille_alphabet; i++)
    {
        afd->Sigma[i] = afndFrom.Sigma[i];
    }
    for (int i = 0; i < nbreEtats; i++)
    {
        afd->Q[i] = *(newStatesMAtrix[i]);
    }
    afd->s = &(afd->Q[0]);
    free(newStatesMAtrix);
    return afd;
}
AFD *constructBilan_AFD(AFD afd)
{
    int *tabInit = (int *)malloc(sizeof(int) * afd.nbre_etats);
    int *tabTypes = (int *)malloc(sizeof(int) * afd.nbre_etats);
    int *tabBilan = (int *)malloc(sizeof(int) * afd.nbre_etats);
    int **deterministicMatrix = (int **)malloc(sizeof(int *));
    int *lastTypeNum = (int *)malloc(sizeof(int));
    *lastTypeNum = 1;

    for (int i = 0; i < afd.nbre_etats; i++)
    {
        concatenated_etat currentEtat = afd.Q[i];
        for (int j = 0; j < afd.taille_alphabet; j++)
        {
            char currentChar = afd.Sigma[j];
            //printf("CHPT %c lm\n", currentChar);
            if (i == 0)
                deterministicMatrix[currentChar] = (int *)malloc(sizeof(int) * afd.nbre_etats);
            // printf("CHPT 2\n");
            if (currentEtat.type == ACCEPTEUR)
            {
                tabInit[currentEtat.num] = 0;
                tabTypes[currentEtat.num] = 0;
            }
            else
            {
                tabInit[currentEtat.num] = 1;
                tabTypes[currentEtat.num] = 1;
            }
            for (int k = 0; k < currentEtat.nbre_transitions; k++)
            {
                if (currentEtat.transitions[k].c == currentChar)
                {

                    if (currentEtat.transitions[k].suiv->type == ACCEPTEUR)
                    {
                        deterministicMatrix[currentEtat.transitions[k].c][currentEtat.num] = 0;
                    }
                    else
                    {
                        deterministicMatrix[currentChar][currentEtat.num] = 1;
                    }
                }
            }
        }
    }

    printf("ffffffffffffffffffffffffffffffffffffff\n");
    for (int i = 0; i < afd.nbre_etats; i++)
    {
        for (int j = 0; j < afd.taille_alphabet; j++)
        {

            printf("[%c][%d] = %d\n", afd.Sigma[j], afd.Q[i].num, deterministicMatrix[afd.Sigma[j]][afd.Q[i].num]);
        }
    }
    constructBilan(deterministicMatrix, afd, tabInit, tabBilan, lastTypeNum, tabTypes);

    AFD *minimised = constructMinisedAFD_from_bilan(deterministicMatrix, afd, tabBilan);

    return minimised;
}
int bilanEqualsInit(int *tabInit, int *tabBilan, int taille)
{
    for (int i = 0; i < taille; i++)
    {

        if (tabInit[i] != tabBilan[i])
            return 0;
    }
    return 1;
}
void constructBilan(int **deterministicMatrix, AFD afd, int *tabInit, int *tabBilan, int *lastTypeNum, int *tabTypes)
{

    int lastNum = *lastTypeNum;
    for (int j = 0; j < afd.nbre_etats; j++)
    {
        printf("TYPE[%d] = %d\n", afd.Q[j].num, tabTypes[afd.Q[j].num]);
    }
    int finished = 0;
    do
    {
        for (int i = 0; i < afd.nbre_etats; i++)
        {
            concatenated_etat currentEtat = afd.Q[i];
            printf("++++++++++++curenntEtat is %d\n", currentEtat.num);
            printf("lllllllllllll = %d\n", lastNum);
            for (int j = 0; j < afd.nbre_etats; j++)
            {
                concatenated_etat currentEtatAux = afd.Q[j];

                if (tabInit[currentEtat.num] == tabTypes[currentEtatAux.num])
                {
                    int same = 1;

                    for (int k = 0; k < afd.taille_alphabet; k++)
                    {
                        char currentChar = afd.Sigma[k];
                        if (deterministicMatrix[currentChar][currentEtat.num] != deterministicMatrix[currentChar][currentEtatAux.num])
                            same = 0;
                    }

                    if (same == 0)
                    {
                        //Chercher si la meme configuration n'existe pas déja
                        int existe = 0;
                        for (int k = 0; k < j; k++)
                        {
                            int found = 1;
                            for (int l = 0; l < afd.taille_alphabet; l++)
                            {
                                if (deterministicMatrix[afd.Sigma[l]][afd.Q[k].num] != deterministicMatrix[afd.Sigma[l]][currentEtatAux.num] && tabInit[currentEtatAux.num] == tabInit[afd.Q[k].num])
                                {
                                    found = 0;
                                    break;
                                }
                            }
                            if (found == 1 && tabInit[currentEtatAux.num] == tabInit[afd.Q[k].num])
                            {
                                printf("conf\n");
                                printf("tabTYpes[%d] = %d f %d\n", currentEtatAux.num, tabTypes[afd.Q[k].num], afd.Q[k].num);
                                printf("tabBilan[%d] = %d\n", currentEtatAux.num, tabTypes[afd.Q[k].num]);
                                tabTypes[currentEtatAux.num] = tabTypes[afd.Q[k].num];
                                tabBilan[currentEtatAux.num] = tabBilan[afd.Q[k].num];

                                existe = 1;
                                break;
                            }
                        }
                        if (existe == 0)
                        {
                            //ne plus remodifier s'il a déja ete modifié
                            int dejaModifie = 0;
                            for (int m = 0; m < i; m++)
                            {
                                if (currentEtatAux.num == afd.Q[m].num)
                                {
                                    dejaModifie = 1;
                                    break;
                                }
                            }
                            if (dejaModifie == 0)
                            {
                                printf("config of %d not found max = %d\n", currentEtatAux.num, lastNum);
                                printf("tabTYpes[%d] = %d\n", currentEtatAux.num, lastNum + 1);
                                printf("tabBilan[%d] = %d\n", currentEtatAux.num, lastNum + 1);
                                tabTypes[currentEtatAux.num] = lastNum + 1;
                                tabBilan[currentEtatAux.num] = lastNum + 1;
                                lastNum++;
                                printf("llllllllllllllllllllllllllnew = %d\n", lastNum);
                            }
                        }
                    }
                    else
                    {
                        printf("config of %d nthing\n", currentEtatAux.num);
                        printf("tabBilan[%d] = %d\n", currentEtatAux.num, tabInit[currentEtat.num]);
                        tabBilan[currentEtatAux.num] = tabInit[currentEtat.num];
                    }
                }
                printf("CHTPT!\n");
            }
            printf("INIT is\n");
            for (int i = 0; i < afd.nbre_etats; i++)
            {
                printf("INIT[%d] = %d\n", afd.Q[i].num, tabInit[afd.Q[i].num]);
            }

            printf("BIL is\n");
            for (int i = 0; i < afd.nbre_etats; i++)
            {
                printf("BIL[%d] = %d\n", afd.Q[i].num, tabBilan[afd.Q[i].num]);
            }

            printf("TYPES is\n");
            for (int i = 0; i < afd.nbre_etats; i++)
            {
                printf("TYPE[%d] = %d\n", afd.Q[i].num, tabTypes[afd.Q[i].num]);
            }
        }

        updateDeterministicMatrix(tabBilan, deterministicMatrix, afd);
        for (int i = 0; i < afd.nbre_etats; i++)
        {
            concatenated_etat currentEtat = afd.Q[i];
            for (int j = 0; j < afd.taille_alphabet; j++)
            {
                char currnetChar = afd.Sigma[j];
                printf("det[%c][%d] = %d\n", currnetChar, currentEtat.num, deterministicMatrix[currnetChar][currentEtat.num]);
            }
        }

        finished = bilanEqualsInit(tabInit, tabBilan, afd.nbre_etats);
        //update init
        for (int i = 0; i < afd.nbre_etats; i++)
        {
            tabInit[afd.Q[i].num] = tabBilan[afd.Q[i].num];
        }
    } while (finished == 0);

    *lastTypeNum = lastNum;
}
AFD *constructMinisedAFD_from_bilan(int **deterministicMatrix, AFD unMinimisedAFD, int *tabBilan)
{

    //We shouldn't create a state two times as the same number can appear in "tabBilan"
    int *createdStates = (int *)malloc(sizeof(int) * unMinimisedAFD.nbre_etats);
    for (int i = 0; i < unMinimisedAFD.nbre_etats; i++)
    {
        createdStates[tabBilan[unMinimisedAFD.Q[i].num]] = 0;
    }
    int newSize = unMinimisedAFD.nbre_etats;
    for (int i = 0; i < unMinimisedAFD.nbre_etats; i++)
    {
        int currentId = tabBilan[i];
        for (int j = i + 1; j < unMinimisedAFD.nbre_etats; j++)
        {
            if (tabBilan[j] == currentId)
                newSize--;
        }
    }

    AFD *afd = (AFD *)malloc(sizeof(AFD));
    afd->Q = (concatenated_etat *)malloc(sizeof(concatenated_etat) * 0);
    afd->Sigma = (char *)malloc(sizeof(char) * unMinimisedAFD.taille_alphabet);
    for (int i = 0; i < unMinimisedAFD.taille_alphabet; i++)
    {
        afd->Sigma[i] = unMinimisedAFD.Sigma[i];
    }

    afd->taille_alphabet = unMinimisedAFD.taille_alphabet;
    afd->nbre_etats = 0;
    int tailleEtatts = 0;
    int lastNum = 0;
    afd->Q = (concatenated_etat *)malloc(sizeof(concatenated_etat) * newSize);
    for (int i = 0; i < unMinimisedAFD.nbre_etats; i++)
    {
        concatenated_etat currentEtat = unMinimisedAFD.Q[i];
        if (createdStates[tabBilan[currentEtat.num]] == 0)
        {
            printf("creating %d n %d\n", currentEtat.num, tabBilan[currentEtat.num]);
            printf("fqsfsfqsf %d\n", deterministicMatrix['a'][0]);
            printf("fqsfsfqsf %d\n", deterministicMatrix['a'][0]);
            concatenated_etat *nouveau = (concatenated_etat *)malloc(sizeof(concatenated_etat));
            nouveau->type = currentEtat.type;
            nouveau->num = tabBilan[currentEtat.num];
            nouveau->concatenatedNumSize = 1;

            afd->Q[afd->nbre_etats] = *nouveau;
            afd->Q[afd->nbre_etats].concatenatedNums = (int *)malloc(sizeof(int));
            afd->Q[afd->nbre_etats].concatenatedNums[0] = currentEtat.num;
            afd->Q[afd->nbre_etats].nbre_transitions = 0;
            afd->Q[afd->nbre_etats].transitions = (AFD_transition *)malloc(sizeof(AFD_transition) * 0);
            afd->nbre_etats++;
            createdStates[tabBilan[currentEtat.num]] = 1;
        }
        else
        {
            if (currentEtat.type != NON_ACCEPTEUR)
            {
                for (int k = 0; k < i; k++)
                {
                    if (afd->Q[k].num == currentEtat.num)
                    {
                        afd->Q[k].type = currentEtat.type;
                    }
                }
            }
        }
    }
    //sleep(15);
    printf("fqsfsfqsf\n");
    for (int i = 0; i < unMinimisedAFD.nbre_etats; i++)
    {
        concatenated_etat currentEtat = unMinimisedAFD.Q[i];
        for (int j = 0; j < unMinimisedAFD.taille_alphabet; j++)
        {
            char currentChar = unMinimisedAFD.Sigma[j];
            //find suiv by this char
            printf("CHPTAbis %d %c = %d\n", currentEtat.num, currentChar, deterministicMatrix[currentChar][currentEtat.concatenatedNums[0]]);
            int numToFind = deterministicMatrix[currentChar][currentEtat.concatenatedNums[0]];
            printf("CHPTA\n");
        }
    }
    printf("dddddddddddddd\n");
    //Creation des transitions
    for (int i = 0; i < afd->nbre_etats; i++)
    {
        concatenated_etat *currentEtat = &(afd->Q[i]);
        for (int j = 0; j < afd->taille_alphabet; j++)
        {
            char currentChar = unMinimisedAFD.Sigma[j];
            //find suiv by this char
            printf("jjjjjjjjjjjjjjjjjjjjjjjjjjjjj%d\n", currentEtat->concatenatedNums[0]);
            printf("CHPTA %d %c %d\n", currentEtat->num, currentChar, deterministicMatrix[currentChar][currentEtat->concatenatedNums[0]]);
            int numToFind = deterministicMatrix[currentChar][currentEtat->concatenatedNums[0]];
            printf("++++++chercher %d\n", numToFind);
            for (int k = 0; k < afd->nbre_etats; k++)
            {
                if (afd->Q[k].num == numToFind)
                { //printf("CHPTB\n");
                    AFD_transition trans;
                    trans.c = currentChar;
                    trans.suiv = &(afd->Q[k]);
                    printf(" suiv de %d par %c =  %d \n", currentEtat->concatenatedNums[0], trans.c, afd->Q[k].num, afd->Q[k].num);

                    currentEtat->transitions = (AFD_transition *)realloc(currentEtat->transitions, sizeof(AFD_transition) * (currentEtat->nbre_transitions + 1));
                    printf("aaab\n");
                    currentEtat->transitions[currentEtat->nbre_transitions] = trans;
                    currentEtat->nbre_transitions++;
                    break;
                }
            }
        }
    }
    afd->s = &(afd->Q[0]);
    return afd;
}
void updateDeterministicMatrix(int *tabBilan, int **deterministicMatrix, AFD afd)
{
    for (int i = 0; i < afd.nbre_etats; i++)
    {
        concatenated_etat currentEtat = afd.Q[i];
        for (int j = 0; j < afd.taille_alphabet; j++)
        {
            char currnetChar = afd.Sigma[j];
            for (int k = 0; k < currentEtat.nbre_transitions; k++)
            {
                if (currentEtat.transitions[k].c == currnetChar)
                {
                    deterministicMatrix[currnetChar][currentEtat.num] = tabBilan[currentEtat.transitions[k].suiv->num];
                }
            }
        }
    }
}
AFD *AFND_to_AFD_recopie(AFND afnd)
{
    AFD *afd = (AFD *)malloc(sizeof(AFD));
    afd->nbre_etats = afnd.nbre_etats;
    afd->taille_alphabet = afnd.taille_alphabet;
    afd->Sigma = (char *)malloc(sizeof(char) * afnd.taille_alphabet);
    afd->Q = (concatenated_etat *)malloc(sizeof(concatenated_etat) * afnd.nbre_etats);
    for (int i = 0; i < afnd.nbre_etats; i++)
    {
        etat currentAFND_etat = afnd.Q[i];
        concatenated_etat currentAFD_etat;
        currentAFD_etat.concatenatedNumSize = 1;
        currentAFD_etat.concatenatedNums = (int *)malloc(sizeof(int));
        currentAFD_etat.concatenatedNums[0] = currentAFND_etat.num;
        currentAFD_etat.num = currentAFND_etat.num;
        currentAFD_etat.type = currentAFND_etat.type;
        currentAFD_etat.nbre_transitions = currentAFND_etat.nbre_transitions;
        currentAFD_etat.transitions = (AFD_transition *)malloc(sizeof(AFD_transition) * currentAFND_etat.nbre_transitions);
        afd->Q[i] = currentAFD_etat;
    }

    for (int i = 0; i < afnd.nbre_etats; i++)
    {
        etat currentAFND_etat = afnd.Q[i];
        concatenated_etat *currentAFD_etat = &(afd->Q[i]);
        for (int j = 0; j < currentAFND_etat.nbre_transitions; j++)
        {
            currentAFD_etat->transitions[j].c = currentAFND_etat.transitions[j].c;
            printf("CHPT11111\n");
            currentAFD_etat->transitions[j].suiv = &(afd->Q[currentAFND_etat.transitions[j].suiv->num]);
        }
    }
    for (int i = 0; i < afnd.taille_alphabet; i++)
    {
        afd->Sigma[i] = afnd.Sigma[i];
    }

    printf("CHPT11111\n");
    afd->s = &(afd->Q[0]);
    return afd;
}
/**
 * Add's a death state if the AFD is not a complete AFD
 * @param afd : the AFD
 * @return    : a complete AFD where laking transitions by caracter points to a deth state
 * 
*/
void addDeathState(AFD *afd)
{
    concatenated_etat *mort = (concatenated_etat *)malloc(sizeof(concatenated_etat));
    mort->type = MORT;
    mort->nbre_transitions = afd->taille_alphabet;
    mort->concatenatedNumSize = 1;
    mort->concatenatedNums = (int *)malloc(sizeof(int));
    mort->transitions = (AFD_transition *)malloc(sizeof(AFD_transition) * afd->taille_alphabet);
    //Find his num
    int max = 0;
    for (int i = 0; i < afd->nbre_etats; i++)
    {
        if (afd->Q[i].num > max)
            max = afd->Q[i].num;
    }
    mort->num = max + 1;
    mort->concatenatedNums[0] = max + 1;

    for (int i = 0; i < afd->taille_alphabet; i++)
    {
        AFD_transition trans;
        trans.c = afd->Sigma[i];
        trans.suiv = mort;

        mort->transitions[i] = trans;
    }

    int created = 0;
    for (int i = 0; i < afd->nbre_etats; i++)
    {
        concatenated_etat *currentEtat = &(afd->Q[i]);
        for (int j = 0; j < afd->taille_alphabet; j++)
        {
            char caract = afd->Sigma[j];
            int found = 0;
            for (int k = 0; k < currentEtat->nbre_transitions; k++)
            {
                if (currentEtat->transitions[k].c == caract)
                {
                    printf("char %c f on %d\n", caract, currentEtat->num);
                    found = 1;
                    break;
                }
                else
                    printf("char %c nf on %d\n", caract, currentEtat->num);
            }
            if (found == 0)
            {
                AFD_transition trans;
                trans.c = caract;
                trans.suiv = mort;

                currentEtat->transitions = (AFD_transition *)realloc(currentEtat->transitions, sizeof(AFD_transition) * (currentEtat->nbre_transitions + 1));
                currentEtat->transitions[currentEtat->nbre_transitions] = trans;
                currentEtat->nbre_transitions++;
                created = 1;
            }
        }
    }
    if (created == 0)
        free(mort);
    else
    {
        //etat mort
        afd->Q = (concatenated_etat *)realloc(afd->Q, sizeof(concatenated_etat) * (afd->nbre_etats + 1));
        afd->Q[afd->nbre_etats] = *mort;
        afd->nbre_etats++;
    }
}

AFND *automateTestPlein()
{

    etat *init = (etat *)(malloc(sizeof(etat)));
    init->num = 0;
    init->type = NON_ACCEPTEUR;

    etat *un = (etat *)(malloc(sizeof(etat)));
    un->num = 1;
    un->type = NON_ACCEPTEUR;

    etat *deux = (etat *)(malloc(sizeof(etat)));
    deux->num = 2;
    deux->type = NON_ACCEPTEUR;

    etat *trois = (etat *)(malloc(sizeof(etat)));
    trois->num = 3;
    trois->type = ACCEPTEUR;

    transition transitionsInit0;
    transitionsInit0.c = 'a';
    transitionsInit0.suiv = init;

    transition transitionsInit1;
    transitionsInit1.c = 'b';
    transitionsInit1.suiv = init;

    transition transitionsInit2;
    transitionsInit2.c = 'a';
    transitionsInit2.suiv = un;
    init->nbre_transitions = 3;
    init->transitions = (transition *)(malloc(sizeof(transition) * 3));
    init->transitions[0] = transitionsInit0;
    init->transitions[1] = transitionsInit1;
    init->transitions[2] = transitionsInit2;
    init->nbre_transitions = 3;

    //2
    transition transitionsInit3;
    transitionsInit3.c = 'a';
    transitionsInit3.suiv = deux;

    transition transitionsInit4;
    transitionsInit4.c = 'b';
    transitionsInit4.suiv = deux;
    un->transitions = (malloc(sizeof(transition) * 2));
    un->transitions[0] = transitionsInit3;
    un->transitions[1] = transitionsInit4;
    un->nbre_transitions = 2;
    //3
    transition transitionsInit5;
    transitionsInit5.c = 'a';
    transitionsInit5.suiv = trois;
    deux->transitions = (malloc(sizeof(transition)));
    deux->transitions[0] = transitionsInit5;
    deux->nbre_transitions = 1;

    AFND *automate = (AFND *)(malloc(sizeof(AFND)));
    automate->s = init;
    automate->Q = (etat *)malloc(sizeof(etat) * 4);
    automate->Q[0] = *init;
    automate->Q[1] = *un;
    automate->Q[2] = *deux;
    automate->Q[3] = *trois;
    automate->nbre_etats = 4;
    //automate.F = NULL;
    automate->Sigma = (char *)malloc(sizeof(char) * 2);
    automate->Sigma[0] = 'a';
    automate->Sigma[1] = 'b';
    automate->taille_alphabet = 2;
    //automate.delta = NULL;

    return automate;
}
AFND *automateTestPlein2()
{

    etat *init = (etat *)(malloc(sizeof(etat)));
    init->num = 0;
    init->type = NON_ACCEPTEUR;

    etat *un = (etat *)(malloc(sizeof(etat)));
    un->num = 1;
    un->type = NON_ACCEPTEUR;

    etat *deux = (etat *)(malloc(sizeof(etat)));
    deux->num = 2;
    deux->type = ACCEPTEUR;

    transition transitionsInit0;
    transitionsInit0.c = 'a';
    transitionsInit0.suiv = un;

    transition transitionsInit1;
    transitionsInit1.c = 'b';
    transitionsInit1.suiv = deux;

    init->nbre_transitions = 2;
    init->transitions = (transition *)(malloc(sizeof(transition) * 2));
    init->transitions[0] = transitionsInit0;
    init->transitions[1] = transitionsInit1;
    init->nbre_transitions = 2;

    //2
    transition transitionsInit3;
    transitionsInit3.c = 'a';
    transitionsInit3.suiv = un;

    transition transitionsInit4;
    transitionsInit4.c = 'b';
    transitionsInit4.suiv = deux;
    un->transitions = (malloc(sizeof(transition) * 2));
    un->transitions[0] = transitionsInit3;
    un->transitions[1] = transitionsInit4;
    un->nbre_transitions = 2;

    deux->nbre_transitions = 0;

    AFND *automate = (AFND *)(malloc(sizeof(AFND)));
    automate->s = init;
    automate->Q = (etat *)malloc(sizeof(etat) * 3);
    automate->Q[0] = *init;
    automate->Q[1] = *un;
    automate->Q[2] = *deux;
    automate->nbre_etats = 3;
    //automate.F = NULL;
    automate->Sigma = (char *)malloc(sizeof(char) * 2);
    automate->Sigma[0] = 'a';
    automate->Sigma[1] = 'b';
    automate->taille_alphabet = 2;
    //automate.delta = NULL;

    return automate;
}

int alreadyDeterministic(AFND afnd)
{
    int alreadyDeterministic = 1;
    for (int i = 0; i < afnd.nbre_etats; i++)
    {

        int transitionExist = 0;
        for (int j = 0; j < afnd.Q[i].nbre_transitions; j++)
        {
            char currentCaracter = afnd.Q[i].transitions[j].c;
            for (int k = j + 1; k < afnd.Q[i].nbre_transitions; k++)
            {

                if (currentCaracter == afnd.Q[i].transitions[k].c)
                {
                    //We found two different transitions on the same caracter
                    printf("SAME AT %d %d for ID %d For caract %c", j, k, afnd.Q[i].num, currentCaracter);
                    transitionExist = 1;
                    j = afnd.Q[i].nbre_transitions;
                    i = afnd.nbre_etats;
                    alreadyDeterministic = 0;
                    break;
                }
            }
        }
    }
    return alreadyDeterministic;
}
void removeDeathState(AFD *afd)
{
    for (int i = 0; i < afd->nbre_etats; i++)
    {
        concatenated_etat *currentEtat = &(afd->Q[i]);
        for (int j = 0; j < currentEtat->nbre_transitions; j++)
        {
            if (currentEtat->type == MORT)
            {
                for (int k = 0; k < afd->nbre_etats; k++)
                {
                    if (afd->Q[k].num != currentEtat->num)
                    {
                        if (currentEtat->num == afd->s->num)
                            afd->s = &(afd->Q[k]);

                        concatenated_etat *currentEtatAux = &(afd->Q[k]);
                        int reduceNbre = 0;
                        for (int m = 0; m < currentEtatAux->nbre_transitions; m++)
                        {
                            if (currentEtatAux->transitions[m].suiv == currentEtat)
                            {
                                currentEtatAux->transitions[m].suiv = NULL;
                                currentEtatAux->transitions[m].c = ' ';
                                reduceNbre++;
                                printf("%d was poin on %d\n", currentEtatAux->concatenatedNums[0], currentEtat->concatenatedNums[0]);
                            }
                        }
                        currentEtatAux->nbre_transitions -= reduceNbre;
                        printf("count is %d %d\n", currentEtatAux->nbre_transitions, reduceNbre);

                        currentEtatAux->transitions = (AFD_transition *)realloc(currentEtatAux->transitions, sizeof(AFD_transition) * currentEtatAux->nbre_transitions);
                    }
                }
                destroyAFDEtat(currentEtat);
                i = afd->nbre_etats;
                afd->nbre_etats--;
                break; //There is a unique death state in a graph
            }
        }
    }
}
void destroyAFDEtat(concatenated_etat *AFDetat)
{
    if (AFDetat != NULL)
    {
        for (int i = 0; i < AFDetat->nbre_transitions; i++)
        {
            AFDetat->transitions[i].suiv = NULL;
        }
        free(AFDetat->concatenatedNums);
        free(AFDetat->transitions);
    }
}
void destroyAFD(AFD *finishedAFD)
{
    if (finishedAFD != NULL)
    {

        for (int i = 0; i < finishedAFD->nbre_etats; i++)
        {
            destroyAFDEtat(&finishedAFD->Q[i]);
        }
        free(finishedAFD->Q);
        free(finishedAFD->Sigma);

        finishedAFD->nbre_etats = 0;
        finishedAFD->taille_alphabet = 0;
        free(finishedAFD);
    }
}
void displayDeterministic_AFD(AFD afd)
{
    printf(" \n ========== Affichage automate ===========\n\n");
    printf("s = [");
    for (int k = 0; k < afd.s->concatenatedNumSize; k++)
    {
        printf("%d", afd.s->concatenatedNums[k]);
    }
    printf("]\n");

    for (int i = 0; i < afd.nbre_etats; i++)
    {
        concatenated_etat currentEtat = afd.Q[i];
        printf("------------------------------\n");
        printf("ETAT : [");
        for (int k = 0; k < currentEtat.concatenatedNumSize; k++)
        {
            printf("%d", currentEtat.concatenatedNums[k]);
        }
        printf("]\n");
        printf("------------------------------\n");
        if (currentEtat.type == ACCEPTEUR)
            printf(" > TYPE: ACCEPTEUR\n ");
        else
        {
            if (currentEtat.type == NON_ACCEPTEUR)
                printf(" > TYPE: NON ACCEPTEUR\n ");
            else
                printf(" > TYPE: ETAT MORT\n ");
        }
        printf(" TRANSITIONS = \n{\n");
        for (int j = 0; j < currentEtat.nbre_transitions; j++)
        {
            printf("[");
            for (int k = 0; k < currentEtat.concatenatedNumSize; k++)
            {
                printf("%d,", currentEtat.concatenatedNums[k]);
            }
            printf("]->");
            printf(" %c ", currentEtat.transitions[j].c);
            printf("->[");
            for (int k = 0; k < currentEtat.transitions[j].suiv->concatenatedNumSize; k++)
            {
                printf("%d,", currentEtat.transitions[j].suiv->concatenatedNums[k]);
            }
            printf("]\n");
        }
        printf("}\n\n");
    }
}