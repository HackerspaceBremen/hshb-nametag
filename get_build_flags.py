#!/usr/bin/env python

import os;

print(f"-Os -DBoard{os.getenv('HSHB_BOARD', '1')}")
