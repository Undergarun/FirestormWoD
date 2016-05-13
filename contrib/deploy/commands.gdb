set logging file /tmp/backtrace.log
set logging overwrite on
set pagination 0
set logging on

run -c /usr/local/etc/firestorm/worldserver.conf

echo \n--- DEBUG: --- START\n\n
info program

echo \n--- DEBUG: BACKTRACE FULL\n\n
bt full

echo \n--- DEBUG: INFO THREAD\n\n
info thread

echo \n--- DEBUG: THREAD APPLY ALL BACKTRACE\n
thread apply all backtrace

echo \n--- DEBUG: --- STOP\n\n

set logging off
quit
