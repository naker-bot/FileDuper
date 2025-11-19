## Network scanning improvements (Nov 2025)

This release includes a set of changes to improve network discovery, port probing, and UI integration:

  - Store `NetworkService` metadata for discovered services in `networkTree` items (IP, port, service), enabling safe double-click connect.
  - Add explicit Scan/Stop control and `scanProgress` updates showing number of hosts probed.
  - Avoid clearing networkTree while an active scan is running.

Notes:
 - `scanSubnetCancelable` uses fork/exec and `nmap`. This is POSIX only; Windows support is not implemented in this
 change.                                                                                                           
 - Future work: make cross-platform probes and discovery (Winsock + CreateProcess/TerminateProcess), add more robust process cleanup for `nmap` subprocesses.

- Add Windows implementation `src/networkscanner_windows.cpp` with Winsock-based `probePort` and CreateProcess-based `scanSubnetCancelable`.
- Update `CMakeLists.txt` to select `NETWORKSCANNER_SRC` platform-specific implementation and use it in tests.

