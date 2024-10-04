import os
os.system("cls")
print("""\033[33m
 _______  _____              _______ _     _ _______  _____       _______ _______ _______ _     _  _____ 
 |______ |   __| |           |_____| |     |    |    |     |      |______ |______    |    |     | |_____]
 ______| |____\| |_____      |     | |_____|    |    |_____|      ______| |______    |    |_____| |      
                                                                                                         \033[0m
""")
print("\033[1m\033[31m /!\\ Veuillez vous assurer que MySQL est installé et en cours d'exécution. /!\\ \033[0m")
print()
print("\033[33m------------------------------------------------------------------------------------------------------------------------")
print("# Informations :")
print("#  Ce script vous aidera à créer une base de données et une table dedans.")
print("#  Vous serez invité à entrer le nom de la base de données, le nom de la table, les noms des colonnes et les types de colonnes.")
print("#  Le script créera la base de données, la table et les colonnes pour vous. Vous pouvez ajouter autant de colonnes que vous le souhaitez.")
print("#  Le script créera automatiquement une colonne id comme clé primaire.")
print("#  Le script prend en charge les types de colonnes suivants :")
print("#  - VARCHAR : Demande la longueur de la colonne (0-4294967295). Si la longueur dépasse le maximum, elle est par défaut à VARCHAR(255).")
print("#  - BOOLEAN : Demande la valeur par défaut de la colonne.")
print("#  - INT, FLOAT, TIMESTAMP : Accepte directement ces types sans saisie supplémentaire.")
print("#  Commençons !")
print("------------------------------------------------------------------------------------------------------------------------")
print("# Avertissement :")
print("#  Ce script écrasera toute base de données existante portant le même nom.")
print("#  Assurez-vous d'avoir sauvegardé vos données avant d'exécuter ce script.")
print("------------------------------------------------------------------------------------------------------------------------\033[0m")
if input("\033[34mVoulez-vous continuer ? (\033[32mo\033[34m/\033[31mn\033[34m) : \033[0m").lower() != "o":
  exit()
#os.system("cls")

try:
  import mysql.connector as mysql
  from mysql.connector import errorcode
except ImportError or ModuleNotFoundError or AssertionError: # Vérifie que le module mysql-connector-python est installé
  print("\033[1m\033[31mMySQL Connector non trouvé. Veuillez installer le connecteur en utilisant 'pip install mysql-connector-python'.\033[0m")
  exit()

default_credentials = ("root", "", "localhost") # Identifiants par défaut

#################################################################
# Connexion à la base de données                                #
#################################################################
try:
  if input("\033[34mVoulez-vous utiliser les identifiants par défaut ? (\033[32mo\033[34m/\033[31mn\033[34m) : \033[0m").lower() == "o":
    host = mysql.connect(user=default_credentials[0], password=default_credentials[1], host=default_credentials[2])
  else:
    host = mysql.connect(user=input("\033[34mEntrez le nom d'utilisateur : \033[0m"), password=input("\033[34mEntrez le mot de passe : \033[0m"), host=input("\033[34mEntrez l'hôte : \033[0m"))
#################################################################
# Gestion des erreurs de connexion à la base de données         #
#################################################################
except mysql.Error as err:
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
    print("\033[1m\033[31m# Accès refusé : nom d'utilisateur ou mot de passe incorrect\033[0m")
    exit()
  elif err.errno == errorcode.ER_BAD_DB_ERROR:  
    print("\033[1m\033[31m# La base de données n'existe pas\033[0m")
    exit()
  else:
    print(f"\033[1m\033[31m${err}\033[0m")
    exit()
else:
  print("\033[1m\033[32m# Connecté au serveur avec succès !\033[0m")
#################################################################
bdd = host.cursor()

def list_databases():
  """
  Une fonction simple pour lister les bases de données sur le serveur MySQL.
  """
  bdd.execute("SHOW DATABASES") # Affiche la liste des bases de données
  print("\033[33m# Liste des bases de données :")
  for db in bdd:
    print(f"> {db[0]}")
  print("\033[0m")

def choose_db():
  """
  Invite l'utilisateur à entrer un nom de base de données, puis crée la base de données si elle n'existe pas.
  """
  dbcreate=input("\033[34mEntrez le nom de la base de données que vous souhaitez créer/modifier : \033[0m").lower() # Demande le nom de la base de données
  # bdd.execute(f"DROP DATABASE IF EXISTS {dbcreate}") # Supprime la base de données si elle existe déjà
  bdd.execute(f"CREATE DATABASE IF NOT EXISTS {dbcreate}") # Crée la base de données si elle n'existe pas
  bdd.execute(f"USE {dbcreate}") # Utilise la base de données

def list_tables():
  """
  Une fonction simple pour lister les tables dans la base de données sélectionnée.
  """
  bdd.execute("SHOW TABLES") # Affiche la liste des tables
  print("\033[33m# Liste des tables :")
  for table in bdd:
    print(f"> {table[0]}")
  print("\033[0m")

def add_column() -> tuple[str, str]:
  """
    Invite l'utilisateur à saisir un nom de colonne et son type, puis renvoie le nom de la colonne et le type.
    La fonction prend en charge les types de colonnes suivants :
    - VARCHAR : Demande la longueur de la colonne (0-4294967295). Si la longueur dépasse le maximum, elle est par défaut à VARCHAR(255).
    - BOOLEAN : Demande la valeur par défaut de la colonne.
    - INT, FLOAT, TIMESTAMP : Accepte directement ces types sans saisie supplémentaire.
    Renvoie :
      tuple : Un tuple contenant le nom de la colonne (str) et le type de la colonne (str).
  """
  col_name=input("\033[34mEntrez le nom de la colonne : \033[0m").lower() # Demande le nom de la colonne
  col_type=input(f"\033[34mEntrez le type de {col_name} (VARCHAR/BOOLEAN/INT/FLOAT/TIMESTAMP) : \033[0m") # Demande le type de la colonne
  if col_type == "VARCHAR":
    col_type += f"({input('\033[34mEntrez la longueur de la colonne (0-4294967295) (255) : \033[0m')})" # Demande la longueur de la colonne si le type est VARCHAR
    if int(col_type[8:-1]) > 4294967295:
      print("\033[1m\033[31mLongueur invalide\033[0m")
      col_type = "VARCHAR(255)"
    elif not(0<= int(col_type[8:-1]) and int(col_type[8:-1]) <= 4294967295):
      print("\033[1m\033[31mLongueur invalide\033[0m")
      col_type = "VARCHAR(255)"
  elif not(col_type == "INT" or col_type == "FLOAT" or col_type == "TIMESTAMP" or col_type == "BOOLEAN"):
    print("\033[1m\033[31mType invalide\033[0m")
    col_type == "VARCHAR(255)"
  return col_name, col_type
  
def create_table():
  """
    Invite l'utilisateur à saisir un nom de table, puis crée la table avec une colonne id comme clé primaire.
    La fonction invite ensuite l'utilisateur à ajouter des colonnes à la table.
  """
  tab_name = input("\033[34mEntrez le nom de la table : \033[0m").lower() # Demande le nom de la table.
  bdd.execute(f"CREATE TABLE IF NOT EXISTS {tab_name} (id INT AUTO_INCREMENT PRIMARY KEY)") # Crée la table avec la colonne id comme clé primaire
  print("\033[1m\033[32m# Table créée avec succès !\033[0m")
  print("\033[33m# Colonne id ajoutée avec auto-incrémentation comme clé primaire\033[0m")
  continue_adding=True
  while continue_adding:
    if input("\033[34mVoulez-vous ajouter une autre colonne ? (\033[32mo\033[34m/\033[31mn\033[34m) : \033[0m").lower() == "o":
      col_name, col_type = add_column() # Ajoute une colonne à la table
      bdd.execute(f"ALTER TABLE {tab_name} ADD COLUMN {col_name} {col_type}") # Ajoute une colonne à la table
      print(f"\033[1m\033[32m# Colonne {col_name}:{col_type} créée avec succès !\033[0m")
    else:
      continue_adding=False

#################################################################
# Main                                                          #
#################################################################
if input("\033[34mVoulez-vous lister les bases de données ? (\033[32mo\033[34m/\033[31mn\033[34m) : \033[0m").lower() == "o":
  list_databases()
choose_db() # Demande le nom de la base de données
print("\033[1m\033[32m# Base de données créée avec succès !\033[0m")
if input("\033[34mVoulez-vous lister les tables ? (\033[32mo\033[34m/\033[31mn\033[34m) : \033[0m").lower() == "o":
  list_tables()
create_table() # Crée la table


host.close() # Ferme la connexion à la base de données
print("\033[1m\033[32m# Connexion à la base de données fermée avec succès !\033[0m")