#!/bin/bash

echo ""
echo ""

echo "############"
echo "Server Check"
echo "############"
echo ""

echo "uptime:"
uptime
echo ""
echo ""

echo "vmstat:"
vmstat 1 10
echo ""
echo ""

echo "nproc:"
nproc
echo ""
echo ""

echo "free:"
free -h
echo ""
echo ""

echo "ps:"
ps -eo pid,ppid,user,stat,pcpu,pmem,etime,comm --sort=-pcpu | head -25
echo ""
echo ""

echo "apache2 processes:"
pid=$(ps -eo pid,comm,stat | awk '$2=="apache2" && $3 ~ /^R/ {print $1; exit}')
if [ -n "$pid" ]; then
  echo "Tracing Apache PID $pid"
  timeout -s INT 5 strace -f -s 180 -e trace=execve -p "$pid" 2>&1 | head -60
else
  echo "No runnable Apache worker found; run again."
fi
echo ""
echo ""

echo "nginx:"
nginx -t
echo ""
echo ""

echo "traffic:"
./traffic.sh
echo ""
echo ""
