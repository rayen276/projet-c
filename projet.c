#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



#define MAX_CLOTHES 100
#define MAX_FILENAME 100

// Structures pour les articles et le panier
typedef struct {
    int id;
    char name[50];
    char size[18];
    float price;
    int quantity;
} clothingItem;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct {
    clothingItem *contents[100];
    int size;
} stack;
//---------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct {
    char name[50];
    char size[5];
    int quantity;
    float price;
} cartItem;
//----------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct {
    void **contents; // Tableau des éléments
    unsigned int nb_values; // Nombre d'éléments actuellement dans la pile
    unsigned int capacity;  // Capacité totale de la pile
} stacks;


//-----------------------------------------------------------------------------------------------------------------------------------------------------------
// Fonctions de gestion du panier
int stack_push(stack *s, clothingItem *item) {
    if (s->size < 100) {
        s->contents[s->size++] = item;
        return 0;
    }
    return -1;
}

clothingItem* pop(stack *s) {
    if (s->size > 0) {
        return s->contents[--s->size];
    }
    return NULL;
}

int stack_size(stack *s) {
    return s->size;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//  afficher le catalogue
void afficherVetements(clothingItem clothes[], int size) {
    printf("\n--- Catalogue des vêtements disponibles ---\n");
    for (int i = 0; i < size; i++) {
        printf("%d. %s (Taille: %s) - %.2f DT\n", clothes[i].id, clothes[i].name, clothes[i].size, clothes[i].price);
    }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void ajouterAuPanier(stack *panier, clothingItem clothes[], int size) {
    if (!panier) {
        printf("Erreur : panier non initialisé.\n");
        return;
    }

    int choix, quantite;
    char taille[5];
    int tailleValide = 0;

    afficherVetements(clothes, size);
    printf("\nEntrez l'ID du vêtement à ajouter au panier : ");
    scanf("%d", &choix);
//-------------------------------------------------------------------------------------------------------------------------------------------------------
    // Vérifier si l'ID de l'article existe
    clothingItem *articleChoisi = NULL;
    for (int i = 0; i < size; i++) {
        if (clothes[i].id == choix) {
            articleChoisi = &clothes[i];
            break;
        }
    }

    if (articleChoisi == NULL) {
        printf("Vêtement introuvable !\n");
        return;
    }
//----------------------------------------------------------------------------------------------------------------------------------------------------------
    //  taille et vérifier qu'elle est valide
    while (!tailleValide) {
        printf("Entrez la taille de l'article (M, L, XL, S) : ");
        scanf("%s", taille);
        if (strcmp(taille, "M") == 0 || strcmp(taille, "L") == 0 || strcmp(taille, "XL") == 0 || strcmp(taille, "S") == 0 || strcmp(taille, "XXL") == 0 || strcmp(taille, "m") == 0 || strcmp(taille, "xl") == 0 || strcmp(taille, "s") == 0) {
            tailleValide = 1;
        } else {
            printf("Taille invalide. Veuillez entrer M, L, XL ou S.\n");
        }
    }

    printf("Entrez la quantité : ");
    scanf("%d", &quantite);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    cartItem *nouveauItem = malloc(sizeof(cartItem));
    if (!nouveauItem) {
        printf("Erreur d'allocation mémoire !\n");
        return;
    }

    strcpy(nouveauItem->name, articleChoisi->name);
    strcpy(nouveauItem->size, taille);  // Utiliser la taille choisie
    nouveauItem->price = articleChoisi->price;
    nouveauItem->quantity = quantite;

    // Ajouter l'article au panier
    if (stack_push(panier, nouveauItem) == -1) {
        printf("Erreur : le panier est plein.\n");
        free(nouveauItem);
    } else {
        printf("Article ajouté au panier : %s (Taille : %s, Quantité : %d, Prix : %.2f DT)\n",
               articleChoisi->name, taille, quantite, articleChoisi->price * quantite);
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------

void afficherHistorique(char *username) {
    FILE *file = fopen("purchase_history.txt", "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier historique.\n");
        return;
    }

    char ligne[256];
    printf("\n--- Historique des achats ---\n");
    while (fgets(ligne, sizeof(ligne), file)) {
        if (strstr(ligne, username)) {
            printf("%s", ligne);
        }
    }

    fclose(file);
}
 void checkout(stack *panier) {
    if (!panier ) {
        printf("\nVotre panier est vide. Ajoutez des articles avant de payer.\n");
        return;
    }


    float totalPrice = 0;
    for (unsigned i = 0; i < stack_size(panier); i++) {
        cartItem *item = (cartItem *)panier->contents[i];
        totalPrice += item->price * item->quantity;
    }



    printf("\nMontant total : %.2f DT\n", totalPrice);
    printf("\nModes de paiement disponibles :\n");
    printf("1. Carte bancaire\n");
    printf("2. Especes\n");
    printf("Choisissez votre mode de paiement : ");
    int paymentChoice;
    scanf("%d", &paymentChoice);

    switch (paymentChoice) {
        case 1:
            printf("Vous avez choisi le paiement par carte bancaire.\n");
            printf("entrer la nemero de votre carte:\n");
            int numerodecarte;
            scanf("%d",&numerodecarte);
            printf("payment avec succes");
            printf("entrer la localisation:\n");
            char loc;
            scanf("%c",&loc);
            printf("vous commende vous parviendra dans un delai de 48 heures");
            break;
        case 2:
            printf("Vous avez choisi le paiement en especes.\n");
            printf("payment avec succes\n");
            printf("entrer la localisation:\n");

            scanf("%c",&loc);
            printf("vous commende vous parviendra dans un delai de 48 heures");
            break;
        default:
            printf("Choix invalide. Veuillez r�essayer.\n");
    }

 }

//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Créer un compte utilisateur
void creerCompteUtilisateur(char *username, char *password) {
    FILE *file = fopen("accounts.txt", "a");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier des comptes.\n");
        return;
    }

    fprintf(file, "%s;%s\n", username, password);
    fclose(file);
    printf("Compte cree avec succes !\n");
}
//---------------------------------------------------------------------------------------------------------------------------------

void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}
// Vérifier si un utilisateur existe
int verifierCompteUtilisateur(char *username, char *password) {
    FILE *file = fopen("accounts.txt", "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier des comptes.\n");
        return 0;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), file)) {
        char fileUsername[50], filePassword[50];

        char *token = strtok(ligne, ";");
        if (token != NULL) {
            strcpy(fileUsername, token);
            remove_newline(fileUsername);
        }

        token = strtok(NULL, ";");
        if (token != NULL) {
            strcpy(filePassword, token);
            remove_newline(filePassword);
        }


        if (strcmp(fileUsername, username) == 0 && strcmp(filePassword, password) == 0) {
            fclose(file);
            return 1; // Connexion réussie
        }
    }

    fclose(file);
    printf("Nom d'utilisateur ou mot de passe incorrect.\n");
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------
// Enregistrer l'historique d'achat


void enregistrerHistorique(char *username, stack *panier,float  total) {
    FILE *file = fopen("purchase_history.txt", "a");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier historique.\n");
        return;
    }


    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d", t);

    fprintf(file, "%s;", username);
    for (unsigned i = 0; i < stack_size(panier); i++) {
        cartItem *item = (cartItem *)panier->contents[i];
        fprintf(file, "%s(%s)x%d:%.2f;", item->name, item->size, item->quantity, item->price * item->quantity);total+= item->price * item->quantity;
    }
    fprintf(file, "Total:%.2f;Date:%s\n", total, date);
    fclose(file);
}

//----------------------------------------------------------------------------------------------------------------------------------
// Fonction pour déclarer un problème
void signalerProbleme(char *username) {
    char probleme[256];
    FILE *file = fopen("client_problems.txt", "a");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier des problèmes.\n");
        return;
    }

    // Obtenir la date actuelle
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", t);

    // Demander la  problème au client
    printf("Veuillez décrire votre problème (maximum 250 caractères) :\n");
    getchar();
    fgets(probleme, sizeof(probleme), stdin);
    probleme[strcspn(probleme, "\n")] = '\0';

    // Enregistrer le problème dans le fichier
    fprintf(file, "Utilisateur: %s | Date: %s | Problème: %s\n", username, date, probleme);
    fclose(file);

    printf("Votre problème a été signalé avec succès. Nous vous contacterons bientôt.\n");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
void mettreAJourCompte(char *username, int articlesAchetes) {
    char line[100], tempFileName[] = "temp_accounts.txt";
    FILE *file = fopen("accounts.txt", "r");
    FILE *tempFile = fopen(tempFileName, "w");
    if (!file || !tempFile) {
        printf("Erreur lors de l'accès au fichier des comptes.\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        char fileUsername[50], filePassword[50];
        int isLoyal, totalItems;
        sscanf(line, "%[^:]:%[^:]:%d:%d", fileUsername, filePassword, &isLoyal, &totalItems);

        if (strcmp(username, fileUsername) == 0) {
            totalItems += articlesAchetes;
            if (totalItems > 5) isLoyal = 1;
            fprintf(tempFile, "%s:%s:%d:%d\n", fileUsername, filePassword, isLoyal, totalItems);
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove("accounts.txt");
    rename(tempFileName, "accounts.txt");
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
int seConnecter(char *username) {
    char inputUsername[50], inputPassword[50], line[100];
    FILE *file = fopen("accounts.txt", "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier des comptes.\n");
        return 0;
    }

    printf("Entrez votre nom d'utilisateur : ");
    scanf("%s", inputUsername);
    printf("Entrez votre mot de passe : ");
    scanf("%s", inputPassword);

    while (fgets(line, sizeof(line), file)) {
        char fileUsername[50], filePassword[50];
        int isLoyal, totalItems;
        sscanf(line, "%[^:]:%[^:]:%d:%d", fileUsername, filePassword, &isLoyal, &totalItems);

        if (strcmp(inputUsername, fileUsername) == 0 && strcmp(inputPassword, filePassword) == 0) {
            strcpy(username, fileUsername);
            fclose(file);
            return 1; // Connexion réussie
        }
    }

    fclose(file);
    printf("Nom d'utilisateur ou mot de passe incorrect.\n");
    return 0;
}


//***************************************************************************************************************************************
int main() {
    stack panier = {{0}, 0};
    clothingItem catalogue[] = {
        {1, "T-shirt","M", 10.0},
        {2, "Jeans","xl", 120.0},
        {3, "Veste","M", 30.0},

        {4, "Jacket","l", 260.00},
        {5, "chemise femme","s", 1130.00},
        {6, "chemise","M",  55.00},
        {7, "Ensemble","xxl", 375.00},
        {8, "Pantalon femme","",  85.00},
        {9, "Pull homme","xl", 65.00},
        {10, "Pull homme","M",  99.90},
        {11, "Ensemble femme","s",  345.99},
        {12, "Pantalon homme ","xxxl",  112.00},
        {13, "Pull femme","l",225.95}

    };

    int catalogueSize = sizeof(catalogue) / sizeof(catalogue[0]);

    char username[50], password[50];
    int choix;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("***************** RS BOUTIQUE *************************\n");
    printf("===========================================================\n");
    printf("Bienvenue dans RS BOUTIQUE - Boutique de vetement en ligne\n");
    printf("===========================================================\n");
    printf("                                                             \n");
    printf("                                                             \n");
    printf("               Contactez-nous:+216 25590299                   \n");
    printf("                                                             \n");
    printf("                                                             \n");
    printf("               E-mail :contact@RSBOUTIQUE.tn                   \n");
    printf("                   Livraison gratuite                          \n");
    printf("                                                             \n");
    printf("        Livraison à domicile Dans toute la Tunisie !             \n");
    printf("                                                             \n");
    printf("  Service après vente:(+216) 23-222-432/(8h:30  - 18h:00 )    \n");
    printf("    ---------------- Nos marques---------------------------    \n");
    printf("         ----Sasio--Koton--Beymen--Deser----                    \n");
    printf("                                                             \n");
    printf("                                                             \n");
    printf("     Suivez-nous sur Facebook & Instagram : RSBOUTIQUE            \n");
    printf("                                                             \n");
    printf("                                                             \n");
    printf("        © 2024 RSBOUTIQUE. Tous les Droits Réservés                   \n");
    printf("                                                             \n");
    printf("                                                             \n");

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (1) {
        printf("\n--- Menu Principal ---\n");
        printf("1. Se connecter\n");
        printf("2. Créer un compte\n");
        printf("3. Afficher le catalogue\n");
        printf("4. Ajouter un article au panier\n");
        printf("5. Afficher le panier\n");
        printf("6. Passer à la caisse\n");
        printf("7. Retirer le dernier article ajouté\n");


        printf("8. Afficher l'historique d'achat\n");
        printf("9.declarer un probleme\n");
        printf("10. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float total;
        switch (choix) {
            case 1:
                printf("Entrez votre nom d'utilisateur : ");
                scanf("%s", username);
                printf("Entrez votre mot de passe : ");
                scanf("%s", password);
                if (verifierCompteUtilisateur(username, password)) {
                    printf("Connexion réussie.\n");
                } else {
                    printf("Utilisateur ou mot de passe incorrect.\n");
                }
                printf("appuier sur importe quelle bouton pour sortir");
                getchar();getchar();
                break;
            case 2:
                printf("Entrez un nom d'utilisateur : ");
                scanf("%s", username);
                printf("Entrez un mot de passe : ");
                scanf("%s", password);
                creerCompteUtilisateur(username, password);
                printf("appuier sur importe quelle bouton pour sortir");
                getchar();getchar();
                break;
            case 3:
                afficherVetements(catalogue, catalogueSize);
                printf("appuier sur importe quelle bouton pour sortir");
                getchar();getchar();
                break;
            case 4:
                ajouterAuPanier(&panier, catalogue, catalogueSize);
                enregistrerHistorique(username, &panier, total);
                printf("appuier sur importe quelle bouton pour sortir");
                getchar();getchar();
                break;
            case 5:
                printf("\n--- Contenu du Panier ---\n");
                for (int i = 0; i < panier.size; i++) {
                    cartItem *item = panier.contents[i];
                    printf("%s (Taille : %s, Quantité : %d, Prix : %.2f DT)\n",
                           item->name, item->size, item->quantity, item->price * item->quantity);
                }
                printf("appuier sur importe quelle bouton pour sortir");
                getchar();getchar();
                break;
            case 6:
                printf("Passage à la caisse .\n");
                checkout(&panier);
                printf("appuier sur importe quelle bouton pour sortir");
                getchar();getchar();
                break;

           case 7: // Retirer le dernier article ajouté
    {
        cartItem *itemRetire = pop(&panier); // Retirer le dernier élément du panier
        if (itemRetire != NULL) {
            printf("Article retiré : %s (Taille: %s) x%d : %.2f\n",
                   itemRetire->name, itemRetire->size, itemRetire->quantity,
                   itemRetire->price * itemRetire->quantity);
            free(itemRetire);
        } else {
            printf("Le panier est vide, aucun article à retirer.\n");
        }
        printf("appuier sur importe quelle bouton pour sortir");
        getchar();getchar();
        break;
    }

                break;


            case 8:
                afficherHistorique(username);
                printf("appuier sur importe quelle bouton pour sortir");
                getchar();getchar();
                break;
            case 9:
                signalerProbleme(username);
                printf("appuier sur importe quelle bouton pour sortir");
                getchar();getchar();
                break;

            case 10:
                printf("Merci de votre visite !\n");
                printf("appuier sur importe quelle bouton pour sortir");
                getchar();getchar();
                return 0;
            default:
                printf("Choix invalide. Essayez à nouveau.\n");
        }
    }

    return 0;
}
