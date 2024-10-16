import numpy as np

# 创建一个 256x256 的差分攻击表，初始值为 0
diff_table = np.zeros((256, 256), dtype=int)

# 16x16 SM4 查找表
newdata ="""
d6 90 e9 fe cc e1 3d b7 16 b6 14 c2 28 fb 2c 05
2b 67 9a 76 2a be 04 c3 aa 44 13 26 49 86 06 99
9c 42 50 f4 91 ef 98 7a 33 54 0b 43 ed cf ac 62
e4 b3 1c a9 c9 08 e8 95 80 df 94 fa 75 8f 3f a6 
47 07 a7 fc f3 73 17 ba 83 59 3c 19 e6 85 4f a8 
68 6b 81 b2 71 64 da 8b f8 eb 0f 4b 70 56 9d 35
1e 24 0e 5e 63 58 d1 a2 25 22 7c 3b 01 21 78 87
d4 00 46 57 9f d3 27 52 4c 36 02 e7 a0 c4 c8 9e 
ea bf 8a d2 40 c7 38 b5 a3 f7 f2 ce f9 61 15 a1 
e0 ae 5d a4 9b 34 1a 55 ad 93 32 30 f5 8c b1 e3 
1d f6 e2 2e 82 66 ca 60 c0 29 23 ab 0d 53 4e 6f 
d5 db 37 45 de fd 8e 2f 03 ff 6a 72 6d 6c 5b 51
8d 1b af 92 bb dd bc 7f 11 d9 5c 41 1f 10 5a d8
0a c1 31 88 a5 cd 7b bd 2d 74 d0 12 b8 e5 b4 b0
89 69 97 4a 0c 96 77 7e 65 b9 f1 09 c5 6e c6 84
18 f0 7d ec 3a dc 4d 20 79 ee 5f 3e d7 cb 39 48
"""

# 将字符串转换为二维列表
sm4_table_16x16 = [row.split() for row in newdata.strip().split('\n')]

# 假设有一个加密函数，返回加密后的结果
def encrypt(input_value):
    row = input_value // 16
    col = input_value % 16
    return int(sm4_table_16x16[row][col], 16)  # 以16进制转换为整数

# 更新差分攻击表
for i in range(256):
    for j in range(256):
        # 计算输入差分和输出差分
        input_diff = i
        output_diff = encrypt(j) ^ encrypt(input_diff ^ j)
        # 增加对应输出差分的计数
        diff_table[input_diff][output_diff] += 1

# # 打印差分攻击表的一部分（例如前10行10列）
# print(diff_table[:10, :10])

# # # 将 diff_table 写入 txt 文件
# # np.savetxt('diff_table.txt', diff_table, fmt='%d', delimiter='\t')

# 写入文本文件
with open('diff_table.txt', 'w') as f:
    # 设置列宽
    col_width = 3  # 设置每列的宽度为 3 个字符
    
    # 写入列标题
    header = "dX/dY".ljust(col_width)+"  "  # 左对齐，列宽
    header += " ".join(f"{j:02X}".ljust(col_width) for j in range(256))
    f.write(header + "\n")
    
    # 写入数据
    for i in range(256):
        row = f"{i:02X}".ljust(col_width)+"    "  # 行开头编号
        row += " ".join(f"{diff_table[i][j]}".ljust(col_width) for j in range(256))
        f.write(row + "\n")



