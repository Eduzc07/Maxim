#!/bin/bash
# Sync files from ~/.conky
rsync -avz \
  --exclude='cache' \
  --exclude='*.bak' \
  --exclude='*.old*' \
  --exclude='__pycache__' \
  --exclude='*.pyc' \
  ~/.conky/ .
