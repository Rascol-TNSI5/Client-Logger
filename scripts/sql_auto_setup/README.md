# Script de Configuration Automatique de Base de Données MySQL 🚀

Ce script aide à créer une base de données MySQL, à ajouter des tables et des colonnes de manière interactive. Il est conçu pour être utilisé avec le connecteur MySQL pour Python (`mysql-connector-python`).

## Prérequis 📋

-   Python 3.x
-   Le connecteur MySQL pour Python (`mysql-connector-python`)

Vous pouvez installer le connecteur MySQL en utilisant pip :

```sh
pip install mysql-connector-python
```

## Utilisation 🛠️

1. **Exécution du script** :
   Lancez le script en utilisant Python :

    ```sh
    python main_fr.py
    python main.py
    ```

    En fonction de votre préférence, choisissez, le `main.py` est en anglais et le `main_fr.py` est en français.

2. **Connexion à la base de données** :
   Le script vous demandera d'entrer les informations de connexion pour la base de données MySQL (nom d'utilisateur, mot de passe, hôte).

3. **Liste des bases de données** :
   Vous aurez l'option de lister les bases de données existantes sur le serveur MySQL.

4. **Création de la base de données** :
   Le script vous demandera d'entrer le nom de la base de données à créer. Si la base de données existe déjà, elle sera utilisée.

5. **Liste des tables** :
   Vous aurez l'option de lister les tables existantes dans la base de données sélectionnée.

6. **Création de la table** :
   Le script vous demandera d'entrer le nom de la table à créer. Une colonne `id` avec auto-incrémentation sera ajoutée comme clé primaire.

7. **Ajout de colonnes** :
   Vous pourrez ajouter autant de colonnes que vous le souhaitez à la table. Le script supporte les types de colonnes suivants :

    - `VARCHAR` : Vous serez invité à entrer la longueur de la colonne.
    - `BOOLEAN` : Vous serez invité à entrer la valeur par défaut de la colonne.
    - `INT`, `FLOAT`, `TIMESTAMP` : Ces types sont acceptés directement sans saisie supplémentaire.
        > (Le programme ne supporte pas plus de valeurs pour éviter des erreurs inutiles.)

8. **Fermeture de la connexion** :
   Une fois toutes les opérations terminées, la connexion à la base de données sera fermée automatiquement.

## Exemple de sortie 📄

```sh
Voulez-vous lister les bases de données ? (o/n) : o
# Liste des bases de données :
> database1
> database2

Entrez le nom de la base de données : testdb
# Base de données créée avec succès !

Voulez-vous lister les tables ? (o/n) : o
# Liste des tables :
> table1
> table2

Entrez le nom de la table : utilisateurs
# Table créée avec succès !
# Colonne id ajoutée avec auto-incrémentation comme clé primaire

Voulez-vous ajouter une autre colonne ? (o/n) : o
Entrez le nom de la colonne : nom
Entrez le type de la colonne (VARCHAR, BOOLEAN, INT, FLOAT, TIMESTAMP) : VARCHAR
Entrez la longueur de la colonne : 255
# Colonne nom:VARCHAR(255) créée avec succès !

Voulez-vous ajouter une autre colonne ? (o/n) : n
# Connexion à la base de données fermée avec succès !
```

## Avertissements ⚠️

-   Ce script écrasera toute base de données existante portant le même nom. Assurez-vous d'avoir sauvegardé vos données avant d'exécuter ce script.
-   Le script est conçu pour être utilisé dans un environnement de développement. Utilisez-le avec précaution dans un environnement de production.

## Auteur 👤

-   NEY_Twix

## Licence 📜

Ce projet est sous licence MIT. Voir le fichier [`LICENSE`](LinkToLicence "LICENSE") pour plus de détails.
