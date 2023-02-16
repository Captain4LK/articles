#!/bin/sh

uncrustify -c uncrustify.cfg --no-backup --replace $(find articles/ -name "*.c") $(find articles/ -name "*.h")
