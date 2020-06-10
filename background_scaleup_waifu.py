# -*- coding: utf-8 -*-
# @brief: background scaleup waifu
# @copyright: T.C.V.
# @license: MIT
# @birth: created by Infinite Synthesis on 2020 Jun 2
# @version: V0.0.1

import os
import sys, argparse
from PIL import Image
import subprocess

MONITOR_SIZE = {'4K' : 3840, '1440p': 2560, '2K' : 1920}
MONITOR_TYPE = '4K'
FOLDER_PATH = 'E:/vertical pixiv/'
WAIFU2X_PATH = 'C:/Program Files (x86)/waifu2x-caffe/waifu2x-caffe-cui.exe'
SUPPORTED_FORMAT = ['.jpg', '.png', '.jpeg', '.tif', '.tiff' , '.bmp', '.tga']
SUITABLE_COUNT = 0
PROCESSED_COUNT = 0
FAILED_SET = []

def excute_command(com):
    # print("\nnow executing:", com)
    print("=", end = "")
    ex = subprocess.Popen(com, stdout=subprocess.PIPE)
    out, err  = ex.communicate()
    return_code = ex.returncode
    ex.wait()
    # print(out.decode("utf-16-le"))
    ex.kill()
    return return_code

def call_waifu2x(file_path, scale):
    global FAILED_SET
    fname, ext = os.path.splitext(file_path)
    target_name = fname + '_' + str(scale) + 'x' + ext
    cmd = WAIFU2X_PATH + ' -s ' + str(scale) +' -i "' + file_path + '" -o "' + target_name + '"'
    result = excute_command(cmd)
    if (result == 0):
        os.remove(file_path)
    else:
        FAILED_SET.append(file_path)

def process_image(file_path):
    global SUITABLE_COUNT, PROCESSED_COUNT
    img = Image.open(file_path)
    imgWidth, imgHeight = img.size
    img.close()
    idealLength = MONITOR_SIZE[MONITOR_TYPE]
    PROCESSED_COUNT += 1
    scale = (idealLength / imgWidth) if (imgWidth > imgHeight) else (idealLength / imgHeight)
    if (scale <= 1):
        SUITABLE_COUNT += 1
        PROCESSED_COUNT -= 1
        print(">", end = "")
    elif (scale <= 1.5):
        call_waifu2x(file_path, 1.5)
    elif (scale <= 2):
        call_waifu2x(file_path, 2)
    else:
        call_waifu2x(file_path, 3)
        
def getOptions(args=sys.argv[1:]):
    global FOLDER_PATH, MONITOR_TYPE, WAIFU2X_PATH
    parser = argparse.ArgumentParser(description = 'Scale-up images to appropriate resolution')
    parser.add_argument('-i', '--input', action = 'store', type = str, help = 'the image folder')
    parser.add_argument('-r', '--resolution', action = 'store', type = str, choices = ['4K', '1440p', '2K'], help = 'monitor resolution')
    parser.add_argument('-w', '--waifu2x', action = 'store', type = str, help = 'waifu2x-caffe cui exe file path')
    args = parser.parse_args()
    if (args.input):
        FOLDER_PATH = args.input
    if (args.resolution):
        MONITOR_TYPE = args.resolution
    if (args.waifu2x):
        WAIFU2X_PATH = args.waifu2x

def main():
    getOptions()
    for root, dirs, files in os.walk(FOLDER_PATH):
        for file in files:
            if (os.path.splitext(file)[1] in SUPPORTED_FORMAT):
                process_image(root + file)
            
    print("\nsuitable image count = \033[1;32m" + str(SUITABLE_COUNT) + "\033[0m, processed image count = \033[1;34m" + str(PROCESSED_COUNT) + "\033[0m.")
    print('failed image set:', FAILED_SET)
        
if __name__ == "__main__":
    main()