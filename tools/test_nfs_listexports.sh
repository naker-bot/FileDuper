#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <server> [server2 ...]"
  exit 2
fi

for server in "$@"; do
  printf "Testing showmount exports for %s...\n" "$server"
  ./test_nfs_listexports "$server" || echo "No exports for $server (maybe showmount missing)"
done

echo "Done." 
