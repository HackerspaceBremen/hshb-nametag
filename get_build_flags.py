#!/usr/bin/env python

import os
import subprocess

board_id = os.getenv(
    "BOARD_ID", "24"
)  # PLACE YOUR BOARD ID HERE OR AS ENVIRONMENT PARAMETER

try:  # FETCH GIT COMMIT HASH
    build_version = (
        subprocess.run(["git", "rev-parse", "--short", "HEAD"], stdout=subprocess.PIPE)
        .stdout.decode()
        .strip()
    )
except:
    build_version = ""

print(f'-Os -DBOARD_ID={board_id} -DBUILD_VERSION="{build_version}"')
