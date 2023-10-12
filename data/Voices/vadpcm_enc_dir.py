import os
import sys
import subprocess

if __name__ == '__main__':
    pack_dir = sys.argv[1]
    print(pack_dir)
    files: list[str] = os.listdir(pack_dir)
    for file in files:
        if file.endswith(".aiff"):
            filename = os.path.join(pack_dir,file)
            print(file)
            subprocess.call(['./vadpcm_enc', '-c', 'shared_pred.tbl', filename, filename+'.aifc'])
