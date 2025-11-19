#!/usr/bin/env bash
set -euo pipefail

echo "Running all NFS helper tests..."

echo "1) check showmount" 
if ! command -v showmount >/dev/null 2>&1; then
  echo "showmount not installed. Install 'nfs-common' to proceed." 
else
  echo "showmount available"
fi

echo "2) run test_nfs_listexports against 127.0.0.1 (expected: none)"
./test_nfs_listexports 127.0.0.1 || true

echo "All done. If you want to attempt mounting in a test, create a temporary NFS server or run with a real server IP."
