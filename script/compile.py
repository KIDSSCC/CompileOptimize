import os

src_path='test/all'
CICD='CICD/'
dst_path='CICD/asm/'
log_path='CICD/asmlog/'


#创建目标文件夹
if not os.path.exists(CICD):
    os.mkdir(CICD)
if not os.path.exists(dst_path):
    os.mkdir(dst_path)
if not os.path.exists(log_path):
    os.mkdir(log_path)
os.system('rm -rf {}*'.format(dst_path))
os.system('rm -rf {}*'.format(log_path))

pass_num=0
fail_num=0

for path, dir_list, file_list in os.walk(src_path):
    for file_name in sorted(file_list):
        if file_name.split('.')[-1]=='sy':
            type_name=file_name[:-3]
            sy_path = os.path.join(path, file_name)
            asm_path = dst_path + type_name + '.s'
            log = log_path+type_name + '.log'
            ret_value = os.system('timeout 2s build/compiler -S {} -o {} > {} 2>&1'.format(sy_path, asm_path, log))
            ret_value >>= 8
            if ret_value == 124:
                print('\033[1;31mFAIL:\033[0m {}\t\033[1;31mCompile Timeout\033[0m'.format(type_name))
                fail_num += 1
                continue
            elif ret_value != 0:
                print('\033[1;31mFAIL:\033[0m {}\t\033[1;31mCompile Error\033[0m'.format(type_name))
                fail_num += 1
                continue
            else:
                pass_num += 1
print('\033[1;33mTotal: {}\t\033[1;32mSuccess: {}\t\033[1;31mFail:{}'.format(pass_num + fail_num, pass_num, fail_num))