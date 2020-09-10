import os

if __name__ == '__main__':
    os.system(r'gyp --depth=. --generator-output=./vs15 --f=msvs -D library="shared_library" -D OS="win" -D build_type="win_exe" -D build_env="msvc2015" -G msvs_version=2015 OFCamera.gyp')