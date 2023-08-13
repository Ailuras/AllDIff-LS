import os
import shutil

# 源文件夹和目标文件夹的路径
source_folder = 'proportion'
target_folder = 'proportion_CP'

# 确保目标文件夹存在，如果不存在则创建
if not os.path.exists(target_folder):
    os.makedirs(target_folder)

# 遍历源文件夹中的文件
for filename in os.listdir(source_folder):
    source_file_path = os.path.join(source_folder, filename)
    
    # 如果是文件，进行处理并保存到目标文件夹
    if os.path.isfile(source_file_path):
        # 读取源文件内容
        with open(source_file_path, 'r') as source_file:
            lines = source_file.readlines()
        
        # 处理每行数据并生成形如 "proportion = [...];" 的形式
        processed_data = "[" + ", ".join([line.strip() for line in lines]) + "]"
        processed_line = "proportion = {};\n".format(processed_data)
        
        # 构建目标文件的路径
        target_file_path = os.path.join(target_folder, f'{filename.split(".")[0]}.dzn')
        
        # 写入处理后的数据到目标文件
        with open(target_file_path, 'w') as target_file:
            target_file.write(processed_line)
        
print("文件处理完成并保存到目标文件夹。")
