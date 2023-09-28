#!/bin/bash

# Pour connaître l'emplacement du script build.sh
MYPATH="$(dirname "$0")"

# crée un répertoire obj s'il n'existe pas

if [ ! -d "$MYPATH/obj" ]; then
   mkdir $MYPATH/obj
fi

make clean

make
