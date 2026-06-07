import sys
import subprocess

platform = sys.argv[1]

try:
    subprocess.run(
        ["cmake", "-B", "build", f"-DMOB_SENSORS_PLATFORM={platform}"],
        check=True
    )
except subprocess.CalledProcessError:
    print("Failed to run setup script")
