import random
import numpy as np
from concurrent.futures import ProcessPoolExecutor

l = 4  # 每组比特数
m = 4  # 组数
Nr = 4  # 轮数
Sbox = [0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7]  # S盒
Pbox = [1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16]  # P盒

def reSbox(Sbox):
    '''
    生成逆S盒
    param Sbox: S盒
    '''
    reSbox = [0 for _ in Sbox]
    for i in range(len(Sbox)):
        reSbox[Sbox[i]] = i
    return reSbox

def xor(w, K):
    '''
    异或运算
    param w: 二进制字符串
    param K: 二进制字符串
    '''
    assert len(w) == len(K)
    u = ''
    for _w, _k in zip(w, K):
        u += str(int(_w) ^ int(_k))
    return u

def sbox(u):
    '''
    S盒运算
    param u: 二进制字符串
    '''
    v = ''
    for i in range(m):
        ui = u[i * l : i * l + l]
        vi = format(Sbox[int(ui, 2)], '04b')
        v += vi
    return v

def pbox(v):
    '''
    P盒运算
    param v: 二进制字符串
    '''
    assert len(v) == l * m
    w = ''
    for i in range(len(v)):
        w += v[Pbox[i] - 1]
    return w

def spn(plain, key):
    '''
    SPN加密算法
    param plain: 明文
    param key: 密钥
    '''
    keys = [key[i * Nr : i * Nr + l * m] for i in range(Nr + 1)]
    w = plain
    for r in range(Nr - 1):
        u = xor(w, keys[r])
        v = sbox(u)
        w = pbox(v)
    u = xor(w, keys[Nr - 1])
    v = sbox(u)
    y = xor(v, keys[Nr])
    return y

def generate_key(n):
    '''
    生成密钥
    param n: 密钥个数
    '''
    keys = []
    for _ in range(n):
        key = ''
        for _ in range(l * m + 4 * Nr):
            key += str(random.randint(0, 1))
        keys.append(key)
    return keys

def generate_data(T, key):
    '''
    生成明文-密文对
    param T: 明文-密文对个数
    '''
    data = []
    for _ in range(T):
        plain = bin(random.randint(0, 2 ** (l * m) - 1))[2:].zfill(l * m)
        y = spn(plain, key)
        data.append((plain, y))
    return data

Sbox = [0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7]  # S盒
reSbox = reSbox(Sbox)  # 逆S盒
T = 10000  # 每个密钥生成的明文-密文对个数
N = 50  # 待攻击密钥个数

def reverse_spn(data):
    '''
    线性攻击算法
    param data: 明文-密文对
    return: 最后一轮的密钥
    '''
    lens = len(data)
    counter = np.zeros((16, 16), dtype=int)
    for pair in data:
        x, y = pair
        y2, y4 = int(y[4:8], 2), int(y[12:], 2)
        x = [None] + [int(i) for i in x]  # 获取明文的每一位
        for L1 in range(16):
            for L2 in range(16):
                u2 = [int(i) for i in format(reSbox[L1 ^ y2], '04b')]
                u4 = [int(i) for i in format(reSbox[L2 ^ y4], '04b')]
                u = [None] * 5 + u2 + [None] * 4 + u4
                z = x[5] ^ x[7] ^ x[8] ^ u[6] ^ u[8] ^ u[14] ^ u[16]
                if z & 1 == 0:
                    counter[L1][L2] += 1
    counter = np.abs(counter - 0.5 * lens)
    k2, k4 = map(lambda x: x[0], np.where(counter == np.max(counter)))  # 找到最大值的位置

    counter1 = np.zeros((16, 16), dtype=int)
    counter2 = np.zeros((16, 16), dtype=int)
    for pair in data:
        x, y = pair
        y1, y2, y3, y4 = int(y[:4], 2), int(y[4:8], 2), int(y[8:12], 2), int(y[12:], 2)
        x = [None] + [int(i) for i in x]  # 获取明文的每一位
        for L1 in range(16):
            for L2 in range(16):
                u = [None] + [int(i) for i in format(reSbox[L1 ^ y1], '04b') +
                              format(reSbox[k2 ^ y2], '04b') +
                              format(reSbox[L2 ^ y3], '04b') +
                              format(reSbox[k4 ^ y4], '04b')]
                z = x[1] ^ x[2] ^ x[4] ^ u[1] ^ u[5] ^ u[9] ^ u[13]
                if z & 1 == 0:
                    counter1[L1][L2] += 1
                z = x[9] ^ x[10] ^ x[12] ^ u[3] ^ u[7] ^ u[11] ^ u[15]
                if z & 1 == 0:
                    counter2[L1][L2] += 1
    counter1 = np.abs(counter1 - 0.5 * lens)
    counter2 = np.abs(counter2 - 0.5 * lens)
    counter = counter1 + counter2
    k1, k3 = map(lambda x: x[0], np.where(counter == np.max(counter)))  # 找到最大值的位置
    key = format(k1, '04b') + format(k2, '04b') + format(k3, '04b') + format(k4, '04b')
    return key

def process_key(key):
    '''
    并行处理
    param key: 密钥
    '''
    data = generate_data(T=T, key=key)
    reverse_key = reverse_spn(data)
    return key, reverse_key

def print_result(future):
    '''
    打印结果
    '''
    key, reverse_key = future.result()
    msg = 'success' if key[16:] == reverse_key else 'fail'
    key = ' '.join([key[i:i+4] for i in range(0, len(key), 4)])
    reverse_key = ' '.join([reverse_key[i:i+4] for i in range(0, len(reverse_key), 4)])
    print(f'key = {key}\nreverse_key = {reverse_key}\nresult: {msg}\n')

if __name__ == '__main__':
    keys = generate_key(n=N)  # 生成密钥
    with ProcessPoolExecutor() as executor:  # 多进程处理
        futures = [executor.submit(process_key, key) for key in keys]
        for future in futures:
            future.add_done_callback(print_result)
