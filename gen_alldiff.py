import random
import os

def generate_data(num_lines, p):
    data = []
    num_ones = int(num_lines * p / 100)
    
    for _ in range(num_lines):
        if num_ones > 0:
            data.append("1")
            num_ones -= 1
        else:
            data.append("0")
    
    random.shuffle(data)
    return data

def write_to_file(filename, data):
    with open(filename, "w") as f:
        for item in data:
            f.write("%s\n" % item)

def generate_files(output_folder, num_files, num_lines):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    
    for p in range(1, num_files+1):
        data = generate_data(num_lines, p)
        filename = os.path.join(output_folder, f"data_{p}.txt")
        write_to_file(filename, data)
        print(f"已生成数据文件：{filename}")

def main():
    output_folder = "proportion"  # 指定文件夹路径
    num_files = 100
    num_lines = 192
    generate_files(output_folder, num_files, num_lines)

if __name__ == "__main__":
    main()
