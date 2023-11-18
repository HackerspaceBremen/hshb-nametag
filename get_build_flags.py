#!/usr/bin/env python

import os;

print(f"-Os -DBOARD_ID={os.getenv('BOARD_ID', '1')}")
