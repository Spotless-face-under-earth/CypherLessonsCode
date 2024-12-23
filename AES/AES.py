
# -*- coding: utf-8 -*-
# Implementation of AES encryption algorithm
"""
有问题，但是不知道问题在哪里QAQ
"""
import concurrent.futures
import numpy as np

# RCON表（轮常量）
RCON = [0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36]

def hex_string_to_byte_array(hex_string):  
    if len(hex_string) % 2 != 0 or not all(c in '0123456789abcdef' for c in hex_string):  
        raise ValueError("Invalid hex string")  
    return bytearray.fromhex(hex_string)

def gf256_multiply(a, b):
    result = 0
    for i in range(8):
        if b & 1:
            result ^= a
        high_bit_set = a & 0x80
        a = (a << 1) & 0xFF
        if high_bit_set:
            a ^= 0x1B
        b >>= 1
    return result

def EuclideanInverse(byte):
    if byte == 0:
        return 0
    a, b = 0x11B, byte
    x0, x1 = 0, 1
    while b != 1:
        q = a // b
        a, b = b, a % b
        x0, x1 = x1, x0 ^ gf256_multiply(q, x1)
    return x1
    
def SCode(byte):
    SMultiple = np.array([[1,1,0,1,0,0,1,1],[1,1,1,0,1,0,0,1],[1,1,1,1,0,1,0,0],[0,1,1,1,1,0,1,0],
                          [0,0,1,1,1,1,0,1],[1,0,0,1,1,1,1,0],[0,1,0,0,1,1,1,1],[1,0,1,0,0,1,1,1]])
    SAddition = np.array([1,1,0,1,0,0,1,1]).reshape(8, 1)
    resultBits = np.zeros(8, dtype=int)
    # 先对BYTE求逆，再将逆元进行计算
    byteReversed = EuclideanInverse(byte)
    bits = np.array([(byteReversed >> i) & 1 for i in range(8)]).reshape(8, 1)
    resultBits = np.bitwise_xor(np.dot(SMultiple, bits), SAddition)
    result = 0
    for i in range(8):
        result |= (resultBits[i][0] << i)
    return result

S_BOX = [
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
]


def MixColumns(state):
    for i in range(4):
        a = state[i*4:(i+1)*4]
        state[i*4] = gf256_multiply(0x2, a[0]) ^ gf256_multiply(0x3, a[1]) ^ a[2] ^ a[3]
        state[i*4+1] = a[0] ^ gf256_multiply(0x2, a[1]) ^ gf256_multiply(0x3, a[2]) ^ a[3]
        state[i*4+2] = a[0] ^ a[1] ^ gf256_multiply(0x2, a[2]) ^ gf256_multiply(0x3, a[3])
        state[i*4+3] = gf256_multiply(0x3, a[0]) ^ a[1] ^ a[2] ^ gf256_multiply(0x2, a[3])
    return state

def ShiftRows(state):
    state[1], state[5], state[9], state[13] = state[5], state[9], state[13], state[1]
    state[2], state[6], state[10], state[14] = state[10], state[14], state[2], state[6]
    state[3], state[7], state[11], state[15] = state[15], state[3], state[7], state[11]
    return state

def KeyExpansion(key):
    KeyExpansionArray = [0] * 176
    for i in range(16):
        KeyExpansionArray[i] = key[i]
    for i in range(16, 176, 4):
        temp = KeyExpansionArray[i-4:i]
        if i % 16 == 0:
            temp = temp[1:] + temp[:1]  # Rotate
            temp = [S_BOX[b] for b in temp]  # Apply S-box
            temp[0] ^= RCON[i // 16 - 1]  # Apply round constant
        for j in range(4):
            KeyExpansionArray[i + j] = KeyExpansionArray[i - 16 + j] ^ temp[j]
    return KeyExpansionArray

def parallel_shift_rows(state):
    with concurrent.futures.ThreadPoolExecutor() as executor:
        future = executor.submit(ShiftRows, state)
        return future.result()

def parallel_mix_columns(state):
    with concurrent.futures.ThreadPoolExecutor() as executor:
        future = executor.submit(MixColumns, state)
        return future.result()
    
def SubBytes(state):
    return [SCode(b) for b in state]

def main():
    keystring = input("Enter 32-hex digit key: ")
    plaintextstring = input("Enter 32-hex digit plaintext: ")
    key = hex_string_to_byte_array(keystring)
    plaintext = hex_string_to_byte_array(plaintextstring)
    k = KeyExpansion(key)
    
    # 初始轮密钥加
    for i in range(16):
        plaintext[i] ^= k[i]
    
    # 9轮标准操作
    for j in range(1, 10):
        plaintext = SubBytes(plaintext)
        plaintext = ShiftRows(plaintext)
        plaintext = MixColumns(plaintext)
        for i in range(16):
            plaintext[i] ^= k[j * 16 + i]
    
    # 最后一轮（没有 MixColumns）
    plaintext = SubBytes(plaintext)
    plaintext = ShiftRows(plaintext)
    for i in range(16):
        plaintext[i] ^= k[10 * 16 + i]
    
    ciphertext = plaintext
    print("Ciphertext: ", ''.join(format(b, '02x') for b in ciphertext))

if __name__ == "__main__":
    main()
