from collections import Counter  
import math  

# 原始字符串  
s = "IYMYSILONRFNCQXQJEDSHBUIBCJUZBOLFQYSCHATPEQGQJEJNGNXZWHHGWFSUKULJQACZKKJOAAHGKEMTAFGMKVRDOPXNEHEKZNKFSKIFROVHHOVXINPHMRTJPYWOGJWPUUVKEPOAWPMRKKOZWLODYAZDRMLPBJKJOBWIWPSEPVVOMBCRYVCRUZAAOUMBCHDAGDIEMSZFZHALIGKEMJJFPCIWKRMLMPINAYOFIREAOLDTHITDVRMSE"  
# S1 = "CHRCHECHRCHRCHREEE"
def Vegina_part(s,m):
    # 初始化子串列表，每个子串初始为空  
    substrings = [''] * m  
      
    # 遍历字符串中的每个字符  
    for i, char in enumerate(s):  
        # 使用模运算来确定当前字符应该放入哪个子串中  
        index = i % m  
        # 将字符添加到对应索引的子串中  
        substrings[index] += char  
      
    return substrings  

def count_uppercase_letters(s):  
    # 初始化计数器  
    count = 0  
    # 遍历字符串中的每个字符  
    for char in s:  
        # 如果字符是大写字母（A-Z），则增加计数器  
        if 'A' <= char <= 'Z':  
            count += 1  
    # 返回计数器的值  
    return count

def count_uppercase_chars_in_string(s):  
    char_count = {}  
    for char in s:  
        if 'A' <= char <= 'Z':  
            char_count[char] = char_count.get(char, 0) + 1  
    return char_count  

def find_recurring_substrings(s, length=3):  
    # 字典用于存储子串及其所有出现位置  
    substr_positions = {}  
  
    # 遍历字符串，从索引0到len(s)-length，因为我们需要检查指定长度的连续字符  
    for i in range(len(s) - length+1):  
        substr = s[i:i+length]  # 获取当前指定长度的子串  
          
        # 如果子串已经在字典中，则添加当前位置到其位置列表中  
        if substr in substr_positions:  
            substr_positions[substr].append(i)  
        else:  
            # 如果是第一次遇到这个子串，则初始化位置列表  
            substr_positions[substr] = [i]  
  
    # 过滤出现过两次及以上的子串及其位置  
    recurring_substrs = {substr: positions for substr, positions in substr_positions.items() if len(positions) >= 2}  
  
    # 输出结果  
    for substr, positions in recurring_substrs.items():  
        print(f"'{substr}': 第一次出现位置 {positions[0]}, 第二次出现位置 {positions[1]}，更多次出现位置 {positions[2:]}") 
  
def calculate_ic(text):  
    # 去除文本中的非字母字符，并转换为小写（或大写，但要保持一致）  
    text = ''.join(filter(str.isalpha, text)).lower()  
      
    # 计算文本长度  
    n = len(text)  
      
    # 计算字母频率  
    freq = Counter(text)  
      
    # 初始化重合指数  
    ic = 0  
      
    # 遍历所有字母频率  
    for count in freq.values():  
        # 计算每个字母对重合指数的贡献  
        # 注意，当n足够大时，这里的近似是合理的  
        ic += (count / n) ** 2  
      
    # 返回重合指数  
    return ic  
  
def transform_string(s,n):  
    result = s[:n]  # 保留前n个字符不变  
    offset = 1  # 初始偏移量为1  
    for i in range(n, len(s), n):  
        # 处理接下来的n个字符（或更少的字符，如果字符串长度不是n的倍数）  
        for j in range(n):  
            if i + j < len(s):  
                # 计算新的字符  
                new_char = chr((ord(s[i + j]) - offset - ord('A') + 26) % 26 + ord('A'))  
                result += new_char  
            else:  
                # 如果已经到达字符串末尾，则停止  
                break  
        offset += 1  # 偏移量增加  
    return result  

# 在这里使用计算机帮我们找到最符合概率的密钥长度
avg_ic=[0]*17
for j in range(3,10):
    # 调用函数并打印结果  
    transformed_string = transform_string(s,j)  
    print(transformed_string)

    Vegnina_strings = Vegina_part(transformed_string,j)
    sum_ic=0
    for i in range(0,j):
        ic_value = calculate_ic(Vegnina_strings[i])  
        sum_ic+=ic_value
        # print(f"The Index of Coincidence ({i}) of the text is: {ic_value:.4f}")
    avg_ic[j-3]=sum_ic/j
print(avg_ic)


find_recurring_substrings(transformed_string)
    # result=count_uppercase_chars_in_string(transform_string)
    # # print(count_uppercase_letters(s))
    # print(result)