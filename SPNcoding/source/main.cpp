// #include <iostream>
// #include <vector>
// #include <string>
// #include <random>
// #include <bitset>
// #include <thread>
// #include <mutex>
// #include <algorithm>

// const int BIT_LENGTH = 4;  // 每组比特数
// const int GROUP_COUNT = 4;  // 组数
// const int ROUNDS = 4;  // 轮数

// // S盒和P盒
// const std::vector<int> SBOX_TABLE = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};
// const std::vector<int> PBOX_TABLE = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};

// // 生成逆S盒
// std::vector<int> inverse_sbox(const std::vector<int>& sbox) {
//     std::vector<int> inv_sbox(sbox.size());
//     for (size_t index = 0; index < sbox.size(); ++index) {
//         inv_sbox[sbox[index]] = index;
//     }
//     return inv_sbox;
// }

// // 二进制异或运算
// std::string bitwise_xor(const std::string& a, const std::string& b) {
//     std::string result;
//     for (size_t i = 0; i < a.size(); ++i) {
//         result += (a[i] ^ b[i]);
//     }
//     return result;
// }

// // S盒运算
// std::string apply_sbox(const std::string& input_bits) {
//     std::string output_bits;
//     for (int i = 0; i < GROUP_COUNT; ++i) {
//         std::string group = input_bits.substr(i * BIT_LENGTH, BIT_LENGTH);
//         int index = std::stoi(group, nullptr, 2);
//         output_bits += std::bitset<BIT_LENGTH>(SBOX_TABLE[index]).to_string();
//     }
//     return output_bits;
// }

// // P盒运算
// std::string apply_pbox(const std::string& input_bits) {
//     std::string output_bits(BIT_LENGTH * GROUP_COUNT, '0');
//     for (size_t i = 0; i < input_bits.size(); ++i) {
//         output_bits[PBOX_TABLE[i] - 1] = input_bits[i];
//     }
//     return output_bits;
// }

// // SPN加密算法
// std::string encrypt_spn(const std::string& plaintext, const std::string& key) {
//     std::vector<std::string> keys(ROUNDS + 1);
//     for (int i = 0; i <= ROUNDS; ++i) {
//         keys[i] = key.substr(i * (BIT_LENGTH * GROUP_COUNT), BIT_LENGTH * GROUP_COUNT);
//     }
    
//     std::string current_bits = plaintext;
//     for (int r = 0; r < ROUNDS - 1; ++r) {
//         std::string intermediate = bitwise_xor(current_bits, keys[r]);
//         std::string substituted = apply_sbox(intermediate);
//         current_bits = apply_pbox(substituted);
//     }
    
//     std::string intermediate = bitwise_xor(current_bits, keys[ROUNDS - 1]);
//     std::string substituted = apply_sbox(intermediate);
//     return bitwise_xor(substituted, keys[ROUNDS]);
// }

// // 生成密钥
// std::vector<std::string> create_key(int count) {
//     std::vector<std::string> keys;
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dis(0, 1);

//     for (int i = 0; i < count; ++i) {
//         std::string key;
//         for (int j = 0; j < BIT_LENGTH * GROUP_COUNT + 4 * ROUNDS; ++j) {
//             key += std::to_string(dis(gen));
//         }
//         keys.push_back(key);
//     }
//     return keys;
// }

// // 生成明文-密文对
// std::vector<std::pair<std::string, std::string>> create_data_pairs(int num_pairs, const std::string& key) {
//     std::vector<std::pair<std::string, std::string>> data_pairs;
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dis(0, (1 << (BIT_LENGTH * GROUP_COUNT)) - 1);

//     for (int i = 0; i < num_pairs; ++i) {
//         std::string plaintext = std::bitset<BIT_LENGTH * GROUP_COUNT>(dis(gen)).to_string();
//         std::string ciphertext = encrypt_spn(plaintext, key);
//         data_pairs.emplace_back(plaintext, ciphertext);
//     }
//     return data_pairs;
// }

// // 线性攻击算法
// std::string attack_reverse_spn(const std::vector<std::pair<std::string, std::string>>& data_pairs) {
//     int total_pairs = data_pairs.size();
//     std::vector<std::vector<int>> counts(16, std::vector<int>(16, 0));

//     auto inverse_sbox_table = inverse_sbox(SBOX_TABLE);
    
//     for (const auto& pair : data_pairs) {
//         const auto& plaintext = pair.first;
//         const auto& ciphertext = pair.second;
//         int partial_y2 = std::stoi(ciphertext.substr(4, 4), nullptr, 2);
//         int partial_y4 = std::stoi(ciphertext.substr(12, 4), nullptr, 2);
        
//         for (int L1 = 0; L1 < 16; ++L1) {
//             for (int L2 = 0; L2 < 16; ++L2) {
//                 int intermediate2 = inverse_sbox_table[L1 ^ partial_y2];
//                 int intermediate4 = inverse_sbox_table[L2 ^ partial_y4];

//                 int z = (plaintext[1] ^ plaintext[3] ^ plaintext[4] ^ 
//                          std::bitset<BIT_LENGTH>(intermediate2).to_string()[0] ^ 
//                          std::bitset<BIT_LENGTH>(intermediate4).to_string()[0]);
                
//                 if (z % 2 == 0) {
//                     counts[L1][L2]++;
//                 }
//             }
//         }
//     }

//     // 计算最大值位置
//     auto max_pos = std::max_element(counts.begin(), counts.end());
//     int k2 = std::distance(counts.begin(), max_pos);
//     int k4 = std::distance(max_pos->begin(), std::max_element(max_pos->begin(), max_pos->end()));

//     std::vector<std::vector<int>> counts1(16, std::vector<int>(16, 0));
//     std::vector<std::vector<int>> counts2(16, std::vector<int>(16, 0));

//     for (const auto& pair : data_pairs) {
//         const auto& plaintext = pair.first;
//         const auto& ciphertext = pair.second;
//         int y1 = std::stoi(ciphertext.substr(0, 4), nullptr, 2);
//         int y2 = std::stoi(ciphertext.substr(4, 4), nullptr, 2);
//         int y3 = std::stoi(ciphertext.substr(8, 4), nullptr, 2);
//         int y4 = std::stoi(ciphertext.substr(12, 4), nullptr, 2);
        
//         for (int L1 = 0; L1 < 16; ++L1) {
//             for (int L2 = 0; L2 < 16; ++L2) {
//                 std::vector<int> combined;
//                 combined.insert(combined.end(), 
//                                 {inverse_sbox_table[L1 ^ y1], 
//                                  inverse_sbox_table[k2 ^ y2], 
//                                  inverse_sbox_table[L2 ^ y3], 
//                                  inverse_sbox_table[k4 ^ y4]});

//                 for (int index : combined) {
//                     counts1[L1][index]++;
//                     counts2[k2][index]++;
//                 }
//             }
//         }
//     }

//     std::vector<int> final_counts(16, 0);
//     for (int L1 = 0; L1 < 16; ++L1) {
//         for (int L2 = 0; L2 < 16; ++L2) {
//             final_counts[L1] += counts1[L1][L2] + counts2[L2][L1];
//         }
//     }

//      // 找到最可能的密钥
//     int final_k2 = -1, final_k4 = -1;
//     int final_max_count = 0;
    
//     for (int i = 0; i < 16; ++i) {
//         if (final_counts[i] > final_max_count) {
//             final_max_count = final_counts[i];
//             final_k2 = i;
//         }
//     }

//     return std::bitset<BIT_LENGTH>(final_k2).to_string() + std::bitset<BIT_LENGTH>(k4).to_string();
// }

// // 处理密钥
// std::pair<std::string, std::string> handle_key(const std::string& key) {
//     int data_count = 10000;  // 每个密钥生成的明文-密文对个数
//     auto data_pairs = create_data_pairs(data_count, key);
//     std::string estimated_key = attack_reverse_spn(data_pairs);
//     return {key, estimated_key};
// }

// // 打印结果
// void display_result(const std::pair<std::string, std::string>& result) {
//     const auto& key = result.first;
//     const auto& estimated_key = result.second;
//     std::string result_status = key.substr(16) == estimated_key ? "success" : "fail";

//     // 格式化密钥输出
//     std::cout << "key = " << key << "\nestimated_key = " << estimated_key << "\nresult: " << result_status << "\n\n";
// }

// int main() {
//     int key_count = 50;  // 待攻击密钥个数
//     auto generated_keys = create_key(key_count);  // 生成密钥
//     std::vector<std::pair<std::string, std::string>> results;

//     for (const auto& key : generated_keys) {
//         results.push_back(handle_key(key));  // 处理密钥并存储结果
//     }

//     for (const auto& result : results) {
//         display_result(result);  // 显示结果
//     }

//     return 0;
// }
