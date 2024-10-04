import os
os.system("cls")
print("""\033[33m
 _______  _____              _______ _     _ _______  _____       _______ _______ _______ _     _  _____ 
 |______ |   __| |           |_____| |     |    |    |     |      |______ |______    |    |     | |_____]
 ______| |____\| |_____      |     | |_____|    |    |_____|      ______| |______    |    |_____| |      
                                                                                                         \033[0m
""")
print("\033[1m\033[31m /!\\ Please make sure you have MySQL installed and running. /!\\ \033[0m")
print()
print("\033[33m------------------------------------------------------------------------------------------------------------------------")
print("# Informations:")
print("#  This script will help you create a database and a table in it.")
print("#  You will be prompted to enter the database name, table name, column names and column types.")
print("#  The script will create the database, table and columns for you. You can add as many columns as you want.")
print("#  The script will automatically create an id column as the primary key.")
print("#  The script supports the following column types:")
print("#  - VARCHAR: Prompts for the length of the column (0-4294967295). If the length exceeds the maximum, it defaults to VARCHAR(255).")
print("#  - BOOLEAN: Prompts for the default value of the column.")
print("#  - INT, FLOAT, TIMESTAMP: Directly accepts these types without additional input.")
print("#  Let's get started!")
print("------------------------------------------------------------------------------------------------------------------------")
print("# Warning:")
print("#  This script will overwrite any existing database with the same name.")
print("#  Make sure you have backed up your data before running this script.")
print("------------------------------------------------------------------------------------------------------------------------\033[0m")
if input("\033[34mDo you want to continue? (\033[32my\033[34m/\033[31mn\033[34m): \033[0m").lower() != "y":
  exit()
#os.system("cls")

try:
  import mysql.connector as mysql
  from mysql.connector import errorcode
except ImportError or ModuleNotFoundError or AssertionError: # Vérifie que le module mysql-connector-python est installé
  print("\033[1m\033[31mMySQL connector not found. Please install the connector using 'pip install mysql-connector-python'.\033[0m")
  exit()

default_credentials = ("root", "", "localhost") # Identifiants par défaut

#################################################################
# Connexion à la base de données                                #
#################################################################
try:
  if input("\033[34mDo you want to use the default credentials? (\033[32my\033[34m/\033[31mn\033[34m): \033[0m").lower() == "y":
    host = mysql.connect(user=default_credentials[0], password=default_credentials[1], host=default_credentials[2])
  else:
    host = mysql.connect(user=input("\033[34mEnter the username: \033[0m"), password=input("\033[34mEnter the password: \033[0m"), host=input("\033[34mEnter the host: \033[0m"))
#################################################################
# Gestion des erreurs de connexion à la base de données         #
#################################################################
except mysql.Error as err:
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
    print("\033[1m\033[31m# Access denied: wrong username or password\033[0m")
    exit()
  elif err.errno == errorcode.ER_BAD_DB_ERROR:  
    print("\033[1m\033[31m# Database does not exist\033[0m")
    exit()
  else:
    print(f"\033[1m\033[31m${err}\033[0m")
    exit()
else:
  print("\033[1m\033[32m# Connected to the database successfully!\033[0m")
#################################################################
bdd = host.cursor()

def list_databases():
  """
  A simple function to list the databases in the MySQL server.
  """
  bdd.execute("SHOW DATABASES") # Affiche la liste des bases de données
  print("\033[33m# List of databases:")
  for db in bdd:
    print(f"> {db[0]}")
  print("\033[0m")

def choose_db():
  """
  Prompts the user to input a database name, then creates the database if it does not exist.
  """
  dbcreate=input("\033[34mEnter the database name you want to edit/create: \033[0m").lower() # Demande le nom de la base de données
  # bdd.execute(f"DROP DATABASE IF EXISTS {dbcreate}") # Supprime la base de données si elle existe déjà
  bdd.execute(f"CREATE DATABASE IF NOT EXISTS {dbcreate}") # Crée la base de données si elle n'existe pas
  bdd.execute(f"USE {dbcreate}") # Utilise la base de données

def list_tables():
  """
  A simple function to list the tables in the selected database.
  """
  bdd.execute("SHOW TABLES") # Affiche la liste des tables
  print("\033[33m# List of tables:")
  for table in bdd:
    print(f"> {table[0]}")
  print("\033[0m")

def add_column() -> tuple[str, str]:
  """
  Prompts the user to input a column name and its type, then returns the column name and type.
  The function supports the following column types:
  - VARCHAR: Prompts for the length of the column (0-4294967295). If the length exceeds the maximum, it defaults to VARCHAR(255).
  - BOOLEAN: Prompts for the default value of the column.
  - INT, FLOAT, TIMESTAMP: Directly accepts these types without additional input.
  Returns:
    tuple: A tuple containing the column name (str) and the column type (str).
  """
  col_name=input("\033[34mEnter the column name: \033[0m").lower() # Demande le nom de la colonne
  col_type=input(f"\033[34mEnter the type of {col_name} (VARCHAR/BOOLEAN/INT/FLOAT/TIMESTAMP): \033[0m") # Demande le type de la colonne
  if col_type == "VARCHAR":
    col_type += f"({input('\033[34mEnter the length of the column (0-4294967295): \033[0m')})" # Demande la longueur de la colonne si le type est VARCHAR
    if int(col_type[8:-1]) > 4294967295:
      print("\033[1m\033[32mInvalid length\033[0m")
      col_type = "VARCHAR(255)"
  elif col_type == "BOOLEAN":
    col_type += f"({input('\033[34mEnter the default value of the column: \033[0m')})"
  elif not(col_type == "INT" or col_type == "FLOAT" or col_type == "TIMESTAMP"):
    print("\033[1m\033[32mInvalid type\033[0m")
    col_type == "VARCHAR(255)"
  return col_name, col_type
  
def create_table():
  """
  Prompts the user to input a table name, then creates the table with an id column as the primary key.
  The function then prompts the user to add columns to the table.
  """
  tab_name=input("\033[34mEnter the table name: \033[0m").lower() # Demande le nom de la table.
  bdd.execute(f"CREATE TABLE IF NOT EXISTS {tab_name} (id INT AUTO_INCREMENT PRIMARY KEY)") # Crée la table avec la colonne id comme clé primaire
  print("\033[1m\033[32m# Table created successfully!\033[0m")
  print("\033[33m# Added column id with auto increment as the primary key\033[0m")
  continue_adding=True
  while continue_adding:
    if input("\033[34mDo you want to add another column? (\033[32my\033[34m/\033[31mn\033[34m): \033[0m").lower() == "y":
      col_name, col_type = add_column() # Ajoute une colonne à la table
      bdd.execute(f"ALTER TABLE {tab_name} ADD COLUMN {col_name} {col_type}") # Ajoute une colonne à la table
      print(f"\033[1m\033[32m# {col_name}:{col_type} column created successfully!\033[0m")
    else:
      continue_adding=False

#################################################################
# Main                                                          #
#################################################################
if input("\033[34mDo you want to list the databases? (\033[32my\033[34m/\033[31mn\033[34m): \033[0m").lower() == "y":
  list_databases()
choose_db() # Demande le nom de la base de données
print("\033[1m\033[32m# Database created successfully!\033[0m")
if input("\033[34mDo you want to list the tables? (\033[32my\033[34m/\033[31mn\033[34m): \033[0m").lower() == "y":
  list_tables()
create_table() # Crée la table


host.close() # Ferme la connexion à la base de données
print("\033[1m\033[32m# Connection to the database closed successfully!\033[0m")