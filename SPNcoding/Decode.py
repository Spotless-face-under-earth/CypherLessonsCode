import numpy as np
import concurrent.futures

def inverse_sbox(sbox):
    '''
    生成逆S盒
    param sbox: S盒
    '''
    inv_sbox = [0] * len(sbox)
    for index in range(len(sbox)):
        inv_sbox[sbox[index]] = index
    return inv_sbox

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
    k2, k4 = map(lambda x: x[0], np.where(counts == np.max(counts)))  # 找到最大值的位置

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

sbox_table = [0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7]  # S盒
inverse_sbox_table = inverse_sbox(sbox_table)  # 逆S盒
data_count = 80000  # 每个密钥生成的明文-密文对个数
key_count = 5  # 待攻击密钥个数

if __name__ == '__main__':
    keys = []
    data_pairs = []
    
    with open("data.txt", "r") as file:
        for line in file:
            line = line.strip()  # 移除前后的空白字符
            if line.startswith("keys:"):  # 检查行是否以 "keys:" 开头
                key = line.split(":")[1].strip()  # 提取密钥
                keys.append(key)
            else:
                # 确保行是明文-密文对并进行处理
                if line:  # 确保行不为空
                    plain, cipher = line.split()
                    data_pairs.append((plain, cipher))

    for key in keys:  # 遍历所有密钥进行攻击
        estimated_key = attack_reverse_spn(data_pairs)  # 估计密钥
        result_status = 'Right' if key == estimated_key else 'Wrong'         
        # 格式化密钥输出
        formatted_key = ' '.join([key[i:i + 4] for i in range(0, len(key), 4)])
        formatted_estimated_key = ' '.join([estimated_key[i:i + 4] for i in range(0, len(estimated_key), 4)])
        
        print(f'key = {formatted_key}\nestimated_key = {formatted_estimated_key}\nresult: {result_status}\n')

