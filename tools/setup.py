import requests
import zipfile
import io
from pathlib import Path
import platform

DSD_VERSION = 'v0.3.1'


tools_path = Path(__file__).parent
root_path = tools_path.parent


EXE = ""
system = platform.system()
if system == "Windows" or system.startswith("MSYS") or system.startswith("MINGW"):
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
with open(root_path / f'dsd{EXE}', 'wb') as f:
    f.write(response.content)


print('\nInstalling toolchain...')
response = requests.get('http://decomp.aetias.com/files/mwccarm.zip')
zip_file = zipfile.ZipFile(io.BytesIO(response.content))
zip_file.extractall(tools_path)

