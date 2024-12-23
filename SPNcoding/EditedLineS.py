import random
import numpy as np
from concurrent.futures import ProcessPoolExecutor
# """
#     这里有个问题，我们都知道线性分析需要找到几个比特进行模拟，找比特时需要尽可能使得偏差的绝对值大，并且异或之后要能够消去某些中间变量。
#     并且最最重要的一点，各个活动S盒的选取是相互独立的，只需要考虑P盒中的置换输出条件即可。
#     但是选取合适的活动S盒是一个非常困难的事！！！
#     在这里密钥解密的第4-8和12-16位密钥解密式按照电子工业出版社《密码学原理与实践第三版》P59页进行设计；
# """

bit_length = 4  # 每组比特数
group_count = 4  # 组数
rounds = 4  # 轮数
sbox_table = [0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7]  # S盒
pbox_table = [1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16]  # P盒

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

sbox_table = [0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7]  # S盒
inverse_sbox_table = inverse_sbox(sbox_table)  # 逆S盒
data_count = 8000  # 每个密钥生成的明文-密文对个数
key_count = 10  # 待攻击密钥个数

def attack_reverse_spn(data_pairs):
    '''
    线性攻击算法
    param data_pairs: 明文-密文对
    return: 最后一轮的密钥
    '''
    total_pairs = len(data_pairs)
    counts = np.zeros((16, 16), dtype=int)
    for pair in data_pairs:
        plaintext, ciphertext = pair
        partial_y2, partial_y4 = int(ciphertext[4:8], 2), int(ciphertext[12:], 2)
        plaintext_bits = [None] + [int(bit) for bit in plaintext]  # 获取明文的每一位
        for L1 in range(16):
            for L2 in range(16):
                intermediate2 = [int(bit) for bit in format(inverse_sbox_table[L1 ^ partial_y2], '04b')]
                intermediate4 = [int(bit) for bit in format(inverse_sbox_table[L2 ^ partial_y4], '04b')]
                combined = [None] * 5 + intermediate2 + [None] * 4 + intermediate4
                z = (plaintext_bits[5] ^ plaintext_bits[7] ^ plaintext_bits[8] ^
                     combined[6] ^ combined[8] ^ combined[14] ^ combined[16])
                if z & 1 == 0:
                    counts[L1][L2] += 1
    counts = np.abs(counts - 0.5 * total_pairs)
    k2, k4 = map(lambda x: x[0], np.where(counts == np.max(counts)))  # 找到等于0的次数的最大值

    counts1 = np.zeros((16, 16), dtype=int)
    counts2 = np.zeros((16, 16), dtype=int)
    for pair in data_pairs:
        plaintext, ciphertext = pair
        y1, y2, y3, y4 = int(ciphertext[:4], 2), int(ciphertext[4:8], 2), int(ciphertext[8:12], 2), int(ciphertext[12:], 2)
        plaintext_bits = [None] + [int(bit) for bit in plaintext]  # 获取明文的每一位
        for L1 in range(16):
            for L2 in range(16):
                combined = [None] + [int(bit) for bit in format(inverse_sbox_table[L1 ^ y1], '04b') +
                                     format(inverse_sbox_table[k2 ^ y2], '04b') +
                                     format(inverse_sbox_table[L2 ^ y3], '04b') +
                                     format(inverse_sbox_table[k4 ^ y4], '04b')]
                z = (plaintext_bits[1] ^ plaintext_bits[2] ^ plaintext_bits[4] ^
                     combined[1] ^ combined[5] ^ combined[9] ^ combined[13])
                if z & 1 == 0:
                    counts1[L1][L2] += 1
                z = (plaintext_bits[9] ^ plaintext_bits[10] ^ plaintext_bits[12] ^
                     combined[3] ^ combined[7] ^ combined[11] ^ combined[15])
                if z & 1 == 0:
                    counts2[L1][L2] += 1
    counts1 = np.abs(counts1 - 0.5 * total_pairs)
    counts2 = np.abs(counts2 - 0.5 * total_pairs)
    final_counts = counts1 + counts2
    k1, k3 = map(lambda x: x[0], np.where(final_counts == np.max(final_counts)))  # 找到最大值的位置
    key = format(k1, '04b') + format(k2, '04b') + format(k3, '04b') + format(k4, '04b')
    return key

def handle_key(key):
    '''
    并行处理
    param key: 密钥
    '''
    data_pairs = create_data_pairs(data_count, key)
    estimated_key = attack_reverse_spn(data_pairs)
    return key, estimated_key

def display_result(future):
    '''
    打印结果
    '''
    key, estimated_key = future.result()
    result_status = 'Right' if key[16:] == estimated_key else 'Wrong'
    formatted_key = ([key[i:i + 4] for i in range(0, len(key), 4)])
    formatted_estimated_key = ([estimated_key[i:i + 4] for i in range(0, len(estimated_key), 4)])
    
    # 写入并比较原始密钥和推测密钥
    with open('results.txt', 'a') as file: 
        file.write(f'key = {formatted_key}\nlineral_decode_key = {formatted_estimated_key}\nresult: {result_status}\n\n')


if __name__ == '__main__':
    generated_keys = create_key(key_count)  # 生成密钥
    with ProcessPoolExecutor() as executor:  # 多进程处理
        answers = [executor.submit(handle_key, key) for key in generated_keys]
        for ans in answers:
            ans.add_done_callback(display_result)
