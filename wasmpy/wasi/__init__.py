import ctypes, os, platform


unstable = os.path.join(os.path.dirname(__file__), "wasi_unstable")
snapshot_preview1 = os.path.join(
    os.path.dirname(__file__), "wasi_snapshot_preview1"
)

if platform.system() == "Linux":
    unstable = ctypes.CDLL(unstable + ".so")
    snapshot_preview1 = ctypes.CDLL(snapshot_preview1 + ".so")

elif platform.system() == "Windows":
    unstable = ctypes.CDLL(unstable + ".dll")
    snapshot_preview1 = ctypes.CDLL(snapshot_preview1 + ".dll")

else:
    raise FileNotFoundError(
        f"Missing wasi native libs for platform {platform.system()}"
    )
