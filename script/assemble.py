import os

src_path='test/all'
asm_path='CICD/asm/'
dst_path='CICD/bin/'
log_path='CICD/binlog/'
sylib_path='sysyruntimelibrary/libsysy.a'
if not os.path.exists(dst_path):
    os.mkdir(dst_path)
if not os.path.exists(log_path):
    os.mkdir(log_path)

os.system('rm -rf {}*'.format(dst_path))
os.system('rm -rf {}*'.format(log_path))

pass_num=0
fail_num=0

for path, dir_list, file_list in os.walk(asm_path):
    for file_name in sorted(file_list):
        if file_name.split('.')[-1]=='s':
            # 文件前缀名
            type_name=file_name[:-2]
            # 完整的汇编代码路径
            s_path = os.path.join(path, file_name)
            bin = dst_path+type_name + '.bin'
            log=log_path+type_name+'.log'
            ret_value = os.system('arm-linux-gnueabihf-gcc -mcpu=cortex-a72 -o {} {} {} > {} 2>&1'.format(bin, s_path, sylib_path, log))
            ret_value >>= 8
            if ret_value != 0:
                print('\033[1;31mFAIL:\033[0m {}\t\033[1;31mAssemble Error\033[0m'.format(type_name))
                fail_num += 1
            else:
                #print('\033[1;32mAssemble Success:\033[0m {}'.format(type_name))
                pass_num+=1
print('\033[1;33mTotal: {}\t\033[1;32mSuccess: {}\t\033[1;31mFail:{}'.format(pass_num + fail_num, pass_num, fail_num))

