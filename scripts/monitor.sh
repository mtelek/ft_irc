#!/usr/bin/env bash
# fd_watch_explained.sh
# Monitors IRC server TCP sockets (LISTEN + client connections)
# and explains what each part means.

PORT=${1:-6667}
PID=${2:-$(pgrep -f 'irc\.exe|ircserv')}

if [ -z "$PID" ]; then
  echo "❌ Process not found (tried pgrep -f 'irc.exe|ircserv')."
  echo "Usage: $0 [PORT] [PID]"
  exit 1
fi

while true; do
  clear
  echo "=== $(date) ==="
  echo "Monitoring IRC server PID: $PID on port $PORT"
  echo

  echo "────────────────────────────────────────────"
  echo "📊 [File Descriptors: server + clients]"
  echo "────────────────────────────────────────────"
  echo "→ Counting only TCP sockets (LISTEN + ESTABLISHED)"
  echo

  FDCOUNT=$(lsof -nP -p "$PID" 2>/dev/null \
    | awk '/ TCP / && $4 ~ /^[0-9]+[a-z]*$/ {c++} END{print c+0}')

  echo "Open TCP FDs: $FDCOUNT"
  echo

  echo "────────────────────────────────────────────"
  echo "🔍 [Detailed Sockets for PID $PID]"
  echo "────────────────────────────────────────────"
  echo "fd=XX → File descriptor number (used internally by your program)"
  echo "LISTEN → Server socket waiting for new connections"
  echo "ESTABL → Active connection to a client (each client gets its own fd)"
  echo

  lsof -nP -p "$PID" 2>/dev/null \
    | awk '
      / TCP / && $4 ~ /^[0-9]+[a-z]*$/ {
        fd=$4; gsub(/[a-z]/,"",fd);
        state="OTHER";
        if ($0 ~ /\(LISTEN\)/) state="LISTEN";
        else if ($0 ~ /->/)    state="ESTABL";
        name="";
        for (i=9;i<=NF;i++) name = name (i==9?"":" ") $i;
        printf "fd=%-3s  %-7s  %s\n", fd, state, name
      }' \
    | sort -k2,2 -k1,1n
  echo

  echo "────────────────────────────────────────────"
  echo "🌐 [Sockets on :$PORT]"
  echo "────────────────────────────────────────────"
  echo "This section shows the OS-level TCP view of your socket:"
  echo "  tcp4 → IPv4 TCP socket"
  echo "  0 0  → recv-q / send-q (queued data waiting to read/send)"
  echo "  *.6667 → Listening on all interfaces on port 6667"
  echo "  *.* → Remote address (any, since it’s listening)"
  echo "  LISTEN / ESTABLISHED → Socket state"
  echo

  if command -v ss >/dev/null 2>&1; then
    ss -tano "sport = :$PORT" || true
  else
    netstat -anv -p tcp 2>/dev/null \
      | grep "[\.\:]$PORT " \
      | grep -v '^[0-9]' \
      || true
  fi

  sleep 1
done
