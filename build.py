import os

PROJ_PATH = os.getcwd()
print(f"Running Python build script for Twilight Escapers in {PROJ_PATH}")

# Check if directories exist and create them if not


def check():
    os.chdir(PROJ_PATH)
    # if not os.path.isdir('bin'):
    #     print("'bin' directory doesn't exist => creating")
    #     os.mkdir("bin")
    if not os.path.isdir('tmp'):
        print("'tmp' directory doesn't exist => creating")
        os.mkdir("tmp")

# Go to temporary directory and build project


def build():
    os.chdir(PROJ_PATH)
    os.chdir("tmp")
    os.system("cmake -DCMAKE_BUILD_TYPE=Release -G \"MinGW Makefiles\" ..")
    os.system("mingw32-make.exe")


def run():
    os.chdir(PROJ_PATH)
    os.system("bin/twe.exe")


check()
build()
run()
