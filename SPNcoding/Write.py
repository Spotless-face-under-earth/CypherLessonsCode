import random
import numpy as np

bit_length = 4  # 每组比特数
group_count = 4  # 组数
rounds = 4  # 轮数
sbox_table = [0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7]  # S盒
pbox_table = [1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16]  # P盒
data_count = 8000  # 每个密钥生成的明文-密文对个数
key_count = 5  # 待攻击密钥个数

def apply_sbox(input_bits):
    '''
    S盒运算
    param input_bits: 二进制字符串
    '''
    output_bits = ''
    for i in range(group_count):
        group = input_bits[i * bit_length: i * bit_length + bit_length]
        transformed = format(sbox_table[int(group, 2)], '04b')
        output_bits += transformed
    return output_bits

def apply_pbox(input_bits):
    '''
    P盒运算
    param input_bits: 二进制字符串
    '''
    assert len(input_bits) == bit_length * group_count
    output_bits = ''
    for i in range(len(input_bits)):
        output_bits += input_bits[pbox_table[i] - 1]
    return output_bits

def inverse_sbox(sbox):
    '''
    生成逆S盒
    param sbox: S盒
    '''
    inv_sbox = [0] * len(sbox)
    for index in range(len(sbox)):
        inv_sbox[sbox[index]] = index
    return inv_sbox

def bitwise_xor(a, b):
    '''
    异或运算
    param a: 二进制字符串
    param b: 二进制字符串
    '''
    assert len(a) == len(b)
    result = ''
    for bit_a, bit_b in zip(a, b):
        result += str(int(bit_a) ^ int(bit_b))
    return result

def encrypt_spn(plaintext, key):
    '''
    SPN加密算法
    param plaintext: 明文
    param key: 密钥
    '''
    keys = [key[i * rounds: i * rounds + bit_length * group_count] for i in range(rounds + 1)]
    current_bits = plaintext
    for r in range(rounds - 1):
        intermediate = bitwise_xor(current_bits, keys[r])
        substituted = apply_sbox(intermediate)
        current_bits = apply_pbox(substituted)
    intermediate = bitwise_xor(current_bits, keys[rounds - 1])
    substituted = apply_sbox(intermediate)
    ciphertext = bitwise_xor(substituted, keys[rounds])
    return ciphertext

def create_key(count):
    '''
    生成密钥
    param count: 密钥个数
    '''
    keys = []
    for _ in range(count):
        key = ''.join(str(random.randint(0, 1)) for _ in range(bit_length * group_count + 4 * rounds))
        keys.append(key)
    return keys

def create_data_pairs(num_pairs, key):
    '''
    生成明文-密文对
    param num_pairs: 明文-密文对个数
    '''
    data_pairs = []
    for _ in range(num_pairs):
        plaintext = bin(random.randint(0, 2 ** (bit_length * group_count) - 1))[2:].zfill(bit_length * group_count)
        ciphertext = encrypt_spn(plaintext, key)
        data_pairs.append((plaintext, ciphertext))
    return data_pairs


if __name__ == "__main__":
    generated_keys = create_key(key_count)  # 生成密钥
    with open("data.txt", "w") as file:
        for key in generated_keys:
            file.write("keys:")
            file.write(f"{key}\n") #密钥写入文件
            data_pairs = create_data_pairs(data_count, key)  # 生成明文-密文对
            for plain, cipher in data_pairs:
                file.write(f"{plain} {cipher}\n")  # 将明文-密文对写入文件
