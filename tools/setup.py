#!/usr/bin/env python3

import requests
import zipfile
import io
from pathlib import Path
import platform
import stat

DSD_VERSION = 'v0.5.0'
WIBO_VERSION = '0.6.16'


tools_path = Path(__file__).parent
root_path = tools_path.parent


EXE = ""
system = platform.system()
if system == "Windows":
    system = "windows"
    EXE = ".exe"
elif system == "Linux":
    system = "linux"
else:
    print(f"Unknown system '{system}'")
    exit(1)
match platform.machine().lower():
    case 'amd64' | 'x86_64': machine = 'x86_64'
    case machine:
        print(f'Unknown machine: {machine}')
        exit(1)


print('\nInstalling dsd...')
response = requests.get(f'https://github.com/AetiasHax/ds-decomp/releases/download/{DSD_VERSION}/dsd-{system}-{machine}{EXE}')
dsd_path = root_path / f'dsd{EXE}'
with open(dsd_path, 'wb') as f:
    f.write(response.content)
dsd_path.chmod(dsd_path.stat().st_mode | stat.S_IEXEC)


print('\nInstalling toolchain...')
response = requests.get('http://decomp.aetias.com/files/mwccarm.zip')
zip_file = zipfile.ZipFile(io.BytesIO(response.content))
zip_file.extractall(tools_path)


if system == "linux":
    print('\nInstalling wibo...')
    response = requests.get(f'https://github.com/decompals/wibo/releases/download/{WIBO_VERSION}/wibo')
    wibo_path = root_path / 'wibo'
    with open(wibo_path, 'wb') as f:
        f.write(response.content)
    wibo_path.chmod(wibo_path.stat().st_mode | stat.S_IEXEC)

