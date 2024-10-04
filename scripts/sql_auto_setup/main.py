import mysql.connector as mysql

from mysql.connector import errorcode
try:
    db = mysql.connect(user='root', password='', host='localhost') # Connexion à la base de données

#################################################################
except mysql.Error as err:                                      #
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:             #
    print("Something is wrong with your user name or password") #
  elif err.errno == errorcode.ER_BAD_DB_ERROR:                  # Teste les erreurs
    print("Database does not exist")                            #   Communes
  else:                                                         #
    print(err)                                                  #
#################################################################

cursor = db.cursor()
dbcreate=input("Enter the database name: ").lower() # Demande le nom de la base de données
# cursor.execute(f"DROP DATABASE IF EXISTS {dbcreate}") # Supprime la base de données si elle existe déjà
cursor.execute(f"CREATE DATABASE IF NOT EXISTS {dbcreate}") # Crée la base de données si elle n'existe pas
cursor.execute(f"USE {dbcreate}") # Utilise la base de données
cursor.execute("CREATE TABLE IF NOT EXISTS account (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255), password VARCHAR(255), email VARCHAR(255), admin BOOLEAN)") # Crée la table account avec les colonnes id, name, password, email et admin

db.close() # Ferme la connexion à la base de données