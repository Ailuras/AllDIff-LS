import random
import os

def generate_data(num1, data):
    while data.count(1) < num1:
        data[random.randint(0, len(data)-1)] = 1
    return data

def write_to_file(filename, data):
    with open(filename, "w") as f:
        for item in data:
            f.write("%s\n" % item)

def generate_files(output_folder, num_files, num_lines, number):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    for num in range(1, number+1):
        number_folder = os.path.join(output_folder, str(num))
        if not os.path.exists(number_folder):
            os.makedirs(number_folder)
        data = [0 for i in range(num_lines)]
        for p in range(1, num_files+1):
            data = generate_data(int(num_lines/num_files * p), data)
            filename = os.path.join(number_folder, f"data_{p:0{2}d}.txt")
            write_to_file(filename, data)
    print(f"已生成数据文件：{filename}")

def main():
    output_folder = "proportion"  # 指定文件夹路径
    num_files = 20
    num_lines = 147
    generate_files(output_folder, num_files, num_lines, 30)

if __name__ == "__main__":
    main()
