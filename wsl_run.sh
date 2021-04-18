#!/bin/bash

if [ "$1" == "debug" ]; then
	qemu-system-x86_64.exe -m 1G -debugcon stdio -drive file=cernel.hdd,format=raw -s -S
else
	qemu-system-x86_64.exe -m 1G -debugcon stdio -drive file=cernel.hdd,format=raw -no-reboot
fi
