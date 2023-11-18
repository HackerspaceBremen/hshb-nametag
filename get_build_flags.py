#!/usr/bin/env python

import os
import subprocess

board_id = os.getenv("BOARD_ID", "1")

try:
    build_version = (
        subprocess.run(
            ["git", "describe", "--tags", "HEAD", "--long"], stdout=subprocess.PIPE
        )
        .stdout.decode()
        .strip()
    )
except:
    build_version = "unknown"

print(f'-Os -DBOARD_ID={board_id} -DBUILD_VERSION="{build_version}"')
