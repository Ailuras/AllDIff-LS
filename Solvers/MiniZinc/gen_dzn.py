import os
import shutil

source_folder = 'proportion'
target_folder = 'proportion_CP'

if not os.path.exists(target_folder):
    os.makedirs(target_folder)

for folder in os.listdir(source_folder):
    
    number_folder = os.path.join(source_folder, folder)
    number_folder2 = os.path.join(target_folder, folder)
    if not os.path.exists(number_folder2):
        os.makedirs(number_folder2)
        
    for filename in os.listdir(number_folder):
        source_file_path = os.path.join(number_folder, filename)
        if os.path.isfile(source_file_path):
            with open(source_file_path, 'r') as source_file:
                lines = source_file.readlines()
            
            processed_data = "[" + ", ".join([line.strip() for line in lines]) + "]"
            processed_line = "proportion = {};\n".format(processed_data)
            
            target_file_path = os.path.join(number_folder2, f'{filename.split(".")[0]}.dzn')
            
            with open(target_file_path, 'w') as target_file:
                target_file.write(processed_line)
        
print("Done")
