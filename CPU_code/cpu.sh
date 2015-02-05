#!/bin/bash
head -1 /proc/stat | grep -o '[0-9]*'
