import serial
import time

responses = {'d': 7, 
             'u': 6}

port = "COM6"
connection = serial.Serial(port, timeout=1)

def send_command(cmd: str,
                 response_len: int) -> str:
    str_resp = None
    connection.write(cmd.encode())
    if response_len != 0:
        resp = connection.read(response_len)
        str_resp = resp.decode()
    return str_resp



