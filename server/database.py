import sqlite3

class Database():
    def __init__(self, dbName: str):
        self.__connection = sqlite3.connect(dbName)
        self.__cursor = self.__connection.cursor()

    def getUser(self, userName):
        query = f"SELECT * FROM user WHERE user_name=\'{userName}\'"
        self.__cursor.execute(query)
        result = self.__cursor.fetchall()
        return result

    def addNewUser(self, userName: str, hashh: str, salt: str):
        check = self.getUser(userName)
        if (len(check) < 1):
            query = f"INSERT INTO user (user_name, user_hash, user_salt) VALUES (\'{userName}\', \'{hashh}\', \'{salt}\')"
            self.__cursor.execute(query)
            self.__connection.commit()
            return "Пользователь добавлен"
        return "Пользователь уже существует"