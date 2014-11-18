#!/bin/bash

# Script for creating multiple disk images
# For use with mkdir_creat automated taste
touch disk_0
mke2fs disk_0 1400

touch disk_1
mke2fs disk_1 1400

touch disk_2
mke2fs disk_2 1400
touch disk_3
mke2fs disk_3 1400

touch disk_4
mke2fs disk_4 1400

ls disk_* > diskNames.txt
