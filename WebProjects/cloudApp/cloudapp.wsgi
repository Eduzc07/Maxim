#!/usr/bin/python
import logging
import sys

logging.basicConfig(stream=sys.stderr)
sys.stdout = sys.stderr
sys.path.insert(0, "/var/www/cloudApp/")

from odroidApp import app as application
