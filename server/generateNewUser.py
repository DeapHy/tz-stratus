from getpass import getpass
from hashlib import md5
from uuid import uuid4
from database import Database

db = Database("database.db")

raw_login = input("Логин: ")
raw_password = getpass("Пароль: ")
salt = str(uuid4())

hashedlogin = md5(f'{raw_login}:{raw_password}'.encode('utf-8')).hexdigest()
hashh = md5(f"{salt}>>{hashedlogin}".encode('utf-8')).hexdigest()

print(db.addNewUser(raw_login, hashh, salt))