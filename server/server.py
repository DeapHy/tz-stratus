from fastapi import FastAPI
from pydantic import BaseModel
from database import Database
from typing import Union
from datetime import datetime
from json import dumps
from pathlib import Path
import os

if (not os.path.exists("metas")):
    os.mkdir("metas")

app = FastAPI()
db = Database("database.db")


"""
error codes:
1 - Пароль неверный
2 - Пользователя не существует
1. Отправка логина, получение соли
2. Отправка хэша, сравнение с тем, что лежит в БД, решение о входе
"""

temp = dict()

class LoginCreds(BaseModel):
    login: str
    hash: Union[str, None] = None

class Metadata(BaseModel):
    x: str
    y: str
    time: str
    login: str

@app.post('/sendMeta')
async def sendMeta(meta: Metadata):
    temp[meta.login].append({"xy": [int(meta.x), int(meta.y)], "t": meta.time})
    return {"Success": "True"}


@app.post('/getOTT')
async def getOTT(lc: LoginCreds):
    result = db.getUser(lc.login)
    if len(result) > 0:
        return {"Success": True, "hash": result[0][3]}
    return {"Success": False, "error_code": 2}

@app.post('/login')
async def login(lc: LoginCreds):
    result = db.getUser(lc.login)
    if len(result) > 0:
        if result[0][2] == lc.hash:
            temp[lc.login] = list()
            return {"Success": True}
        else:
            return {"Success": False, "error_code": 1}
    return {"Success": False, "error_code": 2}

@app.post('/logout')
async def logout(lc: LoginCreds):
    p = Path(f"metas/{lc.login}_meta_{datetime.now().strftime('%d.%m.%Y_%H.%M.%S')}.txt")
    with open(p, "w") as f:
        for item in temp[lc.login]:
            f.write(dumps(item) + "\n")
    temp.pop(lc.login)
    return {"Success": "True"}

# @app.post('/login')
# async def login(logcreds: LoginCreds):
#     # print(logcreds.Login, logcreds.Passw)
#     if logcreds.Login == "admin" and logcreds.Passw == "admin":
#         return {"Success": "True"}
#     return {"Success": "False"}