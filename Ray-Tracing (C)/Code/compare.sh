#!/bin/bash
IMAGE1=$1
IMAGE2=$2

MYPATH=$(dirname "$0")

"$MYPATH"/exec "compare" "${IMAGE1}" "${IMAGE2}"