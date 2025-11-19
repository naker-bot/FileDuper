#!/usr/bin/env python3
"""
FileDuper NetworkDirectoryDialog Debug Test
Startet FileDuper und überwacht Debug-Ausgabe
"""

import subprocess
import time
import signal
import sys
import threading

def monitor_output(process):
    """Monitor FileDuper output for NetworkDirectoryDialog messages"""
    print("🔍 Monitoring FileDuper output...")
    print("💡 Wait for FTP auto-connect, then NetworkDirectoryDialog should open")
    print("-" * 60)
    
    try:
        for line in iter(process.stdout.readline, ''):
            if not line:
                break
            
            line = line.strip()
            
            # Filter for NetworkDirectoryDialog debug messages
            if any(keyword in line for keyword in [
                "NetworkDirectoryDialog", "getSelectedDirectories", "updateSelectionCount",
                "onItemChanged", "🔍", "🔄", "📊", "🔘", "✅ Ausgewählter Pfad"
            ]):
                print(f"🐛 {line}")
            
            # Also show some key events
            elif any(keyword in line for keyword in [
                "FTP connected", "Dialog", "Benutzer hat", "Verzeichnisse ausgewählt"
            ]):
                print(f"📋 {line}")
                
    except Exception as e:
        print(f"Error monitoring: {e}")

def main():
    print("=" * 70)
    print("🔧 FileDuper NetworkDirectoryDialog Debug Test")
    print("🎯 Testing: Checkbox und 'Ausgewählte hinzufügen' Funktionalität")
    print("=" * 70)
    print()
    
    try:
        # Start FileDuper
        print("🚀 Starting FileDuper...")
        process = subprocess.Popen(
            ["./FileDuper"],
            cwd="/home/nex/c++",
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1,
            universal_newlines=True
        )
        
        print("✅ FileDuper started!")
        print("💡 Instructions:")
        print("   1. Wait for FTP auto-discovery")
        print("   2. Double-click on FTP service to open NetworkDirectoryDialog")
        print("   3. Try selecting checkboxes")
        print("   4. Click 'Ausgewählte hinzufügen'")
        print("   5. Check debug output below")
        print()
        
        # Monitor in background thread
        monitor_thread = threading.Thread(target=monitor_output, args=(process,))
        monitor_thread.daemon = True
        monitor_thread.start()
        
        # Run for 2 minutes then terminate
        time.sleep(120)
        
        print("\n🛑 Test completed - terminating FileDuper...")
        process.terminate()
        try:
            process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            process.kill()
        
        print("✅ Test finished!")
        
    except KeyboardInterrupt:
        print("\n⏹️ Test interrupted by user")
        if 'process' in locals():
            process.terminate()
    except Exception as e:
        print(f"❌ Error: {e}")

if __name__ == "__main__":
    main()
