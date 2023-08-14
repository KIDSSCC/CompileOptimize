import os
import re
src_path='test/all/'
bin_path='CICD/bin/'
dst_path='CICD/res/'
log_path='CICD/reslog/'


if not os.path.exists(dst_path):
    os.mkdir(dst_path)
if not os.path.exists(log_path):
    os.mkdir(log_path)
os.system('rm -rf {}*'.format(dst_path))
os.system('rm -rf {}*'.format(log_path))

pass_num=0
fail_num=0

for path, dir_list, file_list in os.walk(bin_path):
    for file_name in sorted(file_list):
        case_name = file_name[:-4]
        bin = os.path.join(path, file_name)
        res=dst_path+case_name+'.res'
        log=log_path+case_name+'_exe'+'.log'
        in_path=src_path+case_name+'.in'
        out_path=src_path+case_name+'.out'
        # 执行
        if os.path.exists(in_path):
            ret_value = os.system('timeout 5s qemu-arm -L /usr/arm-linux-gnueabihf ./{} < {} > {} 2>{}'.format(bin, in_path, res, log))
        else:
            ret_value = os.system('timeout 5s qemu-arm -L /usr/arm-linux-gnueabihf ./{} > {} 2>{}'.format(bin, res, log))
        ret_value >>= 8
        if ret_value == 124:
            print('\033[1;31mFAIL:\033[0m {}\t\033[1;31mExecute Timeout\033[0m'.format(case_name))
            fail_num += 1
            continue
        elif ret_value == 127:
            print('\033[1;31mFAIL:\033[0m {}\t\033[1;31mExecute Error\033[0m'.format(case_name))
            fail_num += 1
            continue
        with open(res, 'a+') as f:
                f.seek(0)
                lines = f.readlines()
                if len(lines):
                    line = lines[-1]
                    if len(line):
                        char = line[-1]
                        if char != '\n':
                            f.write('\n')
                f.write('{}\n'.format(ret_value))
        ret_value = os.system('diff -b {} {} >/dev/null 2>&1'.format(res, out_path))
        ret_value >>= 8
        if ret_value:
            print('\033[1;31mFAIL:\033[0m {}\t\033[1;31mWrong Answer\033[0m'.format(case_name))
            fail_num += 1
            continue
        else:
            total_us=-1
            with open(log, 'r') as f:
                    lines = f.readlines()
            for line in lines[::-1]:
                if line[:5] == 'TOTAL':
                    res = re.search(r'([0-9]+)H-([0-9]+)M-([0-9]+)S-([0-9]+)us', line)
                    h = int(res.group(1))
                    m = int(res.group(2))
                    s = int(res.group(3))
                    us = int(res.group(4))
                    total_s = 3600 * h + 60 * m + s
                    total_us = total_s * 1000000 + us
            print('\033[1;32mPASS:\033[0m {}\t {}us'.format(case_name, total_us))
            pass_num+=1
print('\033[1;33mTotal: {}\t\033[1;32mAccept: {}\t\033[1;31mFail:{}'.format(pass_num + fail_num, pass_num, fail_num))